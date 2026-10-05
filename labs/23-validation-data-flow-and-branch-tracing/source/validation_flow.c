#include <stdio.h>
#include <stdint.h>

__attribute__((noinline))
int checksum(const unsigned char *buf, size_t len) {
    unsigned int sum = 0;
    for (size_t i = 0; i < len; i++) {
        sum = (sum + buf[i]) & 0xff;
    }
    return (int)sum;
}

__attribute__((noinline))
int validate(const char *input) {
    const unsigned char *p = (const unsigned char *)input;
    int csum = checksum(p, 4);

    if (input[0] == 'R' &&
        input[1] == 'E' &&
        input[2] == 'V' &&
        input[3] == '!' &&
        csum == 38) {
        return 1;
    }

    return 0;
}

int main(void) {
    char input[16];

    printf("Enter 4-byte training code: ");
    if (scanf("%15s", input) != 1) {
        return 1;
    }

    if (validate(input)) {
        puts("Accepted");
    } else {
        puts("Rejected");
    }

    return 0;
}
