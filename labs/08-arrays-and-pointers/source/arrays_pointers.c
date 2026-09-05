#include <stdio.h>

int main() {
    int numbers[3] = {10, 20, 30};
    int *ptr = numbers;

    printf("First: %d\n", numbers[0]);
    printf("Second: %d\n", *(ptr + 1));
    printf("Third: %d\n", *(ptr + 2));

    return 0;
}
