#pragma once

#define STACKSIZE 1024

typedef struct
{
	void* parent;
	unsigned short int marked;
	void* data;
} object;

typedef struct
{
	object* stack[STACKSIZE];
	int stack_pos;
	int object_max;
} GC;

GC* gc_init();

void gc_free(GC* gc);

object* gc_root(GC* gc);

object* gc_push(GC* gc, object* parent);

#define new_o(parent, gc) (gc_push(gc, parent))

#define alloc(object_type) (malloc(sizeof(object_type)))

#define new(object_type, parent, gc) ((object_type*)(gc_push(gc, parent)->data = malloc(sizeof(object_type))))

#define new_arr(object_type, size, parent, gc) ((object_type*)(gc_push(gc, parent)->data = malloc((size) * sizeof(object_type))))

#define _push(gc) object* __gc_object = gc_push(gc, gc_root(gc));

#define _cur(gc) (__gc_object) 

#define _last(gc) _cur(gc)

#define _pop(gc) __gc_object->parent = gc_push(gc, __gc_object);
