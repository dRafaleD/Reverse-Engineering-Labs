#include <stdio.h>
struct small { long a; long b; };
struct big { long a; long b; long c; };
__attribute__((noinline)) struct small make_small(long x){struct small s={x,x+1};return s;}
__attribute__((noinline)) struct big make_big(long x){struct big s={x,x+1,x+2};return s;}
int main(void){struct small s=make_small(10);struct big b=make_big(20);printf("%ld %ld %ld %ld %ld\n",s.a,s.b,b.a,b.b,b.c);return 0;}
