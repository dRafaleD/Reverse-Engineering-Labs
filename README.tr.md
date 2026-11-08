# Reverse-Engineering-Labs

[🇬🇧 English](README.md) | [🇹🇷 Türkçe](README.tr.md)

Temelden başlayan bir reverse engineering öğrenme deposu.

Bu repository, reverse engineering öğrenme sürecimi en temel konulardan başlayarak belgelemek için hazırlanmıştır.

Amaç; C, assembly, bellek, binary formatları, debugging ve statik/dinamik analiz gibi konuları inceleyerek derlenmiş programların içeride nasıl çalıştığını anlamaktır.

## Lablar

- [Gün 1 — C Temelleri](labs/01-c-fundamentals/notes.tr.md)
- [Gün 2 — İlk Ghidra Analizi](labs/02-first-ghidra-analysis/notes.tr.md)
- [Gün 3 — C Değişkenlerinin Binary İçindeki Görünümü](labs/03-c-variables-in-binary/notes.tr.md)
- [Gün 4 — C Fonksiyonları ve Calling Convention](labs/04-functions-and-calling-convention/notes.tr.md)
- [Gün 5 — Stack Frame ve Local Değişkenler](labs/05-stack-frames-and-locals/notes.tr.md)
- [Gün 6 — Koşullar ve Conditional Jump'lar](labs/06-conditions-and-conditional-jumps/notes.tr.md)
- [Gün 7 — Döngüler ve Geriye Jump'lar](labs/07-loops-and-backward-jumps/notes.tr.md)
- [Gün 8 — Diziler ve Pointer Temelleri](labs/08-arrays-and-pointers/notes.tr.md)
- [Gün 9 — Struct'lar ve Bellek Düzeni](labs/09-structs-and-memory-layout/notes.tr.md)
- [Gün 10 — Switch Yapıları ve Jump Table'lar](labs/10-switch-and-jump-tables/notes.tr.md)
- [Gün 11 — String'ler ve Char Dizileri](labs/11-strings-and-char-arrays/notes.tr.md)
- [Gün 12 — malloc ve Heap Temelleri](labs/12-malloc-and-heap-basics/notes.tr.md)
- [Gün 13 — Function Pointer'lar ve Indirect Call'lar](labs/13-function-pointers-and-indirect-calls/notes.tr.md)
- [Gün 14 — Global ve Static Değişkenler](labs/14-global-and-static-variables/notes.tr.md)
- [Gün 15 — Signed ve Unsigned Integer'lar](labs/15-signed-and-unsigned-integers/notes.tr.md)
- [Gün 16 — Recursion ve İç İçe Stack Frame'ler](labs/16-recursion-and-nested-stack-frames/notes.tr.md)
- [Gün 17 — Bitwise İşlemler ve Flag'ler](labs/17-bitwise-operations-and-flags/notes.tr.md)
- [Gün 18 — Library Call'ları, PLT/GOT ve String Karşılaştırma](labs/18-library-calls-plt-got/notes.tr.md)
- [Gün 19 — Function Pointer'lara Dönüş: Callback ve Indirect Control Flow](labs/19-callbacks-and-indirect-control-flow/notes.tr.md)
- [Gün 20 — ELF Section'ları, Symbol'lar ve Stripped Binary'ler](labs/20-elf-sections-symbols-and-stripping/notes.tr.md)
- [Gün 21 — GDB Temelleri: Breakpoint, Register ve Adım Adım Çalışma](labs/21-gdb-breakpoints-registers-and-stepping/notes.tr.md)
- [Gün 22 — Process Memory Layout, Virtual Memory, ASLR ve /proc Maps](labs/22-process-memory-layout-aslr-and-proc-maps/notes.tr.md)
- [Gün 23 — Validation Logic, Data Flow ve Ghidra + GDB ile Branch Tracing](labs/23-validation-data-flow-and-branch-tracing/notes.tr.md)
- [Gün 24 — System Call, libc Wrapper, strace ve ltrace](labs/24-syscalls-libc-strace-and-ltrace/notes.tr.md)
- [Gün 25 — Shared Library, Dynamic Linking, Loader Resolution ve LD_DEBUG](labs/25-shared-libraries-dynamic-linking-and-loader/notes.tr.md)
- [Gün 26 — Process Oluşturma, fork/exec, waitpid ve Çalışma Zamanı İzleme](labs/26-process-creation-fork-exec-and-tracing/notes.tr.md)
- [Gün 27 — Signal, Crash Analizi, Core Dump ve GDB Backtrace](labs/27-signals-crash-triage-and-gdb/notes.tr.md)
- [Gün 28 — Dosya Formatları, Magic Byte, Endianness ve Parser İzleme](labs/28-file-formats-magic-bytes-and-parser-tracing/notes.tr.md)
- [Gün 29 — Derleyici Optimizasyonu, Inlining ve Kontrol Akışı](labs/29-compiler-optimization-and-control-flow/notes.tr.md)
- [Gün 30 — ELF Relocation, Semboller ve Çalışma Zamanında Çözümleme](labs/30-elf-relocations-and-symbol-resolution/notes.tr.md)
- [Gün 31 — Thread, Thread-Local Storage ve GDB ile Thread İzleme](labs/31-31-elf-threading-tls-and-debugger-tracing/notes.tr.md)
- [Gün 32 — Fonksiyon Girişleri, Stack Unwinding ve Backtrace Güvenilirliği](labs/32-function-prologues-unwinding-and-backtraces/notes.tr.md)
- [Gün 33 — Dinamik Yükleme, dlopen ve Sembol Çözümleme](labs/33-dynamic-loading-and-dlsym/notes.tr.md)
- [Gün 34 — ELF Sembol Görünürlüğü ve Paylaşılan Kütüphane ABI](labs/34-elf-visibility-and-symbol-interposition/notes.tr.md)
- [Gün 35 — ELF Sertleştirme: PIE, NX, RELRO ve Stack Canary](labs/35-binary-hardening-and-elf-security-flags/notes.tr.md)
- [Gün 36 — ELF Build ID, Debug Sembolleri ve Tekrarlanabilir Analiz](labs/36-elf-notes-build-id-and-debug-symbols/notes.tr.md)
- [Gün 37 — Calling Conventions And Argument Tracing](labs/37-calling-conventions-and-argument-tracing/notes.tr.md)
- [Gün 38 — Stack Frames And Local Variable Layout](labs/38-stack-frames-and-local-variable-layout/notes.tr.md)
- [Ek çalışma — Gün 38 — Stack Frames And Local Variable Lifetimes](labs/38-stack-frames-and-local-variable-lifetimes/notes.tr.md)
- [Gün 39 — Conditionals And Branches In Disassembly](labs/39-conditionals-and-branches-in-disassembly/notes.tr.md)
- [Gün 40 — Return Values And The RAX Register](labs/40-return-values-and-the-rax-register/notes.tr.md)
- [Gün 41 — Loops And Induction Variables In Disassembly](labs/41-loops-and-induction-variables-in-disassembly/notes.tr.md)
- [Gün 42 — Function Pointers And Indirect Calls](labs/42-function-pointers-and-indirect-calls/notes.tr.md)
- [Gün 43 — Struct Return Values RAX RDX And Hidden Pointers](labs/43-struct-return-values-rax-rdx-and-hidden-pointers/notes.tr.md)
- [Gün 44 — String Comparison Loops In Disassembly](labs/44-string-comparison-loops-in-disassembly/notes.tr.md)
- [Gün 45 — Stack Protector Canary In Prologue And Epilogue](labs/45-stack-protector-canary-in-prologue-and-epilogue/notes.tr.md)
- [Gün 46 — Switch Statements Jump Tables Versus Compare Chains](labs/46-switch-statements-jump-tables-versus-compare-chains/notes.tr.md)
- [Gün 47 — Division By Constants And Magic Multiplication](labs/47-division-by-constants-and-magic-multiplication/notes.tr.md)
- [Gün 48 — Bounds Checks And Signed Versus Unsigned Compares](labs/48-bounds-checks-and-signed-versus-unsigned-compares/notes.tr.md)
- [Gün 49 — Tail Calls And Jmp Instead Of Call](labs/49-tail-calls-and-jmp-instead-of-call/notes.tr.md)
- [Gün 50 — Variadic Calls And The Al Register](labs/50-variadic-calls-and-the-al-register/notes.tr.md)
- [Gün 51 — String Literals And The Rodata Section](labs/51-string-literals-and-the-rodata-section/notes.tr.md)
- [Gün 52 — Binary Dizgeleri ve Çapraz Referanslar](labs/52-binary-string-references-and-xrefs/notes.tr.md)
- [Gün 53 — String Karşılaştırmaları ve Çalışma Zamanı Dal Kanıtı](labs/53-string-comparisons-and-runtime-branch-evidence/notes.tr.md)

## Reverse Engineering Nedir?

Reverse engineering, kaynak koduna erişmeden derlenmiş bir programın yapısını, davranışını ve iç mantığını analiz ederek anlamaya çalışma sürecidir.

Bu repository içinde zamanla şu konular yer alacak:

- C ve derlenmiş programlar
- x86/x86-64 assembly
- Registerlar ve CPU instruction'ları
- Stack ve heap belleği
- Fonksiyon çağrıları ve calling convention'lar
- ELF ve PE binary formatları
- Statik analiz
- Dinamik analiz
- Debugging
- Ghidra ve GDB

Bu repo hem kendi tekrar notlarım hem de konuya ilgi duyanlar için basit bir öğrenme rehberi olarak kullanılabilir.
