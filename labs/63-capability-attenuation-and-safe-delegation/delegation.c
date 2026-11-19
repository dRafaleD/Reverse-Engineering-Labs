#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

enum { CAN_READ = 1u, CAN_WRITE = 2u };
typedef struct {
    unsigned generation, policy_epoch, allowed_rights, value;
    bool active;
} Slot;
typedef struct {
    Slot *slot;
    unsigned generation, policy_epoch, rights;
} Handle;

static bool valid(const Handle *h, unsigned need) {
    return h && h->slot && h->slot->active
        && h->generation == h->slot->generation
        && h->policy_epoch == h->slot->policy_epoch
        && (h->rights & need) == need
        && (h->slot->allowed_rights & need) == need;
}
static bool delegate(const Handle *parent, unsigned subset, Handle *out) {
    if (!out || !parent || (subset & ~(CAN_READ | CAN_WRITE)) != 0u
        || (subset & parent->rights) != subset
        || !valid(parent, subset)) return false;
    *out = (Handle){parent->slot, parent->generation,
                    parent->policy_epoch, subset};
    return true;
}
static bool read_value(const Handle *h, unsigned *out) {
    if (!out || !valid(h, CAN_READ)) return false;
    *out = h->slot->value;
    return true;
}
static bool write_value(const Handle *h, unsigned next) {
    if (!valid(h, CAN_WRITE)) return false;
    h->slot->value = next;
    return true;
}
int main(void) {
    Slot slot = {1u, 1u, CAN_READ | CAN_WRITE, 10u, true};
    Handle full = {&slot, 1u, 1u, CAN_READ | CAN_WRITE};
    Handle reader = {NULL, 0u, 0u, 0u};
    unsigned result = 0u;
    unsigned checked = 0u;
#define CHECK(condition) do { assert(condition); ++checked; } while (0)
    CHECK(delegate(&full, CAN_READ, &reader));
    CHECK(read_value(&reader, &result) && result == 10u);
    CHECK(!write_value(&reader, 22u) && slot.value == 10u);
    Handle no_upgrade = {NULL, 0u, 0u, 0u};
    CHECK(!delegate(&reader, CAN_WRITE, &no_upgrade));
    CHECK(no_upgrade.slot == NULL);
    CHECK(!delegate(&full, 8u, &no_upgrade));
    CHECK(write_value(&full, 33u));
    CHECK(read_value(&reader, &result) && result == 33u);
    slot.allowed_rights = CAN_READ;
    slot.policy_epoch++;
    CHECK(!read_value(&reader, &result));  /* Delegated handle revoked. */
    CHECK(!write_value(&full, 44u) && slot.value == 33u);
    Handle new_read = {&slot, slot.generation, slot.policy_epoch, CAN_READ};
    CHECK(read_value(&new_read, &result) && result == 33u);
    printf("%u/%u delegation assertions passed\n", checked, checked);
    return 0;
}
