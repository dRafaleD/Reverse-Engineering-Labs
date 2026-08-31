# Gün 5 — Stack Frame ve Local Değişkenler

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Bu labın amacı bir fonksiyon çalışırken stack üzerinde neler olduğunu daha net görmek ve Ghidra'da local değişkenlerin neden `RBP` veya `RSP` tabanlı adreslerle gösterildiğini anlamaktır.

Özellikle şu kavramları bağlayacağız:

- stack
- `RSP`
- `RBP`
- local değişkenler
- stack frame
- `PUSH`, `POP`, `SUB`, `ADD`, `LEAVE`, `RET`

Amaç stack'i ezberlemek değil; bir fonksiyonun kendine nasıl çalışma alanı ayırdığını görmek.

## Kaynak Kod

```c
#include <stdio.h>

int calculate(int a, int b) {
    int sum = a + b;
    int doubled = sum * 2;
    int result = doubled - 3;

    return result;
}

int main() {
    int x = 7;
    int y = 5;
    int output = calculate(x, y);

    printf("Result: %d\n", output);

    return 0;
}
```

Debug bilgisiyle derlemek için:

```bash
gcc -g stack_frame.c -o stack_frame
```

Stack frame'i daha görünür hale getirmek için optimizasyonu kapatmak faydalı olabilir:

```bash
gcc -g -O0 -fno-omit-frame-pointer stack_frame.c -o stack_frame
```

Ardından binary'yi Ghidra'ya import edip önce `calculate` fonksiyonunu aç.

## Stack Nedir?

Stack, fonksiyon çağrıları sırasında geçici verilerin tutulduğu bellek alanıdır.

Basitçe şu tür bilgiler burada bulunabilir:

- local değişkenler
- bazı fonksiyon argümanları
- kaydedilmiş register değerleri
- return address

Stack genellikle yüksek bellek adreslerinden daha düşük adreslere doğru büyür.

```text
yüksek adresler
     |
     v
+------------------+
| eski stack verisi |
+------------------+
| return address    |
+------------------+
| saved RBP         |
+------------------+
| local variables   |
+------------------+
     ^
     |
daha düşük adresler
```

## `RSP` Nedir?

`RSP`, stack pointer'dır.

Stack'in o anda en üstündeki konumu gösterir.

Örneğin:

```asm
PUSH RBP
```

çalıştığında stack'e veri eklenir ve `RSP` değişir.

Benzer şekilde:

```asm
POP RBP
```

stack'ten veri alınır ve `RSP` tekrar hareket eder.

## `RBP` Nedir?

`RBP`, çoğu basit/debug derlemede fonksiyonun stack frame'i için sabit bir referans noktası olarak kullanılabilir.

Fonksiyon başında sık görülen kalıp:

```asm
PUSH RBP
MOV  RBP, RSP
```

Bunu şimdilik şöyle okuyabiliriz:

```text
eski RBP'yi sakla
    ↓
bu fonksiyonun başlangıç referansını oluştur
```

Bu yüzden local değişkenler Ghidra'da şöyle görünebilir:

```text
[RBP - 0x4]
[RBP - 0x8]
[RBP - 0xc]
```

Bunlar aynı fonksiyonun stack frame'i içindeki farklı konumlardır.

## Local Değişkenler Stack'te Nasıl Görünür?

Kaynak kod:

```c
int sum = a + b;
int doubled = sum * 2;
int result = doubled - 3;
```

Ghidra'da sadeleştirilmiş olarak buna benzer satırlar görülebilir:

```asm
MOV dword ptr [RBP - 0x4], EAX
MOV dword ptr [RBP - 0x8], EAX
MOV dword ptr [RBP - 0xc], EAX
```

Burada:

```text
[RBP - 0x4] -> bir local int
[RBP - 0x8] -> başka bir local int
[RBP - 0xc] -> başka bir local int
```

olabilir.

Ghidra debug sembollerini kullanabiliyorsa bunları daha anlamlı isimlerle de gösterebilir.

## Neden Eksi Kullanılıyor?

Örneğin:

```asm
MOV dword ptr [RBP - 0x4], EAX
```

`RBP - 0x4`, `RBP` referans noktasından 4 byte daha aşağıdaki konumu ifade eder.

Stack'in aşağı doğru büyümesi nedeniyle local değişkenler sık sık negatif offset'lerle görülür.

Basitleştirilmiş görünüm:

```text
RBP        -> saved frame boundary
RBP - 0x4 -> local variable
RBP - 0x8 -> local variable
RBP - 0xc -> local variable
```

## `SUB RSP, ...` Ne Yapıyor?

Fonksiyon başında şöyle bir instruction görebilirsin:

```asm
SUB RSP, 0x20
```

Bu, stack pointer'ı aşağı çekerek fonksiyon için stack üzerinde alan ayırır.

İnsan dilinde:

> "Bu fonksiyonun local işleri için biraz yer aç."

Örneğin:

```text
RSP = 0x1000
SUB RSP, 0x20
RSP = 0x0fe0
```

Aradaki 32 byte artık fonksiyonun kullanabileceği stack alanının bir parçasıdır.

## `ADD RSP, ...` veya `LEAVE`

Fonksiyon bitiminde ayrılan alan temizlenir.

Bazen şöyle görebilirsin:

```asm
ADD RSP, 0x20
POP RBP
RET
```

Bazen de:

```asm
LEAVE
RET
```

`LEAVE`, klasik stack frame yapısında `RSP` ve `RBP` durumunu fonksiyon çıkışı için toparlayan bir instruction'dır.

## `calculate` İçin Basitleştirilmiş Akış

Tam assembly compiler'a göre değişir ama mantık yaklaşık olarak şöyledir:

```text
calculate başlar
    ↓
eski RBP saklanır
    ↓
yeni stack frame hazırlanır
    ↓
a ve b işlenir
    ↓
sum stack'te tutulur
    ↓
doubled stack'te tutulur
    ↓
result stack'te tutulur
    ↓
sonuç EAX'e hazırlanır
    ↓
stack frame temizlenir
    ↓
RET
```

## Ghidra'da Neye Bakmalıyım?

`calculate` fonksiyonunu açıp hem Listing hem Decompiler panelini yan yana incele.

Şunları aramaya çalış:

1. Fonksiyonun başındaki `PUSH RBP` ve `MOV RBP, RSP` kalıbı var mı?
2. `SUB RSP, ...` ile alan ayrılıyor mu?
3. Local değişkenler `[RBP - ...]` şeklinde görünüyor mu?
4. `sum`, `doubled` ve `result` ile assembly satırları arasında bağlantı kurabiliyor musun?
5. Fonksiyonun sonunda `POP RBP`, `LEAVE` veya `RET` görüyor musun?

## Ghidra Stack Penceresi

Ghidra'da bir fonksiyon içindeyken stack variable bilgilerini görmek için fonksiyonun değişkenlerini ve decompiler'daki local isimleri inceleyebilirsin.

Örneğin Decompiler tarafında:

```c
int local_c;
int local_8;
int local_4;
```

gibi isimler görürsen bunlar Ghidra'nın stack üzerindeki local değişkenlere verdiği otomatik isimler olabilir.

İsimlerin güzel olmaması normaldir.

Reverse engineering sırasında çoğu zaman anlamı sen çıkarırsın ve değişkeni yeniden adlandırırsın.

## Önemli Not: Her Binary Aynı Görünmez

Compiler optimizasyonu açık olduğunda bazı local değişkenler stack'e hiç yazılmayabilir.

Örneğin compiler:

```c
int sum = a + b;
```

değerini doğrudan bir register içinde tutabilir.

Bu yüzden Day 5 için:

```bash
-O0 -fno-omit-frame-pointer
```

kullanmak yapıyı daha rahat gözlemlememizi sağlar.

Bu da önemli bir reverse engineering dersidir:

> Kaynak kodda bir değişkenin olması, binary'de mutlaka ayrı bir stack değişkeni olarak bulunacağı anlamına gelmez.

## Mini Alıştırma

Ghidra'da `calculate` fonksiyonunu aç ve cevapları kendin bulmaya çalış:

1. `a` ve `b` ilk geldiğinde hangi register'larda?
2. İlk local değişken için hangi stack offset'i kullanılıyor?
3. Kaç farklı `[RBP - ...]` erişimi görüyorsun?
4. `sum * 2` işlemi gerçekten `MUL` ile mi yapılmış, yoksa compiler başka bir instruction mı kullanmış?
5. Dönüş değerinin `EAX` içine geldiği noktayı bulabiliyor musun?

Özellikle 4. soruda source kodla assembly'nin birebir aynı görünmeyebileceğini hatırla.

## Ana Çıkarım

Day 5'in ana fikri:

```text
fonksiyon çağrılır
      ↓
stack frame hazırlanır
      ↓
RBP referans noktası olur
      ↓
local değişkenler stack alanında tutulabilir
      ↓
RSP stack'in güncel tepesini takip eder
      ↓
fonksiyon işi bitirir
      ↓
stack frame temizlenir
      ↓
RET
```

Artık `[RBP - 0x4]` gibi bir ifade gördüğümüzde bunu rastgele bir adres olarak değil, mevcut fonksiyonun stack frame'i içindeki olası bir local değişken olarak okumaya başlayabiliriz.
