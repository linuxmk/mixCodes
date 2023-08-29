#include "BuddyAllocator.h"

/**
* Return the order size of the address where it points to the block of memory
* @param buddy_allocator buddy allocator pointer
* @param address of the block
* @return the order of the block it points
*/
static size_t getOrder(const buddy_allocator_t *buddy_allocator, const void *address)
{
    size_t order = 0;
    size_t diff = address - (void*)buddy_allocator->memStart;

    if(!diff)
    {
        while( (!buddy_allocator->memPool[order]) &&
               (order < buddy_allocator->memMaxOrder) )
            ++order;

        return order;
    }

    while (diff >>= 1)
    {
        ++order;
    }

    return order;
}

/**
* Return the address of the buddy block
* @param buddy_allocator buddy allocator pointer
* @param block of the memory
* @param order of the block size
* @return the address of buddy block
*/
static void * findBuddy(buddy_allocator_t * buddy_allocator, void * block, size_t order)
{
    return (void *)( buddy_allocator->memStart + ( (block - (void*)buddy_allocator->memStart) ^ (1 << order)) );
}

/**
* Convert order to size. order(i) = 2 ** i in size
* @param raw_memory Backing memory
* @return the size
*/
static inline size_t getSize(size_t order)
{
    return 1 << order;
}


/**
* Create a buddy allocator
* @param raw_memory Backing memory
* @param memory_size Backing memory size
* @return the new buddy allocator
*/
buddy_allocator_t * buddy_allocator_create (void *raw_memory , size_t raw_memory_size )
{
    buddy_allocator_t *buddy = (buddy_allocator_t*)raw_memory;
    size_t order = 0;

    memset(buddy, 0,BUDDY_STRUCT_SIZE);
    buddy->memSize = raw_memory_size;

    while(getSize(order) < buddy->memSize)
    {
        ++order;
    }

    if(order > MAX_ORDER)
    {
        fprintf(stderr, "Cannot create buddy allocator\n");
        free(buddy);
        return NULL;
    }

    buddy->memMaxOrder = order;
    memset(buddy->memPool, 0, MAX_ORDER+1);
    buddy->memPool[order] = buddy->memStart;

    return buddy;
}


/**
* Destroy a buddy allocator
* @param buddy_allocator
*/
void buddy_allocator_destroy ( buddy_allocator_t * buddy_allocator )
{
    if(buddy_allocator)
    {
        memset(buddy_allocator, 0, BUDDY_STRUCT_SIZE);
        memset(buddy_allocator->memStart, 0, MEMORY_SIZE);
        buddy_allocator->memMaxOrder = 0;
        buddy_allocator->memSize = 0;
    }
}

/**
* Allocate memory
* @param buddy_allocator The buddy allocator
* @param size Size of memory to allocate
* @return pointer to the newly allocated memory , or @a NULL if out of memory
*/
void * buddy_allocator_alloc ( buddy_allocator_t * buddy_allocator , size_t size )
{
    size_t sizeOrder = 0;
    size_t order = 0;
    void *block = NULL;
    void *blockBuddy = NULL;

    while(getSize(sizeOrder) < size + 1)
    {
        ++sizeOrder;
    }

    if(sizeOrder < MIN_ORDER)   sizeOrder = MIN_ORDER;

    order = sizeOrder;


    while(sizeOrder <= buddy_allocator->memMaxOrder)
    {
        if(buddy_allocator->memPool[sizeOrder])
            break;
        ++sizeOrder;
    }

    if(sizeOrder > buddy_allocator->memMaxOrder)
    {
        fprintf(stderr, "Memory allocation fail!!! No free memory available.\n");
        return NULL;
    }

    block = buddy_allocator->memPool[sizeOrder];
    buddy_allocator->memPool[sizeOrder] = NULL;

    while (sizeOrder-- > order)
    {
        blockBuddy = findBuddy(buddy_allocator, block, sizeOrder);
        buddy_allocator->memPool[sizeOrder] = blockBuddy;
    }

    return block;
}

/**
* Deallocates a perviously allocated memory area.
* If @a ptr is @a NULL , it simply returns
* @param buddy_allocator The buddy allocator
* @param ptr The memory area to deallocate
*/
void buddy_allocator_free ( buddy_allocator_t * buddy_allocator , void *address )
{
    if(!address) return;

    void *buddy = NULL;
    void **blockPtr = NULL;

    size_t order = getOrder(buddy_allocator, address);

    while(order <= buddy_allocator->memMaxOrder)
    {

        //locate the buddy of an address
        buddy = findBuddy(buddy_allocator,address, order);
        blockPtr = &(buddy_allocator->memPool[order]);


        while (( NULL != *blockPtr) && ( buddy != *blockPtr ))
            blockPtr = (void **) *blockPtr;

        if ( buddy != *blockPtr)
        {
            *(void**)address = buddy_allocator->memPool[order];
            buddy_allocator->memPool[order] = address;

            return;
        }

        if(address > buddy) address = buddy;

        *blockPtr = NULL;
        ++order;
    }
}

