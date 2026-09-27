# Gün 19 — Function Pointer'lara Dönüş: Callback ve Indirect Control Flow

[🇬🇧 English](notes.md) | [🇹🇷 Türkçe](notes.tr.md)

## Amaç

Day 13'te öğrendiğimiz function pointer kavramını gerçek kullanım nedenlerinden biri olan **callback** ile genişletmek. Reverse engineering tarafındaki ana konu indirect control flow: call hedefinin sabit bir function adresi yerine register veya memory içindeki bir değerden gelmesi.

## Kaynak

`source/callbacks.c` iki işlem tanımlar ve bunlardan birini `calculate` fonksiyonuna gönderir.

Önce:

```bash
gcc -g -O0 source/callbacks.c -o callbacks
```

Sonra optimized build:

```bash
gcc -g -O2 source/callbacks.c -o callbacks_O2
```

Optimization gördüğün yapıyı ciddi biçimde değiştirebilir.

## 1. Direct call

```c
add(6, 7);
```

gibi normal bir call'ın hedefi bellidir.

Kavramsal assembly:

```asm
call add
```

## 2. Indirect call

`calculate` içinde:

```c
return operation(x, y);
```

`operation` bir function pointer'dır. Program bir function adresini alır ve o değer üzerinden call yapar.

Şuna benzer bir yapı görebilirsin:

```asm
call rax
```

Exact register'ı ezberleme. Pattern:

```text
function adresi bir yerde tutulur
        ↓
değer call site'a gelir
        ↓
indirect CALL
        ↓
seçilen function çalışır
```

## 3. Callback mantığı

Callback, bir fonksiyonun daha sonra çağırmak üzere başka bir fonksiyon almasıdır.

```text
main
  ↓
add seçilir
  ↓
calculate(add, 6, 7)
  ↓
operation -> add
  ↓
indirect call
  ↓
13
```

Event handler, sorting API, plugin system gibi yapılarda sık görülür.

## 4. Reverse engineering açısından önemi

Direct call kolaydır çünkü destination instruction içinde açıktır.

Indirect call'da şunları araştırırsın:

- Pointer nereden geldi?
- Hangi function'ları gösterebilir?
- Local variable, global table, object veya structure içinden mi geldi?
- Aynı call site'ın birden fazla olası hedefi var mı?

Bu, **control-flow recovery** mantığının başlangıcıdır.

## 5. Ghidra alıştırması

Önce `-O0` binary'sini aç.

Bul:

- `main`
- `calculate`
- `add`
- `multiply`

`calculate` içinde function-pointer parametresini ve indirect call'ı belirle.

Sonra geriye doğru takip et:

```text
indirect call
    ↑
function-pointer variable
    ↑
calculate'a gönderilen argument
    ↑
add veya multiply
```

Generic variable isimlerini yeniden adlandır.

## 6. Function adresleri

`main`, seçilen function'ın adresini bir şekilde argument olarak geçirmek zorundadır.

Binary'ye göre `LEA`, register, stack variable veya relocation görebilirsin.

Tek instruction pattern'i ezberlemek yerine şu soruyu sor:

> Hangi değer sonunda indirect call'ın target'ı oluyor?

## 7. -O0 ve -O2 karşılaştırması

Optimization:

- function inline edebilir,
- sonucu belli branch'i kaldırabilir,
- constant propagation yapabilir,
- callback yolunu tamamen yok edebilir,
- register ve stack kullanımını değiştirebilir.

Kaynakta:

```c
int choice = 1;
```

olduğu için `-O2` compiler sonucu önceden çıkarabilir.

Önemli ders: **source yapısı ile binary yapısı birebir olmak zorunda değildir.**

## 8. Static analysis yöntemi

```asm
call rax
```

gördüğünde sadece bu instruction'a bakarak hangi function'ın çağrıldığını söyleyemezsin.

Data flow gerekir:

```text
indirect call'ı bul
      ↓
target register/memory'yi belirle
      ↓
değerin kaynağını geriye takip et
      ↓
olası destination'ları çıkar
      ↓
davranışı yorumla
```

## 9. Güvenlik bağlantısı

Indirect call normal yazılımlarda çok yaygındır ve tek başına şüpheli değildir.

Malware analysis ve vulnerability research tarafında ise callback, dispatch table, virtual function, imported function pointer veya dynamically resolved API arkasındaki davranışı anlamak için indirect control flow önemlidir.

## Alıştırmalar

1. `-O0` ile derle ve `calculate` içindeki indirect call'ı bul.
2. Callback adresini hangi argument'ın taşıdığını belirle.
3. Callback'i geriye doğru `add` fonksiyonuna kadar takip et.
4. `choice` değerini `0` yapıp davranışı karşılaştır.
5. `-O2` ile derleyip decompiler çıktısını karşılaştır.
6. Optimized binary'de hangi source function'ların ayrı function olarak kaldığını yaz.

## Sorular

1. Direct ve indirect call farkı nedir?
2. Callback nedir?
3. `call rax`, `call add` yapısından neden daha zor analiz edilir?
4. Indirect call target'ını bulmak için hangi bilgi gerekir?
5. `-O2` binary'yi neden source'dan çok farklı gösterebilir?
6. Indirect call malware göstergesi midir?

## Ana çıkarım

```text
pointer value
    ↓
data flow
    ↓
indirect call
    ↓
possible targets
    ↓
program behavior
```

Indirect call gördüğünde sadece instruction okumak yerine **target değerinin nereden geldiğini** takip etmeye başla.
