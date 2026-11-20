#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

enum { R_READ = 1u, R_WRITE = 2u };
typedef struct {
    unsigned id, generation, epoch, allowed, value;
    bool active;
} Resource;
typedef struct {
    Resource *resource;
    unsigned id, generation, epoch, rights;
} Handle;

static bool valid(const Handle *h, unsigned right) {
    return h && h->resource && h->resource->active
        && h->id == h->resource->id
        && h->generation == h->resource->generation
        && h->epoch == h->resource->epoch
        && (h->rights & right) == right
        && (h->resource->allowed & right) == right;
}
static bool delegate(const Handle *parent, unsigned requested,
                     unsigned target_id, Handle *out) {
    if (!out || !parent || !parent->resource
        || requested == 0u || (requested & ~(R_READ | R_WRITE)) != 0u
        || target_id != parent->id || (requested & parent->rights) != requested
        || !valid(parent, requested)) return false;
    *out = (Handle){parent->resource, parent->id,
                    parent->generation, parent->epoch, requested};
    return true;
}
static bool read_value(const Handle *h, unsigned *out) {
    if (!out || !valid(h, R_READ)) return false;
    *out = h->resource->value;
    return true;
}
static bool write_value(const Handle *h, unsigned v) {
    if (!valid(h, R_WRITE)) return false;
    h->resource->value = v;
    return true;
}
int main(void) {
    Resource r = {7u, 1u, 1u, R_READ | R_WRITE, 10u, true};
    Handle parent = {&r, 7u, 1u, 1u, R_READ | R_WRITE};
    Handle child = {0}, unissued = {0};
    unsigned output = 0, count = 0;
#define CHECK(expr) do { assert(expr); ++count; } while (0)
    CHECK(delegate(&parent, R_READ, 7u, &child));
    CHECK(read_value(&child, &output) && output == 10u);
    CHECK(!write_value(&child, 99u));
    CHECK(!delegate(&child, R_WRITE, 7u, &unissued));
    CHECK(!delegate(&parent, R_READ, 8u, &unissued)); /* Cannot retarget. */
    CHECK(!delegate(&parent, 4u, 7u, &unissued)); /* Unknown permission. */
    CHECK(unissued.resource == NULL);
    Handle forged = child;
    forged.id = 8u;
    CHECK(!read_value(&forged, &output));
    CHECK(write_value(&parent, 20u));
    CHECK(read_value(&child, &output) && output == 20u);
    r.epoch++; /* The whole capability family is revoked. */
    CHECK(!read_value(&child, &output));
    CHECK(!write_value(&parent, 22u));
    Handle renewed = {&r, 7u, 1u, r.epoch, R_READ};
    CHECK(read_value(&renewed, &output) && output == 20u);
    printf("%u/%u resource-scope checks passed\n", count, count);
    return 0;
}
