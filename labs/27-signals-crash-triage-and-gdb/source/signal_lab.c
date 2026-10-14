#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Harmless, intentional signal demonstration: no memory corruption. */
static volatile sig_atomic_t seen_usr1 = 0;

static void on_usr1(int signo) {
    (void)signo;
    seen_usr1 = 1;  /* async-signal-safe update */
}

__attribute__((noinline))
static void stage_three(void) {
    raise(SIGSEGV); /* deliberate crash for a lab; no invalid pointer access */
}

__attribute__((noinline))
static void stage_two(void) { stage_three(); }

__attribute__((noinline))
static void stage_one(void) { stage_two(); }

int main(int argc, char **argv) {
    struct sigaction action = {0};
    action.sa_handler = on_usr1;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGUSR1, &action, NULL) != 0) {
        perror("sigaction");
        return EXIT_FAILURE;
    }
    if (argc != 2) {
        fprintf(stderr, "Usage: %s normal|signal|crash\n", argv[0]);
        return 2;
    }
    if (strcmp(argv[1], "normal") == 0) {
        puts("normal: no signal triggered");
    } else if (strcmp(argv[1], "signal") == 0) {
        if (raise(SIGUSR1) != 0) {
            perror("raise");
            return EXIT_FAILURE;
        }
        if (seen_usr1) puts("SIGUSR1 observed; execution continued");
    } else if (strcmp(argv[1], "crash") == 0) {
        stage_one();
    } else {
        fputs("Unknown mode\n", stderr);
        return 2;
    }
    return 0;
}
