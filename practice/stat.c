#include <unistd.h>
#include <stdio.h>

int main() {
    if (symlink("original.txt", "link.txt") == 0) {
        printf("Symbolic link 'link.txt' created pointing to 'original.txt'.\n");
    } else {
        perror("symlink");
    }
    return 0;
}
