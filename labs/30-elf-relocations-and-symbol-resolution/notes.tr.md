# Gün 30 — ELF Relocation, Semboller ve Çalışma Zamanında Çözümleme

[English](notes.md) | [Türkçe](notes.tr.md)

## Amaç

ELF Relocation, Semboller ve Çalışma Zamanında Çözümleme konusunu gerçek sistemlere dokunmadan, yerel ve zararsız örneklerle çalışmak. Hedef yalnızca komutları ezberlemek değil; ölçümün neyi gösterdiğini ve neyi kanıtlamadığını anlamaktır.

## Birbiriyle bağlantılı konular

1. **symbol tables**: Bu kavramın hangi problemi çözdüğünü, veri akışını ve sınırlamalarını açıklayın.
2. **relocations**: Bu kavramın hangi problemi çözdüğünü, veri akışını ve sınırlamalarını açıklayın.
3. **PIE/ASLR**: Bu kavramın hangi problemi çözdüğünü, veri akışını ve sınırlamalarını açıklayın.
4. **PLT/GOT**: Bu kavramın hangi problemi çözdüğünü, veri akışını ve sınırlamalarını açıklayın.
5. **readelf and Ghidra cross-check**: Bu kavramın hangi problemi çözdüğünü, veri akışını ve sınırlamalarını açıklayın.

## Ortamın hazırlanması ve çalışma

`relocation_demo.c` dosyası sentetik eğitim örneğidir. İlgili lab klasöründe çalıştırın:

``bash
gcc -g -O0 -fPIE -pie relocation_demo.c -o demo
```

``bash
readelf -rW demo
```

``bash
readelf -Ws demo
```

``bash
gdb ./demo
```

## Adım adım analiz

1. Çalıştırmadan önce beklediğiniz sonucu not edin.
2. Gerçek çıktıyı ve kullandığınız aracın sürümünü kaydedin.
3. Gözlediğiniz olgular ile tahminlerinizi ayrı yazın.
4. Bir zararsız negatif test ekleyin; sonucu yeniden ölçün.
5. Yanlış pozitif, yanlış negatif ve eksik bağlam ihtimallerini düşünün.
6. Delil niteliğindeki örneği değiştirmeden önce kopyasını alın ve hash kaydedin.
7. Konfigürasyondaki bilgi ile çalışma zamanı sonucunu karşılaştırın.
8. Kesin bildikleriniz, belirsizlikler ve sonraki kontroller için kısa rapor yazın.

## Mini görevler

- **Başlangıç ölçümü:** örneği çalıştırıp çıktı, varsayım ve sınırlamaları belgeleyin.
- **Tek değişken:** zararsız bir işaretçi, log alanı veya ayarı değiştirip etkisini gözleyin.
- **Regresyon:** ilk ayara dönün ve beklenen sonucun korunduğunu doğrulayın.
- **Kanıt yorumu:** tek bir eşleşmenin ya da alarmın neden saldırı kanıtı olmadığını tartışın.

## Tekrar soruları

1. symbol tables nedir, ne sağlar ve tek başına neyi kanıtlamaz?
2. relocations nedir, ne sağlar ve tek başına neyi kanıtlamaz?
3. PIE/ASLR nedir, ne sağlar ve tek başına neyi kanıtlamaz?
4. PLT/GOT nedir, ne sağlar ve tek başına neyi kanıtlamaz?
5. readelf and Ghidra cross-check nedir, ne sağlar ve tek başına neyi kanıtlamaz?
6. Analizin güvenilirliği için hangi ikinci kanıta ihtiyacınız var?
7. Aynı ölçümü başka bir makinede nasıl tekrarlarsınız?

## Güvenlik

Yalnızca kendi örnekleriniz ve açık izinli ortamlar üzerinde çalışın. Sentetik sonuçları gerçek vaka bulgusu gibi sunmayın.