# Gün 68 — Değişmez Dal Kimliği ve Eski Handle İptali

[English](notes.md) · [Türkçe](notes.tr.md) · [Code](branch_incarnation.c)

67. günde üst dalların epoch değerlerini kontrol ettik. Bugün iptal edilmiş bir handle'ın yeni yetki verildiğinde yanlışlıkla tekrar canlanmamasını inceliyoruz. Eski dal pasif bırakılır; yeni yetki için ayrı bir dal nesnesi oluşturulur. Okuma öncesinde aktiflik ve incarnation (nesne örneği kimliği) kontrol edilir. Sekiz assertion eski ve yeni handle davranışını karşılaştırır.

**Alıştırma:** GDB'de read_value fonksiyonunda durup eski/yeni okuyucuyu karşılaştır. Eski nesnenin kimliği neden değiştirilerek yeniden etkinleştirilmemeli? Gerçek uygulamada bellek yaşam süresi ve eşzamanlılık neden önemlidir?

Önceki labdaki değiştirilebilir üst dal bağları, eski handle'ların yeniden canlanmayacağını tek başına garanti etmez. Bu örnek yalnızca bellekte kalan zararsız C nesneleriyle çalışır; gerçek OS yetki sistemi değildir.

## Çalıştır
```bash
cd labs/68-immutable-branch-incarnations-and-stale-handle-rejection
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 branch_incarnation.c -o branch_incarnation
./branch_incarnation
```
