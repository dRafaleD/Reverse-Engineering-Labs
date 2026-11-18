# Gün 62 — Politika Sürümü ve Handle Yetkisini İptal Etme

[English](notes.md) · [Türkçe](notes.tr.md) · [C uygulaması](policy_epochs.c)

## 61. günden devam
Önceki gün, handle'ın doğru nesneyi göstermesi ile okuma/yazma yetkisini birbirinden ayırdık. Bu gün **revocation (yetki iptali)** ekliyoruz: izin sonradan kaldırılırsa eski handle ne olur?

**Generation:** bellek yuvasındaki mantıksal nesnenin nesil numarası. **Policy epoch:** nesnenin yetki politikası değiştikçe artan sürüm. **Capability:** referansın taşıdığı izinler.

Bir işlem için üç kontrol var: nesne aktif ve doğru generation mı, handle'ın policy epoch'u güncel mi, hem handle hem mevcut politika işlemi izinli kılıyor mu?

Örnek: yazma yetkisini kaldırınca eski writer geçersizleşir. Daha sonra yazma iznini geri vermek **eski writer'ı otomatik canlandırmaz**; yeni handle gerekir. Sürüm sayacı taşarsa politika güncellemesi reddedilir.

## Çalıştır
~~~bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 policy_epochs.c -o policy_epochs
./policy_epochs
gdb -q ./policy_epochs
~~~
GDB'de `authorized` içine breakpoint koy; `policy_epoch` ve `generation` karşılaştırmalarını incele. Ghidra'da kontrol akışını ve bit maskesi testlerini bul.

## Sorular
Eski izinler neden otomatik geri gelmemeli? Generation ile policy epoch farkı ne? Eşzamanlı çalışan iki thread bu modeli nasıl etkiler?

## Sınır
Gerçek işletim sistemi yetkilendirmesi değildir; bellek nesnesi serbest bırakılmaz ve thread güvenliği sunulmaz.
