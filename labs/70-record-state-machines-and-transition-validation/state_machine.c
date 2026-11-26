#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

enum State { EMPTY, OPEN, CLOSED };
enum Event { BEGIN = 1, DATA = 2, END = 3 };
typedef struct { enum State state; unsigned count; } Session;

static bool apply(Session *s, unsigned event) {
    if (!s) return false;
    switch (s->state) {
        case EMPTY:
            if (event != BEGIN) return false;
            s->state = OPEN;
            return true;
        case OPEN:
            if (event == DATA && s->count < 3u) {
                ++s->count;
                return true;
            }
            if (event == END) {
                s->state = CLOSED;
                return true;
            }
            return false;
        case CLOSED:
            return false;
        default:
            return false;
    }
}

int main(void) {
    Session s = {EMPTY, 0};
    struct Test { unsigned event; bool accepted; enum State state; unsigned count; };
    const struct Test tests[] = {
        {DATA,false,EMPTY,0}, {END,false,EMPTY,0},
        {BEGIN,true,OPEN,0}, {BEGIN,false,OPEN,0},
        {DATA,true,OPEN,1}, {99,false,OPEN,1},
        {DATA,true,OPEN,2}, {DATA,true,OPEN,3},
        {DATA,false,OPEN,3}, {END,true,CLOSED,3},
        {DATA,false,CLOSED,3}, {BEGIN,false,CLOSED,3}
    };
    size_t n = sizeof tests / sizeof tests[0];
    for (size_t i=0; i<n; ++i) {
        bool accepted = apply(&s, tests[i].event);
        assert(accepted == tests[i].accepted);
        assert(s.state == tests[i].state && s.count == tests[i].count);
        printf("%02zu event=%u accepted=%d state=%d count=%u\n",
               i+1, tests[i].event, (int)accepted, (int)s.state, s.count);
    }
    assert(!apply(NULL, BEGIN));
    puts("13 transition scenarios passed");
    return 0;
}
