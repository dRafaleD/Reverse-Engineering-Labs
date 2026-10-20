#include <stdio.h>
__attribute__((visibility("default"))) int public_add(int n){ return n + 7; }
__attribute__((visibility("hidden"))) int internal_add(int n){ return n + 3; }
int main(void){ printf("sum=%d\n", public_add(5) + internal_add(5)); return 0; }
