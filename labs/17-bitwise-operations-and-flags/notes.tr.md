# Gün 17 — Bitwise İşlemler ve Flag'ler

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Bitwise işlemlerin tek tek bitleri kontrol etmek ve değiştirmek için nasıl kullanıldığını, bu yapıların derlenmiş binary içinde nasıl görünebileceğini anlamak.

## Kaynak kod

```c
#include <stdio.h>

int main(void) {
    unsigned int permissions = 0x5; // binary: 0101
    unsigned int write_flag = 0x2;  // binary: 0010

    if ((permissions & write_flag) != 0) {
        printf("Write permission is enabled\n");
    } else {
        printf("Write permission is disabled\n");
    }

    permissions |= write_flag;

    printf("Updated permissions: 0x%X\n", permissions);

    return 0;
}
```

## Bitwise işlemler neden önemli?

Programlar birden fazla boolean değeri tek bir integer içinde saklayabilir.

Örneğin:

```text
bit 0 -> read
bit 1 -> write
bit 2 -> execute
```

Şu değer:

```text
0101
```

bit 0 ve bit 2'nin açık olduğunu gösterir.

## Bitwise AND

Şu ifade:

```c
permissions & write_flag
```

write bitinin açık olup olmadığını test eder.

Örnek:

```text
permissions = 0101
write_flag  = 0010
-------------------
AND         = 0000
```

Sonuç sıfır olduğu için o bit açık değildir.

Assembly tarafında compiler şunlara benzer instruction'lar üretebilir:

```text
AND
TEST
CMP
JE / JNE
```

Çok yaygın bir pattern:

```text
TEST register, mask
JE   not_enabled
```

## Bitwise OR

Şu satır:

```c
permissions |= write_flag;
```

diğer bitleri bozmadan ilgili biti açar.

```text
0101
0010
----
0111
```

Assembly'de şu şekilde görülebilir:

```text
OR register, immediate
```

veya veri bellekteyse load-modify-store şeklinde olabilir.

## Mask nedir?

Belirli bitleri seçmek için kullanılan değere genellikle **mask** denir.

Örnekler:

```text
0x1 -> 0001
0x2 -> 0010
0x4 -> 0100
0x8 -> 1000
```

Reverse engineering sırasında `1`, `2`, `4`, `8`, `0x10` gibi değerlerin tekrar tekrar kullanılması, kodun flag'lerle çalıştığına dair ipucu olabilir.

## Ghidra'da neye bakacağız?

Derle:

```bash
gcc -g bitwise.c -o bitwise
```

Binary'yi Ghidra'da aç ve `main` fonksiyonunu incele.

Şunları ara:

- `0x5` ve `0x2` gibi sabit değerler
- `AND`, `OR` veya `TEST`
- flag kontrolünden sonraki conditional jump
- enabled / disabled mesajlarını basan branch'ler

Compiler her zaman birebir aynı instruction dizisini üretmeyebilir.

## Reverse engineering bağlantısı

Kaynak kodda:

```c
if ((permissions & write_flag) != 0)
```

Makine seviyesinde şöyle düşün:

```text
değer
  ↓
bit mask uygula
  ↓
sonuç sıfır mı kontrol et
  ↓
conditional jump
```

Bu pattern şuralarda sık görülür:

- permission alanları
- status flag'leri
- feature flag'ler
- dosya formatı flag'leri
- protokol alanları
- configuration seçenekleri

## Ana çıkarım

```text
AND  -> seçilen bitleri test et / koru
OR   -> seçilen bitleri aç
mask -> hangi bitlerin önemli olduğunu belirleyen değer
TEST -> flag kontrolünde sık görülen instruction
```

Şimdilik bütün hexadecimal mask'leri ezberlemeye çalışma. Amaç **mask -> test -> branch** pattern'ini tanımaya başlamak.
