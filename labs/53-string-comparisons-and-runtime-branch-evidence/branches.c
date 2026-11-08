#include <stdio.h>
#include <string.h>

#if defined(__GNUC__) || defined(__clang__)
__attribute__((noinline))
#endif
static int classify(const char *name) {
    if (strcmp(name, "SAFE-LAB") == 0) return 1;
    if (strcmp(name, "REVIEW-LAB") == 0) return 2;
    return 0;
}

static const char *label(int code) {
    switch (code) {
        case 1: return "known";
        case 2: return "review";
        default: return "other";
    }
}

int main(void) {
    const char *samples[] = {"SAFE-LAB", "REVIEW-LAB", "UNKNOWN"};
    const char *expected[] = {"known", "review", "other"};
    for (size_t i = 0; i < 3; ++i) {
        const char *result = label(classify(samples[i]));
        printf("%s -> %s\n", samples[i], result);
        if (strcmp(result, expected[i]) != 0) return 1;
    }
    return 0;
}
