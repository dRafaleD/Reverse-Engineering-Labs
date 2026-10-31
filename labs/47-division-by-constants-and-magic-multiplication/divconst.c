#include <stdio.h>
__attribute__((noinline)) unsigned div10(unsigned x){return x/10;}
int main(void){printf("%u %u\n",div10(1234),div10(99));return 0;}
