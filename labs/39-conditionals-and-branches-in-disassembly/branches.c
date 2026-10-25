#include <stdio.h>
__attribute__((noinline)) int classify(int v){if(v<0)return -1;if(v==0)return 0;return 1;}
int main(void){printf("%d %d %d\n",classify(-5),classify(0),classify(9));return 0;}
