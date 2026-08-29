#include <stdio.h>

int add(int a, int b) {
    int result = a + b;
    return result;
}

int main() {
    int x = 10;
    int y = 20;

    int total = add(x, y);

    printf("Result: %d\n", total);

    return 0;
}
