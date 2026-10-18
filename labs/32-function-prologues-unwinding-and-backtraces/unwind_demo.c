#include <stdio.h>
__attribute__((noinline)) int leaf(int x){ volatile int y=x+3; return y*2; }
__attribute__((noinline)) int middle(int x){ return leaf(x)+1; }
int main(void){ printf("result=%d\n",middle(5)); return 0; }
