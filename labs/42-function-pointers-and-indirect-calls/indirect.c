#include <stdio.h>
__attribute__((noinline)) int add(int a,int b){return a+b;}
__attribute__((noinline)) int mul(int a,int b){return a*b;}
__attribute__((noinline)) int apply(int (*fn)(int,int),int a,int b){return fn(a,b);}
int main(void){int (*table[2])(int,int)={add,mul};printf("%d %d\n",apply(table[0],3,4),apply(table[1],3,4));return 0;}
