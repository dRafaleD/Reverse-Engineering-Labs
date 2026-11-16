#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct {
    unsigned value;
    uint32_t generation;
    bool occupied;
} Slot;

typedef struct {
    Slot *slot;
    uint32_t generation;
} Handle;

/* A generation check rejects references to a recycled logical slot. */
static Handle acquire(Slot *slot) {
    Handle h = {NULL, 0};
    if (slot != NULL && slot->occupied) {
        h.slot = slot;
        h.generation = slot->generation;
    }
    return h;
}

static bool read_value(Handle h, unsigned *output) {
    if (h.slot == NULL || output == NULL) return false;
    if (!h.slot->occupied || h.slot->generation != h.generation) return false;
    *output = h.slot->value;
    return true;
}

static bool recycle(Slot *slot, unsigned replacement) {
    if (slot == NULL || !slot->occupied || slot->generation == UINT32_MAX)
        return false;
    slot->generation++;
    slot->value = replacement;
    return true;
}

int main(void) {
    Slot slot = {7u, 1u, true};
    Handle first = acquire(&slot);
    unsigned result = 0;
    assert(read_value(first, &result) && result == 7u);
    assert(recycle(&slot, 99u));
    assert(!read_value(first, &result)); /* stale logical handle */
    assert(result == 7u);                /* rejected read did not mutate output */
    Handle second = acquire(&slot);
    assert(read_value(second, &result) && result == 99u);
    assert(first.slot == second.slot && first.generation != second.generation);
    assert(!read_value(second, NULL));
    slot.occupied = false;
    assert(!read_value(second, &result));
    Handle invalid = acquire(&slot);
    assert(invalid.slot == NULL);
    Slot maximum = {1u, UINT32_MAX, true};
    assert(!recycle(&maximum, 2u));
    assert(maximum.value == 1u);
    puts("Pointer lifetime: generation and ownership checks passed");
    return 0;
}
