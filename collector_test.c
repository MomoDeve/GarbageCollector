#include <stdio.h>
#include <stdlib.h>
#include "gc.h"

static GC* gc;

typedef struct
{
	int first, second;
} int_pair;

int_pair* pair_sum(object* o, int_pair* p1, int_pair* p2)
{
	int_pair* res = new(int_pair, o, gc);
	res->first = p1->first + p2->first;
	res->second = p1->second + p2->second;
	return res;
}

int main()
{
	gc = gc_init();
	_push(gc);
	int_pair* p1 = new(int_pair, _cur(gc), gc);
	p1->first = 3, p1->second = 4;
	int_pair* p2 = new(int_pair, _cur(gc), gc);
	p2->first = 7, p2->second = -14;
	int_pair* p3 = pair_sum(_cur(gc), p1, p2);
	printf("sum of pairs: {%d, %d}\n", p3->first, p3->second);
	_pop(gc);
	gc_free(gc);
	system("pause");
}
