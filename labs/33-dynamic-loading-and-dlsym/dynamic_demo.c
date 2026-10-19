#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>
int main(void){
 void *h=dlopen("libm.so.6",RTLD_NOW|RTLD_LOCAL);
 if(!h){puts(dlerror());return 1;}
 dlerror();
 double (*f)(double)=NULL;
 *(void **)(&f)=dlsym(h,"cos");
 const char *err=dlerror();
 if(err){puts(err);dlclose(h);return 1;}
 printf("cos(0)=%.1f\n",f(0.0));
 dlclose(h);return 0;
}
