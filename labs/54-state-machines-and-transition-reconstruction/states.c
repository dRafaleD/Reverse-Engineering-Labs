#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum { IDLE, READY, DONE } State;
typedef enum { START, CONFIRM, RESET } Event;

typedef struct {
    State next;
    bool accepted;
} Result;

const char *state_name(State state) {
    switch (state) {
        case IDLE: return "IDLE";
        case READY: return "READY";
        case DONE: return "DONE";
        default: return "UNKNOWN";
    }
}

const char *event_name(Event event) {
    switch (event) {
        case START: return "START";
        case CONFIRM: return "CONFIRM";
        case RESET: return "RESET";
        default: return "UNKNOWN";
    }
}

/* Intentionally simple: investigate the branches in GDB and Ghidra. */
Result transition(State current, Event event) {
    if (current == IDLE && event == START)
        return (Result){READY, true};
    if (current == READY && event == CONFIRM)
        return (Result){DONE, true};
    if (current == DONE && event == RESET)
        return (Result){IDLE, true};
    return (Result){current, false};
}

int main(void) {
    const Event events[] = {CONFIRM, START, CONFIRM, RESET};
    State state = IDLE;

    const Result premature = transition(IDLE, CONFIRM);
    assert(!premature.accepted && premature.next == IDLE);
    assert(transition(READY, START).accepted == false);
    assert(transition(IDLE, START).next == READY);
    assert(transition(READY, CONFIRM).next == DONE);
    assert(transition(DONE, RESET).next == IDLE);

    for (size_t i = 0; i < sizeof(events) / sizeof(events[0]); ++i) {
        Result result = transition(state, events[i]);
        printf("%s + %s => %s (%s)\n",
               state_name(state), event_name(events[i]),
               state_name(result.next),
               result.accepted ? "accepted" : "rejected");
        state = result.next;
    }
    assert(state == IDLE);
    return 0;
}
