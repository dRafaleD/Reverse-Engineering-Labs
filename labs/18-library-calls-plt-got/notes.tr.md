# Gün 18 — Library Call'ları, PLT/GOT ve String Karşılaştırma

## Amaç
Tanıdığın C library fonksiyonlarının dynamically linked bir Linux binary'sinde Ghidra üzerinden nasıl göründüğünü anlamak. Ana hedef `strcmp` ve `strlen` gibi imported function'ları tanımak, call'ları takip etmek ve PLT/GOT'un temel amacını loader detaylarını ezberlemeden öğrenmek.

## Kaynak kod
Program kısa bir kelime alır, `"reverse"` ile karşılaştırır, eşleşme durumunu ve uzunluğu yazdırır.

Derle:
```bash
gcc -g source/library_calls.c -o library_calls
```

İnceleme:
```bash
file library_calls
ldd library_calls
objdump -T library_calls | grep -E 'strcmp|strlen|puts|printf'
```

## 1. Internal ve imported function
Binary kendi fonksiyonlarını içerebilir ve shared library'lerden fonksiyon kullanabilir. Dynamically linked build'de `strcmp`, `strlen`, `printf` ve `puts` normalde libc'den çözülür.

Import isimleri reverse engineering sırasında güçlü ipuçlarıdır. `strcmp` karşılaştırma, `strlen` string uzunluğu işlemi olduğunu düşündürür.

## 2. PLT ve GOT nedir?
ELF sistemlerinde dynamically linked call'larda **Procedure Linkage Table (PLT)** ve **Global Offset Table (GOT)** sık görülür.

Başlangıç modeli:
```text
program code
   ↓ CALL
PLT entry
   ↓
GOT / dynamic linker bilgisi
   ↓
shared-library function
```

Gerçek çıktı compiler, linker, architecture ve seçeneklere göre değişebilir.

## 3. Ghidra alıştırması
Binary'yi aç ve `main` fonksiyonunu bul.

Şunları belirlemeye çalış:
- stack üzerindeki local `input` array,
- `"reverse"` string'i,
- `strcmp` ile ilgili call,
- karşılaştırmadan sonraki branch,
- `puts`/`printf` call'ları,
- `strlen` ile ilgili call.

`"reverse"` string'inin reference'ını takip et. Bilinmeyen binary'de string reference'ları assembly'yi baştan sona okumaktan daha kolay başlangıç noktaları olabilir.

## 4. Comparison pattern
Kaynak:
```c
if (strcmp(input, "reverse") == 0)
```

Decompiler'da kavramsal olarak:
```text
result = strcmp(input, "reverse")
if result != 0 -> no-match
else           -> match
```

`strcmp` sıfır döndürüyorsa string'ler eşittir.

## 5. Assembly pattern
Build'e göre şuna benzer bir yapı görebilirsin:
```asm
CALL strcmp
TEST EAX,EAX
JNZ  no_match
```

Birebir instruction ezberleme. Pattern'i öğren:
```text
comparison function çağır
      ↓
return value kontrol et
      ↓
conditional branch
```

## 6. Importlar neden önemli?
Bütün instruction'ları anlamadan davranış hakkında ipucu verirler.

- `strcmp` -> string karşılaştırma
- `strlen` -> string uzunluğu
- `malloc` -> heap allocation
- `fopen` -> dosya açma
- `socket` -> socket oluşturma

Import bir ipucudur; tek başına zararlı veya zararsız davranış kanıtı değildir.

## Alıştırmalar
1. `main`i bul ve anlaşılmayan local variable'ları yeniden adlandır.
2. `"reverse"` string'ini bul ve cross-reference'ını takip et.
3. Comparison call ve ardından gelen conditional branch'i bul.
4. `strlen` call'ını ve return value'nun nerede kullanıldığını belirle.
5. Ghidra decompiler çıktısını orijinal C ile karşılaştır.
6. Importları inceleyip tanıdığın fonksiyonların neye işaret ettiğini yaz.

## Sorular
1. Imported function isimleri neden faydalıdır?
2. İki string eşitse `strcmp` ne döndürür?
3. Başlangıç seviyesinde PLT/GOT'un amacı nedir?
4. String cross-reference neden kullanışlıdır?
5. Compiler/linker çıktısı neden örnekten farklı olabilir?

## Ana çıkarım
```text
strings/imports
     ↓
cross-references
     ↓
interesting calls
     ↓
return-value checks
     ↓
branches and behavior
```

Bu yöntem assembly'yi yukarıdan aşağı her satırı çözmeye çalışmaktan daha kullanışlıdır.
