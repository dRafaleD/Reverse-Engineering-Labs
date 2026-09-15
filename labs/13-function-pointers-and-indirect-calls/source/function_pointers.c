#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int main() {
    int (*operation)(int, int) = add;

    int result1 = operation(10, 4);
    printf("Add result: %d\n", result1);

    operation = subtract;

    int result2 = operation(10, 4);
    printf("Subtract result: %d\n", result2);

    return 0;
}
