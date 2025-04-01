#include <unistd.h>
#include <stdio.h>

int main(void)
{
    char *str = "/home/zkhojazo/Documents/rank_03_prjs";
    if (chdir(str) == -1)
    {
        perror("chdir");
        return 1;
    }
    char *args[] = {"ls", NULL}; // Corrected declaration
    if (execve("/bin/ls", args, NULL) == -1)
    {
        perror("execve");
        return 1;
    }
    return (0);
}