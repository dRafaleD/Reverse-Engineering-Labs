#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>

typedef struct { unsigned events; } Counter;

/* Two parameters can point to the same object. */
bool add_events(Counter *a, Counter *b, unsigned count) {
    if (!a || !b || count == 0) return false;
    if (a->events > UINT_MAX - count) return false;
    if (a != b && b->events > UINT_MAX - count) return false;
    a->events += count;
    if (a != b) b->events += count;
    return true;
}
int main(void) {
    Counter first = {2}, second = {5};
    assert(add_events(&first, &second, 3));
    assert(first.events == 5 && second.events == 8);
    assert(add_events(&first, &first, 4));
    assert(first.events == 9);
    Counter high = {UINT_MAX}, stable = {10};
    assert(!add_events(&stable, &high, 1));
    assert(stable.events == 10 && high.events == UINT_MAX);
    assert(!add_events(&first, &second, 0));
    assert(!add_events(NULL, &second, 1));
    assert(first.events == 9 && second.events == 8);
    puts("5/5 pointer and mutation cases passed");
    return 0;
}
