#include <unistd.h>
#include <stdio.h>

int main() {
    char *tty = ttyname(0); // Check terminal for stdin
    if (tty != NULL) {
        printf("Terminal name: %s\n", tty);
    } else {
        perror("ttyname failed");
    }
    return 0;
}
