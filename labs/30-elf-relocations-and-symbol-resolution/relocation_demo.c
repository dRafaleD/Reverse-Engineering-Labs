#include <stdio.h>
#include <stdlib.h>
static int offset=7;
int calc(int n){return n+offset;}
int main(void){int *p=malloc(sizeof *p); if(!p)return 1; *p=calc(10); printf("%d\n",*p); free(p); return 0;}
