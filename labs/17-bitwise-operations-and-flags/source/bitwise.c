#include <stdio.h>

int main(void) {
    unsigned int permissions = 0x5; // binary: 0101
    unsigned int write_flag = 0x2;  // binary: 0010

    if ((permissions & write_flag) != 0) {
        printf("Write permission is enabled\n");
    } else {
        printf("Write permission is disabled\n");
    }

    permissions |= write_flag;

    printf("Updated permissions: 0x%X\n", permissions);

    return 0;
}
