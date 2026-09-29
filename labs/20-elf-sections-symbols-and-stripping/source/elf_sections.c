#include <stdio.h>

int initialized_global = 42;
int uninitialized_global;

static const char message[] = "ELF section training";

static int helper(int value) {
    return value * 2;
}

int main(void) {
    uninitialized_global = helper(initialized_global);
    printf("%s: %d\n", message, uninitialized_global);
    return 0;
}
