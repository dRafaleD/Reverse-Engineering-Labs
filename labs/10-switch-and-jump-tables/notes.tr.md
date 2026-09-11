# Gün 10 — Switch Yapıları ve Jump Table'lar

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

C dilindeki `switch` yapısının derlenmiş bir binary içinde nasıl görünebildiğini anlamak ve compare/jump zinciri ile jump table mantığına giriş yapmak.

## Kaynak kod

```c
#include <stdio.h>

int main() {
    int choice = 4;

    switch (choice) {
        case 1:
            printf("Option 1\n");
            break;
        case 2:
            printf("Option 2\n");
            break;
        case 3:
            printf("Option 3\n");
            break;
        case 4:
            printf("Option 4\n");
            break;
        case 5:
            printf("Option 5\n");
            break;
        case 6:
            printf("Option 6\n");
            break;
        default:
            printf("Unknown option\n");
            break;
    }

    return 0;
}
```

## Derleme

Normal debug derlemesi:

```bash
gcc -g switch.c -o switch
```

Karşılaştırmak için optimize edilmiş sürüm:

```bash
gcc -O2 -g switch.c -o switch_O2
```

İki binary'yi de Ghidra'ya açıp `main` fonksiyonlarını karşılaştırabilirsin.

## Switch ne yapar?

Kaynak kod seviyesinde `switch`, bir değişkenin değerine göre birden fazla yoldan birini seçer.

```text
choice = 4
    ↓
switch(choice)
    ↓
case 4
```

Derleyicinin kaynak koddaki `switch` görünümünü birebir koruması gerekmez. Sadece programın aynı davranışı göstermesi gerekir.

## Pattern 1 — Compare ve jump zinciri

Derleyici switch'i bir dizi karşılaştırmaya çevirebilir:

```asm
CMP value, 1
JE  case_1
CMP value, 2
JE  case_2
CMP value, 3
JE  case_3
...
JMP default
```

Bu yapı mantık olarak birden fazla `if / else if` kontrolüne benzer.

Tanımaya başlaman gereken instruction'lar:

```text
CMP  -> değerleri karşılaştırır
JE   -> eşitse jump yapar
JNE  -> eşit değilse jump yapar
JMP  -> koşulsuz jump yapar
```

## Pattern 2 — Jump table

Case değerleri birbirine yakın ve yeterince fazlaysa, özellikle optimization açıkken derleyici her case'i tek tek karşılaştırmak yerine jump table kullanabilir.

Mantık kabaca şöyledir:

```text
case değeri
    ↓
aralık kontrolü
    ↓
değeri index olarak kullan
    ↓
tablodan hedef adresi oku
    ↓
ilgili case bloğuna jump yap
```

Basitleştirilmiş düşünce modeli:

```text
jump_table[0] -> case 1
jump_table[1] -> case 2
jump_table[2] -> case 3
jump_table[3] -> case 4
```

Ancak her `switch` kesinlikle jump table'a dönüşmez. Sonuç; derleyiciye, optimization seviyesine, mimariye ve kodun yapısına göre değişebilir.

## Ghidra'da ne aramalısın?

Fonksiyonu incelerken şunlara dikkat et:

- farklı bloklara giden birden fazla conditional branch,
- case'lerden önce yapılan range check,
- index kullanılan memory access,
- indirect jump,
- farklı case bloklarının daha sonra ortak bir çıkış noktasında birleşmesi.

Ghidra'nın Decompiler paneli bu karmaşık control flow'u tekrar okunabilir bir `switch` yapısı olarak gösterebilir. Listing tarafında ise daha düşük seviyeli jump ve adres işlemlerini görürsün.

## Reverse engineering açısından neden önemli?

Kaynak kod olmadığında switch benzeri yapıları tanımak şu tür mantıkları anlamayı kolaylaştırabilir:

- komut işleyicileri,
- menü mantığı,
- state machine'ler,
- protokol mesaj tipleri,
- enum tabanlı kararlar,
- opcode dispatch yapıları.

Buradaki amaç tek bir assembly patternini ezberlemek değil. Bir değerin birden fazla olası kod yolundan birini seçtiğini fark etmeyi öğrenmek.

## Ana çıkarım

```text
C switch
   ↓
derleyici
   ↓
compare/jump zinciri VEYA indexed table
   ↓
birden fazla control-flow yolu
```

Decompiler temiz bir `switch` gösterebilir ama binary seviyesinde bunun uygulanışı kaynak koddan oldukça farklı olabilir.
