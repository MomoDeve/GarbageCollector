#pragma once

#ifndef bool
typedef unsigned int bool;
#endif

#define STACKSIZE 1024

typedef struct
{
	int* parent;
	bool marked;
	void* data;
} object;

typedef struct
{
	object* stack[STACKSIZE];
	int stack_pos;
	int object_max;
} GC;

GC* gc_init();

void gc_free(GC*);

object* gc_root(GC*);

object* gc_push(GC*, int*);

void gc_mark_objects(GC*);

void gc_collect_garbage(GC*);

void gc_realloc(GC*);

//TODO
