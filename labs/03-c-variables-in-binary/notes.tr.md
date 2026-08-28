# Gün 3 — C Değişkenlerinin Binary İçindeki Görünümü

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Bu labın amacı temel C değişken türleri ile bunların derlenmiş bir binary içinde nasıl göründüğü arasında bağlantı kurmaktır.

Burada assembly ezberlemeye çalışmıyoruz. Amaç, Ghidra içinde basit kalıpları tanımaya başlamak.

## Kaynak Kod

```c
#include <stdio.h>

int main() {
    char mychar = 'C';
    int myinteger = 16;
    float myfloat = 5.10f;
    double mydouble = 213.568;

    printf("%c\n", mychar);
    printf("%d\n", myinteger);
    printf("%f\n", myfloat);
    printf("%f\n", mydouble);

    return 0;
}
```

Debug bilgisiyle derlemek için:

```bash
gcc -g variables.c -o variables
```

Daha sonra oluşan executable dosyasını Ghidra'ya import edip `main` fonksiyonunu açıyoruz.

## Değişken Boyutları

Bu labda kullanılan sistemde temel boyutlar şu şekildeydi:

```text
char   -> 1 byte
int    -> 4 byte
float  -> 4 byte
double -> 8 byte
```

Bu boyutlar, Ghidra'nın neden farklı değişkenler için farklı bellek erişim boyutları gösterdiğini anlamamıza yardımcı olur.

## Ghidra'da Ne Gördük?

### `char`

Basitleştirilmiş bir satır şöyle görünüyordu:

```asm
MOV byte ptr [RBP + mychar], 0x43
```

- `byte ptr` 1 byte anlamına gelir.
- `0x43` hexadecimal bir değerdir.
- ASCII tablosunda `0x43`, `C` karakterine karşılık gelir.

Bu doğrudan şu C satırıyla bağlantılıdır:

```c
char mychar = 'C';
```

### `int`

Basitleştirilmiş bir satır:

```asm
MOV dword ptr [RBP + myinteger], 0x10
```

- `dword ptr` 4 byte anlamına gelir.
- Hexadecimal `0x10`, decimal olarak `16` değeridir.

Bu da şu satırla bağlantılıdır:

```c
int myinteger = 16;
```

### `float`

Ghidra, `float` tarafında şu instruction'ı gösterdi:

```asm
MOVSS
```

`MOVSS`, single-precision floating-point değerleri taşırken sık görülen bir instruction'dır.

Bu lab için basit bağlantı:

```text
MOVSS -> float
```

### `double`

`double` tarafında ise şu instruction görüldü:

```asm
MOVSD
```

Bu lab için basit bağlantı:

```text
MOVSD -> double
```

## Pointer Boyutu İfadeleri

Ghidra'da şu ifadeleri gördük:

```text
byte ptr  -> 1 byte
dword ptr -> 4 byte
qword ptr -> 8 byte
```

Bunları temel C veri tipleriyle şöyle bağlayabiliriz:

```text
char   -> byte ptr
int    -> dword ptr
float  -> dword ptr
double -> qword ptr
```

Bu, C veri tipleriyle assembly seviyesindeki bellek erişimleri arasında kurduğumuz ilk önemli bağlantılardan biridir.

## Gördüğümüz Registerlar

### `RBP`

Ghidra'da şöyle bir ifade gördüğümüzde:

```text
[RBP + myinteger]
```

program mevcut fonksiyonun stack frame'i içindeki local bir değişkene erişiyor demektir.

Şimdilik `RBP` için şu düşünce yeterlidir:

> Local stack değişkenlerine ulaşmak için kullanılan bir referans noktası.

### `XMM0` ve `XMM1`

Bu registerlar `float` ve `double` işlemlerinin çevresinde görüldü.

x86-64 sistemlerde floating-point ve SIMD işlemlerinde sık kullanılırlar.

Şu aşamada ezberlemeye gerek yoktur.

## `CALL printf`

Binary içinde şu instruction da görüldü:

```asm
CALL printf
```

`CALL`, başka bir fonksiyonun çağrıldığını belirtir.

Bu da C kodundaki `printf()` çağrılarıyla doğrudan bağlantılıdır.

## Ana Çıkarım

Gün 3'teki en önemli fikir şu zincirdir:

```text
C değişkeni
    ↓
byte cinsinden boyutu
    ↓
assembly'deki bellek erişim boyutu
    ↓
Ghidra'daki görünümü
```

Basit örnekler:

```text
char   -> 1 byte -> byte ptr
int    -> 4 byte -> dword ptr
float  -> 4 byte -> MOVSS / dword ptr
double -> 8 byte -> MOVSD / qword ptr
```

Buradaki amaç her assembly instruction'ını ezberlemek değil. Amaç, C'de yazdığımız basit yapıların derlenmiş binary içinde nasıl iz bıraktığını fark etmeye başlamaktır.
