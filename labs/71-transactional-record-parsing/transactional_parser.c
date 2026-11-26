#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
typedef struct { unsigned count, sum; } Summary;
/* Invented stream: count, then count pairs of tag (1 or 2) and value. */
static int parse(const uint8_t *data, size_t size, Summary *out) {
    if (!data || !out || size < 1u) return 0;
    unsigned count = data[0];
    if (count > 4u || size != 1u + 2u * count) return 0;
    Summary candidate = {0u, 0u};
    for (unsigned i = 0; i < count; ++i) {
        unsigned tag = data[1u + 2u*i];
        unsigned val = data[2u + 2u*i];
        if (tag < 1u || tag > 2u || val > 100u) return 0;
        ++candidate.count;
        candidate.sum += val;
    }
    *out = candidate;
    return 1;
}
int main(void) {
    const uint8_t empty[]={0}, good[]={2,1,10,2,20};
    const uint8_t bad_late[]={2,1,10,3,20}, short_data[]={2,1,10};
    const uint8_t trailing[]={0,1}, bad_value[]={1,2,101};
    const uint8_t excess[]={5}, max[]={4,1,100,2,100,1,100,2,100};
    struct Case {const uint8_t *p; size_t size; int ok; unsigned n,sum;};
    const struct Case cases[]={
        {empty,sizeof empty,1,0,0},{good,sizeof good,1,2,30},
        {bad_late,sizeof bad_late,0,0,0},{short_data,sizeof short_data,0,0,0},
        {trailing,sizeof trailing,0,0,0},{bad_value,sizeof bad_value,0,0,0},
        {excess,sizeof excess,0,0,0},{max,sizeof max,1,4,400},
        {NULL,0,0,0,0},{empty,0,0,0,0}
    };
    size_t n=sizeof cases/sizeof cases[0];
    for(size_t i=0;i<n;++i) {
        Summary result={99,999};
        int ok=parse(cases[i].p,cases[i].size,&result);
        assert(ok==cases[i].ok);
        assert(result.count==(ok?cases[i].n:99u));
        assert(result.sum==(ok?cases[i].sum:999u));
        printf("Case %zu: %s\n",i+1,ok?"COMMITTED":"UNCHANGED");
    }
    assert(!parse(good,sizeof good,NULL));
    puts("11/11 parser cases passed");
    return 0;
}
