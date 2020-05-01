#include <stdio.h>

// this should cause the kernel to throw my custom exception
int main() {
	int i=1, j=0;
	printf("%d", i/j);
}
