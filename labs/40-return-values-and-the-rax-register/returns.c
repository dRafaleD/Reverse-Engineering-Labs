#include <stdio.h>
__attribute__((noinline)) int add_one(int v){return v+1;}
int main(void){int r=add_one(41);printf("%d\n",r);return 0;}
