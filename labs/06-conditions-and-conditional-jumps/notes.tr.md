# Gün 6 — Koşullar ve Conditional Jump'lar

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Bu labın amacı basit bir C `if/else` yapısının derlendikten sonra binary içinde nasıl göründüğünü ve Ghidra'nın karar verme mantığını nasıl gösterdiğini anlamaktır.

Bu labda şu bağlantıları kuracağız:

- `if` / `else`
- karşılaştırmalar
- CPU flag'leri
- `CMP`
- conditional jump instruction'ları
- Ghidra'da control flow

Amaç bütün jump komutlarını ezberlemek değil. Yüksek seviyeli bir koşulun, binary tarafında bir karşılaştırma ve ardından bir dallanma haline geldiğini görmek.

## Kaynak Kod

```c
#include <stdio.h>

int main() {
    int score = 72;

    if (score >= 50) {
        printf("Passed\n");
    } else {
        printf("Failed\n");
    }

    return 0;
}
```

Kontrol akışını daha rahat görebilmek için debug bilgisiyle ve optimizasyon kapalı şekilde derleyebiliriz:

```bash
gcc -g -O0 conditions.c -o conditions
```

Sonra oluşan ELF binary'sini Ghidra'ya import edip `main` fonksiyonunu inceleyebiliriz.

## C Tarafında Ne Oluyor?

Mantık çok basit:

```text
score = 72
    ↓
score >= 50 mi?
    ↓
evet -> "Passed" yazdır
hayır -> "Failed" yazdır
```

CPU, C dilindeki `if` kelimesini doğrudan anlamaz. Compiler bu koşulu değerleri karşılaştıran ve hangi adresteki kodun çalışacağına karar veren instruction'lara dönüştürür.

## `CMP` Nedir?

Karşılaştırmalarda sık göreceğimiz instruction'lardan biri:

```asm
CMP destination, source
```

`CMP`, içeride çıkarma benzeri bir işlem yapar fakat sonucu normal bir değişkende saklamaz.

Bunun yerine CPU'nun durum flag'lerini günceller.

Ghidra'da örneğin buna benzer bir şey görebiliriz:

```asm
CMP dword ptr [RBP - 0x4], 0x31
```

`0x31`, decimal olarak `49`'dur.

Compiler branch'i nasıl kurduğuna bağlı olarak C'deki `score >= 50` koşulunu, değer `49` veya daha küçükse başarısızlık yoluna atlayacak şekilde kurabilir.

Compiler sürümü ve ayarlarına göre exact assembly değişebilir. Bu yüzden tek bir instruction dizisini ezberlemek yerine mantığa odaklanmak daha önemlidir.

## Conditional Jump Nedir?

Bir karşılaştırmadan sonra CPU, `CMP` tarafından oluşturulan flag'lere göre başka bir adrese atlayabilir.

Sık görülen bazı instruction'lar:

```text
JE   -> eşitse atla
JNE  -> eşit değilse atla
JG   -> büyükse atla (signed)
JGE  -> büyük veya eşitse atla (signed)
JL   -> küçükse atla (signed)
JLE  -> küçük veya eşitse atla (signed)
JA   -> büyükse atla (unsigned)
JB   -> küçükse atla (unsigned)
```

Şimdilik bunların hepsini ezberlemene gerek yok.

Önemli pattern şu:

```text
CMP
 ↓
conditional jump
 ↓
ikiden bir kod yolunu seç
```

## Signed ve Unsigned Jump'lar Neden Ayrı?

C'de integer değerler signed veya unsigned olabilir:

```c
int x;
unsigned int y;
```

Bu değerlerin karşılaştırma sırasında farklı yorumlanması gerekebilir. Bu yüzden x86'da signed karşılaştırmalar için `JG/JL`, unsigned karşılaştırmalar için `JA/JB` gibi ayrı instruction'lar vardır.

Buna daha sonra tekrar döneceğiz. Bu labdaki `score`, normal bir signed `int`.

## Control Flow

Bir `if/else`, programda birden fazla olası çalışma yolu oluşturur.

Basitleştirilmiş bir assembly görünümü şöyle olabilir:

```asm
CMP score, 50
JL  failed

; Passed yolu
CALL puts
JMP end_if

failed:
; Failed yolu
CALL puts

end_if:
MOV EAX, 0
RET
```

Bu birebir compiler çıktısı olmak zorunda değil; sadece mantığı gösteriyor.

`JMP` koşulsuz atlamadır.

Yani kabaca:

> Herhangi bir koşul kontrol etmeden başka bir adresteki instruction'dan devam et.

`if` bloğu çalıştıktan sonra `else` bloğunu atlamak için sıkça kullanılabilir.

## Ghidra'daki Label'lar

Ghidra branch hedeflerine şu tarz isimler verebilir:

```text
LAB_00101180
LAB_00101190
```

Bunlar C kaynak kodunda yazdığımız değişken isimleri değildir. Ghidra'nın jump hedeflerini ve önemli kod konumlarını takip edebilmek için oluşturduğu label'lardır.

Örneğin:

```asm
JLE LAB_00101190
```

kabaca şöyle okunabilir:

> Koşul doğruysa `LAB_00101190` adresindeki koddan devam et.

## Graph View

Koşulları Ghidra'nın graph görünümünde okumak genellikle daha kolaydır çünkü farklı branch'ler ayrı bloklar halinde görünür.

Sadece yukarıdan aşağıya kod okumak yerine şöyle düşün:

```text
        karşılaştırma
        /          \
      doğru       yanlış
       |             |
    Passed         Failed
       \             /
           devam
```

Buna **control flow** denir: instruction'ların hangi sırayla ve hangi olası yollar üzerinden çalışabileceği.

## Tekrar `CALL puts`

Önceki lablarda olduğu gibi:

```c
printf("Passed\n");
```

binary'de şuna dönüşebilir:

```asm
CALL puts
```

Ana ders yine aynı: kaynak kod ile compiled output mantıksal olarak bağlantılıdır fakat birebir satır satır aynı olmak zorunda değildir.

## Basit Bir Reverse Engineering Yaklaşımı

İçinde branch bulunan tanımadığın bir fonksiyona bakarken şu sırayla ilerleyebilirsin:

1. `CMP` veya `TEST` gibi karşılaştırma instruction'larını bul.
2. Yakınındaki conditional jump'a bak.
3. Jump'ın gidebileceği iki yolu da takip et.
4. Her branch içinde anlamlı string, function call veya return value ara.
5. Yüksek seviyeli karar mantığını yeniden kurmaya çalış.

Örneğin:

```text
score karşılaştırılıyor
    ↓
branch A -> "Passed"
branch B -> "Failed"
```

Kaynak kod elimizde olmasa bile programın `score` değerine göre karar verdiğini buradan çıkarabiliriz.

## Günün Ana Fikri

Gün 6'da reverse engineering için çok önemli bir pattern'e giriş yaptık:

```text
C koşulu
    ↓
karşılaştırma instruction'ı
    ↓
CPU flag'leri
    ↓
conditional jump
    ↓
farklı çalışma yolları
```

Şimdilik bütün flag'leri ve jump instruction'larını ezberlemene gerek yok.

Sadece şunları hatırla:

- `CMP` değerleri karşılaştırmak için kullanılır.
- conditional jump'lar hangi kod yoluna gidileceğini seçer.
- `JMP` koşulsuz olarak başka bir yere atlar.
- Ghidra label'ları branch hedeflerini takip etmeyi kolaylaştırır.
- C'deki `if/else`, binary'de control flow haline gelir.
