# Gün 67 — Hiyerarşik Yetki İptali ve Üst Dal Epoch Kontrolü

[English](notes.md) · [Türkçe](notes.tr.md) · [C uygulaması](ancestor_epochs.c)

## 66. günden devam
Önceki gün sol dalın yetkisini iptal ederken sağ dalı çalışır bıraktık. Şimdi **iç içe devredilmiş handle'lar** var: kök → sol dal → torun dal.

**Ancestor (üst dal):** bir handle'ın bağlı olduğu dalın yukarısındaki dal. **Captured epoch:** handle verilirken kaydedilen sürüm. **Hierarchical revocation:** bir üst dalın iptal edilmesinin alt dalları da etkilemesi.

Kod bütün üst dalların sürümlerini denetler. Sol dalın epoch'u değişince sol handle ve torun geçersiz olur; sağ dal çalışır. Kökün epoch'u değişirse her iki taraf da reddedilir. Eski torun dalı, yalnızca yeni sol handle verildi diye otomatik canlanmaz.

## Çalıştır
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 ancestor_epochs.c -o ancestor_epochs
./ancestor_epochs
```

## Sorular
Yalnızca en alttaki dalın sürümünü kontrol etmek neden yeterli değildir? Sol dalı iptal etmek sağ dalı neden etkilemez? Eşzamanlı erişimde hangi sorunlar oluşabilir?

## Sınırlar
Gerçek işletim sistemi yetkilendirmesi değildir; dal nesneleri bellekte kalır ve thread desteği yoktur.
