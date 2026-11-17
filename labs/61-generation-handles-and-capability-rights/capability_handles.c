#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>

enum { CAN_READ = 1u, CAN_WRITE = 2u };
typedef struct { unsigned generation; unsigned value; bool active; } Slot;
typedef struct { Slot *slot; unsigned generation; unsigned rights; } Handle;

static bool valid(const Handle *h, unsigned required) {
    return h && h->slot && h->slot->active
        && h->generation == h->slot->generation
        && (h->rights & required) == required;
}
static bool read_value(const Handle *h, unsigned *out) {
    if (!out || !valid(h, CAN_READ)) return false;
    *out = h->slot->value;
    return true;
}
static bool write_value(const Handle *h, unsigned value) {
    if (!valid(h, CAN_WRITE)) return false;
    h->slot->value = value;
    return true;
}
static bool recycle(Slot *slot, unsigned value) {
    if (!slot || slot->generation == UINT_MAX) return false;
    slot->active = false; /* No dereference of freed memory: slot stays alive. */
    slot->generation++;
    slot->value = value;
    slot->active = true;
    return true;
}
int main(void) {
    Slot slot = {1u, 10u, true};
    Handle reader = {&slot, 1u, CAN_READ};
    Handle writer = {&slot, 1u, CAN_WRITE};
    Handle full = {&slot, 1u, CAN_READ | CAN_WRITE};
    unsigned output = 999u;
    assert(read_value(&reader, &output) && output == 10u);
    assert(!write_value(&reader, 20u) && slot.value == 10u);
    assert(!read_value(&writer, &output) && output == 10u);
    assert(write_value(&writer, 21u) && slot.value == 21u);
    assert(!read_value(&full, NULL));
    assert(recycle(&slot, 30u));
    assert(!write_value(&writer, 80u) && slot.value == 30u);
    assert(!read_value(&reader, &output) && output == 10u);
    Handle renewed = {&slot, slot.generation, CAN_READ | CAN_WRITE};
    assert(read_value(&renewed, &output) && output == 30u);
    assert(write_value(&renewed, 40u) && slot.value == 40u);
    slot.active = false;
    assert(!read_value(&renewed, &output) && output == 30u);
    puts("10/10 generation and capability cases passed");
    return 0;
}
