/*
* For CS 2110 Spring 2024
* Author: Eric Kim
*/

/* we need this for uintptr_t */
#include <stdint.h>
/* we need this for memcpy/memset */
#include <string.h>
/* we need this to print out stuff*/
#include <stdio.h>
/* we need this for the metadata_t struct and my_malloc_err enum definitions */
#include "my_malloc.h"

/* Our freelist structure - our freelist is represented as a singly linked list
* the freelist is sorted by address;
*/
metadata_t *address_list;

/* Set on every invocation of my_malloc()/my_free()/my_realloc()/
* my_calloc() to indicate success or the type of failure. See
* the definition of the my_malloc_err enum in my_malloc.h for details.
* Similar to errno(3).
*/
enum my_malloc_err my_malloc_errno;


// -------------------- PART 1: Helper functions --------------------


/* HELPER FUNCTION: find_right
* Given a pointer to a free block, this function searches the freelist for another block to the right of the provided block.
* If there is a free block that is directly next to the provided block on its right side,
* then return a pointer to the start of the right-side block.
* Otherwise, return null.
* This function may be useful when implementing my_free().
*/
metadata_t *find_right(metadata_t *freed_block) {
    uintptr_t right_of_freed = ((uintptr_t) freed_block) + TOTAL_METADATA_SIZE + freed_block->size;
    metadata_t *curr = address_list;
    while (curr && ((uintptr_t) curr <= right_of_freed)) {
        if (right_of_freed == (uintptr_t) curr) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

/* GIVEN HELPER FUNCTION: find_left
* This function is provided for you by the TAs. You do not need to use it, but it may be helpful to you.
* This function is the same as find_right, but for the other side of the newly freed block.
* This function will be useful for my_free(), but it is also useful for my_malloc(), since whenever you sbrk a new block,
* you need to merge it with the block at the back of the freelist if the blocks are next to each other in memory.
*/

metadata_t *find_left(metadata_t *freed_block) {
    metadata_t *curr = address_list;

    while (curr && ((uintptr_t) freed_block > (uintptr_t) curr)) {
        uintptr_t right_of_curr = ((uintptr_t) curr) + TOTAL_METADATA_SIZE + curr->size;
        if (right_of_curr == (uintptr_t) freed_block) {
            return curr; // if `right_of_curr` is the freed block, then the `curr` block is directly left of `freed_block`
        }
        curr = curr->next;
    }

    return NULL;
}

/* HELPER FUNCTION: merge
* This function should take two pointers to blocks and merge them together.
* The most important step is to increase the total size of the left block to include the size of the right block.
* You should also copy the right block's next pointer to the left block's next pointer. If both blocks are initially in the freelist, this will remove the right block from the list.
* This function will be useful for both my_malloc() (when you have to merge sbrk'd blocks) and my_free().
*/
void merge(metadata_t *left, metadata_t *right) {
    left->size = left->size + right->size + TOTAL_METADATA_SIZE;
    left->next = right->next;
}

/* HELPER FUNCTION: split_block
* This function should take a pointer to a large block and a requested size, split the block in two, and return a pointer to the new block (the right part of the split).
* Remember that you must make the right side have the user-requested size when splitting. The left side of the split should have the remaining data.
* We recommend doing the following steps:
* 1. Compute the total amount of memory that the new block will take up (both metadata and user data).
* 2. Using the new block's total size with the address and size of the old block, compute the address of the start of the new block.
* 3. Shrink the size of the old/left block to account for the lost size. This block should stay in the freelist.
* 4. Set the size of the new/right block and return it. This block should not go in the freelist.
* This function will be useful for my_malloc(), particularly when the best-fit block is big enough to be split.
*/
metadata_t *split_block(metadata_t *block, size_t size) {
    size_t blockSize = size + TOTAL_METADATA_SIZE;
    metadata_t *newBlock = (metadata_t*) ((uintptr_t) block + TOTAL_METADATA_SIZE + block->size - blockSize);
    block->size -= blockSize;
    newBlock->size = size;
    newBlock->next = NULL;
    return newBlock;
}

/* HELPER FUNCTION: add_to_addr_list
* This function should add a block to freelist.
* Remember that the freelist must be sorted by address. You can compare the addresses of blocks by comparing the metadata_t pointers like numbers (do not dereference them).
* Don't forget about the case where the freelist is empty. Remember what you learned from Homework 9.
* This function will be useful for my_malloc() (mainly for adding in sbrk blocks) and my_free().
*/
void add_to_addr_list(metadata_t *block) {
    metadata_t *curr = address_list;
    // Empty addr list
    if (curr == NULL) {
        address_list = block;
        address_list->next = NULL;
        return;
    }
    // New data is before addr list
    if ((uintptr_t) curr > (uintptr_t) block) {
        address_list = block;
        block->next = curr;
        return;
    }
    // Middle or end of addr list
    metadata_t *next = curr->next;
    while (next != NULL && (uintptr_t) next < (uintptr_t) block) {
        curr = next;
        next = curr->next;
    }
    curr->next = block;
    block->next = next;
}

/* GIVEN HELPER FUNCTION: remove_from_addr_list
* This function is provided for you by the TAs. You are not required to use it or our implementation of it, but it may be helpful to you.
* This function should remove a block from the freelist.
* Simply search through the freelist, looking for a node whose address matches the provided block's address.
* This function will be useful for my_malloc(), particularly when the best-fit block is not big enough to be split.
*/
void remove_from_addr_list(metadata_t *block) {
    metadata_t *curr = address_list;
    if (!curr) {
        return;
    } else if (curr == block) {
        address_list = curr->next;
    }

    metadata_t *next;
    while ((next = curr->next) && (uintptr_t) block > (uintptr_t) next) {
        curr = next;
    }
    if (next == block) {
        curr->next = next->next;
    }
}
/* HELPER FUNCTION: find_best_fit
* This function should find and return a pointer to the best-fit block. See the PDF for the best-fit criteria.
* Remember that if you find the perfectly sized block, you should return it immediately.
* You should not return an imperfectly sized block until you have searched the entire list for a potential perfect block.
* Note that you should NOT be splitting the best-fit block in this function (do this within my_malloc() instead).
*/
metadata_t *find_best_fit(size_t size) {
    metadata_t *curr = address_list;
    metadata_t *best = NULL;
    while (curr != NULL) {
        if (curr->size == size) {
            return curr;
        } else if (size < curr->size) {
            if (best == NULL || best->size > curr->size) {
                best = curr;
            }
        }
        curr = curr->next;
    }
    return best;
}




// ------------------------- PART 2: Malloc functions -------------------------


/* MALLOC
* See PDF for documentation
*/
void *my_malloc(size_t size) {
    if (size == 0) {
        my_malloc_errno = NO_ERROR;
        return NULL;
    } else if (size > SBRK_SIZE - TOTAL_METADATA_SIZE) {
        my_malloc_errno = SINGLE_REQUEST_TOO_LARGE;
        return NULL;
    }
    metadata_t* best = find_best_fit(size);
    if (best == NULL) {
        metadata_t* new = my_sbrk(SBRK_SIZE);
        if ((long) new == -1) {
            my_malloc_errno = OUT_OF_MEMORY;
            return NULL;
        }
        new->size = SBRK_SIZE - TOTAL_METADATA_SIZE;
        add_to_addr_list(new);
        metadata_t* checkLeft = find_left(new);
        if (checkLeft != NULL) {
            merge(checkLeft, new);
            metadata_t* toRet = my_malloc(size);
            return toRet;
        } else {
            metadata_t* toRet = my_malloc(size);
            return toRet;
        }
    } else if (best->size == size || best->size - size < TOTAL_METADATA_SIZE + 1) {
        remove_from_addr_list(best);
        my_malloc_errno = NO_ERROR;
        return best + 1;
    } else {
        metadata_t* new = split_block(best, size);
        my_malloc_errno = NO_ERROR;
        return new + 1;
    }
}

/* FREE
* See PDF for documentation
*/
void my_free(void *ptr) {
    if (ptr == NULL) {
        my_malloc_errno = NO_ERROR;
        return;
    }
    metadata_t *toRemove = (metadata_t*) ((uintptr_t) ptr - TOTAL_METADATA_SIZE);
    add_to_addr_list(toRemove);
    metadata_t *rightCheck = find_right(toRemove);
    if (rightCheck != NULL) {
        merge(toRemove, rightCheck);
    }
    metadata_t *leftCheck = find_left(toRemove);
    if (leftCheck != NULL) {
        merge(leftCheck, toRemove);
    }
    my_malloc_errno = NO_ERROR;
}

/* REALLOC
* See PDF for documentation
*/
void *my_realloc(void *ptr, size_t size) {
    my_malloc_errno = NO_ERROR;
    if (ptr == NULL) {
        metadata_t* new = my_malloc(size);
        if (new == NULL) {
            return NULL;
        }
        return new;
    } else if (size == 0) {
        my_free(ptr);
        return NULL;
    } else {
        metadata_t* new = my_malloc(size);
        if (new == NULL) {
            return NULL;
        }
        memcpy(new, ptr, size);
        my_free(ptr);
        return new;
    }
}

/* CALLOC
* See PDF for documentation
*/
void *my_calloc(size_t nmemb, size_t size) {
    my_malloc_errno = NO_ERROR;
    metadata_t *new = my_malloc(nmemb * size);
    if (new == NULL) {
        return NULL;
    }
    memset(new, 0, nmemb * size);
    return new;
}
