#include <stdio.h>
#include <stdlib.h>
static int doubled(int x){return x*2+1;}
__attribute__((noinline)) static int total(int n){int r=0;for(int i=0;i<n;i++)r+=(i&1)?3:2;return r;}
int main(int argc,char**argv){if(argc!=2)return 2;char*end=NULL;long n=strtol(argv[1],&end,10);if(!*argv[1]||*end||n<0||n>1000)return 2;printf("%d\n",total((int)n)+doubled((int)n));return 0;}