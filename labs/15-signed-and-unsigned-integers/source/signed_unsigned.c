#include <stdio.h>

int main(void) {
    int signed_value = -1;
    unsigned int unsigned_value = 4294967295U;

    if (signed_value < 100) {
        printf("signed is less than 100\n");
    } else {
        printf("signed is not less than 100\n");
    }

    if (unsigned_value < 100) {
        printf("unsigned is less than 100\n");
    } else {
        printf("unsigned is not less than 100\n");
    }

    printf("%d\n", signed_value);
    printf("%u\n", unsigned_value);

    return 0;
}
