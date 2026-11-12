#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef enum { INIT, ACTIVE, FAILED, DONE, STATE_COUNT } State;
typedef enum { START, COMPLETE, FAIL, RETRY, RESET, EVENT_COUNT } Event;

typedef struct {
    State state;
    unsigned completed;
    unsigned retries;
} Workflow;

bool apply(Workflow *w, Event event) {
    if (w->state == INIT && event == START) {
        w->state = ACTIVE;
        return true;
    }
    if (w->state == ACTIVE && event == COMPLETE) {
        w->completed += 1;
        w->state = DONE;
        return true;
    }
    if (w->state == ACTIVE && event == FAIL) {
        w->state = FAILED;
        return true;
    }
    if (w->state == FAILED && event == RETRY) {
        w->retries += 1;
        w->state = ACTIVE;
        return true;
    }
    if ((w->state == ACTIVE || w->state == FAILED || w->state == DONE)
        && event == RESET) {
        w->state = INIT;
        return true;
    }
    return false;
}

static void check_all_pairs(void) {
    const State expected_state[STATE_COUNT][EVENT_COUNT] = {
        {ACTIVE, INIT,   INIT,   INIT,   INIT},
        {ACTIVE, DONE,   FAILED, ACTIVE, INIT},
        {FAILED, FAILED, FAILED, ACTIVE, INIT},
        {DONE,   DONE,   DONE,   DONE,   INIT}
    };
    const bool expected_accept[STATE_COUNT][EVENT_COUNT] = {
        {true,  false, false, false, false},
        {false, true,  true,  false, true },
        {false, false, false, true,  true },
        {false, false, false, false, true }
    };
    const unsigned expected_completed_inc[STATE_COUNT][EVENT_COUNT] = {
        {0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0}
    };
    const unsigned expected_retries_inc[STATE_COUNT][EVENT_COUNT] = {
        {0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0}
    };

    unsigned verified = 0;
    for (int state = INIT; state < STATE_COUNT; ++state) {
        for (int event = START; event < EVENT_COUNT; ++event) {
            Workflow w = {(State)state, 7u, 3u};
            Workflow before = w;
            bool accepted = apply(&w, (Event)event);
            assert(accepted == expected_accept[state][event]);
            assert(w.state == expected_state[state][event]);
            assert(w.completed == 7u + expected_completed_inc[state][event]);
            assert(w.retries == 3u + expected_retries_inc[state][event]);
            if (!accepted) {
                assert(w.state == before.state);
                assert(w.completed == before.completed);
                assert(w.retries == before.retries);
            }
            ++verified;
        }
    }
    printf("%u/%u state-event pairs and side effects verified\n",
           verified, (unsigned)(STATE_COUNT * EVENT_COUNT));
}

static void check_trace(void) {
    const Event events[] = {
        START, FAIL, RETRY, COMPLETE, COMPLETE, RESET, RETRY, START, COMPLETE
    };
    const bool accepted[] = {
        true, true, true, true, false, true, false, true, true
    };
    const State states[] = {
        ACTIVE, FAILED, ACTIVE, DONE, DONE, INIT, INIT, ACTIVE, DONE
    };
    const unsigned completions[] = {0, 0, 0, 1, 1, 1, 1, 1, 2};
    const unsigned retries[] = {0, 0, 1, 1, 1, 1, 1, 1, 1};
    Workflow trace = {INIT, 0u, 0u};

    for (size_t i = 0; i < sizeof events / sizeof events[0]; ++i) {
        bool ok = apply(&trace, events[i]);
        assert(ok == accepted[i]);
        assert(trace.state == states[i]);
        assert(trace.completed == completions[i]);
        assert(trace.retries == retries[i]);
        printf("step=%zu event=%d accepted=%d state=%d completed=%u retries=%u\n",
               i + 1, events[i], ok, trace.state, trace.completed, trace.retries);
    }
}

int main(void) {
    check_all_pairs();
    check_trace();
    puts("Trace verified");
    return 0;
}
