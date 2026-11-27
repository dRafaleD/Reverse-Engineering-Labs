#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static int decode_signed_byte(uint8_t byte) {
    return byte < 128u ? (int)byte : (int)byte - 256;
}
static unsigned decode_le16(const uint8_t b[2]) {
    return (unsigned)b[0] | ((unsigned)b[1] << 8);
}
int main(void) {
    const uint8_t values[] = {0,1,127,128,129,254,255};
    const int expected[] = {0,1,127,-128,-127,-2,-1};
    for (size_t i=0;i<sizeof values;++i) {
        int result=decode_signed_byte(values[i]);
        assert(result==expected[i]);
        printf("%02x -> %d\n",(unsigned)values[i],result);
    }
    const uint8_t a[]={0x34,0x12}, b[]={0x12,0x34}, c[]={0xff,0xff};
    assert(decode_le16(a)==0x1234u);
    assert(decode_le16(b)==0x3412u);
    assert(decode_le16(c)==65535u);
    puts("10/10 decoding cases passed");
    return 0;
}
