#pragma once

#define DEBUG_MODE 1
#define assert(decimal) printf(#decimal": %d\n", decimal);

#include <stdio.h>
#include <stdlib.h>
#include "gc.h"

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

#ifndef NULL
#define NULL (void*)0
#endif

object* reserved_buffer[STACKSIZE];

void gc_mark_objects(GC* gc);

void gc_collect_garbage(GC* gc);

void gc_realloc(GC* gc);

GC* gc_init()
{
	GC* gc = (GC*)malloc(sizeof(GC));
	if (!gc) return NULL;

	gc->object_max = STACKSIZE / 8;
	gc->stack_pos = 0;

	object* root = gc_push(gc, NULL);
	if (!root) return NULL;

	root->marked = true;
	return gc;
}

void gc_free(GC* gc)
{
	gc_root(gc)->marked = false;
	gc_collect_garbage(gc);
	free(gc_root(gc));
	free(gc);
	if (DEBUG_MODE) printf("cleared 1 object\ncleared gc\n");
}

object* gc_root(GC* gc)
{
	return gc->stack[0];
}

object* gc_push(GC* gc, object* parent)
{
	if (gc->stack_pos == gc->object_max)
	{
		if (gc->stack_pos < STACKSIZE) gc->object_max *= 2;
		gc_mark_objects(gc);
		gc_collect_garbage(gc);
	}
	if (gc->stack_pos == STACKSIZE) return NULL;

	object* obj = (object*)malloc(sizeof(object));
	if (obj == NULL) return obj;

	obj->marked = false;
	obj->parent = (void*)parent;
	obj->data = NULL;

	gc->stack[gc->stack_pos++] = obj;
	if (DEBUG_MODE) printf("pushed [%d]\n", gc->stack_pos - 1);
	return obj;
}

void gc_collect_garbage(GC* gc)
{
	#if DEBUG_MODE == 1
	int counter = 0;
	#endif
	for (int i = gc->stack_pos - 1; i > 0; i--)
	{
		if (!gc->stack[i]->marked)
		{
			void* data = gc->stack[i]->data;
			if (data != NULL)
			{
				free(data);
				data = NULL;
			}

			free(gc->stack[i]);
			gc->stack[i] = NULL;

			#if DEBUG_MODE == 1
			counter++;
			#endif
		}
		else
		{
			gc->stack[i]->marked = false;
		}
	}
	#if DEBUG_MODE == 1
	printf("cleared %d objects\n", counter);
	#endif
	gc_realloc(gc);
}

void gc_mark_objects(GC* gc)
{
	for (int i = 1; i < gc->stack_pos; i++)
	{
		object* parent = (object*)gc->stack[i]->parent;
		gc->stack[i]->marked = parent->marked;
		if (DEBUG_MODE && gc->stack[i]->marked) printf("marked object[%d]\n", i);
	}
	if (DEBUG_MODE) printf("----------------------------------------marking ended----------------------------------------\n");
}

void gc_realloc(GC* gc) 
{
	int j = 0;
	for (int i = 0; i < gc->stack_pos; i++)
	{
		if (gc->stack[i] == NULL) continue;
		reserved_buffer[j++] = gc->stack[i];
	}
	gc->stack_pos = j;
	for (int i = 0; i < gc->stack_pos; i++)
	{
		gc->stack[i] = reserved_buffer[i];
	}
	if (DEBUG_MODE) printf("reallocated %d element in stack\n", gc->stack_pos);
}
