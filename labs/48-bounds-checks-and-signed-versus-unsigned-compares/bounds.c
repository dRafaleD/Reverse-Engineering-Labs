#include <stdio.h>
__attribute__((noinline)) int weak_get(const int *a,int i,int n){if(i<n)return a[i];return -1;}
__attribute__((noinline)) int strict_get(const int *a,int i,int n){if(i>=0&&i<n)return a[i];return -1;}
int main(void){int a[4]={10,20,30,40};printf("%d %d\n",weak_get(a,2,4),strict_get(a,2,4));return 0;}
