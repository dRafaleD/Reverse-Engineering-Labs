# Gün 70 — Kayıt Durum Makinesi ve Geçiş Doğrulama

[English](notes.md) · [Türkçe](notes.tr.md) · [Code](state_machine.c)

69. günde tek tek ikili kayıtların biçimini doğruladık. Bugün biçimi doğru olan kaydın mevcut oturumda kabul edilip edilmeyeceğini inceliyoruz. EMPTY yalnızca BEGIN kabul eder; OPEN en fazla üç DATA ve END kabul eder; CLOSED yeni olay kabul etmez. Reddedilen geçişlerde durum ve sayaç değişmez.

| Durum / olay | Beklenen |
|---|---|
| EMPTY / DATA | Ret |
| EMPTY / BEGIN | OPEN |
| OPEN / BEGIN | Ret |
| OPEN / dördüncü DATA | Ret |
| OPEN / END | CLOSED |
| CLOSED / BEGIN | Ret |

GDB'de apply fonksiyonunda durup switch dallarını izle. -O2 ile derleyip Ghidra'da karşılaştır. BEGIN ardından END içeren ayrı test ekle. Biçim kontrolü ile durum kontrolünün farkını açıkla. Beklenen özet: 13 transition scenarios passed. Gerçek protokol veya canlı sistem kullanılmaz.

## Çalıştır
```bash
cd labs/70-record-state-machines-and-transition-validation
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 state_machine.c -o state_machine
./state_machine
```
