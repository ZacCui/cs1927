//
//  COMP1927 Assignment 1 - Vlad: the memory allocator
//  allocator.c ... implementation
//
//  Created by Liam O'Connor on 18/07/12.
//  Modified by John Shepherd in August 2014, August 2015
//  Copyright (c) 2012-2015 UNSW. All rights reserved.
//

#include "allocator.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define FREE_HEADER_SIZE  sizeof(struct free_list_header)
#define ALLOC_HEADER_SIZE sizeof(struct alloc_block_header)
#define MAGIC_FREE     0xDEADBEEF
#define MAGIC_ALLOC    0xBEEFDEAD

#define BEST_FIT       1
#define WORST_FIT      2
#define RANDOM_FIT     3

typedef unsigned char byte;
typedef u_int32_t vsize_t;
typedef u_int32_t vlink_t;
typedef u_int32_t vaddr_t;

typedef struct free_list_header {
    u_int32_t magic;  // ought to contain MAGIC_FREE
    vsize_t size;     // # bytes in this block (including header)
    vlink_t next;     // memory[] index of next free block
    vlink_t prev;     // memory[] index of previous free block
} free_header_t;

typedef struct alloc_block_header {
    u_int32_t magic;  // ought to contain MAGIC_ALLOC
    vsize_t size;     // # bytes in this block (including header)
} alloc_header_t;

// Global data

static byte *memory = NULL;   // pointer to start of allocator memory
static vaddr_t free_list_ptr; // index in memory[] of first block in free list
static vsize_t memory_size;   // number of bytes malloc'd in memory[]
static u_int32_t strategy;    // allocation strategy (by default BEST_FIT)

// Private functions

static void vlad_merge();
vsize_t getFreeSize(free_header_t* L);
vsize_t getAllocsize(alloc_header_t* alloc);
void * Create_p(vaddr_t index);
//int IsPowerOfTow(double size);
vsize_t Roundup2(vsize_t size);
vsize_t Roundup4(vsize_t size);
free_header_t * Slipt_delete(vsize_t size, free_header_t* freep);
alloc_header_t * Create_ap(vsize_t size, free_header_t * freep);
vaddr_t getAddrIndex(void * pointer);
free_header_t * FindtheLast(free_header_t * L);
free_header_t * FindtheNext(free_header_t * L);
free_header_t * FindthePrev(free_header_t * L);
void Unlink(free_header_t * L);


// Input: size - number of bytes to make available to the allocator
// Output: none
// Precondition: Size >= 1024
// Postcondition: `size` bytes are now available to the allocator
//
// (If the allocator is already initialised, this function does nothing,
//  even if it was initialised with different size)

void vlad_init(u_int32_t size)
{
    // dummy statements to keep compiler happy
    // remove them when you implement your solution
    // memory = NULL;
    // free_list_ptr = (vaddr_t)0;
    // memory_size = 0;
    // strategy = BEST_FIT;
    // TODO for Milestone 1
    if( memory != NULL ) {
        return;
    }else{
        size = Roundup2(size);
        memory = malloc(sizeof(size));
        if ( memory == NULL ){
            fprintf(stderr, "vlad_init: Insufficient memory\n");
            exit(EXIT_FAILURE);
        }
        strategy = BEST_FIT;
        memory_size = (vsize_t)size;
        free_list_ptr = 0;
        
        // not sure the line below is useful or not in this function
        free_header_t * free_block =(free_header_t *) memory;
        free_block->magic = MAGIC_FREE;
        free_block->size = memory_size;
        free_block->next = free_list_ptr;
        free_block->prev = free_list_ptr;
    }
    
}


// Input: n - number of bytes requested
// Output: p - a pointer, or NULL
// Precondition: n < size of largest available free block
// Postcondition: If a region of size n or greater cannot be found, p = NULL
//                Else, p points to a location immediately after a header block
//                      for a newly-allocated region of some size >=
//                      n + header size.

void *vlad_malloc(u_int32_t n){
    // TODO for Milestone 2
    vsize_t size = Roundup4(n) + ALLOC_HEADER_SIZE;
    printf("The size is %d\n",size);
    if(memory_size < FREE_HEADER_SIZE + size){
        fprintf(stderr,"Unable to create a new free header\n");
        return NULL;
    }
    vsize_t Threshold = 2*FREE_HEADER_SIZE + size;
    printf("Threshold size %d\n",Threshold);
    free_header_t * FLP_OLD = (free_header_t *)Create_p(free_list_ptr);
    while(getFreeSize(FLP_OLD) < size){
        FLP_OLD = (free_header_t *)Create_p(FLP_OLD->next);
        if(getAddrIndex(FLP_OLD) == free_list_ptr){
            fprintf(stderr,"vald_alloc: No suitable free regions\n");
            return NULL;
        }
    }
    printf("Free region size %d\n",getFreeSize(FLP_OLD));
    if(getFreeSize(FLP_OLD) > Threshold){
        Slipt_delete(size,FLP_OLD);
        if(free_list_ptr == getAddrIndex(FLP_OLD)){
            free_list_ptr += size;
        }
    }else{
        if(free_list_ptr == getAddrIndex(FLP_OLD)){
            if(FLP_OLD->prev == FLP_OLD->next){
                fprintf(stderr,"vald_alloc: The last free region cannot be alloced\n");
                return NULL;
            }else{
                free_list_ptr = FLP_OLD->next;
            }
        }
        size = Threshold;
        Unlink(FLP_OLD);
    }
    alloc_header_t * alloced_block = Create_ap(size,FLP_OLD);
    
    memory_size -=size;
    printf("memory size %d\n",memory_size);
    return ((void*)(alloced_block +ALLOC_HEADER_SIZE));
}
    
    

//return ((void*)(chosen_ptr + ALLOC_HEADER_SIZE))
//fprintf(stderr, "vald_alloc: Memory corruption\n");
//exit(EXIT_FAILURE);

// Input: object, a pointer.
// Output: none
// Precondition: object points to a location immediately after a header block
//               within the allocator's memory.
// Postcondition: The region pointed to by object has been placed in the free
//                list, and merged with any adjacent free blocks; the memory
//                space can be re-allocated by vlad_malloc

void vlad_free(void *object)
{
    // TODO for Milestone 3
    vlad_merge();
}

// Input: current state of the memory[]
// Output: new state, where any adjacent blocks in the free list
//            have been combined into a single larger block; after this,
//            there should be no region in the free list whose next
//            reference is to a location just past the end of the region

static void vlad_merge()
{
    // TODO for Milestone 4
}

// Stop the allocator, so that it can be init'ed again:
// Precondition: allocator memory was once allocated by vlad_init()
// Postcondition: allocator is unusable until vlad_int() executed again

void vlad_end(void)
{
    // TODO for Milestone 1
    free(memory);
    
}


// Precondition: allocator has been vlad_init()'d
// Postcondition: allocator stats displayed on stdout

void vlad_stats(void)
{
    // TODO
    // put whatever code you think will help you
    // understand Vlad's current state in this function
    // REMOVE all of these statements when your vlad_malloc() is done
    printf("memory_size %d\n",memory_size);
    printf("free_list_ptr %d\n",free_list_ptr);
    free_header_t * FLP_OLD = (free_header_t *)Create_p(free_list_ptr);
    printf("Index number %d\n",getAddrIndex(FLP_OLD));
    printf("vlad_stats() won't work until vlad_malloc() works\n");
    return;
}

// judge the size is the power of 2 or not
/*
int IsPowerOfTow(double size){
    double a = 2.0;
    while (size > 1){
        size = size/a;
    }
    return (size == 1);
}
*/

// Round up to the least value, which is power of 2
// if size < 1024 then size = 1024
vsize_t Roundup2(vsize_t size){
    vsize_t n = 1024;
    if(size > 1024){
        while( size - n > 0 ){
            n = n*2;
        }
    }
    return (vsize_t)n;
}

// Round up to the least value, which is multiple of 4
// if size < 8 then size = 8
vsize_t Roundup4(vsize_t size){
    if(size < 8){
        size = 8;
    }
    if(size % 4 != 0){
        size += 4 - (size % 4);
    }
    return (vsize_t)size;
}


// get free region size
vsize_t getFreeSize(free_header_t* L){
    assert( L != NULL );
    return L->size;
}

// get alloced region size;
vsize_t getAllocsize(alloc_header_t* alloc){
    assert( alloc != NULL);
    return alloc->size;
}

// transfer address to a poniter, which point to the address in the certain memory
void * Create_p(vaddr_t index){
    assert(index >= 0);
    return((void *) memory+index);
}


// Create a new freeblock after the old free region, which will be alloced.
// And delete the old free region from the DList
free_header_t * Slipt_delete(vsize_t size, free_header_t* freeP){
    assert(freeP != NULL);
    assert(freeP->magic == MAGIC_FREE);
    assert(size < getFreeSize(freeP));
    free_header_t * freeP1 = (free_header_t *)(memory + size + free_list_ptr);
    freeP1->magic = MAGIC_FREE;
    freeP1->size = getFreeSize(freeP) - size;
    if(freeP->prev == freeP->next){
        freeP1->next = getAddrIndex(freeP1);
        freeP1->prev = getAddrIndex(freeP1);
    }else{
        freeP1->next = freeP->next;
        freeP1->prev = freeP->prev;
        free_header_t* Next = FindtheNext(freeP);
        free_header_t* Prev = FindthePrev(freeP);
        Next->prev = getAddrIndex(freeP1);
        Prev->next = getAddrIndex(freeP1);
    }
    freeP->size = size;
    freeP->next = (vlink_t)NULL;
    freeP->prev = (vlink_t)NULL;
    if( freeP1->magic != MAGIC_FREE){
        fprintf(stderr, "vald_alloc: Memory corruption\n");
        exit(EXIT_FAILURE);
    }
    return((free_header_t *) freeP1);
}

// Create an alloced block
alloc_header_t * Create_ap(vsize_t size, free_header_t * freep){
    assert(freep != NULL);
    assert(size <= getFreeSize(freep));
    alloc_header_t * alloced_block = (alloc_header_t *) freep;
    alloced_block->magic = MAGIC_ALLOC;
    alloced_block->size = size;
    if( alloced_block->size != size ){
        fprintf(stderr, "vald_alloc: Memory corruption\n");
        exit(EXIT_FAILURE);
    }
    return((alloc_header_t*) alloced_block);
}

//Get the first address of an pointer
vaddr_t getAddrIndex(void * pointer){
    assert(pointer != NULL);
    vaddr_t addr = (vaddr_t)pointer - (vaddr_t)memory;
    return addr;
}


//Find the last free region
free_header_t * FindtheLast(free_header_t * L){
    assert(L != NULL);
    assert(L->magic == MAGIC_FREE);
    free_header_t * curr = L;
    while(curr->next != getAddrIndex(L)){
        curr = (free_header_t *)Create_p(curr->next);
    }
    return (free_header_t *)curr;
}

//Find the next free region
free_header_t * FindtheNext(free_header_t * L){
    assert(L != NULL);
    assert(L->magic == MAGIC_FREE);
    free_header_t * curr = L;
    curr = (free_header_t *)Create_p(curr->next);
    return (free_header_t *)curr;
}

//Find the prev free region
free_header_t * FindthePrev(free_header_t * L){
    assert(L != NULL);
    assert(L->magic == MAGIC_FREE);
    free_header_t * curr = L;
    curr = (free_header_t *)Create_p(curr->prev);
    return (free_header_t *)curr;
}

//Unlink the curr node from Dlist
void Unlink(free_header_t * L){
    assert(L != NULL);
    assert(L->magic == MAGIC_FREE);
    if(L->prev != L->next){
        free_header_t* Next = FindtheNext(L);
        free_header_t* Prev = FindthePrev(L);
        Next->prev = L->prev;
        Prev->next = L->next;
    }
    L->prev = L->next =(vlink_t)NULL;
}


