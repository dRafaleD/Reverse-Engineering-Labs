#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int parse(const unsigned char *b, size_t n) {
 if(n<6 || memcmp(b,"RE28",4)!=0)return -1;
 size_t payload=(size_t)b[4]|((size_t)b[5]<<8);
 if(payload!=n-6)return -2;
 printf("valid payload bytes: %zu\n",payload);
 return 0;
}
int main(int argc,char **argv){
 if(argc!=2)return 2;
 FILE *f=fopen(argv[1],"rb");if(!f){perror("fopen");return 2;}
 unsigned char b[4096];size_t n=fread(b,1,sizeof b,f);
 if(ferror(f)||(!feof(f)&&n==sizeof b)){fclose(f);return 2;}
 fclose(f);int result=parse(b,n);printf("result=%d\n",result);
 return result==0?0:1;
}