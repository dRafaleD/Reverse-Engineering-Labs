#include <stdio.h>
#include <string.h>
__attribute__((noinline)) int copy_name(const char *src){char buf[16];strncpy(buf,src,sizeof(buf)-1);buf[sizeof(buf)-1]=0;return (int)strlen(buf);}
int main(void){printf("%d\n",copy_name("short name"));return 0;}
