#include <stdio.h>

__attribute__((noinline))
static int inspect(int first, int second) {
    int total = first + second;
    int doubled = total * 2;
    printf("first=%d second=%d total=%d doubled=%d\n",
           first, second, total, doubled);
    return doubled;
}

int main(void) {
    int result = inspect(7, 5);
    printf("result=%d\n", result);
    return result == 24 ? 0 : 1;
}
