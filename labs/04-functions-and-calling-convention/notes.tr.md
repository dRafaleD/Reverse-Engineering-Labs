# Gün 4 — C Fonksiyonları ve Calling Convention

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Bu labın amacı C dilinde yazılmış basit bir fonksiyonun derlendikten sonra Ghidra içinde nasıl göründüğünü anlamaktır.

Özellikle şu bağlantıları kurmaya çalışıyoruz:

- C fonksiyon parametreleri
- x86-64 register'ları
- `CALL` instruction'ı
- fonksiyon dönüş değeri
- stack frame
- Linux x86-64 System V calling convention

Buradaki amaç calling convention'ı ezberlemek değil. Amaç, bir fonksiyon çağrısının binary içinde nasıl iz bıraktığını görmeye başlamaktır.

## Kaynak Kod

Bu lab için basit bir toplama fonksiyonu kullanıyoruz:

```c
#include <stdio.h>

int add(int a, int b) {
    int result = a + b;
    return result;
}

int main() {
    int x = 10;
    int y = 20;

    int total = add(x, y);

    printf("Result: %d\n", total);

    return 0;
}
```

Debug bilgisiyle derlemek için:

```bash
gcc -g functions.c -o functions
```

Daha sonra `functions` binary'sini Ghidra'ya import edip önce `main`, ardından `add` fonksiyonunu inceliyoruz.

## Önce C Tarafında Ne Oluyor?

Kaynak kodda şu satır var:

```c
int total = add(x, y);
```

İnsan gözüyle çok basit:

```text
x = 10
y = 20

add(10, 20)
    ↓
30 döndürür
    ↓
total = 30
```

Ama CPU "fonksiyona iki sayı gönder" gibi yüksek seviyeli kavramları bilmez.

Derleyici, bu işlemi register'lar ve instruction'lar kullanarak gerçekleştirir.

## Calling Convention Nedir?

Calling convention, fonksiyonların birbirleriyle nasıl iletişim kuracağını belirleyen kurallar bütünüdür.

Örneğin:

- Argümanlar hangi register'lara konur?
- Dönüş değeri hangi register'da gelir?
- Stack nasıl kullanılır?
- Hangi register'ları kim korur?

Bu lab Linux x86-64 üzerinde System V ABI mantığını temel alır.

Integer/pointer türündeki ilk argümanlar genel olarak şu sırayla gönderilir:

```text
1. argüman -> RDI
2. argüman -> RSI
3. argüman -> RDX
4. argüman -> RCX
5. argüman -> R8
6. argüman -> R9
```

32-bit `int` değerlerinde bunların alt 32-bit parçaları kullanılabilir:

```text
RDI -> EDI
RSI -> ESI
```

Şimdilik ilk iki tanesi bizim için yeterli.

## Ghidra'da `main`

Derleyici ve GCC sürümüne göre instruction'lar biraz değişebilir, ancak mantık yaklaşık olarak şöyledir:

```asm
MOV dword ptr [RBP - 0x4], 0xa
MOV dword ptr [RBP - 0x8], 0x14

MOV EDX, dword ptr [RBP - 0x8]
MOV EAX, dword ptr [RBP - 0x4]

MOV ESI, EDX
MOV EDI, EAX

CALL add

MOV dword ptr [RBP - 0xc], EAX
```

Şimdi bunu parçalayalım.

### `0xa` ve `0x14`

Hexadecimal değerler:

```text
0x0a = 10
0x14 = 20
```

Bunlar kaynak koddaki:

```c
int x = 10;
int y = 20;
```

değerleridir.

## Argümanların Hazırlanması

Fonksiyon çağrısından hemen önce Ghidra'da buna benzer instruction'lar görebiliriz:

```asm
MOV ESI, EDX
MOV EDI, EAX
CALL add
```

Buradaki önemli nokta:

```text
EDI -> add fonksiyonunun ilk int argümanı
ESI -> add fonksiyonunun ikinci int argümanı
```

Kaynak kod:

```c
add(x, y);
```

Binary seviyesinde kabaca:

```text
EDI = x
ESI = y
CALL add
```

haline gelir.

## `CALL add`

```asm
CALL add
```

CPU'ya başka bir fonksiyona geçmesini söyler.

Ancak `CALL` yalnızca "oraya atla" değildir.

Aynı zamanda fonksiyon bittikten sonra nereye dönüleceğini belirleyen return address'i stack üzerinde saklar.

Basitleştirilmiş akış:

```text
main
  |
  | EDI = 10
  | ESI = 20
  |
  +---- CALL add ----+
                     |
                     v
                    add
                     |
                     | 10 + 20
                     |
                     v
                   return
                     |
                     +----> main devam eder
```

## `add` Fonksiyonunun İçinde

Ghidra'da `add` fonksiyonunun sadeleştirilmiş assembly'si buna benzeyebilir:

```asm
PUSH RBP
MOV  RBP, RSP

MOV dword ptr [RBP - 0x14], EDI
MOV dword ptr [RBP - 0x18], ESI

MOV EDX, dword ptr [RBP - 0x14]
MOV EAX, dword ptr [RBP - 0x18]

ADD EAX, EDX

MOV dword ptr [RBP - 0x4], EAX
MOV EAX, dword ptr [RBP - 0x4]

POP RBP
RET
```

Tam instruction dizilimi compiler sürümüne ve optimization ayarlarına göre değişebilir.

Önemli olan kalıbı anlamaktır.

## `ADD`

```asm
ADD EAX, EDX
```

İki değeri toplar.

Basitleştirilmiş olarak:

```text
EAX = EAX + EDX
```

Bu, kaynak koddaki:

```c
int result = a + b;
```

işleminin assembly seviyesindeki izlerinden biridir.

## Fonksiyon Dönüş Değeri

Linux x86-64 System V calling convention'da integer dönüş değerleri genellikle `EAX` / `RAX` üzerinden döndürülür.

Kaynak kod:

```c
return result;
```

Assembly tarafında fonksiyon bitmeden önce sonuç `EAX` içine yerleştirilmiş olabilir.

Sonra:

```asm
RET
```

ile `main` fonksiyonuna dönülür.

Bu yüzden `CALL add` sonrasında:

```asm
MOV dword ptr [RBP - 0xc], EAX
```

gibi bir satır görebiliriz.

Buradaki `EAX`, `add()` fonksiyonunun döndürdüğü değeri taşır.

Yani:

```text
add(10, 20)
    ↓
EAX = 30
    ↓
total = EAX
```

## Stack Frame'i Neden Tekrar Görüyoruz?

Fonksiyon başında:

```asm
PUSH RBP
MOV RBP, RSP
```

ve sonunda:

```asm
POP RBP
RET
```

gibi instruction'lar görebiliriz.

Bunlar fonksiyonun stack frame'inin hazırlanması ve temizlenmesiyle ilişkilidir.

Day 2'de gördüğümüz bu yapı artık biraz daha anlamlı hale geliyor:

```text
fonksiyon başlar
    ↓
kendi local değişkenleri için bir çalışma alanı oluşturur
    ↓
işini yapar
    ↓
önceki durumu geri yükler
    ↓
çağıran fonksiyona döner
```

## Ghidra Decompiler'da Ne Görmeliyiz?

Decompiler panelinde `add` fonksiyonu kaynak koda benzer şekilde görünebilir:

```c
int add(int param_1, int param_2)
{
    int local_var;

    local_var = param_1 + param_2;
    return local_var;
}
```

Eğer debug sembolleri korunuyorsa isimler daha anlamlı olabilir.

Ancak şunu unutmamak gerekir:

> Ghidra orijinal C kaynak kodunu geri getirmez.

Decompiler, machine code'dan yüksek seviyeli bir temsil üretmeye çalışır.

Bu nedenle:

```text
a        -> param_1
b        -> param_2
result   -> local_10
```

gibi farklı isimler görebiliriz.

## Listing ve Decompiler'ı Birlikte Okumak

Bu labda iki pencereyi beraber kullanmak önemlidir.

Decompiler:

```c
return param_1 + param_2;
```

Listing:

```asm
MOV ...
ADD ...
MOV ...
RET
```

Decompiler bize "program ne yapıyor?" sorusunda yardım eder.

Listing ise CPU seviyesinde "bunu nasıl yapıyor?" sorusuna yaklaşmamızı sağlar.

Reverse engineering sırasında ikisini birlikte okumak çok daha faydalıdır.

## Day 4 Mini Alıştırma

Ghidra'da `main` ve `add` fonksiyonlarını aç.

Kendin bulmaya çalış:

1. `10` ve `20` değerleri binary içinde hangi hexadecimal değerlerle görünüyor?
2. `CALL add` instruction'ından hemen önce hangi register'lar hazırlanıyor?
3. `add` fonksiyonunun dönüş değeri hangi register üzerinden geliyor?
4. `ADD` instruction'ını bulabiliyor musun?
5. Decompiler'daki kod ile Listing'deki assembly arasında hangi satırları eşleştirebiliyorsun?

Cevapları ezberlemek yerine Ghidra içinde takip etmeye çalış.

## Ana Çıkarım

Day 4'ün en önemli bağlantısı:

```text
C fonksiyon çağrısı
        ↓
argümanlar register'lara hazırlanır
        ↓
CALL
        ↓
fonksiyon kendi işlemlerini yapar
        ↓
sonuç EAX/RAX üzerinden döner
        ↓
RET
        ↓
çağıran fonksiyon devam eder
```

Bu labdan sonra `CALL`, `RDI/EDI`, `RSI/ESI`, `RAX/EAX`, `RBP` ve `RET` artık birbirinden bağımsız kelimeler değil; bir fonksiyon çağrısının parçaları olarak görülmeye başlanabilir.
