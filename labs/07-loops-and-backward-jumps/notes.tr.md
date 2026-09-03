# Gün 7 — Döngüler ve Geriye Jump'lar

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Bu labın amacı basit bir C döngüsünün derlenmiş binary içinde nasıl göründüğünü ve tekrar eden çalışmanın karşılaştırmalar ile jump instruction'ları kullanılarak nasıl oluşturulduğunu anlamaktır.

Bu labda şu bağlantıları kuracağız:

- `for` döngüsü
- döngü sayacı
- `CMP`
- conditional jump'lar
- geriye yapılan jump'lar
- tekrar eden control flow

Amaç bütün jump instruction'larını ezberlemek değil. Assembly ve Ghidra içinde bir döngünün temel şeklini tanımaya başlamak.

## Kaynak Kod

```c
#include <stdio.h>

int main() {
    for (int i = 0; i < 5; i++) {
        printf("Iteration: %d\n", i);
    }

    return 0;
}
```

Yapıyı daha rahat incelemek için optimizasyon kapalı şekilde derleyebiliriz:

```bash
gcc -g -O0 loops.c -o loops
```

Binary'yi Ghidra'ya import edip `main` fonksiyonunu inceleyebilirsin.

## C Seviyesinde Ne Oluyor?

Bu döngüyü dört basit adıma ayırabiliriz:

```text
1. i = 0
2. kontrol et: i < 5 mi?
3. döngü gövdesini çalıştır
4. i++ yap ve tekrar kontrole dön
```

Koşul yanlış olana kadar bu işlem tekrar eder.

## Assembly'de Döngü Nasıl Görünebilir?

Tam çıktı compiler ve ayarlara göre değişebilir ama basit bir yapı şuna benzeyebilir:

```asm
MOV  dword ptr [RBP - 0x4], 0

loop_check:
CMP  dword ptr [RBP - 0x4], 4
JG   loop_end

; döngü gövdesi
CALL printf

ADD  dword ptr [RBP - 0x4], 1
JMP  loop_check

loop_end:
```

Burada önemli olan birebir instruction isimlerini ezberlemek değil, control flow mantığını görmek.

## `CMP`

`CMP` iki değeri karşılaştırır.

Örneğin:

```asm
CMP dword ptr [RBP - 0x4], 4
```

bu satır döngü sayacının sınıra gelip gelmediğini kontrol etmenin bir parçası olabilir.

`CMP`, C'deki gibi normal bir `true` veya `false` değeri üretmez. CPU flag'lerini günceller ve sonraki jump instruction'ı bu flag'lere bakar.

## Conditional Jump

Karşılaştırmadan sonra program şöyle bir conditional jump kullanabilir:

```asm
JG loop_end
```

Koşul sağlanırsa execution `loop_end` adresine gider.

Compiler, C'deki koşulu farklı şekilde düzenleyebileceği için her zaman aynı jump instruction'ını görmeyebilirsin.

## Geriye Jump

Döngüyü fark etmenin en kolay işaretlerinden biri daha önceki bir instruction'a geri dönen jump'tır:

```asm
JMP loop_check
```

Mantık olarak:

```text
koşulu kontrol et
    ↓
gövdeyi çalıştır
    ↓
sayacı artır
    ↓
geriye jump yap
    └──────────→ koşulu tekrar kontrol et
```

Control flow içinde böyle geriye giden bir bağlantı görmek, bir loop olabileceğine dair güçlü bir ipucudur.

## Sayacın Artırılması

C kodundaki:

```c
i++;
```

binary içinde örneğin şöyle görünebilir:

```asm
ADD dword ptr [RBP - 0x4], 1
```

Ama compiler bunu farklı, eşdeğer instruction'larla da oluşturabilir.

Bu yüzden kaynak kod syntax'ının birebir korunmasını beklemiyoruz.

## Ghidra'da Neye Bakmalıyım?

Basit bir loop ararken şu paterni takip et:

```text
bir değer başlatılıyor
    ↓
karşılaştırılıyor
    ↓
loop'tan çıkmak için conditional jump
    ↓
loop gövdesi
    ↓
sayaç veya state değiştiriliyor
    ↓
karşılaştırmaya geri jump
```

Ghidra'nın graph view'unda daha önceki bir basic block'a dönen ok da görebilirsin.

## `for` ve `while`

C seviyesinde farklı görünürler:

```c
for (int i = 0; i < 5; i++) {
    // ...
}
```

```c
int i = 0;
while (i < 5) {
    // ...
    i++;
}
```

Ama derlendiklerinde oldukça benzer control-flow yapılarına dönüşebilirler.

Reverse engineering için önemli ders:

> Farklı kaynak kod yapıları, derlendikten sonra birbirine çok benzeyen machine code üretebilir.

## Ana Çıkarım

Binary içinde `for` veya `while` diye bir kelime bulunmaz.

Döngüyü genellikle şu paternden yeniden çıkarırsın:

```text
başlangıç değeri
    ↓
karşılaştırma
    ↓
conditional jump
    ↓
gövde
    ↓
durum/sayaç güncelleme
    ↓
geriye jump
```

Şimdilik bu paterni tanıyabilmek, jump instruction'larını ezberlemekten daha önemlidir.
