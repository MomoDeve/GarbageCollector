#include <stdio.h>
#include <stdlib.h>
#include "gc.h"

//creating 1024 elements in stack, making all their parents unmarked (except gc_root) in a cycle

int main()
{
	object* o  = (object*)malloc(sizeof(object));
	o->marked = 0;
	o->parent = NULL;
	GC* gc = gc_init();
	for (int i = 1; i < 1024; i++)
		gc_push(gc, gc_root(gc));
	for(unsigned int i = 0; ; i++)
	{
		printf("cycle: %u\n", i);
		for (int i = 1; i < 1024; i++)
			gc->stack[i]->parent = o;
		for (int i = 1; i < 1024; i++)
			gc_push(gc, gc_root(gc));
	}
	gc_free(gc);
	free(o);
	printf("success\n");
	system("pause");
}
