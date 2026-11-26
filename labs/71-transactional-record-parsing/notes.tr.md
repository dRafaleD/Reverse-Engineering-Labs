# Gün 71 — İşlemsel Kayıt Ayrıştırma ve Çıktı Bütünlüğü

[English](notes.md) · [Türkçe](notes.tr.md) · [Code](transactional_parser.c)

70. günde kayıt geçişlerini inceledik. Bugün ilk baytı kayıt sayısı, devamı tür/değer çiftleri olan hayalî bir format kullanıyoruz. Geçici özet ancak bütün kayıtlar geçerliyse çıktı değişkenine yazılır. Son kayıttaki hata önceki başarılı kayıtları kısmen yayınlamaz. 11 senaryo boş/maksimum kayıt, eksik/fazla bayt, geçersiz değer ve null kontrolünü kapsar. GDB'de candidate ve out değişkenlerini karşılaştır. Geçerli iki kayıt (2,30), sonradan hatalı kayıt ise değişmeyen (99,999) sonucunu vermeli. Sıfır değerli yeni geçerli kayıt ekle. Bu yerel, tek iş parçacıklı eğitim örneğidir.

## Çalıştır
```bash
cd labs/71-transactional-record-parsing
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror -g -O0 transactional_parser.c -o parser
./parser
```
