#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* Harmless Linux process-lifecycle target. */
int main(void) {
    /* Avoid duplicate buffered lines across fork(). */
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("PARENT before fork: pid=%ld ppid=%ld\n",
           (long)getpid(), (long)getppid());

    pid_t child = fork();
    if (child == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child == 0) {
        printf("CHILD before exec: pid=%ld ppid=%ld\n",
               (long)getpid(), (long)getppid());

        /* Brief observation window for /proc and ps. */
        sleep(2);

        /* Replace THIS child's image; not a new PID. */
        execl("/bin/echo", "echo", "DAY26_CHILD_EXEC_OK", (char *)NULL);

        /* Only reachable if execl fails. */
        perror("execl");
        _exit(127);
    }

    printf("PARENT: child pid=%ld; waiting\n", (long)child);
    int status = 0;
    if (waitpid(child, &status, 0) == -1) {
        perror("waitpid");
        return EXIT_FAILURE;
    }
    if (WIFEXITED(status)) {
        printf("PARENT: exit status=%d\n", WEXITSTATUS(status));
    } else if (WIFSIGNALED(status)) {
        printf("PARENT: child terminated by signal=%d\n", WTERMSIG(status));
    }
    return EXIT_SUCCESS;
}
