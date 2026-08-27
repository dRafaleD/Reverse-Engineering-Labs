[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

# Gün 2 — İlk Ghidra Analizi

## Amaç

Bu labın amacı Gün 1'de yazılan basit C programından üretilen binary'yi Ghidra ile incelemek ve kaynak koddaki bazı kavramların derlenmiş programda nasıl göründüğünü anlamaktır.

Analizde kullanılan kaynak kod Gün 1'deki `hello.c` dosyasıdır:

```c
#include <stdio.h>

int main() {
    printf("This is my first c code\n");
    return 0;
}
```

Debug bilgileriyle derlemek için:

```bash
gcc -g hello.c -o hello
```

Daha sonra oluşan ELF executable Ghidra'ya import edilir ve `main` fonksiyonu analiz edilir.

## Ghidra'ya İlk Kez Girerken Ne Yapıyoruz?

Ghidra'yı ilk kez açan biri için en önemli nokta şudur: **Ghidra kaynak kodu açan bir editör değildir.** Biz `hello.c` dosyasını değil, GCC'nin ürettiği `hello` executable dosyasını inceliyoruz.

Akış şu şekildedir:

```text
hello.c
   ↓ gcc
hello (ELF binary)
   ↓ Ghidra
Disassembly + decompiler görünümü
```

Yani Ghidra, programın derlenmiş halini anlamaya çalışır. Bu yüzden ekranda gördüğümüz kod, C kaynak kodunun birebir aynısı olmak zorunda değildir.

### 1. Yeni Proje Oluşturma

Ghidra açıldığında:

1. `File -> New Project` seçilir.
2. `Non-Shared Project` seçilir.
3. Proje için bir klasör ve isim belirlenir.

Bu proje sadece analiz ettiğimiz dosyaları ve Ghidra'nın oluşturduğu analiz verilerini düzenli tutmak için kullanılır.

### 2. Binary'yi Import Etme

Projeyi oluşturduktan sonra:

1. `File -> Import File` seçilir.
2. `hello.c` **değil**, derlediğimiz `hello` dosyası seçilir.
3. Ghidra dosyanın ELF olduğunu ve mimari bilgisini otomatik olarak algılar.
4. Varsayılan import ayarlarıyla devam etmek bu lab için yeterlidir.

Buradaki kritik fikir şudur:

> Reverse engineering sırasında genellikle elimizde kaynak kod değil, derlenmiş executable bulunur.

Bu labda kaynak kodu sadece karşılaştırma yapabilmek için biliyoruz.

### 3. CodeBrowser ile Açma

Import edilen binary'ye çift tıklayınca Ghidra'nın **CodeBrowser** ekranı açılır.

İlk açılışta Ghidra genellikle dosyanın henüz analiz edilmediğini söyler ve analiz yapmak isteyip istemediğimizi sorar.

`Yes` seçilir.

### 4. Analyze Ne İşe Yarıyor?

Analyze aşamasında Ghidra binary'yi otomatik olarak inceleyip bazı şeyleri tahmin etmeye çalışır:

- Fonksiyonların nerede başlayıp bittiğini
- String'leri
- Import edilen fonksiyonları
- Kod ile veri arasındaki farkı
- Fonksiyon çağrılarını
- Referansları (XREF)

İlk lab için Analyze penceresindeki varsayılan seçenekleri değiştirmek gerekmez. `Analyze` butonuna basmak yeterlidir.

Ghidra'nın yaptığı analiz kusursuz olmak zorunda değildir. Özellikle daha karmaşık binary'lerde bazı fonksiyonları veya veri yapılarını yanlış yorumlayabilir. Reverse engineering'in önemli bir kısmı da Ghidra'nın tahminlerini kontrol etmektir.

## CodeBrowser Ekranını Tanıyalım

İlk kez açıldığında Ghidra ekranı karmaşık görünebilir. Bu lab için sadece birkaç bölümü bilmek yeterlidir.

### Listing

Ortadaki ana bölüm genellikle **Listing** görünümüdür.

Burada:

- Memory address
- Machine-code byte'ları
- Assembly instruction'ları
- Ghidra'nın eklediği yorumlar ve referanslar

görülür.

Örneğin:

```text
00101159    55             PUSH RBP
0010115a    48 89 e5       MOV  RBP,RSP
```

CPU aslında `55`, `48 89 e5` gibi byte'ları çalıştırır. Ghidra bunları bizim okuyabileceğimiz assembly instruction'larına çevirir.

### Decompiler

Sağ tarafta genellikle **Decompiler** penceresi bulunur.

Decompiler, assembly'yi okuyup C'ye benzeyen daha anlaşılır bir temsil üretmeye çalışır.

Örneğin şuna benzer bir görüntü görebiliriz:

```c
int main(void)
{
    puts("This is my first c code");
    return 0;
}
```

Bu kodun önemli bir özelliği vardır:

> Decompiler çıktısı orijinal kaynak kod değildir.

Ghidra sadece machine code'dan yola çıkarak yüksek seviyeli bir temsil üretir. Bu nedenle değişken isimleri, fonksiyon çağrıları ve kod yapısı kaynak koddan farklı olabilir.

### Symbol Tree

Sol tarafta bulunan **Symbol Tree**, binary içinde Ghidra'nın tanıdığı sembolleri kategoriler halinde gösterir.

Özellikle:

```text
Symbol Tree
└── Functions
    └── main
```

kısmı bu lab için önemlidir.

`main` üzerine çift tıklayarak doğrudan ana fonksiyona gidebiliriz.

Bazı binary'lerde `main` ismi görünmeyebilir. Bunun nedeni binary'nin strip edilmiş olması, debug/sembol bilgilerinin bulunmaması veya Ghidra'nın fonksiyonu henüz doğru şekilde tanımlayamamış olması olabilir.

### Defined Strings

Ghidra binary içindeki string'leri de bulabilir.

`Window -> Defined Strings` üzerinden bulunan string'ler görülebilir.

Burada:

```text
This is my first c code
```

string'ini bulup üzerine çift tıklamak, bizi string'in bellekte bulunduğu yere götürür.

String'e sağ tıklayıp veya XREF bilgilerini kullanarak bu string'in nereden kullanıldığını takip etmek mümkündür. Reverse engineering sırasında string'ler çoğu zaman programın davranışı hakkında hızlı ipuçları verir.

## `main` Fonksiyonunu Bulma

Bu labda en kolay yöntem:

```text
Symbol Tree -> Functions -> main
```

üzerinden `main` fonksiyonuna çift tıklamaktır.

Alternatif olarak `Defined Strings` penceresinden `This is my first c code` string'ini bulup referanslarını takip ederek de `main` fonksiyonuna ulaşabiliriz.

Bu ikinci yöntem ileride çok işimize yarar çünkü gerçek analizlerde fonksiyon isimleri her zaman elimizde olmayabilir.

## Ghidra'da `main` Fonksiyonu

İlgili assembly'nin sadeleştirilmiş hali yaklaşık olarak şöyledir:

```asm
PUSH RBP
MOV  RBP,RSP
LEA  RAX,["This is my first c code"]
MOV  RDI,RAX
CALL puts
MOV  EAX,0x0
POP  RBP
RET
```

## Öğrendiklerim

### Fonksiyon Başlığı

Ghidra fonksiyonu şu şekilde tanır:

```text
int main(void)
```

x86-64 üzerinde dönüş değeri `EAX` register'ı ile ilişkilidir.

### `PUSH RBP`

`RBP` register'ının önceki değerini stack üzerine kaydeder.

Şimdilik bunu fonksiyonun başlangıç hazırlığının bir parçası olarak düşünmek yeterlidir.

### `MOV RBP, RSP`

Mevcut stack pointer olan `RSP` değerini `RBP`'ye kopyalar.

`PUSH RBP` ile birlikte fonksiyon için stack frame hazırlığının bir parçasıdır.

### `LEA RAX, [...]`

`LEA`, **Load Effective Address** anlamına gelir.

Bu programda Ghidra, şu string'in adresinin:

```text
"This is my first c code"
```

`RAX` register'ına yüklendiğini gösterir.

Buradaki önemli fikir, string'in bellekte bir yerde tutulması ve programın doğrudan string yerine onun adresiyle çalışmasıdır.

### `MOV RDI, RAX`

`RAX` içindeki adresi `RDI` register'ına kopyalar.

Linux x86-64 System V calling convention'da `RDI` genellikle bir fonksiyonun ilk argümanını taşır.

Bu nedenle string'in adresi bir sonraki fonksiyon çağrısının ilk argümanı olarak hazırlanır.

### `CALL puts`

Harici `puts()` fonksiyonunu çağırır.

İlginç olan nokta, kaynak kodda:

```c
printf("This is my first c code\n");
```

yazılmış olmasına rağmen derlenmiş binary'de `puts()` çağrısının görülmesidir.

Bu önemli bir reverse engineering dersidir:

> Derlenmiş machine code, kaynak kodun her zaman birebir kopyası değildir.

Compiler, kaynak koddaki işlemleri eşdeğer instruction veya fonksiyon çağrılarına dönüştürebilir ve optimize edebilir.

### `MOV EAX, 0x0`

`EAX` register'ına `0` değerini yerleştirir.

Bu, kaynak koddaki:

```c
return 0;
```

satırına karşılık gelir.

### `POP RBP`

Stack üzerinde saklanan önceki `RBP` değerini geri yükler.

Fonksiyondan çıkmadan önce yapılan temizliğin bir parçasıdır.

### `RET`

Mevcut fonksiyondan çağıran yere geri döner.

Basitçe `main` fonksiyonunun sona erdiğini düşünebiliriz.

## Bu Labda Görülen Registerlar

- `RAX` — genel amaçlı register; fonksiyon dönüş değerleriyle de ilişkilidir (`EAX`, `RAX`'ın alt 32 bitidir)
- `RDI` — Linux x86-64 System V'de genellikle ilk fonksiyon argümanını taşır
- `RSP` — stack pointer
- `RBP` — çoğunlukla stack frame/base referansı olarak kullanılır
- `EAX` — `RAX`'ın alt 32 biti; burada `main` fonksiyonunun integer dönüş değerini tutar

Bunları şu aşamada ezberlemek gerekmiyor. Amaç, tekrar karşılaşıldığında tanımaya başlamaktır.

## Ghidra Kavramları

### Instruction

Instruction, CPU'nun çalıştırdığı tek bir komuttur.

Bu labda görülen örnekler:

```asm
MOV
LEA
CALL
PUSH
POP
RET
```

### Address

Ghidra her instruction'ın yanında bir virtual memory address gösterir. Örneğin:

```text
00101159
```

Bu değer, Ghidra'nın programın memory görünümünde o instruction'ı nereye yerleştirdiğini gösterir.

### Machine-Code Byte'ları

Ghidra ayrıca instruction'ların ham byte karşılıklarını da gösterir.

Örneğin bir assembly instruction'ın yanında şöyle byte'lar görülebilir:

```text
48 89 e5
```

CPU bu encoded byte'ları çalıştırır; Ghidra ise onları insanın okuyabileceği assembly biçimine çevirir.

### XREF

`XREF`, **Cross Reference** anlamına gelir.

Bir fonksiyona, adrese veya veri parçasına binary'nin başka hangi noktalarından referans verildiğini gösterir.

## ELF ve `.fini`

Derlenen Linux executable, ELF (**Executable and Linkable Format**) dosya formatını kullanır.

Bir ELF dosyası yalnızca programcının doğrudan yazdığı instruction'lardan oluşmaz. Metadata, section'lar, runtime kodları, import edilen fonksiyonlar, sabit veriler ve programın yüklenip çalıştırılması için gereken başka bilgiler de içerir.

Bu nedenle Ghidra, C kaynak kodunda doğrudan yazılmamış fonksiyon ve section'lar gösterebilir.

Bunlardan biri:

```text
.fini
```

`.fini`, ELF runtime/toolchain tarafından kullanılan finalization/cleanup kodlarıyla ilişkilidir. İlk lablar için bunun `main()` içinde yazdığımız ana program mantığının bir parçası olmadığını bilmek yeterlidir.

## File Offset ve Virtual Address

**File offset**, bir byte'ın executable dosyanın disk üzerindeki hangi konumunda bulunduğunu söyler.

**Virtual address**, kod veya verinin program çalıştırılmak üzere belleğe map edildikten sonra programın address space'inde nerede göründüğünü söyler.

İki farklı soruya cevap verirler:

```text
File offset     -> Dosyanın içinde nerede?
Virtual address -> Programın bellek görünümünde nerede?
```

## Basitleştirilmiş Program Akışı

`main` fonksiyonunu kabaca şöyle düşünebiliriz:

```text
main başlar
    ↓
stack frame hazırlanır
    ↓
string'in adresi bulunur
    ↓
string adresi ilk argüman register'ına yerleştirilir
    ↓
puts çağrılır
    ↓
dönüş değeri 0 yapılır
    ↓
stack-frame durumu geri yüklenir
    ↓
fonksiyondan dönülür
```

## Yeni Başlayan İçin Mini Ghidra Alıştırması

Bu labı bitirmeden önce doğrudan cevaba bakmadan şunları bulmaya çalış:

1. `Symbol Tree` üzerinden `main` fonksiyonunu bul.
2. Listing ekranında `CALL puts` instruction'ını bul.
3. `Defined Strings` ekranından `This is my first c code` string'ini bul.
4. String'in XREF bilgisini kullanarak hangi kod tarafından kullanıldığını takip et.
5. Decompiler görünümü ile Listing görünümünü yan yana karşılaştır.

Buradaki amaç bütün assembly'yi anlamak değil. Aynı davranışın C, decompiler ve assembly seviyelerinde nasıl farklı göründüğünü fark etmektir.

## Ana Çıkarım

Gün 2'nin en önemli noktası assembly instruction'larını ezberlemek değildir.

Önemli olan şu ilişkiyi anlamaya başlamaktır:

```text
C kaynak kodu
    ↓
Compiler
    ↓
ELF executable
    ↓
Machine-code byte'ları
    ↓
Ghidra disassembly
```

Binary, compiler'ın ürettiği sonuçtur; orijinal C kaynak kodunun satır satır birebir kopyası değildir.
