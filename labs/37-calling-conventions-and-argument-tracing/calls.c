#include <stdio.h>
__attribute__((noinline)) long calculate(long a,long b,long c){return (a+b)*c;}
int main(void){printf("%ld\n",calculate(2,5,3));return 0;}
