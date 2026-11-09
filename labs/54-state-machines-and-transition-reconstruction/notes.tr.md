# Gün 54 — Dallardan Durum Makinesi Çıkarma

[English](notes.md) · [Türkçe](notes.tr.md) · [C alıştırması](states.c)

## Amaç
53. günde string karşılaştırmalarını çalışma anındaki dallarla ilişkilendirmiştik. Şimdi derlenmiş küçük bir programdan **sonlu durum makinesini (finite-state machine)** çıkarmayı öğreneceğiz. Bir kod yolunun bulunması, o yolun gerçekten çalıştırıldığı anlamına gelmez.

## Temel mantık
Durum makinesi; **durumlar**, **olaylar** ve **geçiş kurallarından** oluşur. Zararsız örneğimizde üç durum var: `IDLE` (bekleme), `READY` (hazır), `DONE` (tamamlandı).

İzin verilen geçişler:
- `IDLE + START → READY`
- `READY + CONFIRM → DONE`
- `DONE + RESET → IDLE`

Diğer birleşimler reddedilir ve durum değişmez. C kodundaki `Result` hem yeni durumu hem de `accepted` bilgisini taşır. **Durumun değişmemesi tek başına reddedildiğini kanıtlamaz**; gerçek sistemde geçerli ama değişiklik yaratmayan bir işlem de olabilir.

## Uygulama (Linux, GCC, GDB)
Dosyaların bulunduğu klasörde:
```bash
gcc -std=c11 -Wall -Wextra -g -O0 -fno-omit-frame-pointer states.c -o states
./states
gdb -q -batch -ex 'break transition' -ex run -ex 'info args' -ex 'bt' -ex continue -ex quit ./states
objdump -d -M intel states | less
```
İlk `CONFIRM` erken geldiği için reddedilir. Ardından `START`, `CONFIRM` ve `RESET` kabul edilir. Kodda beklenen geçişleri doğrulayan `assert` kontrolleri bulunur.

## Görevler
1. Üç durumlu diyagram çiz ve geçerli okları göster.
2. Ghidra'da `transition` fonksiyonunu aç. Karşılaştırmaları, koşullu dalları ve dönüş verilerini bul. **Jump table** kullanımı derleyiciye bağlıdır.
3. GDB'nin ilk durakta gösterdiği `current` ve `event` değerlerini kaydet. Neden bu gözlem tüm olası davranışı kanıtlamaz?
4. Programı `-O2 -g` ile yeniden derle ve kontrol akışı / değişken görünürlüğünü karşılaştır.
5. Geçersiz bir olay ekleyip `assert` ile test et. Gözlem ve çıkarımı ayrı yaz.

## Güvenlik ve sınırlar
Kimlik doğrulama, oturum ve ağ protokollerinde durum geçişleri önemlidir. Ancak bu örnek gerçek erişim kontrolü değildir. Strip edilmiş dosyalarda sembol adları bulunmayabilir. Yalnızca kendi derlediğin veya inceleme iznin olan programları kullan.
