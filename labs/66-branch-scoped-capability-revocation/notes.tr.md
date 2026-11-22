# Gün 66 — Capability Devri İçin Dal Bazlı Yetki İptali

[English](notes.md) · [Türkçe](notes.tr.md) · [C uygulaması](branch_revoke.c)

## 65. günden devam
Önceki gün bir handle'ın diğer handle'lara yetki aktarmasını ve hepsinin birlikte iptal edilmesini gördük. Şimdi iki ayrı yetki **dalından (branch)** yalnızca birini iptal edeceğiz.

**Branch epoch:** yalnızca ilgili dalın yetki sürümü. **Resource generation:** asıl nesnenin nesil numarası. **Attenuation:** verilen izni daraltarak devretme.

İki dal aynı kaynağa bakar. Sol dalın epoch'u değişince yalnızca sol dalın eski handle'ları reddedilir; sağ dal çalışır. Kaynağın generation değeri değişirse **her iki dal** geçersizleşir.

## Çalıştırma
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -O0 -g branch_revoke.c -o branch_revoke
./branch_revoke
```

## Sorular
Dal bazlı iptal ile kaynak genelinde iptal arasında ne fark var? Alt handle niçin üst handle'dan daha güçlü olamaz? Gerçek thread'lerde neden ek senkronizasyon gerekir?

## Sınırlar
Gerçek OS yetkilendirmesi değildir; bellek nesneleri serbest bırakılmaz ve yarış durumları modellenmez.
