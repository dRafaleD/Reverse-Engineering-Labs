# Gün 65 — Çok Aşamalı Yetki Devri ve Zincirleme İptal

[English](notes.md) · [Türkçe](notes.tr.md) · [C uygulaması](delegation_tree.c)

## 64. günden devam
Önceki gün handle'ın yalnızca **belirli kaynak** üzerinde kullanılmasını sağladık. Şimdi yetkiyi birden fazla aşamada devrediyoruz: **root → child → grandchild**.

**Delegation depth:** yetkinin kaç kez aktarıldığı. **Attenuation:** devredilen iznin genişletilmemesi. **Transitive revocation:** üst kaynağın yetkisi değişince alt referansların da geçersizleşmesi.

Root hem okur hem yazar. Child yalnızca okur, grandchild da yalnızca okur. Grandchild'a daha fazla yetki verilmez ve derinlik sınırı geçilemez. Kaynağın `epoch` değeri artınca eski üç referansın tamamı reddedilir.

## Çalıştırma
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 delegation_tree.c -o delegation_tree
./delegation_tree
gdb -q ./delegation_tree
```

## Sorular
Alt handle neden izinlerini artıramaz? Tek bir policy epoch değişikliği nasıl hepsini iptal eder? Sadece bir child'ı iptal etmek için nasıl farklı tasarım gerekir?

## Sınırlar
Gerçek güvenli bellek yöneticisi, işletim sistemi capability sistemi veya thread güvenliği değildir.
