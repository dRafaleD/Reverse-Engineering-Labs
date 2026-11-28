#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stddef.h>

/* Fictional signed 16-bit LE temperature, in tenths of a degree. */
static int decode(const uint8_t *p, size_t n, int32_t *out) {
    if (!p || !out || n != 2u) return 0;
    uint32_t raw = (uint32_t)p[0] | ((uint32_t)p[1] << 8);
    int32_t value = raw >= 32768u ? (int32_t)raw - 65536 : (int32_t)raw;
    if (value < -400 || value > 1250) return 0;
    *out = value;
    return 1;
}
int main(void) {
    struct Case { uint8_t bytes[2]; int ok; int32_t value; };
    const struct Case cases[] = {
        {{0,0},1,0}, {{0xfa,0},1,250}, {{0xff,0xff},1,-1},
        {{0x70,0xfe},1,-400}, {{0x6f,0xfe},0,0},
        {{0xe2,4},1,1250}, {{0xe3,4},0,0},
        {{0,0x80},0,0}, {{0xff,0x7f},0,0}
    };
    size_t count = sizeof cases / sizeof cases[0];
    for (size_t i=0;i<count;++i) {
        int32_t value=99999;
        int ok=decode(cases[i].bytes,2,&value);
        assert(ok==cases[i].ok);
        assert(value==(ok?cases[i].value:99999));
        printf("%zu %s %ld\n",i+1,ok?"OK":"REJECT",(long)value);
    }
    int32_t v=99999;
    assert(!decode(NULL,2,&v));
    assert(!decode(cases[0].bytes,1,&v));
    assert(!decode(cases[0].bytes,2,NULL));
    assert(v==99999);
    puts("12 cases passed");
    return 0;
}
