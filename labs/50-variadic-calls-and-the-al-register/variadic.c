#include <stdio.h>
__attribute__((noinline)) void show(int n){printf("value=%d\n",n);}
int main(void){show(7);return 0;}
