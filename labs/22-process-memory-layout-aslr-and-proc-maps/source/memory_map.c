#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int global_counter = 7;
static const char banner[] = "RE_DAY22_MEMORY_MAP";

int main(void) {
    int stack_value = 21;

    char *heap_message = malloc(64);
    if (heap_message == NULL) {
        return 1;
    }

    strcpy(heap_message, "hello from the heap");

    printf("%s\n", banner);
    printf("pid=%d\n", getpid());
    printf("&main=%p\n", (void *)&main);
    printf("&global_counter=%p value=%d\n", (void *)&global_counter, global_counter);
    printf("&stack_value=%p value=%d\n", (void *)&stack_value, stack_value);
    printf("heap_message=%p text=%s\n", (void *)heap_message, heap_message);
    puts("Press Enter to exit...");
    getchar();

    free(heap_message);
    return 0;
}
