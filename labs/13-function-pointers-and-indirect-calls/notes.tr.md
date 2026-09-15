# Gün 13 — Function Pointer'lar ve Indirect Call'lar

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Function pointer'ın ne olduğunu ve derlenmiş binary içinde neden indirect call olarak görünebildiğini anlamak.

## Kaynak kod

```c
#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int main() {
    int (*operation)(int, int) = add;

    int result1 = operation(10, 4);
    printf("Add result: %d\n", result1);

    operation = subtract;

    int result2 = operation(10, 4);
    printf("Subtract result: %d\n", result2);

    return 0;
}
```

## Function pointer nedir?

Normal bir pointer adres tutar. Function pointer ise bir fonksiyonun adresini tutar.

```c
int (*operation)(int, int) = add;
```

Bunu basitçe şöyle düşünebiliriz:

```text
operation
   ↓
add() fonksiyonunun adresi
```

Daha sonra aynı pointer başka bir fonksiyona yönlendirilebilir:

```c
operation = subtract;
```

Artık `subtract()` fonksiyonunun adresini tutar.

## Direct call ve indirect call farkı

Normal bir fonksiyon çağrısında hedef bellidir:

```text
CALL add
```

Function pointer kullanıldığında ise hedef adres bir register veya memory konumundan gelir. Assembly'de kavramsal olarak şöyle görülebilir:

```text
CALL RAX
```

veya:

```text
CALL qword ptr [adres]
```

Buna **indirect call** denir.

Kullanılan register ve instruction sırası compiler'a ve optimizasyon seviyesine göre değişebilir.

## Reverse engineering açısından neden önemli?

Direct call'ları takip etmek kolaydır çünkü hedef fonksiyon doğrudan görünür.

Indirect call gördüğümüzde ekstra bir soru sormamız gerekir:

```text
Bu fonksiyon adresi nereden geldi?
```

Basit analiz akışı:

```text
indirect CALL
    ↓
CALL'ın kullandığı register/pointer'ı bul
    ↓
bu değerin nerede atandığını geriye doğru takip et
    ↓
olası hedef fonksiyon(lar)ı belirle
```

Bu pattern normal yazılımlarda callback'lerde, dispatch table'larda, event handler'larda, plugin sistemlerinde ve object-oriented kodlarda sık görülebilir.

## Ghidra'da neye bakacağız?

İlk inceleme için optimizasyonu kapatarak derlemek daha anlaşılır olur:

```bash
gcc -g -O0 function_pointers.c -o function_pointers
```

Sonra `main`, `add` ve `subtract` fonksiyonlarını incele.

Şunlara bak:

- `add` ve `subtract` adresleri nerede alınıyor?
- function pointer nerede tutuluyor?
- pointer tekrar nerede yükleniyor?
- hedefi register veya pointer üzerinden gelen bir `CALL` var mı?

Şuna benzer bir pattern görebilirsin:

```text
LEA   RAX, [add]
MOV   [local_pointer], RAX
...
MOV   RDX, [local_pointer]
...
CALL  RDX
```

Aynı register isimlerini birebir bekleme.

## Calling convention yine geçerli

Hedef indirect olsa bile fonksiyon argümanları platformun calling convention'ına göre hazırlanır.

Örneğin Linux x86-64 System V'de integer argümanlar genellikle şöyle başlar:

```text
RDI -> ilk argüman
RSI -> ikinci argüman
```

Bu yüzden indirect call'dan önce `10` ve `4` değerlerinin argument register'larına hazırlandığını görebilirsin.

## Reverse engineering bağlantısı

Kaynak kodda:

```c
operation(10, 4);
```

Binary seviyesinde bunu şöyle düşün:

```text
argümanları hazırla
      ↓
fonksiyon adresini yükle
      ↓
o adres üzerinden CALL yap
      ↓
dönüş değerini al
```

Önemli fark şu: `CALL` instruction'ı doğrudan `add` veya `subtract` adını göstermeyebilir.

## Ana çıkarım

```text
function pointer -> fonksiyon adresi tutar
direct CALL      -> hedef doğrudan bellidir
indirect CALL    -> hedef register/memory üzerinden gelir
RE görevi        -> hedef adresin nereden geldiğini takip etmektir
```

Şimdilik tüm indirect-call pattern'larını ezberlemeye çalışma. Amaç, bir `CALL` instruction'ının her zaman sabit bir fonksiyona gitmek zorunda olmadığını anlamak.
