#include <stdio.h>

void *malloc_b(unsigned);
void free_b(void*);


int main(int argc, char const *argv[])
{

	char *foo = (char *) malloc_b(15);
	char *bar = (char *) malloc_b(31);
	
	printf("foo: %p\n", foo);
	printf("bar: %p\n", bar);
	printf("%ld\n\n", foo - bar);
	
	free_b(bar);
	
	char *buzz = (char *) malloc_b(16);
	printf("foo: %p\n", foo);
	printf("bar: %p\n", buzz);
	printf("%ld\n\n", foo-buzz);

	free_b(foo);
	
	char *foo_bar = (char *) malloc_b(15);
	printf("foo_bar: %p\n", foo);
	printf("bar: %p\n", buzz);
	printf("%ld\n", foo_bar-buzz);
	return 0;
}