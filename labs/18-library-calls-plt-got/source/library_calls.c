#include <stdio.h>
#include <string.h>

int main(void) {
    char input[32];

    printf("Enter a short word: ");
    if (scanf("%31s", input) != 1) {
        return 1;
    }

    if (strcmp(input, "reverse") == 0) {
        puts("Match");
    } else {
        puts("No match");
    }

    printf("Length: %zu\n", strlen(input));
    return 0;
}
