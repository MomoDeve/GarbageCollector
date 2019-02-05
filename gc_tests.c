#include <stdio.h>
#include <stdlib.h>
#include "gc.h"

static GC* gc;

typedef struct
{
	int first, second;
} int_pair;

void first_test() // stack changes his size correctly, objects, are created
{
	_push(gc);
	for (int i = 0; i < STACKSIZE - 4; i++)
	{
		int* var = new(int, _cur(gc), gc);
		*var = i + 2; // root + _push
	}
	for (int i = 2; i < gc->stack_pos; i++)
	{
		printf("index: %d, variable: %d\n", i, *(int*)(gc->stack[i]->data));
	}
	_pop(gc);
}

void second_test() //nested objects are deleted
{
	object* o1 = new_o(gc_root(gc), gc);
	object* o2 = new_o(gc_root(gc), gc);
	o2->parent = o1;
	o1->parent = o2;
	// watch debug log of GC (all objects should be deleted)
}

void third_test() // custom struct initialization
{
	_push(gc);
	int_pair* p1 = new(int_pair, _cur(gc), gc);
	p1->first = p1->second = -1;
	int_pair* p2 = new(int_pair, _cur(gc), gc);
	p2->first = p2->second = 1;
	int_pair* p3 = new(int_pair, _cur(gc), gc);
	p3->first = p1->first + p2->first;
	p3->second = p1->second + p2->second;
	printf("{%d; %d} + {%d; %d} = {%d; %d;}\n",
			p1->first, p1->second,
			p2->first, p2->second,
			p3->first, p3->second);
	_pop(gc);
}

void forth_test(int* o)
{
	if (o == NULL)
	{
		_push(gc);
		int* var = new(int, _cur(gc), gc);
		*var = 128;
		forth_test(var); // check if elements can be translated through functions
		_pop(gc);
	}
	else
	{
		printf("element contains: %d\n", *o);
	}
}

int main()
{
	gc = gc_init();
	printf("FIRST TEST\n");
	first_test();
	printf("SECOND TEST\n");
	second_test();
	printf("THIRD TEST\n");
	third_test();
	printf("FORTH TEST\n");
	forth_test(NULL);
	gc_free(gc);
	system("pause");
}
