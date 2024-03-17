#include <stdlib.h>
#include <stdio.h>
#include "heaplib.h"
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

/* -------------------- DEFINITIONS ----------------- */

typedef struct _block_header_t {
    unsigned int allocated;
    void *prev;
    void *next;
} block_header_t;

typedef struct _heap_header_t {
    unsigned int heap_size;
    block_header_t *heap_start;
} heap_header_t;

/* ------------------ PRINT DEBUG -------------------------*/

void print_debug_init(){
#ifdef PRINT_DEBUG
    printf("starting the init function\n");
#endif
}

void print_debug_alloc(){
#ifdef PRINT_DEBUG
    printf("starting the alloc function\n");
#endif
}

void print_debug_alloc1(){
#ifdef PRINT_DEBUG
    printf("in while loop, looking for free block\n"); 
#endif
}

void print_debug_alloc2(){
#ifdef PRINT_DEBUG
    printf("found a free block, checking if enough size\n");
#endif
}

void print_debug_alloc3(){
#ifdef PRINT_DEBUG
    printf("block not large enough\n");
#endif
}

void print_debug_alloc4(){
#ifdef PRINT_DEBUG
    printf("block large enough, checking if need to break into multiple blocks\n");
#endif
}

void print_debug_alloc5(){
#ifdef PRINT_DEBUG
    printf("breaking the block into two parts\n");
#endif
}

void print_debug_alloc6(){
#ifdef PRINT_DEBUG
    printf("finished allocating, leaving function\n");
#endif
}

void print_debug_alloc7(){
#ifdef PRINT_DEBUG
    printf("outside of while loop, checking the last block\n");
#endif
}

void print_debug_alloc8(){
#ifdef PRINT_DEBUG
    printf("allocating in the last block\n");
#endif
}

void print_debug_alloc9(){
#ifdef PRINT_DEBUG
    printf("block is already allocated, checking next one\n");
#endif
}

void print_debug_create_block(){
#ifdef PRINT_DEBUG
    printf("inside create block\n");
#endif
}

void print_debug_create_block1(){
#ifdef PRINT_DEBUG
    printf("just created the new block\n");
#endif
}

void print_debug_create_block2(){
#ifdef PRINT_DEBUG
    printf("exiting create block\n");
#endif
}

void print_debug_create_block3(){
#ifdef PRINT_DEBUG
    printf("set next to block->next\n");
#endif
}

void print_debug_create_block4(){
#ifdef PRINT_DEBUG
    printf("set prev to block\n");
#endif
}

void print_debug_create_block5(){
#ifdef PRINT_DEBUG
    printf("new->next is free\n");
#endif
}

void print_debug_create_block6(){
#ifdef PRINT_DEBUG
    printf("new->next isn't null but it's not free\n");
#endif
}

void print_debug_create_block7(){
#ifdef PRINT_DEBUG
    printf("new->next is null\n");
#endif
}

void print_debug_create_block8(){
#ifdef PRINT_DEBUG
    printf("new->next isn't null\n");
#endif
}

void print_debug_release1(){
#ifdef PRINT_DEBUG
    printf("set allocated to 0\n");
#endif
}

void print_debug_release2(){
#ifdef PRINT_DEBUG
    printf("next is not null\n");
#endif
}

void print_debug_release3(){
#ifdef PRINT_DEBUG
    printf("next is not allocated\n");
#endif
}

void print_debug_release4(){
#ifdef PRINT_DEBUG
    printf("previous is not null and next is not null\n");
#endif
}

void print_debug_release5(){
#ifdef PRINT_DEBUG
    printf("prev is not allocated\n");
#endif
}

void print_debug_release6(){
#ifdef PRINT_DEBUG
    printf("next is null and previous is not allocated\n");
#endif
}

void print_debug_release7(){
#ifdef PRINT_DEBUG
    printf("exiting release\n");
#endif
}

void print_debug_release8(){
#ifdef PRINT_DEBUG
    printf("set prev->nexto to current next\n");
#endif
}

void print_debug_release9(){
#ifdef PRINT_DEBUG
    printf("set next->prev to current previous\n");
#endif
}


void print_debug_resize1(){
#ifdef PRINT_DEBUG
    printf("starting resize\n");
#endif
}

void print_debug_resize2(){
#ifdef PRINT_DEBUG
    printf("next is not null\n");
#endif
}

void print_debug_resize3(){
#ifdef PRINT_DEBUG
    printf("next is not allocated\n");
#endif
}
void print_debug_resize4(){
#ifdef PRINT_DEBUG
    printf("next has enough space to resize\n");
#endif
}
void print_debug_resize5(){
#ifdef PRINT_DEBUG
    printf("resized to combine next and current block\n");
#endif
}
void print_debug_resize6(){
#ifdef PRINT_DEBUG
    printf("next doesn't have enough space, but prev is not null\n");
#endif
}
void print_debug_resize7(){
#ifdef PRINT_DEBUG
    printf("prev is not allocated\n");
#endif
}
void print_debug_resize8(){
#ifdef PRINT_DEBUG
    printf("prev and next together have enough space to resize\n");
#endif
}
void print_debug_resize9(){
#ifdef PRINT_DEBUG
    printf("resized by combining prev and next\n");
#endif
}
void print_debug_resize10(){
#ifdef PRINT_DEBUG
    printf("next is null, but prev is not null\n");
#endif
}
void print_debug_resize11(){
#ifdef PRINT_DEBUG
    printf("prev has enough space to resize\n");
#endif
}
void print_debug_resize12(){
#ifdef PRINT_DEBUG
    printf("resized by combining prev\n");
#endif
}


/* ------------------- HELPER FUNCTIONS ----------------- */

void create_block(block_header_t *block, unsigned int block_size) {
    print_debug_create_block();
    void *temp_next = block->next;
    uintptr_t next_addr = (uintptr_t) block + block_size + sizeof(block_header_t) + 8;
    next_addr -= next_addr % 8;
    block->next = (void *) next_addr;
    block_header_t *new_block = (block_header_t *) next_addr;
    print_debug_create_block1();
    new_block->next = temp_next;
    print_debug_create_block3();
    new_block->prev = block;
    print_debug_create_block4();
    new_block->allocated = 0;
    if (new_block->next != NULL) {
        block_header_t *next_block = new_block->next;
        print_debug_create_block8();
        if (next_block->allocated == 0) {
            print_debug_create_block5();
            new_block->next = next_block->next;
            if (next_block->next != NULL) {
                print_debug_create_block6();
                ((block_header_t *) (next_block->next))->prev = new_block;
            }
        } else {
            print_debug_create_block7();
            ((block_header_t *) (new_block->next))->prev = new_block;
        }
    }
    print_debug_create_block2();
}

int calc_block_size(heap_header_t *heap, block_header_t *block) {
    int temp_block_size = 0;
    uintptr_t current_addr = (uintptr_t) block;
    if (block->next == NULL) {
        temp_block_size = (uintptr_t) heap + heap->heap_size - current_addr;
        temp_block_size -= temp_block_size % 8;
    } else {
        uintptr_t next_addr = (uintptr_t) block->next;
        temp_block_size = (int) (next_addr - current_addr);
    }
    return temp_block_size - sizeof(block_header_t);
}

/* -------------------- THE BIG FOUR FNS ----------------- */

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
int hl_init(void *heap, unsigned int heap_size) {
    
    if (heap_size < MIN_HEAP_SIZE) {
        return FAILURE;
    }
    
    print_debug_init();

    heap_header_t *heap_header = (heap_header_t *) heap;
    heap_header->heap_size = heap_size;
    int header_size = sizeof(heap_header_t);
    uintptr_t start_addr = (uintptr_t) heap + (uintptr_t) header_size;
 
    block_header_t *block_header = (block_header_t *)start_addr;
    block_header->allocated = 0;
    block_header->prev = NULL;
    block_header->next = NULL;

    heap_header->heap_start = block_header;
    return SUCCESS;
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */

void *hl_alloc(void *heap, unsigned int block_size) {
    
    print_debug_alloc();
    
    heap_header_t *heap_header = (heap_header_t *) heap;
    block_header_t *block_ptr = heap_header->heap_start;

    int smallest_block_size = sizeof(block_header_t) + 8;
    int temp_block_size = 0;
    
    while (block_ptr->next != NULL) {
        print_debug_alloc1();
        if (block_ptr->allocated == 1) {
            block_ptr = (block_header_t *)(block_ptr->next);
            print_debug_alloc9();
        } else {
            print_debug_alloc2();
            temp_block_size = calc_block_size(heap_header, block_ptr);
            if (temp_block_size >= block_size) {
                print_debug_alloc4();
                block_ptr->allocated = 1;
                if (temp_block_size - block_size >= smallest_block_size) {
                    print_debug_alloc5();
                    create_block(block_ptr, block_size);      
                }
                print_debug_alloc6();
                return (void *) ((uintptr_t) block_ptr + sizeof(block_header_t));
            } else {
                print_debug_alloc3();
                block_ptr = (block_header_t *)(block_ptr->next);
            }
        }
    }
    
    print_debug_alloc7();
    temp_block_size = calc_block_size(heap_header, block_ptr);
    if (block_ptr->allocated == 0 && block_size <= temp_block_size){
        print_debug_alloc8();
        block_ptr->allocated = 1;
        int aligned_size = block_size + 8;
        aligned_size -= aligned_size % 8;
        if (temp_block_size - aligned_size > smallest_block_size) {
            create_block(block_ptr, block_size);
        }
        return (void *) ((uintptr_t) block_ptr + sizeof(block_header_t));
    }    
    return FAILURE;
}


/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void hl_release(void *heap, void *block) {
    void *block_start = (void *) ((uintptr_t) block - sizeof(block_header_t));
    block_header_t *block_head = (block_header_t *) block_start;
    block_head->allocated = 0;
    print_debug_release1();

    if (block_head->next != NULL) {
        print_debug_release2();
        block_header_t *next_block = (block_header_t *) (block_head->next); 
        if (next_block->allocated == 0){
            print_debug_release3();
            block_head->next = next_block->next;
            if (block_head->next != NULL) {
                ((block_header_t *) block_head->next)->prev = block_head;
            }
        }
        if (block_head->prev != NULL && ((block_header_t *) (block_head->prev))->allocated == 0){
            print_debug_release4();
            block_header_t *prev_block = (block_header_t *) (block_head->prev);
            prev_block->next = block_head->next;
            print_debug_release8();
            if (block_head->next != NULL) {
                ((block_header_t *) block_head->next)->prev = prev_block;
            }
            print_debug_release9();
        }
    } else {
        print_debug_release5();
        if (block_head->prev != NULL && ((block_header_t *) (block_head->prev))->allocated == 0) {
            print_debug_release6();
            block_header_t *prev_block = (block_header_t *) (block_head->prev);
            prev_block->next = NULL;
        }
    }
    print_debug_release7();
}

/* See the .h for the advertised behavior of this library function.
 * These comments describe the implementation, not the interface.
 *
 * YOUR COMMENTS GO HERE.
 */
void *hl_resize(void *heap, void *block, unsigned int new_size) {
    
    heap_header_t *heap_head = (heap_header_t *)heap;
    void *block_start = (void *) ((uintptr_t) block - sizeof(block_header_t));
    block_header_t *block_head = (block_header_t *) block_start;
    
    int block_size = calc_block_size(heap_head, block_head);
    int smallest_block_size = sizeof(block_header_t) + 8;
    
    print_debug_resize1();
 
    if (new_size == block_size){
        if (block_size - new_size >= smallest_block_size) {
            create_block(block_head, new_size);
        }
        return block;

    } else if (new_size < block_size) {
        if (block_size - new_size >= smallest_block_size) {
            create_block(block_head, new_size);
        }
        return block;
    } else {
        if (block_head->next != NULL) {
            print_debug_resize2();
            block_header_t *next_block = (block_header_t *) (block_head->next);
            if (next_block->allocated == 0) {
                print_debug_resize3();
                int next_block_size = calc_block_size(heap_head, next_block);
                if (block_size + next_block_size > new_size) {
                    print_debug_resize4();
                    block_head->next = next_block->next;
                    int new_block_size = calc_block_size(heap_head, block_head);
                    int aligned_size = new_size + 8;
                    aligned_size -= aligned_size % 8;
                    int aligned_block_size = new_block_size % 8;
                    if (aligned_block_size - aligned_size >= smallest_block_size) {
                        create_block(block_head, new_block_size);
                    } else {
                        if (next_block->next != NULL) {
                            ((block_header_t *) (next_block->next))->prev = block_head;
                        }
                    }
                    print_debug_resize5();
                    return block;
                } else if (block_head->prev != NULL) {
                    print_debug_resize6();
                    block_header_t *prev_block = (block_header_t *) (block_head->prev);
                    if (prev_block->allocated == 0) {
                        print_debug_resize7();
                        int prev_block_size = calc_block_size(heap_head, prev_block);
                        if (block_size + next_block_size + prev_block_size > new_size) {
                            print_debug_resize8();
                            block_head->allocated = 0;
                            prev_block->next = next_block->next;
                            prev_block->allocated = 1;
                            void *prev_block_start = (void *) ((uintptr_t) prev_block + sizeof(block_header_t));
                            memmove(prev_block_start, block, block_size);
                            int aligned_size = new_size + 8;
                            aligned_size -= aligned_size % 8;
                            if (block_size + next_block_size + prev_block_size - aligned_size >= smallest_block_size) {
                                create_block(prev_block, new_size);
                            } else {
                                if (next_block->next != NULL) {
                                    ((block_header_t *) (next_block->next))->prev = prev_block;
                                }
                            }
                            print_debug_resize9();
                            return prev_block_start;
                        }
                    }
                }
            }
        }         
        if (block_head->prev != NULL) {
            print_debug_resize10();
            block_header_t *prev_block = (block_header_t *) (block_head->prev);
            if (prev_block->allocated == 0) {
                print_debug_resize7();
                int prev_block_size = calc_block_size(heap_head, prev_block);
                if (block_size + prev_block_size > new_size) {
                    print_debug_resize11();
                    block_head->allocated = 0;
                    prev_block->next = block_head->next;
                    prev_block->allocated = 1;
                    void *prev_block_start = (void *) ((uintptr_t) prev_block + sizeof(block_header_t));
                    memmove(prev_block_start, block, block_size);
                    if (block_size + prev_block_size - new_size >= smallest_block_size) {
                        create_block(prev_block, new_size);
                    }
                    print_debug_resize12();
                    return prev_block_start;
                }
            }
        }
        
    }
    
    return FAILURE;
}

