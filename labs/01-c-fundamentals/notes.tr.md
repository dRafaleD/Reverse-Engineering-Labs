[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

# Gün 1 — C Temelleri

## Amaç

Bu labın amacı temel bir C programının yapısını anlamak ve daha sonra reverse engineering çalışmalarında inceleyebileceğimiz basit bir executable üretmektir.

## Kaynak Kod

Bu labın kaynak dosyası:

`source/hello.c`

```c
#include <stdio.h>

int main() {
    printf("This is my first c code\n");
    return 0;
}
```

## Öğrendiklerim

### `#include <stdio.h>`

Standard Input/Output header dosyasını programa dahil eder. `printf()` ve `scanf()` gibi fonksiyonların bildirimlerini sağlar.

### `int main()`

`main`, C programının ana giriş fonksiyonudur. Programın çalışması buradan başlar.

Baştaki `int`, fonksiyonun işletim sistemine integer bir değer döndürdüğünü belirtir.

### `printf()`

`printf()` standard output'a biçimlendirilmiş metin yazdırır.

```c
printf("This is my first c code\n");
```

Buradaki `\n`, yazıdan sonra yeni satıra geçilmesini sağlar.

### `return 0;`

`main` fonksiyonunu bitirir ve işletim sistemine `0` değerini döndürür. `0` genellikle programın başarıyla tamamlandığı anlamına gelir.

## Derleme

Programı GCC ile derlemek için:

```bash
gcc hello.c -o hello
```

Executable'ı çalıştırmak için:

```bash
./hello
```

Beklenen çıktı:

```text
This is my first c code
```

## Reverse Engineering Bağlantısı

Bu, kaynak kodun nasıl binary executable'a dönüştüğünü anlamanın ilk adımıdır.

Temel akış:

```text
C kaynak kodu
    ↓
Compiler (GCC)
    ↓
ELF executable
    ↓
Disassembler / Decompiler (örneğin Ghidra)
```

İlerleyen lablarda değişkenler, koşullar, fonksiyonlar ve döngüler gibi C kavramlarının derlenmiş binary içinde nasıl göründüğünü inceleyeceğiz.

## Notlar

Bu ilk lab için önemli kavramlar:

- Temel C program yapısı
- `stdio.h`
- `main()`
- `printf()`
- `return 0`
- GCC ile derleme
- Linux executable çalıştırma
