#include <stdio.h>
__attribute__((noinline)) int total(int n){int s=0;for(int i=1;i<=n;i++){s+=i;}return s;}
int main(void){printf("%d\n",total(10));return 0;}
