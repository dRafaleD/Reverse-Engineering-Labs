# Gün 53 — String Karşılaştırmaları ve Çalışma Zamanı Dal Kanıtı

[English](notes.md) · [Türkçe](notes.tr.md) · [Uygulama](branches.c)

## Hedef
52. günde binary içerisindeki metinlerin XREF (çapraz referans) ilişkilerini görmüştük. Şimdi bu **statik bulguyu** GDB'de **gerçekten yürütülen kod** ile ilişkilendiriyoruz.

## Kavram 1: XREF neyi kanıtlar?
Ghidra'daki bir XREF, bir kod parçasının belli veriyle ilişkili olduğunu gösterir. **O kodun mutlaka çalıştığını kanıtlamaz.** GDB'de breakpoint'e düşmek ise yalnızca incelenen çalıştırmada fonksiyonun ilgili noktaya ulaştığını gösterir; bütün girdiler için aynı davranış beklenemez.

## Kavram 2: strcmp ve koşullu dallanma
`strcmp(a,b)` değeri **0** ise metinler eşittir. Bunun ardından `if` sırası hangi dönüş değerinin seçileceğini belirler. `branches.c` içindeki `classify` fonksiyonunu incele. `-O0` optimizasyonsuz derleme, `-O2` ise optimize edilmiş derlemedir; debugger'da göreceğin değişkenler farklılaşabilir.

## Uygulama — Linux, GCC ve GDB
```bash
gcc -std=c11 -Wall -Wextra -g -O0 -fno-omit-frame-pointer branches.c -o branches
./branches
gdb -q -batch -ex 'break classify' -ex run -ex 'info args' -ex 'bt' -ex continue -ex quit ./branches
```
Program üç sentetik girdiyi sırasıyla `known`, `review`, `other` olarak sınıflandırır.

## Görevler
1. İlk breakpoint'te gelen argümanı kaydet ve `main` içindeki hangi çağrıdan geldiğini göster.
2. İkinci metni `SAFE-LAB` yap, yeniden derle ve sonucu tahmin et.
3. `gcc -O2 -g` ile derle; değişken görünürlüğündeki farkları karşılaştır.
4. **Kodun varlığı**, **kodun çalışması** ve **kötü niyet** arasındaki farkı yaz.
5. Araç sürümlerini, mimariyi ve komutları raporla.

## Güvenlik ve sınırlar
Yalnızca zararsız yerel C kodu. Bu lab başka yazılımlara müdahale etmez; tek çalışma izlemi bütün olası yolları kanıtlamaz.
