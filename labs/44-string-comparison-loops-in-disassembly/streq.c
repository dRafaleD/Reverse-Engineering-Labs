#include <stdio.h>
__attribute__((noinline)) int same(const char *a, const char *b){while(*a && *a==*b){a++;b++;}return *a==*b;}
int main(void){printf("%d %d\n",same("lab","lab"),same("lab","lad"));return 0;}
