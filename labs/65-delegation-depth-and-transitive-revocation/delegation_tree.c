#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

enum { RIGHT_READ = 1u, RIGHT_WRITE = 2u, MAX_DEPTH = 2u };
typedef struct {
    unsigned id, generation, epoch, allowed, value;
    bool active;
} Resource;
typedef struct {
    Resource *resource;
    unsigned id, generation, epoch, rights, depth;
} Handle;

static bool permitted(const Handle *h, unsigned need) {
    return h && h->resource && h->resource->active
        && h->id == h->resource->id
        && h->generation == h->resource->generation
        && h->epoch == h->resource->epoch
        && (h->rights & need) == need
        && (h->resource->allowed & need) == need;
}
static bool delegate(const Handle *parent, unsigned subset, Handle *out) {
    if (!out || !parent || parent->depth >= MAX_DEPTH
        || subset == 0u || (subset & ~(RIGHT_READ | RIGHT_WRITE)) != 0u
        || (subset & parent->rights) != subset
        || !permitted(parent, subset)) return false;
    *out = (Handle){parent->resource, parent->id, parent->generation,
                    parent->epoch, subset, parent->depth + 1u};
    return true;
}
static bool read_value(const Handle *h, unsigned *out) {
    if (!out || !permitted(h, RIGHT_READ)) return false;
    *out = h->resource->value;
    return true;
}
static bool write_value(const Handle *h, unsigned value) {
    if (!permitted(h, RIGHT_WRITE)) return false;
    h->resource->value = value;
    return true;
}
int main(void) {
    Resource resource = {7u, 1u, 1u, RIGHT_READ | RIGHT_WRITE, 10u, true};
    Handle root = {&resource, 7u, 1u, 1u, RIGHT_READ | RIGHT_WRITE, 0u};
    Handle child = {0}, grandchild = {0}, rejected = {0};
    unsigned value = 0u, checks = 0u;
#define CHECK(condition) do { assert(condition); ++checks; } while (0)
    CHECK(delegate(&root, RIGHT_READ, &child));
    CHECK(child.depth == 1u);
    CHECK(delegate(&child, RIGHT_READ, &grandchild));
    CHECK(grandchild.depth == 2u);
    CHECK(!delegate(&grandchild, RIGHT_READ, &rejected));
    CHECK(!delegate(&child, RIGHT_WRITE, &rejected));
    CHECK(rejected.resource == NULL);
    CHECK(read_value(&grandchild, &value) && value == 10u);
    CHECK(!write_value(&grandchild, 99u) && resource.value == 10u);
    CHECK(write_value(&root, 25u));
    CHECK(read_value(&grandchild, &value) && value == 25u);
    resource.epoch++; /* All captured epochs, including descendants, stale. */
    CHECK(!read_value(&child, &value));
    CHECK(!read_value(&grandchild, &value));
    CHECK(!write_value(&root, 30u));
    Handle fresh = {&resource, resource.id, resource.generation,
                    resource.epoch, RIGHT_READ, 0u};
    CHECK(read_value(&fresh, &value) && value == 25u);
    printf("%u/%u transitive capability checks passed\n", checks, checks);
    return 0;
}
