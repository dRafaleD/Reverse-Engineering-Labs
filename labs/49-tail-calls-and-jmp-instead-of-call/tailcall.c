#include <stdio.h>
__attribute__((noinline)) int inner(int x){return x*3+1;}
__attribute__((noinline)) int outer(int x){return inner(x+1);}
int main(void){printf("%d\n",outer(4));return 0;}
