#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

enum { READ = 1u, WRITE = 2u };
typedef struct {
    unsigned id, generation, value, allowed;
    bool active;
} Resource;
typedef struct {
    Resource *resource;
    unsigned revocation_epoch;
    bool active;
} Branch;
typedef struct {
    Branch *branch;
    unsigned resource_id, generation, branch_epoch, rights;
} Capability;

static bool allowed(const Capability *h, unsigned required) {
    return h && h->branch && h->branch->active
        && h->branch->resource && h->branch->resource->active
        && h->resource_id == h->branch->resource->id
        && h->generation == h->branch->resource->generation
        && h->branch_epoch == h->branch->revocation_epoch
        && (h->rights & required) == required
        && (h->branch->resource->allowed & required) == required;
}
static bool attenuate(const Capability *parent, unsigned subset, Capability *out) {
    if (!out || !parent || subset == 0u
        || (subset & ~(READ | WRITE)) != 0u
        || (subset & parent->rights) != subset
        || !allowed(parent, subset)) return false;
    *out = (Capability){parent->branch, parent->resource_id,
                        parent->generation, parent->branch_epoch, subset};
    return true;
}
static bool read_value(const Capability *h, unsigned *out) {
    if (!out || !allowed(h, READ)) return false;
    *out = h->branch->resource->value;
    return true;
}
static bool write_value(const Capability *h, unsigned value) {
    if (!allowed(h, WRITE)) return false;
    h->branch->resource->value = value;
    return true;
}
int main(void) {
    Resource r = {7u, 1u, 10u, READ | WRITE, true};
    Branch left = {&r, 1u, true}, right = {&r, 1u, true};
    Capability left_root = {&left, 7u, 1u, 1u, READ | WRITE};
    Capability right_root = {&right, 7u, 1u, 1u, READ | WRITE};
    Capability left_reader = {0}, right_reader = {0}, rejected = {0};
    unsigned value = 0, checked = 0;
#define CHECK(expr) do { assert(expr); ++checked; } while (0)
    CHECK(attenuate(&left_root, READ, &left_reader));
    CHECK(attenuate(&right_root, READ, &right_reader));
    CHECK(!write_value(&left_reader, 25u));
    CHECK(!attenuate(&left_reader, WRITE, &rejected));
    CHECK(read_value(&right_reader, &value) && value == 10u);
    left.revocation_epoch++; /* Only the left delegation family goes stale. */
    CHECK(!read_value(&left_reader, &value));
    CHECK(!write_value(&left_root, 30u));
    CHECK(read_value(&right_reader, &value) && value == 10u);
    CHECK(write_value(&right_root, 40u));
    CHECK(read_value(&right_reader, &value) && value == 40u);
    Capability fresh_left = {&left, 7u, 1u, left.revocation_epoch, READ};
    CHECK(read_value(&fresh_left, &value) && value == 40u);
    r.generation++; /* Resource-wide invalidation affects both branches. */
    CHECK(!read_value(&fresh_left, &value));
    CHECK(!read_value(&right_reader, &value));
    printf("%u/%u branch revocation assertions passed\n", checked, checked);
    return 0;
}
