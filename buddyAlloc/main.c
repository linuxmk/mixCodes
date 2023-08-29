#include <stdio.h>
#include "BuddyAllocator.h"

char memory[BUDDY_STRUCT_SIZE + MEMORY_SIZE];

void testCase1( buddy_allocator_t* buddy)
{
    void *ptr = buddy_allocator_alloc(buddy, 1000);
    void *ptr1 = buddy_allocator_alloc(buddy, 60);
    void *ptr2 = buddy_allocator_alloc(buddy, 60);
    void *ptr3 = buddy_allocator_alloc(buddy, 255);
    void *ptr4 = buddy_allocator_alloc(buddy, 500);
    void *ptr5 = buddy_allocator_alloc(buddy, 30);


    //    memset(ptr, 0, 30);
    strncpy((char*)ptr, "Zlatko",7);
    fprintf(stderr, "Test ptr allocation and setting value: %s\n", (char*)ptr);

    buddy_allocator_free(buddy, ptr5);
    buddy_allocator_free(buddy, ptr1);
    buddy_allocator_free(buddy, ptr2);
    buddy_allocator_free(buddy, ptr3);
    buddy_allocator_free(buddy, ptr4);

    buddy_allocator_free(buddy, ptr);
    ptr = NULL;

    void *ptr6 = buddy_allocator_alloc(buddy, 63);
    void *ptr7 = buddy_allocator_alloc(buddy, 255);
    buddy_allocator_free(buddy, ptr6);
    buddy_allocator_free(buddy, ptr7);


    void *ptr11 = buddy_allocator_alloc(buddy, 63);
    void *ptr12 = buddy_allocator_alloc(buddy, 255);
    buddy_allocator_free(buddy, ptr11);
    buddy_allocator_free(buddy, ptr12);
}

void testCase2( buddy_allocator_t* buddy)
{
    void *ptr = buddy_allocator_alloc(buddy, 9);
    void *ptr1 = buddy_allocator_alloc(buddy, 60);
    void *ptr2 = buddy_allocator_alloc(buddy, 60);
    void *ptr3 = buddy_allocator_alloc(buddy, 255);
    void *ptr4 = buddy_allocator_alloc(buddy, 500);
    void *ptr5 = buddy_allocator_alloc(buddy, 30);

    strncpy((char*)ptr, "Test 2",7);
    fprintf(stderr, "Test ptr allocation and setting value: %s\n", (char*)ptr);
    buddy_allocator_free(buddy, ptr);

    ptr = NULL;

    buddy_allocator_free(buddy, ptr5);
    buddy_allocator_free(buddy, ptr1);
    buddy_allocator_free(buddy, ptr2);
    buddy_allocator_free(buddy, ptr3);
    buddy_allocator_free(buddy, ptr4);

    buddy_allocator_free(buddy, ptr);

    void *ptr6 = buddy_allocator_alloc(buddy, 63);
    void *ptr7 = buddy_allocator_alloc(buddy, 255);
    buddy_allocator_free(buddy, ptr6);
    buddy_allocator_free(buddy, ptr7);


    void *ptr11 = buddy_allocator_alloc(buddy, 63);
    void *ptr12 = buddy_allocator_alloc(buddy, 255);
    buddy_allocator_free(buddy, ptr11);
    buddy_allocator_free(buddy, ptr12);
}

void testCase0( buddy_allocator_t* buddy)
{
    void *ptr = buddy_allocator_alloc(buddy, 18);

    buddy_allocator_free(buddy, ptr);

}

int main()
{

    buddy_allocator_t * buddy = buddy_allocator_create((void*)memory, MEMORY_SIZE);
    if(!buddy)
    {
        return EXIT_FAILURE;
    }

    testCase0(buddy);
    testCase1(buddy);
    testCase2(buddy);

    buddy_allocator_destroy(buddy);

    return EXIT_SUCCESS;
}

