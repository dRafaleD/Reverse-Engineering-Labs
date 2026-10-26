# Gün 40 — Dönüş Değerleri ve RAX Register'ı

[English](notes.md) · [Türkçe](notes.tr.md)

## Dersin amacı
Bu gün **Dönüş Değerleri ve RAX Register'ı** konusunu kontrollü, zararsız bir örnekte öğreniyoruz. Kavramları anlamak, hipotez kurmak ve gözlemi yorumdan ayırmak öncelikli.

## Temel bilgiler
1. x86-64 System V'de tamsayı ve pointer dönüş değerleri `rax` ile taşınır; `int` için 32 bitlik `eax` kullanılır.
2. Sonucu kullanan çağıran, `call`'dan hemen sonra `eax`/`rax` okur; kullanmayan okumaz.
3. Bazı yollarda `return` olmayan bir fonksiyon `rax`'ta ne varsa onu bırakır; fark etmeye değer bir hatadır.

## Kavramsal ayrımlar
- `eax`'a yazmak `rax`'ın üst yarısını sıfırlar; `ax` veya `al`'a yazmak sıfırlamaz.
- Optimizasyon çağrıyı tamamen ortadan kaldırabilir; `-O0` ile `-O2`'yi karşılaştır.
- Disassembly derleyicinin ürettiğini gösterir, programcının niyetini değil.

## Lab uygulaması
Önce `returns.c` dosyasını incele. Komutları lab klasöründen çalıştır:

~~~bash
gcc -g -O0 returns.c -o returns
~~~

~~~bash
./returns
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn returns | awk '/<add_one>:/,/ret/'
~~~

~~~bash
objdump -d -M intel --no-show-raw-insn returns | awk '/<main>:/,/ret/'
~~~


## Görevler
1. Kaynaktan program çıktısını tahmin et.
2. Derleyip çalıştır; `add_one`'ın sonucunu disassembly'de nereye koyduğunu bul.
3. `main` içinde `call`'dan hemen sonra sonucu okuyan komutu bul.
4. `-O2` ile yeniden derle ve çağrıya ne olduğunu anlat.
5. Dönüş tipini `long` yap ve `eax`'ın `rax`'a dönüşüp dönüşmediğine bak.
6. Okumanı GDB'de `finish` ve `info registers rax` ile doğrula.

## Kısa sınav
1. Girdi nereden geliyor ve onu kim kontrol ediyor?
2. Doğrudan gözlenen ile çıkarım yapılan arasındaki fark nedir?
3. Hangi varsayımlar gerçek sistemde geçerli olmayabilir?
4. Hangi kanıt tek başına yetersiz kalır?
5. Bir otomatik test neyi kontrol etmeli?

## Etik ve sınırlamalar
Yalnızca izinli sistemlerde ve sentetik verilerle çalış. Bu örnek tam bir tespit veya koruma ürünü değildir.
