#pragma once

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
}

object* gc_root(GC* gc)
{
	return gc->stack[0];
}

object* gc_push(GC* gc, object* parent)
{
	object* obj = (object*)malloc(sizeof(object));
	if (!obj) return obj;

	obj->marked = false;
	obj->parent = (void*)parent;
	obj->data = NULL;

	gc->stack[gc->stack_pos++] = obj;

	if (gc->stack_pos == gc->object_max)
	{
		gc_mark_objects(gc);
		gc_collect_garbage(gc);
		gc->object_max *= 2;
	}
}

void gc_collect_garbage(GC* gc)
{
	for (int i = gc->stack_pos - 1; i > 0; i--)
	{
		if (!gc->stack[i]->marked)
		{
			void* data = gc->stack[i]->data;
			if (data != NULL) free(data);
			free(gc->stack[i]);
		}
		else
		{
			gc->stack[i]->marked = false;
		}
	}
	if (gc->stack_pos > 1) gc_realloc(gc);
}

void gc_mark_objects(GC* gc)
{
	for (int i = 1; i < gc->stack_pos; i++)
	{
		object* parent = (object*)gc->stack[i]->parent;
		gc->stack[i]->marked = parent->marked;
	}
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
}
