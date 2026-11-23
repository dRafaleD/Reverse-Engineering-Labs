#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

enum { RIGHT_READ = 1u, RIGHT_WRITE = 2u };
typedef struct {
    unsigned id, generation, rights, value;
    bool active;
} Resource;
typedef struct Branch Branch;
struct Branch {
    Branch *parent;
    unsigned parent_epoch_at_creation;
    unsigned epoch;
    bool active;
};
typedef struct {
    Resource *resource;
    Branch *branch;
    unsigned id, generation, captured_epoch, rights;
} Handle;

/* All branch objects in this fixture remain allocated for its whole lifetime. */
static bool path_current(const Branch *node) {
    unsigned steps = 0u;
    while (node) {
        if (!node->active || ++steps > 4u) return false;
        if (node->parent &&
            node->parent_epoch_at_creation != node->parent->epoch) return false;
        node = node->parent;
    }
    return true;
}
static bool can(const Handle *h, unsigned need) {
    return h && h->resource && h->branch
        && h->resource->active && path_current(h->branch)
        && h->resource->id == h->id
        && h->resource->generation == h->generation
        && h->branch->epoch == h->captured_epoch
        && (h->rights & need) == need
        && (h->resource->rights & need) == need;
}
static bool delegate(const Handle *parent, Branch *child,
                     unsigned rights, Handle *out) {
    if (!parent || !child || !out || child->parent != parent->branch
        || child->parent_epoch_at_creation != parent->branch->epoch
        || rights == 0u || (rights & ~(RIGHT_READ | RIGHT_WRITE)) != 0u
        || (rights & parent->rights) != rights || !can(parent, rights))
        return false;
    *out = (Handle){parent->resource, child, parent->id,
                    parent->generation, child->epoch, rights};
    return true;
}
static bool read_value(const Handle *h, unsigned *out) {
    if (!out || !can(h, RIGHT_READ)) return false;
    *out = h->resource->value;
    return true;
}
int main(void) {
    Resource r = {7u, 1u, RIGHT_READ | RIGHT_WRITE, 42u, true};
    Branch root = {NULL, 0u, 1u, true};
    Branch left = {&root, 1u, 1u, true};
    Branch grandchild = {&left, 1u, 1u, true};
    Branch right = {&root, 1u, 1u, true};
    Handle admin = {&r, &root, 7u, 1u, 1u, RIGHT_READ | RIGHT_WRITE};
    Handle a = {0}, nested = {0}, b = {0}, denied = {0};
    unsigned value = 0u, n = 0u;
#define CHECK(cond) do { assert(cond); ++n; } while (0)
    CHECK(delegate(&admin, &left, RIGHT_READ, &a));
    CHECK(delegate(&a, &grandchild, RIGHT_READ, &nested));
    CHECK(delegate(&admin, &right, RIGHT_READ, &b));
    CHECK(read_value(&nested, &value) && value == 42u);
    CHECK(!delegate(&a, &grandchild, RIGHT_WRITE, &denied));
    left.epoch++; /* Revoke left branch and its existing descendants. */
    CHECK(!read_value(&a, &value));
    CHECK(!read_value(&nested, &value));
    CHECK(read_value(&b, &value) && value == 42u);
    Handle new_left = {&r, &left, 7u, 1u, left.epoch, RIGHT_READ};
    CHECK(read_value(&new_left, &value));
    CHECK(!delegate(&new_left, &grandchild, RIGHT_READ, &denied));
    grandchild.parent_epoch_at_creation = left.epoch; /* Reissue branch binding. */
    Handle new_nested = {0};
    CHECK(delegate(&new_left, &grandchild, RIGHT_READ, &new_nested));
    CHECK(read_value(&new_nested, &value));
    root.epoch++; /* Revokes both descendant trees. */
    CHECK(!read_value(&new_nested, &value));
    CHECK(!read_value(&b, &value));
    CHECK(!can(&admin, RIGHT_WRITE));
    printf("%u/%u hierarchical revocation checks passed\n", n, n);
    return 0;
}
