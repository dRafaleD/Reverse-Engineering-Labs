# Gün 43 — Struct Dönüş Değerleri: RAX:RDX ve Gizli Pointer

[English](notes.md) · [Türkçe](notes.tr.md)

## Dersin amacı
Bu gün **Struct Dönüş Değerleri: RAX:RDX ve Gizli Pointer** konusunu kontrollü, zararsız bir örnekte öğreniyoruz. Kavramları anlamak, hipotez kurmak ve gözlemi yorumdan ayırmak öncelikli.

## Temel bilgiler
1. x86-64 System V'de iki tamsayıdan oluşan küçük bir struct (16 bayt) `rax` ve `rdx` ile döner.
2. Daha büyük bir struct gizli ilk argüman üzerinden döner: çağıran `rdi` ile bir pointer verir, çağrılan da aynı pointer'ı `rax` ile döndürür.
3. Disassembly'de çağıran stack'te yer ayırıp adresini verir; bu yüzden `call` öncesindeki açıklanamayan bir `lea rdi, [rbp-...]` çoğu zaman struct dönüşü demektir.

## Kavramsal ayrımlar
- 16 baytlık sınır ve alan türleri register sınıfını belirler; float'lar xmm register'larını kullanır.
- Optimizasyon çağrıyı satır içine alıp gizli pointer'ı tamamen kaldırabilir; `-O0` ile `-O2`'yi karşılaştır.
- Yalnızca hafızana değil, ABI belgesine ve bir debugger'a karşı doğrula.

## Lab uygulaması
Önce `structret.c` dosyasını incele. Komutları lab klasöründen çalıştır:

~~~bash
gcc -g -O0 structret.c -o structret
~~~

~~~bash
./structret
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn structret | awk '/<make_small>:/,/ret/'
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn structret | awk '/<make_big>:/,/ret/'
~~~


## Görevler
1. Kaynaktan program çıktısını tahmin et.
2. Derleyip çalıştır; `make_small`'ın iki alanı disassembly'de nereye koyduğunu bul.
3. `make_big` içinde gizli pointer'ı taşıyan register'ı ve onu dolduran store komutlarını bul.
4. `-O2` ile yeniden derle ve iki çağrıya ne olduğunu anlat.
5. Küçük struct'a üçüncü bir `long` alan ekle ve çağrı kuralının nasıl değiştiğini gözle.
6. GDB'de `finish` ve `info registers rax rdx` ile doğrula.

## Kısa sınav
1. Girdi nereden geliyor ve onu kim kontrol ediyor?
2. Doğrudan gözlenen ile çıkarım yapılan arasındaki fark nedir?
3. Hangi varsayımlar gerçek sistemde geçerli olmayabilir?
4. Hangi kanıt tek başına yetersiz kalır?
5. Bir otomatik test neyi kontrol etmeli?

## Etik ve sınırlamalar
Yalnızca izinli sistemlerde ve sentetik verilerle çalış. Bu örnek tam bir tespit veya koruma ürünü değildir.
