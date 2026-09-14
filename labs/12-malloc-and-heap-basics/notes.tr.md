# Gün 12 — malloc ve Heap Temelleri

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

C'de dinamik bellek ayırmanın temel mantığını öğrenmek ve derlenmiş binary içinde `malloc()` ile `free()` fonksiyonlarının bıraktığı izleri tanımaya başlamak.

## Kaynak kod

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    int *numbers = malloc(3 * sizeof(int));

    if (numbers == NULL) {
        return 1;
    }

    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;

    printf("%d %d %d\n", numbers[0], numbers[1], numbers[2]);

    free(numbers);
    return 0;
}
```

## Heap nedir?

Heap, program çalışırken dinamik olarak ayrılan veriler için kullanılan bir bellek alanıdır. Basit local değişkenlerden farklı olarak heap belleği çalışma sırasında istenir.

Bu örnekte:

```c
malloc(3 * sizeof(int))
```

üç adet `int` değeri tutacak kadar bellek ister.

Eğer mevcut platformda bir `int` 4 byte ise istenen boyut genellikle:

```text
3 × 4 = 12 byte
```

olur.

## `malloc()` ne döndürür?

`malloc()` ayrılan belleğin adresini, yani bir pointer döndürür.

Bu yüzden:

```c
int *numbers = malloc(...);
```

satırında `numbers` bir adres tutar.

Bellek ayrılamazsa `malloc()` `NULL` döndürür. Bu nedenle program:

```c
if (numbers == NULL)
```

kontrolünü yapar.

## Array erişimi ve pointer mantığı

Şu ifadeler:

```c
numbers[0]
numbers[1]
numbers[2]
```

ayrılan blok içindeki art arda duran `int` değerlerini ifade eder.

Daha düşük seviyede şöyle düşün:

```text
base address + index × element size
```

4 byte'lık `int` için:

```text
numbers[0] -> base + 0
numbers[1] -> base + 4
numbers[2] -> base + 8
```

Bu, önceki array ve pointer lablarındaki pattern ile doğrudan bağlantılıdır.

## Ghidra'da neye bakacağız?

Debug bilgisiyle derle:

```bash
gcc -g heap.c -o heap
```

Sonra Ghidra'da `main` fonksiyonunu aç.

Şu çağrılar önemli ipuçlarıdır:

```text
CALL malloc
CALL printf
CALL free
```

Ayrıca şunları da görebilirsin:

- `malloc` sonrasında sıfırla karşılaştırma
- dönen pointer üzerinden offset'li memory write işlemleri
- aynı pointer'ın daha sonra `free` fonksiyonuna verilmesi

Compiler ve optimization ayarları instruction'ları değiştirebilir. Bu yüzden birebir satır ezberlemek yerine genel pattern'e odaklan.

## Reverse engineering pattern'i

Basit akış:

```text
boyut hazırlanır
    ↓
CALL malloc
    ↓
pointer döner
    ↓
NULL kontrolü
    ↓
pointer + offset üzerinden yazma
    ↓
CALL free
```

Bir binary içinde bu akışı gördüğünde dinamik olarak ayrılan belleğin oluşturulup kullanıldığını ve sonra serbest bırakıldığını düşünebilirsin.

## Stack ve heap — başlangıç seviyesi

Şimdilik şu ayrım yeterli:

```text
stack -> local function verileri, otomatik yaşam süresi
heap  -> çalışma sırasında ayrılan bellek, C'de genelde elle serbest bırakılır
```

Gerçek programlarda konu daha karmaşık olabilir ama bu lab için bu kadar yeterli.

## Ana çıkarım

```text
malloc(size) -> heap'ten bellek ister
return value -> pointer/adres
NULL         -> allocation başarısız olabilir
[index]      -> adres + index × element size
free(ptr)    -> ayrılan belleği serbest bırakır
```

Şimdilik allocator iç yapısını ezberlemek gerekmiyor. Amaç allocation, pointer tabanlı memory access ve deallocation işlemlerini tek bir pattern olarak tanımaya başlamak.
