#include <stdio.h>
#include <stdlib.h>

int main() {
    int *numbers = malloc(3 * sizeof(int));

    if (numbers == NULL) {
        return 1;
    }

    numbers[0] = 10;
    numbers[1] = 20;
    numbers[2] = 30;

    printf("%d %d %d\n", numbers[0], numbers[1], numbers[2]);

    free(numbers);
    return 0;
}
