#include <stdio.h>
#include <string.h>
__attribute__((noinline))
static int classify(const char *value) {
    if (strcmp(value, "ALPHA-TRAINING") == 0) return 1;
    if (strcmp(value, "BETA-TRAINING") == 0) return 2;
    return 0;
}
int main(void) {
    const char *inputs[] = {"ALPHA-TRAINING", "BETA-TRAINING", "UNKNOWN"};
    for (unsigned int i = 0; i < 3; ++i) {
        printf("%s -> %d\n", inputs[i], classify(inputs[i]));
    }
    return 0;
}
