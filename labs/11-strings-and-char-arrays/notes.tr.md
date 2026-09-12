# Gün 11 — String'ler ve Char Dizileri

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Basit C string'lerinin `char` dizileri olarak nasıl tutulduğunu ve derlenmiş binary içinde hangi izleri bıraktığını anlamak.

## Kaynak kod

```c
#include <stdio.h>
#include <string.h>

int main() {
    char name[] = "Rafale";
    char code[] = "RE_LAB_11";

    printf("Name: %s\n", name);
    printf("Code: %s\n", code);
    printf("Name length: %zu\n", strlen(name));

    return 0;
}
```

## C string'i nedir?

C'de bir string genellikle sonunda null byte bulunan bir `char` dizisidir:

```text
R  a  f  a  l  e  \0
```

Sondaki `\0`, string'in bittiğini belirtir.

Yani:

```c
char name[] = "Rafale";
```

makine seviyesinde tek parça soyut bir "string nesnesi" değildir. Bellekte art arda duran karakter byte'larıdır.

## Reverse engineering açısından neden önemli?

Okunabilir string'ler binary içinde fark edilmesi en kolay ipuçlarından biridir. Şunları gösterebilir:

- program mesajları
- dosya yolları
- hata metinleri
- menü yazıları
- protokol adları
- yapılandırma değerleri

Tek başına bir string programın ne yaptığını kanıtlamaz; fakat o string'e referans veren kod bizi önemli fonksiyonlara götürebilir.

## Ghidra'da neye bakacağız?

Debug bilgisiyle derle:

```bash
gcc -g strings.c -o strings
```

Binary'yi Ghidra'da açıp `main` fonksiyonunu incele.

Şuna benzer pattern'lar görebilirsin:

```text
LEA  register, [adres]
MOV  argument_register, register
CALL printf
```

Ayrıca:

```text
CALL strlen
```

gibi bir çağrı da görebilirsin.

Compiler tam instruction dizisini değiştirebilir. Bu yüzden her satırın birebir aynı olmasını beklemek yerine genel pattern'i tanımaya çalış.

## Strings penceresi

Ghidra binary içinde bulunan string'leri listeleyebilir. Genellikle şu bölüm kullanışlıdır:

```text
Window -> Defined Strings
```

Ghidra sürümüne ve pencere düzenine göre string arama araçları da kullanılabilir.

`RE_LAB_11` string'ini bulduğunda XREF'lerini takip ederek nerede kullanıldığını görebilirsin.

## XREF hatırlatma

`XREF`, cross-reference anlamına gelir.

Bir string'in XREF'i `main` fonksiyonunu gösteriyorsa, `main` içindeki kod o string'e referans veriyor demektir.

Bu da çok kullanışlı bir RE akışı oluşturur:

```text
ilginç string
    ↓
XREF bul
    ↓
referans veren fonksiyonu aç
    ↓
çevresindeki instruction'ları incele
```

## `strlen()`

`strlen(name)`, null terminator'a gelene kadar karakterleri sayar.

```text
Rafale\0
```

için sonuç `6` olur. Sondaki `\0` uzunluğa dahil edilmez.

Binary içinde dış fonksiyon çağrısı şu şekilde görülebilir:

```text
CALL strlen
```

Bu da yakınındaki verinin metin olarak kullanıldığına dair bir ipucudur.

## Reverse engineering bağlantısı

Kaynak kod seviyesinde:

```c
char name[] = "Rafale";
printf("%s", name);
```

Binary seviyesinde ise şöyle düşün:

```text
karakterleri içeren byte'lar
          ↓
bu byte'ların adresi
          ↓
adres bir fonksiyona argüman olarak verilir
          ↓
printf / puts / strlen / başka bir fonksiyon
```

Ana fikir: string'ler byte'lara dönüşür ve program bu byte'ların adresleriyle çalışır.

## Ana çıkarım

```text
C string       -> 00 ile biten char byte'ları
string address -> ilk karakterin adresi
XREF           -> string'e referans veren kod
CALL strlen    -> verinin text olarak kullanıldığına dair ipucu
```

Şimdilik instruction ezberlemeye çalışma. Amaç; string, bellek adresi, XREF ve fonksiyon çağrısı arasındaki bağlantıyı tanımaya başlamak.
