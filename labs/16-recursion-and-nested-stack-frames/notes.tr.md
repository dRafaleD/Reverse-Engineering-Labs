# Gün 16 — Recursion ve İç İçe Stack Frame'ler

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Kendini çağıran bir fonksiyonun derlenmiş binary içinde nasıl göründüğünü ve her recursive çağrının, döngüdeki gibi geriye jump atmak yerine neden yeni bir stack frame oluşturduğunu öğrenmek.

Bu lab Gün 4 (fonksiyon çağrıları), Gün 5 (stack frame'ler), Gün 6 (conditional jump'lar) ve Gün 7 (döngüler) ile bağlanır.

## Kaynak Kod

```c
#include <stdio.h>

int factorial(int n) {
    if (n <= 1) {
        return 1;
    }

    return n * factorial(n - 1);
}

int main(void) {
    int result = factorial(4);
    printf("%d\n", result);
    return 0;
}
```

Recursive çağrının kolay görünmesi için önce optimizasyonsuz derle:

```bash
gcc -g -O0 recursion.c -o recursion
```

Sonra binary'yi Ghidra'ya alıp `factorial` fonksiyonunu incele.

## Recursion Nedir?

Recursive bir fonksiyon, aynı problemin daha küçük bir halini kendini çağırarak çözer.

Bu örnekte `factorial(4)` şunu anlamına gelir:

```text
4 * factorial(3)
      3 * factorial(2)
            2 * factorial(1)
                  1
```

Sonuç `24`'tür.

Reverse engineering açısından önemli olan matematik değil. Aynı fonksiyon gövdesinin, her seferinde farklı bir `n` ile, önceki çağrılar bitmeden birden fazla kez çalışmasıdır.

## Recursion Döngü Değildir

Gün 7, tekrarı tek fonksiyon içinde geriye jump ile göstermişti:

```text
karşılaştır
  ↓
geriye jump
  ↓
aynı stack frame yeniden çalışır
```

Recursion işi başka türlü tekrarlar:

```text
aynı fonksiyonu CALL et
  ↓
yeni bir stack frame oluşur
  ↓
önceki çağrı bekler
```

Döngü mevcut frame'i yeniden kullanır. Recursive çağrı, eskisinin üstüne yeni bir frame istifler.

## Base Case

Durma koşulu olmasa recursion hiç dönmez.

```c
if (n <= 1) {
    return 1;
}
```

Binary içinde bu hâlâ Gün 6 pattern'idir:

```text
CMP n, 1
  ↓
conditional jump
  ↓
ya 1 döndür
ya da recursive çağrıya devam et
```

Bilinmeyen bir fonksiyonu reverse ederken, baştaki bir karşılaştırma ve erken `RET` çoğu zaman base case'tir.

## İç İçe Stack Frame'ler

`factorial` çağrılarının her birinin kendi `n` kopyası olabilir.

`factorial(4)` için sadeleştirilmiş bir resim:

```text
factorial n=4
  factorial n=3
    factorial n=2
      factorial n=1   <- base case döner
    2 * 1
  3 * 2
4 * 6
```

Gün 5, bir fonksiyonun bir stack frame hazırladığını göstermişti. Recursion, aynı fonksiyonun birkaç frame'inin aynı anda canlı olabileceğini gösterir.

Bu yüzden Ghidra `factorial` fonksiyonunun `factorial`'ı çağırdığını gösterebilir. Hedef başka bir yardımcı fonksiyon değildir. Aynı kodun yeni bir çalışmasıdır.

## Ghidra'da Nelere Bakmalı

Derledikten sonra `factorial` içinde şunları bulmaya çalış:

1. `n <= 1` koşulunu uygulayan karşılaştırma
2. Erken `1` dönüşü
3. `n - 1` argümanının hazırlanması
4. Hedefi `factorial` olan bir `CALL`
5. Recursive çağrı döndükten sonra bir çarpma
6. Son `RET`

Şuna benzer bir pattern görebilirsin:

```text
CMP   n, 1
JLE   base_case
MOV   EDI, n-1
CALL  factorial
IMUL  EAX, n
RET
```

Tam register'lar değişebilir. Faydalı şekil şudur:

```text
base case'i kontrol et
  ↓
aynı fonksiyonu CALL et
  ↓
dönen değeri mevcut n ile birleştir
```

`main` içindeki ilk çağrı sıradandır: argümanlar hazırlanır, sonra `CALL factorial` gelir. Asıl farklı kısım `factorial` içindedir.

## Reverse Engineering'de Neden Önemli?

Yalnızca listing'e bakınca recursion normal bir fonksiyon çağrısı gibi durabilir.

Ek soru şudur:

```text
Bu CALL başka bir fonksiyona mı gidiyor,
yoksa şu anda okuduğum fonksiyona mı dönüyor?
```

Hedef aynı fonksiyonsa:

```text
bu recursion olabilir
  ↓
base case'i ara
  ↓
iç içe stack frame bekle
  ↓
sonraki çağrılar öncekilere geri döner
```

Bu, Gün 13'ten farklıdır. Orada indirect call birkaç olası hedef arasından seçiyordu. Burada hedef sabittir, ama fonksiyon kendini çağırmaktadır.

## Ana Çıkarım

```text
recursion            -> fonksiyon kendini çağırır
base case            -> karşılaştırma artı erken dönüş
aynı fonksiyona CALL -> yeni stack frame, geriye jump değil
döngü                -> tek frame içinde tekrarlar
recursion            -> base case'e kadar frame istifler
RE görevi            -> durma koşulunu ve sonuçların nasıl birleştiğini bulmak
```

Amaç her iç içe frame'i elle izlemek değil. Amaç self-call'u, base case'i ve recursive `CALL` döndükten sonra yapılan işi tanımaktır.
