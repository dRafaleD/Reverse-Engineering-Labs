# Gün 26 — Process Oluşturma, fork/exec, waitpid ve Çalışma Zamanı İzleme

[English](notes.md) · [Türkçe](notes.tr.md)

## Hedefler

Bu günde birbiriyle bağlantılı **dört beceriyi** birlikte öğreneceksin: (1) process kimliği ve parent/child ilişkisi, (2) `fork()`, `exec()`, `waitpid()`, (3) Ghidra ile statik analiz ve `strace`/GDB ile dinamik gözlem, (4) `/proc` üzerinden delil toplama ve program argümanlarının güvenlik etkisi. Gün 24'teki syscall ve Gün 25'teki ELF loader bilgisinin devamı.

**Ortam:** Linux VM'de kendi derlediğin zararsız program. Malware veya üçüncü taraf sisteme müdahale yok.

## 1. Executable, process ve thread aynı değildir

Diskteki `day26` bir executable dosyasıdır. Çalışan **process** ise PID, sanal adres alanı, dosya tanıtıcıları (FD), kimlik bilgileri ve execution state taşır. Aynı executable birden çok process tarafından çalıştırılabilir. Thread'ler bazı kaynakları paylaşır ama bu labın odağı ayrı **process** oluşturmaktır.

```text
Parent (PID P) -- fork --> Child (PID C)
Parent bekler                    |
                                 +-- exec --> /bin/echo (PID C değişmez)
                                 |
                               process biter
Parent waitpid döner  <-----------+
```

PID sayılarını kod değil işletim sistemi seçer.

## 2. fork(): iki ayrı yürütme yolu

Kod `fork()` çağırır. Başarılı olunca aynı satırdan sonra iki process devam eder:

- **Parent:** dönüş değeri pozitif child PID.
- **Child:** dönüş değeri sıfır.
- **Hata:** mevcut process'te `-1`.

Child'ın kendine ait sanal adres alanı oluşur. Başlangıç içeriği parent ile aynı görünebilir; ancak bu iki process'in belleği mantıksal olarak ayrıdır. Linux genellikle **copy-on-write** uygular: her sayfa hemen fiziksel olarak kopyalanmak zorunda değildir.

### stdout neden tekrar basılabilir?

`fork` öncesi buffer'a alınmış çıktı iki process tarafından sonradan yazdırılabilir. Bu hedefte `setvbuf(...,_IONBF,...)` ile buffer davranışı sadeleştirildi.

## 3. exec(): yeni PID değil, yeni program image

Child `execl("/bin/echo", ...)` çağırıyor. `execl`, libc'nin exec ailesinden bir fonksiyondur; Linux'ta programı değiştirmek için `execve` gibi kernel mekanizmalarına dayanır.

Başarılı **exec yeni process oluşturmaz**: child aynı PID ile `/bin/echo` kodunu çalıştırır. Eski executable'ın user-space image'ı yerini yenisine bırakır.

| İşlem | Yeni process? | PID | Başarılı olunca döner mi? |
| --- | --- | --- | --- |
| fork | Evet | Child yeni PID alır | Parent ve child'da evet |
| exec | Hayır | Aynı kalır | **Hayır** |
| waitpid | Hayır | Parent'ın PID'si aynı | Child durumu değişince evet |

Bazı FD'ler exec sonrasında da kalabilir; `close-on-exec` işaretliler kapanır. Dolayısıyla exec'in bütün process durumunu sıfırladığını düşünme.

## 4. waitpid(): child'ın sonucunu güvenli yorumlamak

Parent `waitpid(child, &status, 0)` ile belirli child'ın bitmesini bekleyip son durumunu alır; bu labda zombie'nin temizlenmesini de sağlar.

- `WIFEXITED`: normal exit olmuş mu?
- `WEXITSTATUS`: yalnızca WIFEXITED doğruysa normal exit kodunu al.
- `WIFSIGNALED`: sinyalle mi sonlandı?
- `WTERMSIG`: yalnızca sinyalle bittiği durumda ilgili sinyali öğren.

Exit code 0 görmek, uygulamanın bütün güvenlik özelliklerinin doğru olduğu anlamına gelmez.

## 5. Derle ve çalıştır

Bu günün klasöründe:

```bash
gcc -std=c11 -g -O0 -Wall -Wextra -Wpedantic \
  source/process_lifecycle.c -o day26
./day26
```

Beklenen çıktı biçimi (PID'ler değişir):

```text
PARENT before fork: pid=...
PARENT: child pid=...; waiting
CHILD before exec: pid=... ppid=...
DAY26_CHILD_EXEC_OK
PARENT: exit status=0
```

Parent ve child'ın ilk mesajları scheduler nedeniyle farklı sıraya girebilir. Parent'ın son durum çıktısı child tamamlandıktan sonra gelir.

## 6. Önce statik hipotez: Ghidra

```bash
readelf -Ws day26 | grep -E 'fork|execl|waitpid|getpid|sleep'
objdump -d day26 | less
```

Ghidra'da `main` fonksiyonunu aç; `fork` sonrası sıfır karşılaştırmasını, child'daki `sleep` ve `execl` çağrısını, parent'daki `waitpid` ve status kontrolünü işaretle.

**Önemli fark:** Imported symbol, o fonksiyonun runtime'da kesin çalıştığını kanıtlamaz. Statik analiz ancak olası davranış hipotezi kurar.

`DAY26_CHILD_EXEC_OK` string'inin XREF'ini izle. String neden orijinal binary'de bulunuyor, fakat çıktı neden başka executable tarafından basılıyor? Bu soruya cevap ver.

## 7. strace: child'ı da takip et

```bash
strace -f -e trace=process ./day26
```

Process başına trace dosyası:

```bash
strace -ff -o day26.trace ./day26
ls day26.trace.*
```

Şunları bul:

- ilk programın `execve` kaydı,
- child oluşturma (`clone` ailesi veya ortamına bağlı `fork`),
- child `execve("/bin/echo", ...)`,
- parent'ın `wait4` ya da benzer bekleme syscall'ı,
- child exit, parent wait sonucu.

Gün 24'teki `open` → `openat` örneğini hatırla: kaynak kodundaki API ile kernel'de görülen syscall adı birebir aynı olmak zorunda değil.

## 8. GDB ile fork ve exec izleme

```bash
gdb ./day26
```

GDB komutları:

```text
set follow-fork-mode child
set detach-on-fork off
catch fork
catch exec
break main
run
info inferiors
continue
```

`info inferiors` ile GDB'nin takip ettiği process'leri incele. Parent/child ayrıldıktan sonra `inferior N`, `bt` ve `info registers` kullan. Exec gerçekleşince child artık farklı programın image'ına sahiptir; memory mapping ve breakpoint davranışlarını karşılaştır.

## 9. /proc: zamanlama farkını unutma

Child exec öncesi 2 saniye beklediği için başka terminalden bakabilirsin:

```bash
ps -eo pid,ppid,stat,comm | grep -E 'day26|echo'
cat /proc/<CHILD_PID>/status
readlink /proc/<CHILD_PID>/exe
tr '\0' ' ' < /proc/<CHILD_PID>/cmdline
```

`<CHILD_PID>` kısmına gerçek PID yaz. Process kısa sürede kapanabilir; `/proc` kaydı yoksa bu çoğu zaman bir **race/timing** meselesidir. Kendi gözlemin yoksa varmış gibi yazma.

### Güvenlik bağlantısı

Komut satırı ve environment bilgileri bazı sistemlerde process inspection ile görülebilir. Labda gerçek şifre, token veya gizli bilgi kullanma.

## 10. Day 25 ile bağlantı

Child exec öncesi `day26` image'ını çalıştırır; exec sonrası `/bin/echo` ve onun ELF loader/shared-library gereksinimleri devreye girer. `readelf -l /bin/echo` ve `info proc mappings` ile eski/yeni image'ı karşılaştır.

```text
Ghidra -> olası kontrol akışı
strace -> gerçekleşen syscall'lar
GDB /proc -> o andaki process durumları
waitpid -> sonlanma bilgisi
```

Bir araç bütün hikâyeyi tek başına vermez.

## 11. Sorun giderme

- Derleme hatası: Linux/GCC ve POSIX header'ları kontrol et.
- `strace` yoksa: VM'nin paket yöneticisi üzerinden yükle.
- Ptrace izin hatası: VM/container debugging izinlerini kontrol et; laboratuvar için üretim sisteminin güvenlik ayarlarını rastgele değiştirme.
- Child `ps` ile yakalanmıyor: uyku süresini çalışma kopyanda artır veya GDB'de durdur.
- `execl` hata veriyor: `/bin/echo` var mı bak; hata yolunda `perror` ve exit 127 görülür.

## 12. Mini challenge'lar

1. **Hata yolu:** Çalışma kopyasında exec yolunu bulunmayan bir dosyaya çevir. `perror` neden yalnızca başarısızlıkta çalışıyor? Parent neden 127 görüyor?
2. **Aynı PID, farklı program:** GDB ile exec öncesi/sonrası `/proc/<PID>/exe` değerini kontrol et; PID'nin neden değişmediğini anlat.
3. **FD mirası:** Zararsız bir dosyayı aç; normal FD ve `FD_CLOEXEC` işaretli FD'nin exec sonrasındaki durumunu karşılaştır.
4. **Process tree:** Parent/child çizimini yap. Her log satırını hangi process basıyor, belirt.

## Alıştırmalar

1. Warning flag'leriyle derle.
2. Fork sonrası kaç process olduğunu tahmin et.
3. Beş kere çalıştır; PID/sıra değişimini yaz.
4. Ghidra'da fork branch'ini bul.
5. execl argument'larını izle.
6. waitpid result yorumunu incele.
7. `strace -f` kullan.
8. `-ff` ile ayrı trace üret.
9. Exec boyunca PID'yi takip et.
10. Day 25 ELF loading ile ilişkilendir.
11. Stdout buffer meselesini açıkla.
12. Statik-dinamik karşılaştırmalı kısa rapor yaz.

## Tekrar soruları

1. Executable ile process farkı ne?
2. fork dönüş değeri parent/child'da ne?
3. exec yeni process oluşturur mu?
4. Exec sonrası PID ne olur?
5. Copy-on-write neden kullanılır?
6. execl ne zaman return eder?
7. waitpid neyi sağlar?
8. WIFEXITED kontrolü neden gerekli?
9. strace'de fork neden clone görünebilir?
10. GDB child'ı nasıl takip eder?
11. /proc gözlemi neden timing'e bağlı?
12. Import edilen API runtime kanıtı mıdır?
13. Exec sonrası FD'lere ne olur?
14. Argüman/environment'ta secret neden saklanmaz?
15. Process analizi için hangi ek deliller gerekir?

## Ana çıkarım

**fork yeni process oluşturur; exec mevcut process'in program image'ını değiştirir; waitpid child'ın son durumunu alır ve temizler.** Ghidra'da kurduğun hipotezi strace/GDB ile doğrulamaya çalış.
