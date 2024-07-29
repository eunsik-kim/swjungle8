/*
 * This is assignment from swjungle week5.
 * Assignment goal is to make 4 version of malloc-lab.
 * Codes to be implemented are written in Computer Systems, chapter 9.9 by Randal, David
 * 1. implicit free list  
 * 2. explicit free list 
 * 3. segregated-fit << search first-fit in asending order
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
#define INDEXSIZE ((1<<7) + 7)
#define EPILOGUESIZE 0 
#define MAX(x, y) ((x) > (y) ? (x): (y))
#define MIN(x, y) ((x) > (y) ? (y): (x))

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

#define FOOTPTR(bptr)               ((char *)(bptr) + GET_SIZE(bptr) - DSIZE)
#define FOOTPTR_MS(bptr, size)      ((char *)(bptr) + (size) - DSIZE)

#define NEXT_BLKP(bptr)             ((char *)(bptr) + GET_SIZE(bptr))
#define NEXT_BLKP_MS(bptr, size)    ((char *)(bptr) + (size))
#define PREV_BLKP(bptr)             ((char *)(bptr) - GET_SIZE((char *)(bptr) - WSIZE))
#define CHECK_LAST(bptr)            ((char *)mem_heap_hi() + 1 == NEXT_BLKP(bptr))
#define DELETE_DANGLE(bptr)         {PUT_PTR(SUCCPTR(bptr), NULL); \
                                    PUT_PTR(PREDPTR(bptr), NULL); }              

// prototype
void *extend_heap(int size);
void *coalesce(void *bptr);
void *first_fit(int size);
void set_succ_pred_ascend(void *bptr);
void drop_freeblock(void *bptr);
void **get_headptr(int alignSize);
void print_freeblock();
void check_freeblock_alive();
void check_freeblock_small();

static void **head = NULL;
static void *lastBptr;
typedef enum {FREE, ALLOC} bit_check; 
//static void *survive_ptr = 0xf68c6ed8;
//static int alive_flg = 0;

/* 
 * initialize free linked list header blocks to indicates pointer of each linked list
 * there are 128 headers until 1024 divided by 8 bytes(memory aligned) and additional 7 blocks until (2**16)[total 135 header blocks]
 */
int mm_init(void)
{   
    void *bptr;
    if ((bptr = mem_sbrk(WSIZE * (INDEXSIZE+1))) == (void *) -1)
        return -1;
    head = bptr;
    lastBptr = NULL;
    for (int i = 0; i < (INDEXSIZE); i++)
        PUT_PTR(((unsigned int *)head + i), NULL);
    PUT_VALUE((unsigned int *)head + INDEXSIZE, 1); // dummy block
    return 0;
}

/* extend heap memory aligned to 8 bytes */
void *extend_heap(int size)
{   
    void *bptr;
    if ((bptr = mem_sbrk(size)) == (void *) -1)
        return (void *) -1;

    // set free block and merge adjacent free block 
    PUT_VALUE(HEADPTR(bptr), size);
    PUT_VALUE(FOOTPTR_MS(bptr, size), size); 
    return coalesce(bptr);
}

/* find adjacent free block using header bit and merge usable free memories(4 case exists)*/
void *coalesce(void *freeBptr)
{   
    int notFstBlk = ((char *)head != ((char *)(freeBptr) - (INDEXSIZE+1) * WSIZE));
    int notLstBlk = !CHECK_LAST(freeBptr);
    int prevAllocBit = ALLOC;
    int nextAllocBit = ALLOC;
    void *prevBptr, *nextBPtr;
    if (notFstBlk)
    {
        prevBptr = PREV_BLKP(freeBptr);
        prevAllocBit = GET_ALLOC_BIT(prevBptr);
    }
    if (notLstBlk)
    {
        nextBPtr = NEXT_BLKP(freeBptr);
        nextAllocBit = GET_ALLOC_BIT(nextBPtr);
    }    
    int resize = GET_SIZE(freeBptr);
    if ((prevAllocBit == ALLOC) && (nextAllocBit == FREE)) 
    {   
        resize += GET_SIZE(nextBPtr);
        drop_freeblock(nextBPtr);               // delete from linked free list
        DELETE_DANGLE(nextBPtr);
        PUT_VALUE(HEADPTR(freeBptr), resize);
        PUT_VALUE(FOOTPTR(nextBPtr), resize);
    }
    else if ((prevAllocBit == FREE) && (nextAllocBit == ALLOC)) 
    {
        resize += GET_SIZE(prevBptr);
        drop_freeblock(prevBptr);
        PUT_VALUE(HEADPTR(prevBptr), resize);
        PUT_VALUE(FOOTPTR(freeBptr), resize);
        freeBptr = prevBptr;
    }
    else if ((prevAllocBit == FREE) && (nextAllocBit == FREE))
    {   
        resize += GET_SIZE(prevBptr) + GET_SIZE(nextBPtr);
        drop_freeblock(nextBPtr);
        DELETE_DANGLE(nextBPtr);
        drop_freeblock(prevBptr);
        PUT_VALUE(HEADPTR(prevBptr), resize);
        PUT_VALUE(FOOTPTR(nextBPtr), resize);
        freeBptr = prevBptr;
    }
    set_succ_pred_ascend(freeBptr);             // reconnect entire merged block
    if (CHECK_LAST(freeBptr))
        lastBptr = freeBptr;  
    return freeBptr;
}

/* escape the block from linked free list (it is different from addre)*/
void drop_freeblock(void *bptr)
{   
    int size = GET_SIZE(bptr);
    void **headBptr = get_headptr(size);
    void *freeNxtBptr = GET_PTR(SUCCPTR(bptr));
    void *freeBefBptr = GET_PTR(PREDPTR(bptr));

    if (freeNxtBptr != NULL)
        PUT_PTR(PREDPTR(freeNxtBptr), freeBefBptr);

    if (freeBefBptr != NULL)
        PUT_PTR(SUCCPTR(freeBefBptr), freeNxtBptr);
    else
        PUT_PTR(headBptr, freeNxtBptr);
    check_freeblock_small(); //why does this function make dummy data clean?
}

/* insert the bptr reconnecting between adjecent blocks in ascending order */
void set_succ_pred_ascend(void *bptr)
{   
    int size = GET_SIZE(bptr);
    void **headBptr = get_headptr(size);
    void *nowBptr = GET_PTR(headBptr);
    if (nowBptr != NULL)
    {   
        int nowSize;
        void *nextBptr;
        while (size > (nowSize = GET_SIZE(nowBptr)))
            {   
                nextBptr = GET_PTR(SUCCPTR(nowBptr));
                if (nextBptr == NULL)           // checking whether last block is NULL
                    break;
                nowBptr = nextBptr;
            }
        if (nextBptr == NULL)                   // when insert into last block
        {
            PUT_PTR(SUCCPTR(nowBptr), bptr);
            PUT_PTR(PREDPTR(bptr), nowBptr);   
            PUT_PTR(SUCCPTR(bptr), NULL);    
        }
        else
        {   
            void *befBptr = GET_PTR(PREDPTR(nowBptr));
            if (befBptr != NULL)                // when insert into middle block
                PUT_PTR(SUCCPTR(befBptr), bptr);
            else    
                PUT_PTR(headBptr, bptr);        // when insert into first block
            PUT_PTR(PREDPTR(bptr), befBptr);   
            
            PUT_PTR(SUCCPTR(bptr), nowBptr);
            PUT_PTR(PREDPTR(nowBptr), bptr);
        }
    }
    else    // no headBptr
    {   
        PUT_PTR(SUCCPTR(bptr), NULL);
        PUT_PTR(PREDPTR(bptr), NULL);
        PUT_PTR(headBptr, bptr);
    }
}

/* find the head ptr of linked free list which fits size */
void **get_headptr(int alignSize)
{   
    int index;
    alignSize -= 8; // header and footer 
    if (alignSize <= 1024)
        index = (alignSize >> 3) - 1;
    else
    {   
        alignSize -=1 ;
        int resize_idx = 0;
        alignSize >>= 10;
        while (alignSize && (resize_idx < 7)) // maximum 7
        {
            alignSize >>= 1;
            resize_idx++;
        }
        index = resize_idx + (1 << 7) - 1;
    }
    return (void **)((unsigned int *)head + index);
}

/* mm_free - Free a block and coalesce surrounding blocks */
void mm_free(void *bptr)
{   
    int size = GET_SIZE(bptr);
    PUT_VALUE(HEADPTR(bptr), size);
    PUT_VALUE(FOOTPTR_MS(bptr, size), size);
    coalesce(bptr);
    // if (bptr == survive_ptr)
    //   alive_flg = 1;
}

/* find first place to fit in starting from head of free blocks */
void *first_fit(int targetSize) 
{   
    unsigned int *lastHeadBptr = (unsigned int *)head + INDEXSIZE; // to check the end of linked lists
    unsigned int *headBptr = (unsigned int *)get_headptr(targetSize);
    void *nowBptr;
    do {   
        nowBptr = GET_PTR(headBptr);
        headBptr += 1;

        if ((nowBptr != NULL) && (targetSize <= 1032))
            return nowBptr;

        int nowSize;
        while ((nowBptr != NULL) && (targetSize > (nowSize = GET_SIZE(nowBptr))))
            {   
                nowBptr = GET_PTR(SUCCPTR(nowBptr));
            }

        if ((nowBptr != NULL) && (targetSize <= nowSize))
            return nowBptr;
    } while ((nowBptr == NULL) && (headBptr != lastHeadBptr));

    return nowBptr;
}

/* debugging function that visualize all of the free linked list */
void print_freeblock() 
{   
    printf("=======================printf free block start===used:%d=========\n", mem_heapsize());
    int i = 0;
    unsigned int *lastHeadBptr = (unsigned int *)head + INDEXSIZE; // to check the end of linked lists
    unsigned int *headBptr = (unsigned int *)get_headptr(16);
    void *nowBptr;
    int headerSize = 8;
    int j = 0;
    do {   
        nowBptr = GET_PTR(headBptr);
        if (nowBptr != NULL)
            printf("headerblock location %p | header size %d\n", (unsigned int *)headBptr, headerSize);
        while (nowBptr != NULL) 
            {   
                printf("%d. free block location %p | now size %d | front ptr %p | back ptr %p\n", ++i, (unsigned int *)nowBptr, GET_WORD(HEADPTR(nowBptr)), GET_PTR(PREDPTR(nowBptr)), GET_PTR(SUCCPTR(nowBptr))); 
                nowBptr = GET_PTR(SUCCPTR(nowBptr));
            }
        headBptr += 1;
        j++;
        if (j <= 128)
            headerSize += 8; 
        else
            headerSize = (headerSize-8) * 2 + 8; 
    } while ((nowBptr == NULL) && (headBptr != lastHeadBptr));
    printf("=======================printf free block end===========================\n");
}

/* check the specific block is alive from linked free list */
// void check_freeblock_alive() 
// {   
//     unsigned int *lastHeadBptr = (unsigned int *)head + INDEXSIZE; // to check the end of linked lists
//     unsigned int *headBptr = (unsigned int *)get_headptr(16);
//     void *nowBptr;
//     int headerSize = 8;
//     int j = 0;
//     int isalive = 0;
//     do {   
//         nowBptr = GET_PTR(headBptr);
//         while (nowBptr != NULL) 
//         {   
//             if (alive_flg && (nowBptr == 0xf68c6ed8))
//                 isalive = 1;
//             nowBptr = GET_PTR(SUCCPTR(nowBptr));  
//         }
//         headBptr += 1; 
//         j++;
//         if (j <= 128)
//             headerSize += 8; 
//         else
//             headerSize = (headerSize-8) * 2 + 8; 
//     } while ((nowBptr == NULL) && (headBptr != lastHeadBptr));
//     assert(alive_flg == isalive);
// }

/* check that all headptr block is smaller than child */
void check_freeblock_small() 
{   
    unsigned int *lastHeadBptr = (unsigned int *)head + INDEXSIZE; // to check the end of linked lists
    unsigned int *headBptr = (unsigned int *)get_headptr(16);
    void *nowBptr = NULL;
    int headerSize = 8;
    int j = 0;
    int nowSize;
    do {  
        break;
        nowBptr = GET_PTR(headBptr);
        while (nowBptr != NULL) 
        {   
            nowSize = GET_SIZE(nowBptr);
            nowBptr = GET_PTR(SUCCPTR(nowBptr));   
            assert(headerSize <= nowSize);
        }
        headBptr += 1; 
        j++;
        if (j <= 128)
            headerSize += 8; 
        else
            headerSize = (headerSize-8) * 2 + 8; 
    } while ((nowBptr == NULL) && (headBptr != lastHeadBptr));
}

/* 
 * 1. find the best fit position using first-fit.
 * 2. allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{   
    if (size == 0)
        return NULL;
    // 1. find fit block
    int resize = ALIGN(size + DSIZE); // add header and footer size
    void *bptr = first_fit(resize); 
   
    // if there is no place to put memory as much as size, then extend heap area.
    if (bptr == NULL) // last free block
    {   
        int extendSize = resize;
        if ((lastBptr != NULL) && (GET_ALLOC_BIT(lastBptr) == FREE)) // add up the last free block
            extendSize -= GET_SIZE(lastBptr);
            
        if ((bptr = extend_heap(extendSize)) == (void *) - 1)
        {
            fprintf(stderr, "ERROR: mm_malloc failed due to mem_sbrk\n");
            return NULL;
        }
    }
    int findSize = GET_SIZE(bptr);
    int freeSize = findSize - resize;
    void *nextBptr;
    // 2. place block

    drop_freeblock(bptr); 
    DELETE_DANGLE(bptr);
    if (freeSize >= QSIZE) 
    {   
        PUT_VALUE(HEADPTR(bptr), PACKING_BIT(resize, 1));
        PUT_VALUE(FOOTPTR_MS(bptr, resize), PACKING_BIT(resize, 1));
        nextBptr = NEXT_BLKP_MS(bptr, resize);
        PUT_VALUE(HEADPTR(nextBptr), freeSize);
        PUT_VALUE(FOOTPTR_MS(nextBptr, freeSize), freeSize);
        set_succ_pred_ascend(nextBptr);
        if (CHECK_LAST(nextBptr))
            lastBptr = nextBptr;
    }
    else // if the rest of block is 16 byte(which can't put value)
    {   
        PUT_VALUE(HEADPTR(bptr), PACKING_BIT(findSize, 1));
        PUT_VALUE(FOOTPTR_MS(bptr, findSize), PACKING_BIT(findSize, 1));
        if (CHECK_LAST(bptr))
            lastBptr = bptr;
    }
    return bptr;
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
    int resize = ALIGN(size + DSIZE); 
    void *newBptr = first_fit(resize);     

    if (newBptr == NULL) // last free block
    {   
        int extendSize = resize;
        if (GET_ALLOC_BIT(lastBptr) == FREE)
            extendSize -= GET_SIZE(lastBptr);

        if ((newBptr = extend_heap(extendSize)) ==  (void *)-1)
        {
            fprintf(stderr, "ERROR: mm_realloc failed due to mem_sbrk\n");
            return NULL;
        }
    }
    int findSize = GET_SIZE(newBptr);
    int freeSize = findSize - resize;
    // migrate data
    if (resize < oldSize)
        oldSize = resize; 

    void *nextBptr;
    drop_freeblock(newBptr); 
    DELETE_DANGLE(newBptr);
    if (oldBptr > newBptr) // it is posible to overlapping data by the order of calling memmove()
    {
        if (freeSize >= QSIZE) 
        {   
            PUT_VALUE(HEADPTR(newBptr), PACKING_BIT(resize, 1));                // set a second alloc bit whether alloc or not
            memmove(newBptr, oldBptr, oldSize - DSIZE);
            PUT_VALUE(FOOTPTR_MS(newBptr, resize), PACKING_BIT(resize, 1));
            nextBptr = NEXT_BLKP(newBptr);
            PUT_VALUE(HEADPTR(nextBptr), freeSize);
            PUT_VALUE(FOOTPTR_MS(nextBptr, freeSize), freeSize);
            set_succ_pred_ascend(nextBptr);
            if (CHECK_LAST(nextBptr))
                lastBptr = nextBptr;
        }
        else // avoid fragmentation
        {   
            PUT_VALUE(HEADPTR(newBptr), PACKING_BIT(findSize, 1));
            if (CHECK_LAST(newBptr))
                lastBptr = newBptr;
            memmove(newBptr, oldBptr, oldSize - DSIZE);
            PUT_VALUE(FOOTPTR_MS(newBptr, findSize), PACKING_BIT(findSize, 1));
        }
    }
    else 
    {
        if (freeSize >= QSIZE) 
        {   
            PUT_VALUE(FOOTPTR_MS(newBptr, resize), PACKING_BIT(resize, 1));
            memmove(newBptr, oldBptr, oldSize - WSIZE);
            PUT_VALUE(HEADPTR(newBptr), PACKING_BIT(resize, 1));
            
            nextBptr = NEXT_BLKP(newBptr);
            PUT_VALUE(HEADPTR(nextBptr), freeSize);
            PUT_VALUE(FOOTPTR_MS(nextBptr, freeSize), freeSize);
            set_succ_pred_ascend(nextBptr);
            if (CHECK_LAST(nextBptr))
                lastBptr = nextBptr;
        }
        else // avoid fragmentation
        {   
            PUT_VALUE(FOOTPTR_MS(newBptr, findSize), PACKING_BIT(findSize, 1));
            memmove(newBptr, oldBptr, oldSize - WSIZE);
            PUT_VALUE(HEADPTR(newBptr), PACKING_BIT(findSize, 1));
            if (CHECK_LAST(newBptr))
                lastBptr = newBptr;
        }        
    }
    PUT_VALUE(SUCCPTR(newBptr), tempSucc);
    PUT_VALUE(PREDPTR(newBptr), tempPred);
    if (originSize <= resize)
        PUT_VALUE((char *)(newBptr) + originSize - DSIZE, tempFoot);
    return newBptr;
}
