#include <termios.h>
#include <unistd.h>
#include <stdio.h>

char getch(void) {
    struct termios original_attrs, modified_attrs;
    char ch;

    // Get current terminal attributes
    tcgetattr(STDIN_FILENO, &original_attrs);
    modified_attrs = original_attrs;

    // Set non-canonical mode and disable echoing
    modified_attrs.c_lflag &= ~(ICANON | ECHO);

    // Set minimum characters (VMIN) to 1 and timeout (VTIME) to 0
    modified_attrs.c_cc[VMIN] = 1;
    modified_attrs.c_cc[VTIME] = 0;

    // Apply the modified attributes immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &modified_attrs);

    // Read one character
    read(STDIN_FILENO, &ch, 1);

    // Restore the original attributes
    tcsetattr(STDIN_FILENO, TCSANOW, &original_attrs);

    return ch;
}

int main() {
    printf("Press any key: ");
	fflush(stdout);
    char c = getch();
    printf("\nYou pressed: %c\n", c);
    return 0;
}