#include <stdio.h>
__attribute__((noinline)) int sum_locals(int x,int y){int a=x*2;int b=y+3;return a+b;}
int main(void){printf("%d\n",sum_locals(4,5));return 0;}
