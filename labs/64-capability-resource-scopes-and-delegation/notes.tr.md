# Gün 64 — Capability Kaynak Kapsamı ve Yetki Devri

[English](notes.md) · [Türkçe](notes.tr.md) · [C uygulaması](scoped_handles.c)

## 63. günden devam
Önceki gün bir handle'ın izinlerini daraltarak devretmeyi öğrendik. Şimdi **resource scope (kaynak kapsamı)** kontrolü ekliyoruz. Bir handle yalnızca belirli nesneye erişmek için verildiyse, başka nesneye yönlendirilememeli.

**Rights:** okuma/yazma izinleri. **Resource ID:** hedef nesneyi belirleyen kimlik. **Generation:** nesnenin mantıksal nesil numarası. **Policy epoch:** izin değişikliklerinde güncellenen sürüm.

Kod, salt-okunur child handle oluşturur; yazma yetkisi kazanmasını ve başka kaynak ID'sine devri reddeder. Taklit edilmiş ID ve artık geçerli olmayan policy epoch da reddedilir.

## Çalıştır
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 scoped_handles.c -o scoped_handles
./scoped_handles
gdb -q ./scoped_handles
```

## Sorular
İzinler doğru olsa bile hedef kaynak neden doğrulanmalı? Yetki devreden handle kaynak değiştirebilir mi? Eski policy epoch neden erişimi engeller?

## Sınırlar
Gerçek işletim sistemi yetkilendirmesi değildir; nesne bellekten silinmez, eşzamanlı kullanım güvenliği sağlamaz.
