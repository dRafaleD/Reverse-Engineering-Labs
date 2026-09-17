# Gün 14 — Global ve Static Değişkenler

## Amaç

Global ve static değişkenlerin local stack değişkenlerinden farkını ve derlenmiş binary içinde nasıl görünebileceklerini öğrenmek.

## Kaynak Kod

`source/globals.c` örneğinde bir global değişken, bir static global değişken ve bir static local değişken bulunur.

## Temel Fikirler

- Global değişkenlerin programın ömrü boyunca storage alanı vardır.
- `static`, kullanıldığı yere göre linkage veya storage duration davranışını değiştirir.
- Bu değişkenler normal stack frame'in dışında tutulabilir.
- Başlangıç değeri verilmiş global/static veriler genellikle `.data` gibi section'larda bulunur.
- Sıfırla başlatılan global/static veriler genellikle `.bss` bölümünde bulunur.
- x86-64 Linux üzerinde assembly erişimleri RIP-relative addressing şeklinde görünebilir.

## Ghidra Alıştırması

1. Kaynak kodu debug bilgisiyle derle:

```bash
gcc -g globals.c -o globals
```

2. Binary'yi Ghidra'da aç.
3. `main` ve yardımcı fonksiyonu bul.
4. Global ve static değişkenlere yapılan referansları incele.
5. Bu erişimleri önceki lablarda gördüğümüz local değişken erişimleriyle karşılaştır.

## Başlangıç Seviyesi RE Pattern'i

Local değişken:

```text
[RBP - offset]
```

x86-64 üzerinde global/static veri erişimi şöyle görünebilir:

```text
[RIP + offset]
```

Tam instruction dizisi compiler ve build seçeneklerine göre değişebilir. Bu yüzden tek bir çıktıyı ezberlemek yerine temel mantığı tanımaya çalışıyoruz.

## Çıkarım

Bir programı reverse ederken değişkenin bulunduğu yer, lifetime ve storage hakkında ipucu verebilir. Stack-relative erişimler çoğunlukla local değişkenleri gösterirken stack frame dışındaki veri referansları global veya static storage işareti olabilir.
