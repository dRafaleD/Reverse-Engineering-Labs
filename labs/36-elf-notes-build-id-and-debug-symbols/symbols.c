#include <stdio.h>
static int transform(int x) { return x * 3 + 1; }
int main(void) { printf("result=%d\n", transform(7)); return 0; }
