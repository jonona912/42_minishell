#include <stdio.h>


union u_test {
	int x;
	int y;
	char c[40];
} ;

struct s_test {
	int x;
	int y;
	char c[40];
} ;

int main() {
	union u_test u;
	struct s_test s;
	printf("Size of struct: %zu bytes\n", sizeof(s));
	printf("Size of union: %zu bytes\n", sizeof(u));
	u.x = 5;
	u.y = 10;
	printf("x: %d, y: %d\n", u.x, u.y);
	return 0;
}
