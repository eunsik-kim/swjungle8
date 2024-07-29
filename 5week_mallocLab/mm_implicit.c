/*
 * This is assignment from swjungle week5.
 * Assignment goal is to make 4 version of malloc-lab.
 * Codes to be implemented are written in Computer Systems, chapter 9.9 by Randal, David
 * 1. implicit free list << 
 * 2. explicit free list
 * 3. segregated-fit
 * If you want to test this file, change the name of file into "mm.c"
 * Since allocated blocks lack a footer for memory reduction, the second bit of each header 
 *         serves as a check bit to determine whether the preceding block has been allocated.
 * and global var(isLastAlloc) also exists to check last block when extending the heap.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

team_t team = {
    /* Team name */
    "LGTM",
    /* First member's full name */
    "eunsik-kim",
    /* First member's email address */
    "gracely9901@gmail.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

/* define constant to make ease to use word size */
#define WSIZE 4
#define DSIZE 8
#define QSIZE 16
#define CHUNKSIZE (1<<12)
#define EPILOGUESIZE 0 
#define MAX(x, y) ((x) > (y) ? (x): (y))

/* define util macro to handle block info easily */
#define PACKING_BIT(size, alloc_bit) ((size) | (alloc_bit))
#define GET_WORD(ptr)                (*(unsigned int *)(ptr))
#define PUT_VALUE(ptr, val)          (*(unsigned int *)(ptr) = (val))

#define HEADPTR(bptr)               ((char *)(bptr) - WSIZE)
#define GET_SIZE(bptr)              (GET_WORD(HEADPTR(bptr)) & ~0x7)
#define GET_ALLOC_BIT(bptr)         (GET_WORD(HEADPTR(bptr)) & 1)
#define FOOTPTR(bptr)               ((char *)(bptr) + GET_SIZE(bptr) - DSIZE)
#define FOOTPTR_MS(bptr, size)      ((char *)(bptr) + (size) - DSIZE)

#define NEXT_BLKP(bptr) ((char *)(bptr) + GET_SIZE(bptr))
#define NEXT_BLKP_MS(bptr, size) ((char *)(bptr) + (size))
#define PREV_BLKP(bptr) ((char *)(bptr) - GET_SIZE((char *)(bptr) - WSIZE))

// prototype
void *extend_heap(int size);
void *coalesce(void *bptr);
void *first_fit(int size);
void *next_fit(int size, void *nextBptr);
void mm_memcpy(void *newBptr, void *oldBptr, int oldSize);
void mm_memcpy_reverse(void *newBptr, void *oldBptr, int oldSize);

static void *nextBptr;
typedef enum {FREE, ALLOC} bit_check; 

/* 
 * 1. initialize heap memory as much as chunksize
 * 2. set default prologue(to make starting ptr in heap memory) and epilogue block(to mark end of heap memory)
 */
int mm_init(void)
{   
    void *bptr;
    if (CHUNKSIZE <=16)
    {
        fprintf(stderr, "ERROR: mm_init failed. CHUNKSIZE should be larger than 16. Now : %d \n", CHUNKSIZE);
        return -1;
    }
    else if ((bptr = mem_sbrk(QSIZE)) != (void *) -1)
    {   
        PUT_VALUE((char *)bptr, 0);
        PUT_VALUE((char *)bptr + WSIZE, PACKING_BIT(8, 1)); // prologue
        PUT_VALUE((char *)bptr + DSIZE, PACKING_BIT(8, 1)); // prologue
        PUT_VALUE((char *)bptr + WSIZE + DSIZE, PACKING_BIT(0, 1)); // epilogue
        nextBptr = (char *)bptr + DSIZE;
    }
    else 
        return -1;

    if ((bptr = extend_heap(CHUNKSIZE)) == (void *) -1)
        return -1;
    return 0;
}

/* extend heap memory aligned to 8 bytes */
void *extend_heap(int size)
{
    int resize = ALIGN(size); 
    void *bptr;
    if ((bptr = mem_sbrk(resize)) == (void *) -1)
        return (void *) -1;

    // set free block and merge adjacent free block 
    PUT_VALUE(HEADPTR(bptr), resize); 
    PUT_VALUE(FOOTPTR_MS(bptr, resize), resize); 
    PUT_VALUE(HEADPTR(NEXT_BLKP_MS(bptr, resize)), PACKING_BIT(0, 1)); 
    return coalesce(bptr);
}

/* find adjacent free block using header bit and merge usable free memories(4 case exists)*/
void *coalesce(void *deleteBptr)
{
    void *prevBlkPtr = PREV_BLKP(deleteBptr);
    void *nextBlkPtr = NEXT_BLKP(deleteBptr);
    int prevAllocBit = GET_ALLOC_BIT(prevBlkPtr);
    int nextAllocBit = GET_ALLOC_BIT(nextBlkPtr);
    int resize = GET_SIZE(deleteBptr);
    
    if ((prevAllocBit == ALLOC) && (nextAllocBit == FREE)) 
    {   
        resize += GET_SIZE(nextBlkPtr);
        PUT_VALUE(HEADPTR(deleteBptr), resize);
        PUT_VALUE(FOOTPTR(nextBlkPtr), resize);
    }
    else if ((prevAllocBit == FREE) && (nextAllocBit == ALLOC)) 
    {
        resize += GET_SIZE(prevBlkPtr);
        PUT_VALUE(HEADPTR(prevBlkPtr), resize);
        PUT_VALUE(FOOTPTR(deleteBptr), resize);
        deleteBptr = prevBlkPtr;
    }
    else if ((prevAllocBit == FREE) && (nextAllocBit == FREE))
    {
        resize += GET_SIZE(prevBlkPtr) + GET_SIZE(nextBlkPtr);
        PUT_VALUE(HEADPTR(prevBlkPtr), resize);
        PUT_VALUE(FOOTPTR(nextBlkPtr), resize);
        deleteBptr = prevBlkPtr;
    }
    // if nextBptr postion is changed due to free(nextBptr)
    if ((deleteBptr < nextBptr) && (nextBptr < NEXT_BLKP(deleteBptr))) 
        nextBptr = deleteBptr;
    return deleteBptr;
}

/* mm_free - Freeing a block immediately or delayed  */
void mm_free(void *bptr)
{   
    int size = GET_SIZE(bptr);
    PUT_VALUE(HEADPTR(bptr), size);
    PUT_VALUE(FOOTPTR_MS(bptr, size), size);
    coalesce(bptr);
}

/* find first place to fit in starting from prologue (normal first_fit get 51 points) */
void *first_fit(int targetSize) 
{   
    int nowSize = DSIZE;
    void *bptr = (char *)mem_heap_lo() + nowSize; // start block
    do  // if size larger and free block, then break until epilogue
        bptr = NEXT_BLKP_MS(bptr, nowSize);
    while (((nowSize = GET_SIZE(bptr)) != EPILOGUESIZE)
            && !((GET_ALLOC_BIT(bptr) == FREE) && (nowSize >= targetSize)));
    return bptr;
}

/* find the first place to fit in starting from nextBptr (normal first_fit get 75 ~ 79 points)*/
void *next_fit(int targetSize, void *beforeNextBptr)
{   
    void *bptr = beforeNextBptr; // startisng from lastly chosen block(beforeNextBptr)
    int nowSize;
    while (((nowSize = GET_SIZE(bptr)) != EPILOGUESIZE)  // if size larger and free block, then break until epilogue
            && !((GET_ALLOC_BIT(bptr) == FREE) && (nowSize >= targetSize)))   
        bptr = NEXT_BLKP_MS(bptr, nowSize); 
    if (nowSize == EPILOGUESIZE) // if fail to find, then starting from prologue until beforeNextBptr
    {   
        void *epilogueBptr = bptr;
        nowSize = DSIZE;
        bptr = (char *)mem_heap_lo() + nowSize;
        do { 
            if (bptr == beforeNextBptr) // if travere all possibe free block, return epilogue block to extend heap
                return epilogueBptr;
            bptr = NEXT_BLKP_MS(bptr, nowSize);
            nowSize = GET_SIZE(bptr);
        } while (!((GET_ALLOC_BIT(bptr) == FREE) && (nowSize >= targetSize)));
    }
    return bptr;
}

void *mix_fit(int targetSize, void *beforeNextBptr)
{      
    void *bptr;
    int nowSize = GET_SIZE(beforeNextBptr);
    if (nowSize <= 128)
        first_fit(targetSize);    
    else 
        bptr = next_fit(targetSize, beforeNextBptr);
        nextBptr = bptr;
    return bptr;
        
}

/* 
 * 1. find the best fit position using next-fit.
 * 2. allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{   
    if (size == 0)
        return NULL;
    // 1. find position
    int resize = ALIGN(size + DSIZE); // add header and footer size
    void *bptr = next_fit(resize, nextBptr); 
    // void *bptr = first_fit(resize);
    int findSize = GET_SIZE(bptr);
    
    // if there is no place to put memory as much as size, then extend heap area.
    if (findSize == EPILOGUESIZE) // epilogue
    {
        int extendSize = MAX(resize, CHUNKSIZE); // setting 4K page size
        if ((bptr = extend_heap(extendSize)) ==  (void *)-1)
        {
            fprintf(stderr, "ERROR: mm_malloc failed due to mem_sbrk\n");
            return NULL;
        }
        findSize = GET_SIZE(bptr);
    }
    int freeSize = findSize - resize;
    nextBptr = bptr;
    // 2. place block
    if (freeSize >= QSIZE) 
    {   
        PUT_VALUE(HEADPTR(bptr), PACKING_BIT(resize, 1));
        PUT_VALUE(FOOTPTR_MS(bptr, resize), PACKING_BIT(resize, 1));
        PUT_VALUE(HEADPTR(NEXT_BLKP_MS(bptr, resize)), freeSize);
        PUT_VALUE(FOOTPTR_MS(NEXT_BLKP_MS(bptr, resize), freeSize), freeSize);
    }
    else // if the rest of block is 8 byte(which can't put value), then pad rest of block
    {
        PUT_VALUE(HEADPTR(bptr), PACKING_BIT(findSize, 1));
        PUT_VALUE(FOOTPTR_MS(bptr, findSize), PACKING_BIT(findSize, 1));
    }
    return bptr;
}
/* copy memory aligned by 4 byte(word size) */
void mm_memcpy(void *newBptr, void *oldBptr, int oldSize)
{   // migrate per 4 bytes without header and footer
    for (int i = 0; i < (oldSize - DSIZE); i += WSIZE) 
        PUT_VALUE((char *)newBptr + i, GET_WORD((char *)oldBptr + i));
}

void mm_memcpy_reverse(void *newBptr, void *oldBptr, int oldSize)
{  
    for (int i = (oldSize - DSIZE - WSIZE); i >= 0; i -= WSIZE) 
        PUT_VALUE((char *)newBptr + i, GET_WORD((char *)oldBptr + i));
}

/* free allocated space first and then migrate data for next-fit */
void *mm_realloc(void *oldBptr, size_t size)
{   
    int oldSize = GET_SIZE(oldBptr);
    if (size == oldSize)
        return oldBptr;

    mm_free(oldBptr); 
    if (size == 0)
        return oldBptr;

    // next-fit is same as mm_malloc
    int resize = ALIGN(size + DSIZE); 
    void *newBptr = next_fit(resize, nextBptr); 
    int findSize = GET_SIZE(newBptr);
    
    if (findSize == EPILOGUESIZE) 
    {
        int extendSize = MAX(resize, CHUNKSIZE); 
        if ((newBptr = extend_heap(extendSize)) ==  (void *)-1)
        {
            fprintf(stderr, "ERROR: mm_realloc failed due to mem_sbrk\n");
            return NULL;
        }
        findSize = GET_SIZE(newBptr);
    }
    int freeSize = findSize - resize;
    nextBptr = newBptr;
    // migrate data
    if (resize < oldSize)
        oldSize = resize; 

    if (freeSize >= QSIZE) 
    {   
        PUT_VALUE(HEADPTR(newBptr), PACKING_BIT(resize, 1));
        memmove(newBptr, oldBptr, oldSize - DSIZE);
        PUT_VALUE(FOOTPTR_MS(newBptr, resize), PACKING_BIT(resize, 1));
        PUT_VALUE(HEADPTR(NEXT_BLKP_MS(newBptr, resize)), freeSize);
        PUT_VALUE(FOOTPTR_MS(NEXT_BLKP_MS(newBptr, resize), freeSize), freeSize);
    }
    else // avoid fragmentation
    {
        PUT_VALUE(HEADPTR(newBptr), PACKING_BIT(findSize, 1));
        memmove(newBptr, oldBptr, oldSize - DSIZE);
        PUT_VALUE(FOOTPTR_MS(newBptr, findSize), PACKING_BIT(findSize, 1));
    }

    return newBptr;
}

/* original realloc */
// void *mm_realloc(void *bptr, size_t size)
// {
//     void *oldBptr = bptr;
//     void *newBptr;
//     size_t oldSize = GET_SIZE(bptr); 
//     newBptr = mm_malloc(size);
//     if (newBptr == NULL)
//         return NULL;
//     if (size < oldSize)
//         oldSize = size;
//     memcpy(newBptr, oldBptr, oldSize);
//     mm_free(oldBptr);    
//     return newBptr;
// }