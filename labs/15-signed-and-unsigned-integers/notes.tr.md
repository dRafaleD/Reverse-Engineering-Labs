# Gün 15 — Signed ve Unsigned Integer'lar

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Aynı bit dizisinin signedness'e göre neden farklı anlamlara gelebileceğini ve bu farkın derlenmiş binary içinde signed ile unsigned karşılaştırmalar olarak nasıl göründüğünü öğrenmek.

Gün 6, conditional jump'ları tanıtmış ve `JG/JL` ile `JA/JB` instruction'larının bir nedenle ayrı olduğunu söylemişti. Bu lab, o fikre küçük bir örnekle geri dönüyor.

## Kaynak Kod

```c
#include <stdio.h>

int main(void) {
    int signed_value = -1;
    unsigned int unsigned_value = 4294967295U;

    if (signed_value < 100) {
        printf("signed is less than 100\n");
    } else {
        printf("signed is not less than 100\n");
    }

    if (unsigned_value < 100) {
        printf("unsigned is less than 100\n");
    } else {
        printf("unsigned is not less than 100\n");
    }

    printf("%d\n", signed_value);
    printf("%u\n", unsigned_value);

    return 0;
}
```

İki karşılaştırmanın kolay görünmesi için önce optimizasyonsuz derle:

```bash
gcc -g -O0 signed_unsigned.c -o signed_unsigned
```

Sonra binary'yi Ghidra'ya alıp `main` fonksiyonunu incele.

## Aynı Bitler, Farklı Anlam

Tipik 32-bit `int` üzerinde her iki değişken de şu bit dizisini tutabilir:

```text
11111111 11111111 11111111 11111111
```

Bu pattern hexadecimal olarak `0xFFFFFFFF`'tir.

Signed `int` olarak genellikle `-1` anlamına gelir.

`unsigned int` olarak ise `4294967295`'tir.

Yani bellek içeriği aynı görünebilir, program mantığı farklıdır:

```text
signed_value    -> -1
unsigned_value  -> 4294967295
```

Bu yüzden reverse engineering yalnızca byte okumak değildir. Bu byte'ların nasıl yorumlandığını da sormak gerekir.

## Karşılaştırmalar Neden Ayrılır?

Kaynak kod seviyesinde iki `if` neredeyse aynı durur:

```c
if (signed_value < 100)
if (unsigned_value < 100)
```

Sonuçları aynı değildir:

```text
-1 < 100                 -> true
4294967295 < 100         -> false
```

Compiler bu farkı kodlamak zorundadır. Bir karşılaştırma değeri signed kabul eder. Diğeri unsigned kabul eder.

## Signed Jump'lar ve Unsigned Jump'lar

Gün 6 bu ayrımı göstermişti:

```text
JG / JL  -> signed karşılaştırmalar
JA / JB  -> unsigned karşılaştırmalar
```

Başlangıç için faydalı bir eşleme:

```text
JL  -> jump if less          (signed)
JG  -> jump if greater       (signed)
JB  -> jump if below         (unsigned)
JA  -> jump if above         (unsigned)
```

*less* ve *greater* signed düşünceye aittir. *below* ve *above* unsigned düşünceye aittir.

Şuna benzer bir pattern görebilirsin:

```text
CMP  signed_value, 100
JL   ...
```

ve daha sonra:

```text
CMP  unsigned_value, 100
JB   ...
```

Her sistemde aynı register veya label'ı bekleme. Önemli ipucu, kullanılan jump ailesidir.

## Ghidra'da Nelere Bakmalı

Derledikten sonra `main` içinde şunları bulmaya çalış:

1. `0xFFFFFFFF` veya `-1` sabiti
2. `100` / `0x64` ile yapılan iki karşılaştırma
3. Bir dalın signed jump, diğerinin unsigned jump kullanıp kullanmadığı
4. Ghidra decompiler'ın iki local değişkeni nasıl tiplendirdiği
5. `%d` ve `%u` format string'leri

Ghidra şuna yakın bir şey zaten üretebilir:

```c
int signed_value;
unsigned int unsigned_value;
```

Binary stripped olsa ve tipler daha az açık olsa bile jump ailesi hâlâ işe yarar bir ipucudur:

```text
JL / JG  -> bu karşılaştırma muhtemelen signed
JB / JA  -> bu karşılaştırma muhtemelen unsigned
```

## Sign Extension ve Zero Extension

Küçük bir değer daha büyük bir register'a kopyalanırken compiler ekstra bitleri doldurmak zorunda kalabilir.

Sadeleştirilmiş fikir:

```text
MOVSX  -> kopyala ve işareti koru     (signed)
MOVZX  -> kopyala ve sıfırla doldur   (unsigned)
```

Şimdilik her extension instruction'ını ezberlemen gerekmiyor. Extra bitler sign bit'ten dolduruluyorsa değer signed kabul ediliyordur. Extra bitler sıfır oluyorsa unsigned kabul ediliyordur.

## Reverse Engineering Bağlantısı

Faydalı bir analiz sorusu:

```text
Bir karşılaştırma görüyorum.
Bu değer signed mı unsigned mı yorumlanıyor?
```

Bu soru, yeniden kurulan mantığı tamamen değiştirebilir. Aynı `0xFFFFFFFF` şu anlama gelebilir:

```text
"bu -1"
```

veya:

```text
"bu çok büyük bir sayı"
```

hangisinin doğru olduğu, ardından gelen karşılaştırmaya bağlıdır.

## Ana Çıkarım

```text
aynı bitler          -> her zaman aynı anlam değildir
signed int           -> negatif olabilir
unsigned int         -> yalnız sıfır ve üzeri
JL / JG              -> signed karşılaştırma
JB / JA              -> unsigned karşılaştırma
RE görevi            -> değerin nasıl yorumlandığını geri kazanmak
```

Amaç henüz bütün integer dönüşüm kurallarını ezberlemek değil. Amaç signedness'in program mantığının bir parçası olduğunu ve Ghidra'daki jump'lar ile yeniden kurulan tiplerin bunu yakalamak için ipucu olduğunu görmektir.
