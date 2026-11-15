#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include <stdio.h>

typedef struct {
    unsigned count;
    unsigned flags;
} Record;

typedef struct { Record *target; } View;
typedef struct { View *view; } Handle;

bool add_via_chain(Handle *handle, unsigned delta) {
    if (!handle || !handle->view || !handle->view->target || !delta)
        return false;
    Record *record = handle->view->target;
    if (record->count > UINT_MAX - delta)
        return false;
    record->count += delta;
    return true;
}

int main(void) {
    Record first = {1u, 0x10u};
    Record second = {4u, 0x20u};
    View a = {&first}, b = {&first};
    Handle ha = {&a}, hb = {&b};
    assert(add_via_chain(&ha, 2u));
    assert(first.count == 3u);
    assert(add_via_chain(&hb, 3u));
    assert(first.count == 6u); /* Both chains reached first. */

    b.target = &second;       /* Repoint only one chain. */
    assert(add_via_chain(&hb, 5u));
    assert(first.count == 6u && second.count == 9u);

    assert(!add_via_chain(NULL, 1u));
    assert(!add_via_chain(&ha, 0u));
    View empty = {NULL};
    Handle invalid = {&empty};
    assert(!add_via_chain(&invalid, 1u));
    Record maximum = {UINT_MAX, 0x30u};
    View vmax = {&maximum};
    Handle hmax = {&vmax};
    assert(!add_via_chain(&hmax, 1u));
    assert(maximum.count == UINT_MAX);

    assert(first.flags == 0x10u && second.flags == 0x20u);
    printf("Record.count offset=%zu, Record.flags offset=%zu\n",
           offsetof(Record, count), offsetof(Record, flags));
    puts("7/7 pointer-chain scenarios passed");
    return 0;
}
