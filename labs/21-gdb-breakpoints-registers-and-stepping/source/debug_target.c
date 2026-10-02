#include <stdio.h>

int transform(int value) {
    int doubled = value * 2;
    int adjusted = doubled + 3;
    return adjusted;
}

int classify(int value) {
    if (value > 20) {
        return 1;
    }

    return 0;
}

int main(void) {
    int input = 10;
    int result = transform(input);
    int category = classify(result);

    printf("input=%d result=%d category=%d\n", input, result, category);
    return 0;
}
