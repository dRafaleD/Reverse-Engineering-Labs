#include <stdio.h>

int add_bonus(int value);
void print_banner(void);

int main(void) {
    print_banner();
    int input = 10;
    int result = add_bonus(input);
    printf("input=%d result=%d\\n", input, result);
    return 0;
}
