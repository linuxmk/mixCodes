#ifndef BUDDYALLOCATOR_H
#define BUDDYALLOCATOR_H
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MEMORY_SIZE  1024
#define MIN_ORDER 4
#define MAX_ORDER 32

typedef struct buddy_allocator {
    size_t memSize;                 /* size of the memory for this buddy allocator*/
    size_t memMaxOrder;                /* maximum order for this buddy alloator */
    void * memPool[MAX_ORDER + 1];  /* pool of ordered size memory blocks */
    char memStart[1];               /* start of the memory pool for this buddy allocator */
}buddy_allocator_t;

#define BUDDY_STRUCT_SIZE 283

/**
* Create a buddy allocator
* @param raw_memory Backing memory
* @param memory_size Backing memory size
* @return the new buddy allocator
*/
buddy_allocator_t * buddy_allocator_create (void *raw_memory , size_t raw_memory_size );


/**
* Destroy a buddy allocator
* @param buddy_allocator
*/
void buddy_allocator_destroy ( buddy_allocator_t * buddy_allocator );


/**
* Allocate memory
* @param buddy_allocator The buddy allocator
* @param size Size of memory to allocate
* @return pointer to the newly allocated memory , or @a NULL if out of memory
*/
void * buddy_allocator_alloc ( buddy_allocator_t * buddy_allocator , size_t size );


/**
* Deallocates a perviously allocated memory area.
* If @a ptr is @a NULL , it simply returns
* @param buddy_allocator The buddy allocator
* @param ptr The memory area to deallocate
*/
void buddy_allocator_free ( buddy_allocator_t * buddy_allocator , void *ptr );
#endif // BUDDYALLOCATOR_H
