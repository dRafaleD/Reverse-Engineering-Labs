#include <stdio.h>

int calculate(int a, int b) {
    int sum = a + b;
    int doubled = sum * 2;
    int result = doubled - 3;

    return result;
}

int main() {
    int x = 7;
    int y = 5;
    int output = calculate(x, y);

    printf("Result: %d\n", output);

    return 0;
}
