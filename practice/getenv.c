#include <stdio.h>
#include <stdlib.h>

int main() {
    char *home = getenv("PATH"); // Get the PATH environment variable
    if (home != NULL) {
        printf("Your home directory is: %s\n", home);
    } else {
        printf("HOME environment variable is not set.\n");
    }
    return 0;
}