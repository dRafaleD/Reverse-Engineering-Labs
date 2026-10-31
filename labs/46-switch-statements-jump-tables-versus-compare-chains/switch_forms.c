#include <stdio.h>
__attribute__((noinline)) int dense(int v){switch(v){case 0:return v+100;case 1:return v*7;case 2:return v-3;case 3:return v^0x55;case 4:return v<<4;case 5:return v%7;default:return -1;}}
__attribute__((noinline)) int sparse(int v){switch(v){case 3:return v+1;case 400:return v*2;case 9000:return v-5;default:return 0;}}
int main(void){printf("%d %d\n",dense(2),sparse(400));return 0;}
