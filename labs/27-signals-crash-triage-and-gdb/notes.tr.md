# Gün 27 — Signal, Crash Analizi, Core Dump ve GDB Backtrace

[English](notes.md) · [Türkçe](notes.tr.md)

**Amaç:** Day 26 process lifecycle bilgisini sinyaller, kontrollü crash, GDB stack trace ve kanıta dayalı hata incelemesiyle birleştirmek.

Gerekenler: Linux VM, GCC, GDB; isteğe bağlı Ghidra ve strace. Tüm işlemler kendi derlediğin zararsız örnekte yapılır.

## 1. Sinyaller eşzamansız bildirimlerdir

Signal, process'e bir olay veya istek bildirir. Hangi davranışın gerçekleşeceği sinyalin türüne, varsayılan disposition'a ve uygulamanın handler'ına bağlıdır. Her signal'ı dil seviyesindeki exception ile aynı sayma.

## 2. SIGUSR1 ve SIGSEGV

SIGUSR1 uygulamanın handler tanımlayabileceği bir sinyaldir. SIGSEGV ise normalde yakalanmazsa process'i sonlandırır. Buradaki SIGSEGV kasıtlı raise() ile üretilir; geçersiz bellek erişimi değildir. Sinyal ismi tek başına memory corruption kanıtı değildir.

## 3. Handler ve async-signal-safety

Signal handler normal kod gibi her işlevi güvenle çağırabileceğin bir yer değildir. Örneğin printf'i handler içinde kullanmak uygun olmayabilir. Demo sadece volatile sig_atomic_t flag'ini değiştirir; mesajı handler dışında basar. Bu örnek genel thread synchronization çözümü değildir.

## 4. Call stack ve backtrace

stage_one → stage_two → stage_three → raise şeklinde çağrı zinciri var. Optimize edilmemiş debug build'de GDB bunu backtrace ile gösterebilir. Stack frame ve return address yürütme yolunu açıklar; tek başına root cause analizini bitirmez.

## 5. Debug ve optimize build farkı

-g ile kaynak satırları ve isimler daha kolay incelenir. -O2 fonksiyonları inline edebilir; backtrace farklılaşır. strip bazı symbol'ları kaldırsa da signal ve register gözlemi mümkün olabilir. Orijinal binary'yi koruyup çalışma kopyalarında karşılaştır.

## 6. Core dump ve delil hassasiyeti

Core dump bazı koşullarda process memory snapshot'ıdır; parola veya kişisel veri barındırabilir. Ulimit, kernel ayarı ya da systemd-coredump yüzünden oluşmayabilir. Bu labda önce GDB ile canlı crash gözlemle; core dosyalarını public repoya yükleme.

## 7. Statik ve dinamik kanıt

Ghidra'da sigaction/raise importlarını, mode kontrollerini ve fonksiyonları bulabilirsin. GDB hangi branch'in gerçekten çalıştığını ve nerede durduğunu gösterir. strace -e trace=signal ilgili system davranışını gösterir. Potansiyel kod ile gözlenen çalışmayı ayır.

## 8. Sonuçtan önce crash triage

Mode, binary SHA-256, komut, ortam, signal, current frame, backtrace ve disassembly kaydet. Raporun observation / inference / unknown alanları olsun. SIGSEGV görmek otomatik exploitability göstergesi değildir.

## 9. Exit status ve parent gözlemi

Shell sinyalle sonlanmayı çoğu zaman 128 + signal number olarak gösterebilir, ancak shell'e bağlıdır. Esas fark normal/signal modlarının bitmesi ve crash modunun signal ile sonlanmasıdır. Day 26 waitpid/WIFSIGNALED mantığıyla bağla.

## 10. Güvenli yeniden üretim

İyi reproduction küçük ve tekrar edilebilirdir: kendi yerel target'ın, belli compiler flag'leri, sabit mode, gerçek secret yok, production host yok. Rastgele memory corruption üretmek yerine bu eğitim target'ındaki raise(SIGSEGV) davranışını izle.

## Yerel lab: derleme ve çalıştırma

Lab klasöründe:

```bash
gcc -std=c11 -g -O0 -fno-omit-frame-pointer -Wall -Wextra \
  source/signal_lab.c -o signal_lab
./signal_lab normal
./signal_lab signal
```

Normal mode sıfır koduyla çıkar. Signal mode'da handler flag günceller, program sonradan çıktı basar. **Crash mode'u yalnızca GDB altında çalıştır:**

```bash
gdb --args ./signal_lab crash
```

GDB içinde:

```text
set pagination off
break stage_three
run
continue
bt
frame 1
info registers
x/8i $pc
```

`continue` sonrası SIGSEGV ile durmasını bekle. `bt` ile stage_three → stage_two → stage_one → main zincirini bul. GDB'den çıkmak için `quit` kullan.

### Ghidra ve strace bağlantısı

```bash
readelf -Ws signal_lab | grep -E 'raise|sigaction|stage_'
strace -e trace=signal ./signal_lab signal
sha256sum signal_lab
```

Signal handler kodunu ve crash çağrı zincirini Ghidra decompiler'da incele. Runtime'da seçmediğin branch'lerin execute edildiğini iddia etme.

### Core dump (isteğe bağlı)

```bash
ulimit -c
cat /proc/sys/kernel/core_pattern
```

Core dump sistemde devre dışı olabilir ya da collector'a gidebilir. Sadece izole eğitim VM'sinde çalış ve core dosyasını GitHub'a gönderme.

## Mini challenge

1. `normal` / `signal` / `crash` modları için observed execution tablosu oluştur.
2. `-O2` build derle ve backtrace'i `-O0` ile karşılaştır; optimize edilmiş kodda function name görünmeyebilir.
3. Kendi kopyanda handler'ın flag güncellemesini kaldır; sinyalin devam eden akışındaki farkı açıklamaya çalış.
4. Ghidra'da crash çağrı zincirinin pseudocode'unu kendi cümlelerinle yaz.

## Alıştırmalar

1. GCC flag'lerinin görevini açıkla.
2. Üç mode'u ayrı analiz et.
3. `sigaction` kullanımını bul.
4. `sig_atomic_t` neden kullanılmış, söyle.
5. GDB'de breakpoint ekle.
6. `bt` çıktısını kaydet.
7. `info registers` ile PC'yi karşılaştır.
8. Call chain'i çiz.
9. Ghidra'da XREF izle.
10. strace signal kaydını bul.
11. SHA-256 hesapla.
12. Kısa crash triage raporu yaz.

## Tekrar soruları

1. Signal ne demek?
2. SIGUSR1 ile SIGSEGV farkı?
3. Her SIGSEGV memory corruption mu?
4. Handler neden printf çağırmamalı?
5. `sig_atomic_t` ne işe yarar?
6. `raise` ne yapar?
7. GDB backtrace neyi gösterir?
8. Debug symbols ne sağlar?
9. -O2 neden farklı frame gösterebilir?
10. Core dump hangi bilgileri içerebilir?
11. `core_pattern` neyi etkiler?
12. Static import neden runtime kanıtı değil?
13. Signal ile sonlanma normal exit'ten nasıl ayrılır?
14. waitpid ile bağlantı nedir?
15. Crash triage raporunda hangi sınırlamaları belirtirsin?

**Ana çıkarım:** Signal gözlemi, crash'in nedenini otomatik kanıtlamaz; statik kontrol akışı + GDB backtrace + runtime evidence birlikte yorumlanır.
