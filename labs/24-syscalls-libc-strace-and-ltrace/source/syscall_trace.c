#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
    const char *message = "Reverse Engineering Day 24\n";

    puts("Starting syscall/libc training...");
    printf("PID: %d\n", getpid());

    char *copy = malloc(strlen(message) + 1);
    if (copy == NULL) {
        perror("malloc");
        return 1;
    }

    strcpy(copy, message);

    int fd = open("day24_output.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        free(copy);
        return 1;
    }

    ssize_t written = write(fd, copy, strlen(copy));
    if (written == -1) {
        perror("write");
    }

    close(fd);
    free(copy);

    puts("Finished.");
    return 0;
}
