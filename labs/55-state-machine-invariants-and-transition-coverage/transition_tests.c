#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef enum { IDLE, READY, DONE, STATE_COUNT } State;
typedef enum { START, CONFIRM, RESET, EVENT_COUNT } Event;
typedef struct { State next; bool accepted; } Result;

static Result transition(State current, Event event) {
    if (current == IDLE && event == START)
        return (Result){READY, true};
    if (current == READY && event == CONFIRM)
        return (Result){DONE, true};
    if (current == DONE && event == RESET)
        return (Result){IDLE, true};
    return (Result){current, false};
}

/* Independent specification: rows are states, columns are events. */
static const Result expected[STATE_COUNT][EVENT_COUNT] = {
    /* IDLE  */ {{READY, true}, {IDLE, false}, {IDLE, false}},
    /* READY */ {{READY, false}, {DONE, true}, {READY, false}},
    /* DONE  */ {{DONE, false}, {DONE, false}, {IDLE, true}}
};

int main(void) {
    size_t verified = 0;
    for (int state = 0; state < STATE_COUNT; ++state) {
        for (int event = 0; event < EVENT_COUNT; ++event) {
            Result actual = transition((State)state, (Event)event);
            Result want = expected[state][event];
            assert(actual.next >= IDLE && actual.next < STATE_COUNT);
            assert(actual.next == want.next);
            assert(actual.accepted == want.accepted);
            if (!actual.accepted)
                assert(actual.next == (State)state);
            printf("PASS state=%d event=%d accepted=%d next=%d\n",
                   state, event, actual.accepted, actual.next);
            ++verified;
        }
    }
    assert(verified == (size_t)(STATE_COUNT * EVENT_COUNT));
    printf("%zu/%d transitions verified\n",
           verified, STATE_COUNT * EVENT_COUNT);
    return 0;
}
