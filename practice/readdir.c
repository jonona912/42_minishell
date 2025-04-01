#include <stdio.h>
#include <dirent.h>

int main() {
    DIR *dir = opendir("../../."); // Open current directory
    if (!dir) {
        perror("opendir");
        return 1;
    }

    struct dirent *entry;
    printf("Contents of current directory:\n");
    while ((entry = readdir(dir)) != NULL) {
        printf("%s\n", entry->d_name); // Print filenames
    }

    closedir(dir); // Cleanup
    return 0;
}
