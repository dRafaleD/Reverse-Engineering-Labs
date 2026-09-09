# Gün 9 — Struct'lar ve Bellek Düzeni

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Basit bir C `struct` yapısının birden fazla değeri nasıl tek bir yapı altında topladığını ve bu alanların Ghidra'da bellekte sabit offset'ler üzerinden nasıl görünebildiğini anlamak.

Amaç derleyici çıktısını ezberlemek değil. Reverse engineering sırasında şu temel pattern'i tanımaya başlamak:

```text
base address + field offset
```

## Kaynak Kod

```c
#include <stdio.h>

struct Player {
    int health;
    int ammo;
    char rank;
};

int main() {
    struct Player player = {100, 30, 'A'};

    printf("Health: %d\n", player.health);
    printf("Ammo: %d\n", player.ammo);
    printf("Rank: %c\n", player.rank);

    return 0;
}
```

İlk inceleme daha temiz olsun diye optimizasyon kapalı derleyebilirsin:

```bash
gcc -g -O0 structs.c -o structs
```

Sonra binary'yi Ghidra'ya import edip `main` fonksiyonuna bak.

## Struct Nedir?

`struct`, birbiriyle ilişkili değerleri tek bir veri tipi altında toplar.

```c
struct Player {
    int health;
    int ammo;
    char rank;
};
```

Bir `Player` nesnesinin içinde kabaca şu alanlar vardır:

```text
health
ammo
rank
```

Yani bunlar üç ayrı bağımsız değişken gibi değil, aynı yapının parçalarıdır.

## Bellek Düzeni Mantığı

Struct içindeki alanlar, struct'ın başlangıç adresine göre farklı offset'lerde tutulur.

Basitleştirilmiş bir model:

```text
Player başlangıcı
+0x00 -> health
+0x04 -> ammo
+0x08 -> rank
```

Toplam struct boyutunda padding olabilir. Bu yüzden alan boyutlarını sadece toplayıp kesin struct boyutu diye düşünmemek gerekir.

## Padding Nedir?

Compiler, alignment gereksinimleri nedeniyle alanların arasına veya struct'ın sonuna kullanılmayan byte'lar ekleyebilir.

Bu örnekte:

```text
int  -> genellikle 4 byte
int  -> genellikle 4 byte
char -> 1 byte
```

olmasına rağmen `sizeof(struct Player)` 9'dan büyük çıkabilir.

Reverse engineering açısından bu önemli çünkü bir alan sabit bir offset'te görünürken sonraki nesnenin başlangıcı beklediğinden daha ileride olabilir.

## Ghidra'da Neye Bakacağız?

Şuna benzer erişimler görebilirsin:

```asm
MOV dword ptr [RBP - 0x10], 0x64
MOV dword ptr [RBP - 0x0c], 0x1e
MOV byte ptr  [RBP - 0x08], 0x41
```

Offset'ler sende farklı çıkabilir. Buradaki önemli şey birkaç değerin birbirine yakın ve düzenli adreslerde tutulmasıdır.

Bu örnekteki değerler:

```text
0x64 = 100
0x1e = 30
0x41 = 'A'
```

## Field Erişimi Pattern'i

Bir struct'ın adresi bir register'da tutuluyorsa bir alan erişimi kabaca şöyle görünebilir:

```asm
MOV EAX, dword ptr [RDI + 0x4]
```

RE açısından bunu şöyle yorumlayabilirsin:

```text
RDI        -> bir nesnenin adresi
[RDI+0x4] -> o nesnenin içindeki bir alan
```

Şu tarz tekrar eden erişimler:

```text
[base + 0x0]
[base + 0x4]
[base + 0x8]
```

programın bir struct veya gruplanmış veri yapısıyla çalıştığına dair ipucu olabilir.

## Ghidra ve Struct Yeniden Oluşturma

Debug bilgisi varsa Ghidra bazı isimleri ve tipleri doğrudan gösterebilir.

Ama stripped veya bilinmeyen bir binary'de orijinal field isimleri genellikle kaybolur. Reverse engineer önce sadece offset'leri görür, sonra bunların aynı nesneye ait alanlar olduğunu davranıştan çıkarabilir.

Örneğin:

```text
object + 0x00 -> muhtemelen health
object + 0x04 -> muhtemelen ammo
object + 0x08 -> muhtemelen rank/state
```

Bu isimler program davranışıyla desteklenene kadar sadece hipotezdir.

## Ana Çıkarım

```text
struct
  ↓
birbiriyle ilişkili alanlar
  ↓
alanların sabit offset'lerde tutulması
  ↓
base address + offset
  ↓
Ghidra'da tanınabilir bellek erişim pattern'leri
```

Gün 9 için akılda kalması gereken ana fikir şu: aynı base address üzerinden birbirine yakın offset'lere tekrar tekrar erişiliyorsa, bunlar aynı veri yapısının field'ları olabilir.
