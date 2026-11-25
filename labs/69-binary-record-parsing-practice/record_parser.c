#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

/* Invented format: 'L','B', version=1, type=1/2, uint16 LE length, payload. */
enum Result { OK, SHORT_HEADER, BAD_MAGIC, BAD_VERSION, BAD_TYPE,
              BAD_LENGTH, TRUNCATED, TRAILING, NULL_INPUT };

static enum Result parse(const uint8_t *p, size_t size, uint16_t *length) {
    if (!p || !length) return NULL_INPUT;
    if (size < 6u) return SHORT_HEADER;
    if (p[0] != 'L' || p[1] != 'B') return BAD_MAGIC;
    if (p[2] != 1u) return BAD_VERSION;
    if (p[3] != 1u && p[3] != 2u) return BAD_TYPE;
    uint16_t n = (uint16_t)((uint16_t)p[4] | ((uint16_t)p[5] << 8));
    if (n > 8u) return BAD_LENGTH;
    if ((size_t)n > size - 6u) return TRUNCATED;
    if ((size_t)n < size - 6u) return TRAILING;
    *length = n;
    return OK;
}
static const char *label(enum Result r) {
    const char *names[] = {"OK","SHORT_HEADER","BAD_MAGIC","BAD_VERSION",
        "BAD_TYPE","BAD_LENGTH","TRUNCATED","TRAILING","NULL_INPUT"};
    return names[r];
}
int main(void) {
    const uint8_t empty[] = {'L','B',1,1,0,0};
    const uint8_t one[] = {'L','B',1,2,1,0,42};
    const uint8_t max[] = {'L','B',1,1,8,0,1,2,3,4,5,6,7,8};
    const uint8_t magic[] = {'X','B',1,1,0,0};
    const uint8_t version[] = {'L','B',2,1,0,0};
    const uint8_t type[] = {'L','B',1,9,0,0};
    const uint8_t large[] = {'L','B',1,1,9,0};
    const uint8_t endian[] = {'L','B',1,1,0,1};
    const uint8_t missing[] = {'L','B',1,1,2,0,42};
    const uint8_t extra[] = {'L','B',1,1,0,0,42};
    struct Case { const char *name; const uint8_t *p; size_t size;
                  enum Result expected; uint16_t expected_len; };
    const struct Case cases[] = {
        {"empty payload",empty,sizeof empty,OK,0},
        {"one byte",one,sizeof one,OK,1},
        {"max payload",max,sizeof max,OK,8},
        {"zero size",empty,0,SHORT_HEADER,0},
        {"short header",empty,5,SHORT_HEADER,0},
        {"magic mismatch",magic,sizeof magic,BAD_MAGIC,0},
        {"version mismatch",version,sizeof version,BAD_VERSION,0},
        {"type mismatch",type,sizeof type,BAD_TYPE,0},
        {"too large",large,sizeof large,BAD_LENGTH,0},
        {"little endian 256",endian,sizeof endian,BAD_LENGTH,0},
        {"missing payload",missing,sizeof missing,TRUNCATED,0},
        {"unexpected tail",extra,sizeof extra,TRAILING,0},
        {"null pointer",NULL,0,NULL_INPUT,0},
    };
    size_t count = sizeof cases / sizeof cases[0];
    for (size_t i = 0; i < count; ++i) {
        uint16_t length = 999u;
        enum Result got = parse(cases[i].p,cases[i].size,&length);
        assert(got == cases[i].expected);
        assert(length == (got == OK ? cases[i].expected_len : 999u));
        printf("%-24s %s\n",cases[i].name,label(got));
    }
    assert(parse(empty,sizeof empty,NULL) == NULL_INPUT);
    printf("%zu/%zu cases passed\n",count+1u,count+1u);
    return 0;
}
