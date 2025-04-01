#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    char *buff = getcwd(NULL, 0); // Let getcwd allocate memory
    if (buff == NULL) {
        perror("getcwd() failed");
        return EXIT_FAILURE;
    }
    printf("CWD: %s\n", buff);
    free(buff); // Remember to free!
    return EXIT_SUCCESS;
}
