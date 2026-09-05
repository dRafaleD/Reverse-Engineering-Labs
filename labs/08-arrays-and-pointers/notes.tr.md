# Gün 8 — Diziler ve Pointer Temelleri

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Bu labın amacı diziler, pointer'lar, adresler ve indexed memory access arasındaki temel bağlantıyı anlamak ve sonra aynı yapının Ghidra'da nasıl göründüğüne bakmaktır.

## Kaynak Kod

```c
#include <stdio.h>

int main() {
    int numbers[3] = {10, 20, 30};
    int *ptr = numbers;

    printf("First: %d\n", numbers[0]);
    printf("Second: %d\n", *(ptr + 1));
    printf("Third: %d\n", *(ptr + 2));

    return 0;
}
```

Debug bilgisiyle derlemek için:

```bash
gcc -g -O0 arrays_pointers.c -o arrays_pointers
```

## Dizi Temeli

```c
int numbers[3] = {10, 20, 30};
```

bellekte art arda üç adet `int` değeri oluşturur.

Tipik bir sistemde `int` 4 byte ise mantıksal olarak şöyle düşünebiliriz:

```text
numbers[0] -> 10
numbers[1] -> 20
numbers[2] -> 30
```

Reverse engineering açısından önemli fikir, array elemanlarının bellekte birbirine komşu tutulmasıdır.

## Pointer Temeli

```c
int *ptr = numbers;
```

`ptr`, dizinin ilk elemanının adresini tutar.

Şimdilik pointer'ı çok basit şekilde şöyle düşün:

```text
pointer = verinin bellekte nerede olduğunu söyleyen bir adres
```

## Pointer Arithmetic

```c
*(ptr + 1)
```

şu anlama gelir:

1. `ptr` içindeki adresten başla
2. bir sonraki `int` elemanına ilerle
3. o adresteki değeri oku

`ptr` bir `int *` olduğu için `+1`, bir raw byte değil bir `int` elemanı kadar ilerler.

Bu yüzden:

```c
numbers[0] == *(ptr + 0)
numbers[1] == *(ptr + 1)
numbers[2] == *(ptr + 2)
```

aynı elemanlara ulaşır.

## Ghidra'da Neye Bakacağız?

Binary'yi Ghidra'ya attıktan sonra `main` içinde şunları arayacağız:

- stack üzerinde local array alanı
- `RBP` veya `RSP` tabanlı adresler
- adres hesaplamak için kullanılan `LEA`
- bir adres + offset üzerinden yapılan memory read işlemleri
- eleman boyutuna bağlı scaling işlemleri

Örneğin şöyle bir memory ifadesi görebilirsin:

```asm
[RAX + RCX*4]
```

Buradaki `*4`, 4 byte'lık `int` elemanlarının indexlendiğine dair bir ipucu olabilir.

Kullanılan register ve instruction'lar compiler sürümüne ve ayarlarına göre değişebilir.

## Reverse Engineering Bağlantısı

C tarafında:

```c
numbers[i]
```

çok basit görünür.

Makine kodu seviyesinde ise bu iş bir adres hesabına dönüşür:

```text
başlangıç adresi
+
index × eleman boyutu
=
istenen elemanın adresi
```

Bu pattern; array, table, buffer ve structure analizlerinde sürekli karşımıza çıkar.

## Ana Çıkarım

Pointer syntax'ını ve assembly'yi aynı anda ezberlemeye çalışma.

Şimdilik şu üç bağlantı yeterli:

```text
array   -> bellekte art arda duran elemanlar
pointer -> bir adres tutar
index   -> eleman boyutuna göre adres hesabı yapılır
```

Gün 8'in amacı Ghidra'da memory-address patternlerini tanımaya başlamaktır.
