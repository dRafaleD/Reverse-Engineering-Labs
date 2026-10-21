#include <stdio.h>
#include <string.h>
static int checksum(const char *s) { int n=0; while(*s)n+=(unsigned char)*s++;return n; }
int main(int argc,char **argv){ char buf[64]={0}; if(argc>1)snprintf(buf,sizeof buf,"%s",argv[1]); printf("checksum=%d\n",checksum(buf)); return 0; }
