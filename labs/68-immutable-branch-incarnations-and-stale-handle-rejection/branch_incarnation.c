#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct {
    unsigned incarnation;
    unsigned value;
    bool active;
} Branch;
typedef struct {
    const Branch *branch;
    unsigned incarnation;
} Reader;

static bool read_value(Reader reader, unsigned *value) {
    if (!value || !reader.branch || !reader.branch->active ||
        reader.incarnation != reader.branch->incarnation) return false;
    *value = reader.branch->value;
    return true;
}

int main(void) {
    Branch old = {1u, 42u, true};
    Reader stale = {&old, old.incarnation};
    unsigned value = 0;
    assert(read_value(stale, &value) && value == 42u);
    old.active = false;
    assert(!read_value(stale, &value));
    /* Reissue in a distinct object; never mutate an old branch back to life. */
    Branch fresh = {2u, 42u, true};
    Reader current = {&fresh, fresh.incarnation};
    assert(read_value(current, &value) && value == 42u);
    assert(!read_value(stale, &value));
    Reader wrong = {&fresh, 1u};
    assert(!read_value(wrong, &value));
    assert(!read_value(current, NULL));
    fresh.active = false;
    assert(!read_value(current, &value));
    assert(!read_value(stale, &value));
    puts("8/8 branch-incarnation checks passed");
    return 0;
}
