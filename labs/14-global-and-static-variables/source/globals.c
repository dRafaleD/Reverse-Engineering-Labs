#include <stdio.h>

int global_counter = 10;
static int static_global = 20;

void show_values(void) {
    static int static_local = 30;

    printf("%d %d %d\n", global_counter, static_global, static_local);
}

int main(void) {
    show_values();
    global_counter++;
    show_values();

    return 0;
}
