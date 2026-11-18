#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>

enum { RIGHT_READ = 1u, RIGHT_WRITE = 2u };
typedef struct {
    unsigned generation;
    unsigned policy_epoch;
    unsigned allowed_rights;
    unsigned value;
    bool active;
} Slot;
typedef struct {
    Slot *slot;
    unsigned generation;
    unsigned policy_epoch;
    unsigned granted_rights;
} Handle;

static Handle issue(Slot *s, unsigned requested) {
    Handle h = {NULL, 0u, 0u, 0u};
    if (s && s->active && (requested & ~s->allowed_rights) == 0u) {
        h = (Handle){s, s->generation, s->policy_epoch, requested};
    }
    return h;
}
static bool authorized(const Handle *h, unsigned required) {
    return h && h->slot && h->slot->active
        && h->generation == h->slot->generation
        && h->policy_epoch == h->slot->policy_epoch
        && (h->granted_rights & required) == required
        && (h->slot->allowed_rights & required) == required;
}
static bool read_value(const Handle *h, unsigned *out) {
    if (!out || !authorized(h, RIGHT_READ)) return false;
    *out = h->slot->value;
    return true;
}
static bool write_value(const Handle *h, unsigned value) {
    if (!authorized(h, RIGHT_WRITE)) return false;
    h->slot->value = value;
    return true;
}
static bool set_policy(Slot *s, unsigned new_rights) {
    if (!s || !s->active || s->policy_epoch == UINT_MAX
        || (new_rights & ~(RIGHT_READ | RIGHT_WRITE)) != 0u) return false;
    s->allowed_rights = new_rights;
    s->policy_epoch++; /* Old capabilities cannot become valid again. */
    return true;
}
int main(void) {
    Slot slot = {1u, 1u, RIGHT_READ | RIGHT_WRITE, 10u, true};
    Handle reader = issue(&slot, RIGHT_READ);
    Handle writer = issue(&slot, RIGHT_WRITE);
    unsigned out = 0u;
    assert(read_value(&reader, &out) && out == 10u);
    assert(!write_value(&reader, 20u));
    assert(write_value(&writer, 30u));
    assert(!read_value(&writer, &out));
    assert(set_policy(&slot, RIGHT_READ));
    assert(!write_value(&writer, 40u) && slot.value == 30u);
    assert(!read_value(&reader, &out)); /* Old epoch is stale. */
    Handle fresh = issue(&slot, RIGHT_READ);
    assert(read_value(&fresh, &out) && out == 30u);
    Handle denied = issue(&slot, RIGHT_WRITE);
    assert(denied.slot == NULL);
    assert(set_policy(&slot, RIGHT_READ | RIGHT_WRITE));
    assert(!write_value(&writer, 50u)); /* Regrant does not revive old handle. */
    Handle renewed = issue(&slot, RIGHT_WRITE);
    assert(write_value(&renewed, 55u) && slot.value == 55u);
    slot.policy_epoch = UINT_MAX;
    assert(!set_policy(&slot, RIGHT_READ));
    assert(!read_value(&fresh, &out));
    puts("All policy-epoch and capability assertions passed.");
    return 0;
}
