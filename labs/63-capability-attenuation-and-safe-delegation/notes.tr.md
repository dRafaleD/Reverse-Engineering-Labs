# Gün 63 — Yetkileri Daraltarak Güvenli Handle Devretme

[English](notes.md) · [Türkçe](notes.tr.md) · [C uygulaması](delegation.c)

## 62. günden devam
Önceki gün **policy epoch** ile eski handle'ların yetkisini iptal ettik. Bu gün **capability attenuation (yetki daraltma)** konusunu öğreniyoruz.

Bir handle hem okuma hem yazma yetkisine sahipse yalnızca okuma yetkisi olan yeni bir handle oluşturabilir. Ama salt-okuma yetkisi olan handle, kendisinde olmayan yazma iznini başkasına veremez.

- **Capability:** izin taşıyan referans.
- **Delegation:** başka bir referansa yetki devretme.
- **No amplification:** devredilen yetki mevcut hakları aşamaz.
- **Epoch revocation:** politika sürümü değişince eski parent ve child handle'lar geçersiz olur.

Kodda tam yetkili handle'dan salt-okuma handle oluşturulur, yazma isteği reddedilir, ardından politika sürümü değiştirilerek eskileri iptal edilir.

## Çalıştır
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 delegation.c -o delegation
./delegation
gdb -q ./delegation
```

## Sorular
Yetki daraltma neden güvenlidir? Eski handle'ın yetkisi politikadan sonra neden yeniden kontrol edilir? Bellek serbest bırakılsaydı veya farklı thread kullansaydı hangi riskler oluşurdu?

## Sınırlar
Eğitim için yazılmış C modelidir; gerçek işletim sistemi yetkilendirmesi veya eşzamanlı işlem güvencesi değildir.
