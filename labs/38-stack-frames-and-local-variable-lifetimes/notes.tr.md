# Gün 38 — Yığın Çerçeveleri, Yerel Değişkenler ve Derleyici Optimizasyonu

[English](notes.md) · [Türkçe](notes.tr.md)

## Amaç
Zararsız bir C uygulamasını GDB ile inceleyerek **stack frame** (fonksiyonun çağrı sırasındaki yığın bağlamı), yerel değişkenlerin ömrü ve optimizasyonun gözlemleri nasıl değiştirdiğini öğrenmek.

## Temel kavramlar
- Fonksiyon çağrısında parametreler, dönüş bilgisi ve yerel değişkenlerle ilgili bilgiler tutulabilir; düzeni ABI ve derleyici belirler.
- Otomatik yerel değişkenlerin ömrü ilgili kapsamla sınırlıdır. Fonksiyondan yerel değişken adresini döndürmek geçersizdir; böyle bir işaretçiyi kullanma.
- Frame pointer her derlemede bulunmaz; fonksiyonlar inline edilebilir.
- Debug sembolleri okumayı kolaylaştırır fakat kaynak kodun güvenilirliğini tek başına kanıtlamaz.

## Uygulama
Önce `frames.c` dosyasını oku. Linux'ta GCC ve GDB ile:

```bash
gcc -std=c11 -Wall -Wextra -g -O0 -fno-omit-frame-pointer frames.c -o frames_debug
./frames_debug
gdb -q -ex 'break inspect' -ex run -ex 'info args' -ex 'info locals' -ex 'bt' -ex continue -ex quit ./frames_debug
gcc -std=c11 -Wall -Wextra -g -O2 frames.c -o frames_opt
./frames_opt
```

`-O0` ve `-O2` sürümlerini karşılaştır. Adreslerin veya stack düzeninin hep aynı olacağını varsayma.

## Görevler
1. Parametreleri ve yerel değişkenleri ayır.
2. Fonksiyon döndükten sonra hangi bilginin artık geçerli olmadığını açıkla.
3. Optimizasyonun `info locals` sonucuna etkisini gözlemle.
4. Derleyici sürümü, mimari, komutlar ve gözlemlerini not et.
5. Tek bir stack görüntüsünden neden kesin aktör atfı yapılamayacağını tartış.

## Güvenlik ve sınırlar
Yalnızca yerel eğitim kodu. Gerçek zararlı yazılım veya bellek sömürüsü gerektirmez.
