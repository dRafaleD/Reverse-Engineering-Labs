#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum { INIT, ACTIVE, FAILED } State;
typedef enum { START, FAIL, RETRY, RESET } Event;
typedef struct { State next; bool accepted; } Result;

Result step(State s, Event e) {
    if (s == INIT && e == START) return (Result){ACTIVE, true};
    if (s == ACTIVE && e == FAIL) return (Result){FAILED, true};
    if (s == FAILED && e == RETRY) return (Result){ACTIVE, true};
    if ((s == ACTIVE || s == FAILED) && e == RESET)
        return (Result){INIT, true};
    return (Result){s, false};
}

int main(void) {
    const int next[3][4] = {
        { ACTIVE, INIT,   INIT,   INIT },
        { ACTIVE, FAILED, ACTIVE, INIT },
        { FAILED, FAILED, ACTIVE, INIT }
    };
    const bool accepted[3][4] = {
        { true,  false, false, false },
        { false, true,  false, true  },
        { false, false, true,  true  }
    };
    int passed = 0;
    for (int s = INIT; s <= FAILED; ++s) {
        for (int e = START; e <= RESET; ++e) {
            Result r = step((State)s, (Event)e);
            assert(r.next >= INIT && r.next <= FAILED);
            assert(r.next == next[s][e] && r.accepted == accepted[s][e]);
            if (!r.accepted) assert(r.next == s);
            ++passed;
        }
    }
    State state = INIT;
    const Event flow[] = {START, FAIL, RETRY, RESET};
    for (size_t i = 0; i < sizeof(flow)/sizeof(flow[0]); ++i) {
        Result result = step(state, flow[i]);
        assert(result.accepted);
        printf("event=%d state=%d -> next=%d\n", flow[i], state, result.next);
        state = result.next;
    }
    assert(state == INIT);
    printf("%d/12 state-event pairs verified\n", passed);
    return 0;
}
