/*
 * This is assignment from swjungle week5.
 * Assignment goal is to make 4 version of malloc-lab.
 * Codes to be implemented are written in Computer Systems, chapter 9.9 by Randal, David
 * 1. implicit free list  
 * 2. explicit free list << with no footer alloc block and ordering by addr
 * 3. simple segregated-fit
 * If you want to test this file, change the name of file into "mm.c"
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
#define PUT_PTR(ptr, inptr)          (*(void **)(ptr) = (inptr))
#define GET_PTR(ptr)                 ((unsigned int *)*(unsigned int *)(ptr))

#define HEADPTR(bptr)               ((char *)(bptr) - WSIZE)
#define PREDPTR(bptr)               (bptr)
#define SUCCPTR(bptr)               ((char *)(bptr) + WSIZE)
#define GET_SIZE(bptr)              (GET_WORD(HEADPTR(bptr)) & ~0x7)
#define GET_ALLOC_BIT(bptr)         (GET_WORD(HEADPTR(bptr)) & 1)
#define GET_PREV_ALLOC_BIT(bptr)    ((GET_WORD(HEADPTR(bptr)) & 2)/2)

#define FOOTPTR(bptr)               ((char *)(bptr) + GET_SIZE(bptr) - DSIZE)
#define FOOTPTR_MS(bptr, size)      ((char *)(bptr) + (size) - DSIZE)

#define NEXT_BLKP(bptr)             ((char *)(bptr) + GET_SIZE(bptr))
#define NEXT_BLKP_MS(bptr, size)    ((char *)(bptr) + (size))
#define PREV_BLKP(bptr)             ((char *)(bptr) - GET_SIZE((char *)(bptr) - WSIZE))
#define CHECK_LAST(bptr)            ((char *)mem_heap_hi() + 1 == NEXT_BLKP(bptr))

// prototype
void *extend_heap(int size);
void *coalesce(void *bptr);
void *first_fit(int size);
void set_succ_pred_addr(void *bptr);
void drop_freeblock(void *bptr);

static void *tail = NULL;
static void *head = NULL;
static void *lastBptr;
typedef enum {FREE, ALLOC} bit_check; 

static bit_check isLastAlloc;

/* 
 * 1. initialize heap memory as much as chunksize
 * 2. set dummy block to align memory
 */

int mm_init(void)
{   
    void *bptr;
    if ((bptr = mem_sbrk(DSIZE)) == (void *) -1)
        return -1;
    PUT_VALUE(bptr, 1); // dummy block
    head = NULL;
    lastBptr = NULL;
    isLastAlloc = FREE;
    if (extend_heap(CHUNKSIZE) == (void *) -1)
        return -1;
    return 0;
}

/* extend heap memory aligned to 8 bytes */
void *extend_heap(int size)
{
    int resize = ALIGN(size); 
    void *bptr = mem_sbrk(resize);
    if (bptr == (void *) -1)
        return (void *) -1;

    // set free block and merge adjacent free block 
    if (isLastAlloc == ALLOC)    
        resize = PACKING_BIT(resize, 2);
    PUT_VALUE(HEADPTR(bptr), resize);
    PUT_VALUE(FOOTPTR_MS(bptr, resize), resize); // this bit indicates preceding block whether alloc or not
    isLastAlloc = FREE;
    return coalesce(bptr);
}

/* find adjacent free block using header bit and merge usable free memories(4 case exists)*/
void *coalesce(void *freeBptr)
{   
    int notFstBlk = (mem_heap_lo() != (void *)((char *)(freeBptr) - DSIZE));
    int notLstBlk = !CHECK_LAST(freeBptr);
    int prevAllocBit = ALLOC; 
    int nextAllocBit = ALLOC;
    void *prevBptr = NULL;
    void *nextBPtr = NULL;
    if (notFstBlk && ((prevAllocBit = GET_PREV_ALLOC_BIT(freeBptr)) == FREE))
        prevBptr = PREV_BLKP(freeBptr);
    if (notLstBlk)
    {
        nextBPtr = NEXT_BLKP(freeBptr);
        nextAllocBit = GET_ALLOC_BIT(nextBPtr);
    }
        
    int resize = GET_SIZE(freeBptr);
    // checking previous and next blocks to merge while there are variables    
    if ((prevAllocBit == ALLOC) && (nextAllocBit == ALLOC))
    {   
        set_succ_pred_addr(freeBptr);
        return freeBptr;
    }
    else if ((prevAllocBit == ALLOC) && (nextAllocBit == FREE)) 
    {   
        resize += GET_SIZE(nextBPtr);
        PUT_VALUE(HEADPTR(freeBptr), PACKING_BIT(resize, 2));
        PUT_VALUE(FOOTPTR(nextBPtr), PACKING_BIT(resize, 2));
        drop_freeblock(nextBPtr);
        set_succ_pred_addr(freeBptr);
    }
    else if ((prevAllocBit == FREE) && (nextAllocBit == ALLOC)) 
    {
        resize += GET_SIZE(prevBptr);
        PUT_VALUE(HEADPTR(prevBptr), PACKING_BIT(resize, 2));
        PUT_VALUE(FOOTPTR(freeBptr), PACKING_BIT(resize, 2));
        drop_freeblock(prevBptr);
        set_succ_pred_addr(prevBptr);
        freeBptr = prevBptr;
    }
    else if ((prevAllocBit == FREE) && (nextAllocBit == FREE))
    {
        resize += GET_SIZE(prevBptr) + GET_SIZE(nextBPtr);
        PUT_VALUE(HEADPTR(prevBptr), PACKING_BIT(resize, 2));
        PUT_VALUE(FOOTPTR(nextBPtr), PACKING_BIT(resize, 2));
        drop_freeblock(nextBPtr);
        drop_freeblock(prevBptr);
        set_succ_pred_addr(prevBptr);
        freeBptr = prevBptr;
    }
    if (CHECK_LAST(freeBptr))
        lastBptr = freeBptr;
    return freeBptr;
}
/* place the new free bptr at in front of the linked free list */
void set_succ_pred_addr(void *bptr)
{   
    if (head == NULL)
    {   
        PUT_PTR(PREDPTR(bptr), NULL);  
        PUT_PTR(SUCCPTR(bptr), NULL);
        head = bptr;
        tail = bptr;
        return;
    }
    void *nowBptr = head;
    while ((nowBptr != NULL) && (bptr > nowBptr))
        nowBptr = GET_PTR(SUCCPTR(nowBptr));

    if (nowBptr == NULL)
    {   
        PUT_PTR(SUCCPTR(tail), bptr);
        PUT_PTR(PREDPTR(bptr), tail);
        PUT_PTR(SUCCPTR(bptr), NULL);
        tail = bptr;
    }
    else if (nowBptr == head)
    {
        PUT_PTR(PREDPTR(bptr), NULL);
        head = bptr;
        PUT_PTR(PREDPTR(nowBptr), bptr);
        PUT_PTR(SUCCPTR(bptr), nowBptr);
    }
    else
    {   
        void *nowBefBptr = GET_PTR(PREDPTR(nowBptr));
        PUT_PTR(PREDPTR(bptr), nowBefBptr);
        PUT_PTR(SUCCPTR(nowBefBptr), bptr);        
        PUT_PTR(PREDPTR(nowBptr), bptr);
        PUT_PTR(SUCCPTR(bptr), nowBptr);
    }
    
}

/* mm_free - Free a block and connects surrounding blocks   */
void mm_free(void *bptr)
{   
    int size = GET_WORD(HEADPTR(bptr));
    PUT_VALUE(HEADPTR(bptr), (size & ~0x1));    // set a first alloc bit to free bit
    PUT_VALUE(FOOTPTR(bptr), (size & ~0x1));
    
    if (CHECK_LAST(bptr))
        isLastAlloc = FREE;
    else // check an second alloc bit of the next block to being free bit
    {
        void *nextBptr = NEXT_BLKP_MS(bptr, size & ~0x7);
        PUT_VALUE(HEADPTR(nextBptr), (GET_WORD(HEADPTR(nextBptr)) & ~0x2));
        if (GET_ALLOC_BIT(nextBptr) == FREE)
            PUT_VALUE(FOOTPTR(nextBptr), (GET_WORD(HEADPTR(nextBptr)) & ~0x2));
    }
    coalesce(bptr);
}

/* find first place to fit in starting from head of free blocks */
void *first_fit(int targetSize) 
{   
    int nowSize;
    void *nowBptr = head;
    while ((nowBptr != NULL) && (targetSize > (nowSize = GET_SIZE(nowBptr))))
        nowBptr = GET_PTR(SUCCPTR(nowBptr));
    return nowBptr;
}

/* 
 * 1. find the best fit position using next-fit.
 * 2. allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{   
    if (size == 0)
        return NULL;
    // 1. find fit block
    int resize = ALIGN(size + WSIZE); // add only header size
    void *bptr = first_fit(resize); 
    int findSize;
    // if there is no place to put memory as much as size, then extend heap area.
    if (bptr == NULL) // last free block
    {   
        int extendSize = MAX(resize, CHUNKSIZE); // setting 4K page size
        if ((lastBptr != NULL) && GET_ALLOC_BIT(lastBptr) == FREE)
            extendSize -= GET_SIZE(lastBptr);
            
        if ((bptr = extend_heap(extendSize)) ==  (void *)-1)
        {
            fprintf(stderr, "ERROR: mm_malloc failed due to mem_sbrk\n");
            return NULL;
        }
        findSize = GET_SIZE(bptr);
    }
    else
        findSize = GET_SIZE(bptr);

    int freeSize = findSize - resize;
    void *nextBptr;
    // 2. place block
    drop_freeblock(bptr); 
    if (freeSize > QSIZE) 
    {   
        if (GET_PREV_ALLOC_BIT(bptr) == ALLOC)
            PUT_VALUE(HEADPTR(bptr), PACKING_BIT(resize, 3));   // set a second alloc bit whether alloc or not
        else
            PUT_VALUE(HEADPTR(bptr), PACKING_BIT(resize, 1));

        nextBptr = NEXT_BLKP_MS(bptr, resize); 
        PUT_VALUE(HEADPTR(nextBptr), PACKING_BIT(freeSize, 2));
        PUT_VALUE(FOOTPTR_MS(nextBptr, freeSize), PACKING_BIT(freeSize, 2));

        set_succ_pred_addr(nextBptr);
        if (CHECK_LAST(nextBptr))
        {
            isLastAlloc = FREE;
            lastBptr = nextBptr;
        }
    }
    else // if the rest of block is 16 byte(which can't put value)
    {   
        if (GET_PREV_ALLOC_BIT(bptr) == ALLOC)
            PUT_VALUE(HEADPTR(bptr), PACKING_BIT(findSize, 3));
        else
            PUT_VALUE(HEADPTR(bptr), PACKING_BIT(findSize, 1));

        if (CHECK_LAST(bptr))
        {
            isLastAlloc = ALLOC;
            lastBptr = bptr;
        }
        else // check an second alloc bit of the next block to being free bit
        {
            nextBptr = NEXT_BLKP_MS(bptr, findSize);
            PUT_VALUE(HEADPTR(nextBptr), PACKING_BIT(GET_WORD(HEADPTR(nextBptr)), 2));
            if (GET_ALLOC_BIT(nextBptr) == FREE)
                PUT_VALUE(FOOTPTR(nextBptr), GET_WORD(HEADPTR(nextBptr)));
        }
    }
    return bptr;
}

/* escape the block from linked free list (it is different from LIFO) */
void drop_freeblock(void *bptr)
{   
    void *freeNxtBptr = GET_PTR(SUCCPTR(bptr));
    void *freeBefBptr = GET_PTR(PREDPTR(bptr));
    if (freeNxtBptr != NULL)
        PUT_PTR(PREDPTR(freeNxtBptr), freeBefBptr);
    else
        tail = freeBefBptr;

    if (freeBefBptr != NULL)
        PUT_PTR(SUCCPTR(freeBefBptr), freeNxtBptr);
    else
        head = freeNxtBptr;
    // delete dangling ptr
    PUT_PTR(SUCCPTR(bptr), NULL);
    PUT_PTR(PREDPTR(bptr), NULL);
    if ((freeNxtBptr == NULL) && (freeBefBptr == NULL))
    {
        head = NULL;
        tail = NULL;
    }
        
}

/* free allocated space first and then migrate data */
void *mm_realloc(void *oldBptr, size_t size)
{   
    if (oldBptr == NULL)
        return (void *)-1;

    // to back-up data for mm_free
    int oldSize = GET_SIZE(oldBptr);
    int tempSucc, tempPred, tempFoot, originSize;
    tempSucc = GET_WORD(SUCCPTR(oldBptr));
    tempPred = GET_WORD(PREDPTR(oldBptr));
    tempFoot = GET_WORD(FOOTPTR(oldBptr));
    originSize = GET_SIZE(oldBptr);
    
    mm_free(oldBptr); 
    if (size == 0)
        return oldBptr;

    // find-fit is same as mm_malloc
    int resize = ALIGN(size + WSIZE); 
    void *newBptr = first_fit(resize);     
    int findSize;
    if (newBptr == NULL) // last free block
    {   
        int extendSize = MAX(resize, CHUNKSIZE); 
        if (GET_ALLOC_BIT(lastBptr) == FREE)
            extendSize -= GET_SIZE(lastBptr);

        if ((newBptr = extend_heap(extendSize)) ==  (void *)-1)
        {
            fprintf(stderr, "ERROR: mm_realloc failed due to mem_sbrk\n");
            return NULL;
        }
        findSize = GET_SIZE(newBptr);
    }
    else 
        findSize = GET_SIZE(newBptr);

    int freeSize = findSize - resize;
    // migrate data
    if (resize < oldSize)
        oldSize = resize; 
    
    drop_freeblock(newBptr); 
    if (oldBptr > newBptr) // it is posible to overlapping data by the order of calling memmove
    {
        if (freeSize > QSIZE) 
        {   
            if (GET_PREV_ALLOC_BIT(newBptr) == ALLOC)
                PUT_VALUE(HEADPTR(newBptr), PACKING_BIT(resize, 3)); // set a second alloc bit whether alloc or not
            else
                PUT_VALUE(HEADPTR(newBptr), PACKING_BIT(resize, 1));
            memmove(newBptr, oldBptr, oldSize - WSIZE);

            void *nextBptr = NEXT_BLKP(newBptr);
            PUT_VALUE(HEADPTR(nextBptr), PACKING_BIT(freeSize, 2));
            PUT_VALUE(FOOTPTR_MS(nextBptr, freeSize), PACKING_BIT(freeSize, 2));
            set_succ_pred_addr(nextBptr);
            if (CHECK_LAST(nextBptr))
            {
                isLastAlloc = FREE;
                lastBptr = nextBptr;
            }
        }
        else // avoid fragmentation
        {   
            
            if (GET_PREV_ALLOC_BIT(newBptr) == ALLOC)
                PUT_VALUE(HEADPTR(newBptr), PACKING_BIT(findSize, 3));
            else
                PUT_VALUE(HEADPTR(newBptr), PACKING_BIT(findSize, 1));
            
            memmove(newBptr, oldBptr, oldSize - WSIZE);
            if (CHECK_LAST(newBptr))
            {
                isLastAlloc = ALLOC;
                lastBptr = newBptr;
            }
            else
            {
                void *nextBptr = NEXT_BLKP(newBptr);
                PUT_VALUE(HEADPTR(nextBptr), PACKING_BIT(GET_WORD(HEADPTR(nextBptr)), 2));
                if (GET_ALLOC_BIT(nextBptr) == FREE)
                    PUT_VALUE(FOOTPTR(nextBptr), PACKING_BIT(GET_WORD(HEADPTR(nextBptr)), 2));
            }
        }
    }
    else 
    {
        if (freeSize > QSIZE) 
        {   
            memmove(newBptr, oldBptr, oldSize - WSIZE);
            if (GET_PREV_ALLOC_BIT(newBptr) == ALLOC)
                PUT_VALUE(HEADPTR(newBptr), PACKING_BIT(resize, 3));
            else
                PUT_VALUE(HEADPTR(newBptr), PACKING_BIT(resize, 1));

            void *nextBptr = NEXT_BLKP(newBptr);
            PUT_VALUE(HEADPTR(nextBptr), PACKING_BIT(freeSize, 2));
            PUT_VALUE(FOOTPTR_MS(nextBptr, freeSize), PACKING_BIT(freeSize, 2));
            
            set_succ_pred_addr(nextBptr);
            if (CHECK_LAST(nextBptr))
            {
                isLastAlloc = FREE;
                lastBptr = nextBptr;
            }
        }
        else // avoid fragmentation
        {   
            memmove(newBptr, oldBptr, oldSize - WSIZE);
            if (GET_PREV_ALLOC_BIT(newBptr) == ALLOC)
                PUT_VALUE(HEADPTR(newBptr), PACKING_BIT(findSize, 3));
            else
                PUT_VALUE(HEADPTR(newBptr), PACKING_BIT(findSize, 1));
            
            if (CHECK_LAST(newBptr))
            {
                isLastAlloc = ALLOC;
                lastBptr = newBptr;
            }
            else
            {
                void *nextBptr = NEXT_BLKP(newBptr);
                PUT_VALUE(HEADPTR(nextBptr), PACKING_BIT(GET_WORD(HEADPTR(nextBptr)), 2));
                if (GET_ALLOC_BIT(nextBptr) == FREE)
                    PUT_VALUE(FOOTPTR(nextBptr), PACKING_BIT(GET_WORD(HEADPTR(nextBptr)), 2));
            }
        }        
    }
    PUT_VALUE(SUCCPTR(newBptr), tempSucc);
    PUT_VALUE(PREDPTR(newBptr), tempPred);
    if (originSize <= resize)
        PUT_VALUE((char *)(newBptr) + originSize - DSIZE, tempFoot);

    return newBptr;
}