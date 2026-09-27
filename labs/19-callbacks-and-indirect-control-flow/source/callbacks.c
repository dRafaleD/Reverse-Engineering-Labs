#include <stdio.h>

int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

int calculate(int (*operation)(int, int), int x, int y) {
    return operation(x, y);
}

int main(void) {
    int choice = 1;
    int result;

    if (choice == 1) {
        result = calculate(add, 6, 7);
    } else {
        result = calculate(multiply, 6, 7);
    }

    printf("Result: %d\n", result);
    return 0;
}
