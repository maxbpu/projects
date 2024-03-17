#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "heaplib.h"

#define HEAP_SIZE 1024
#define NUM_TESTS 24
#define NPOINTERS 100

// TODO: Add test descriptions as you add more tests...
const char* test_descriptions[] = {
    /* our SPEC tests */
    /* 0 */ "single init, should return without error",
    /* 1 */ "single init then single alloc, should pass",
    /* 2 */ "single alloc which should fail b/c heap is not big enough",
    /* 3 */ "multiple allocs, verifying no hard-coded block limit",
    /* your SPEC tests */
    /* 4  */ "single alloc, should return true if the address is divisible by 8",
    /* 5  */ "single release, should return true if can reallocate the released space",
    /* 6  */ "single resize, should return true if the address is divisible by 8",
    /* 7  */ "single resize, should succeed if address does not change",
    /* 8  */ "single resize, should succeed if resizing the block down",
    /* 9  */ "single resize, should fail because resizing block to greater than heap size",
    /* 10 */ "your description here",
    /* 11 */ "your description here",
    /* 12 */ "your description here",
    /* 13 */ "your description here",
    /* 14 */ "your description here",
    /* 15 */ "your description here",
    /* STRESS tests */
    /* 16 */ "alloc & free, stay within heap limits",
    /* 17 */ "alloc, release, & resize, pointer should not be outside of heap",
    /* 18 */ "alloc, release, & resize, data should not be outside of heap",
    /* 19 */ "alloc, release, & resize, should not corrupt previously stored data",
    /* 20 */ "alloc, release, & resize, should not allocate outside of heap",
    /* 21 */ "alloc, release, & resize, should not allocate outside of heap",
    /* 22 */ "your description here",
    /* 23 */ "alloc, release, & resize, should not corrupt previously stored data",
};

/* ------------------ COMPLETED SPEC TESTS ------------------------- */

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init
 * SPECIFICATION BEING TESTED:
 * hl_init should successfully complete (without producing a seg
 * fault) for a HEAP_SIZE of 1024 or more.
 *
 * MANIFESTATION OF ERROR:
 * A basic test of hl_init.  If hl_init has an eggregious programming
 * error, this simple call would detect the problem for you by
 * crashing.
 *
 * Note: this shows you how to create a test that should succeed.
 */
int test00() {

    // simulated heap is just a big array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    return SUCCESS;
}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * If there is room in the heap for a request for a block, hl_alloc
 * should sucessfully return a pointer to the requested block.
 *
 * MANIFESTATION OF ERROR:
 * The call to hl_alloc will return NULL if the library cannot find a
 * block for the user (even though the test is set up such that the
 * library should be able to succeed).
 */
int test01() {

    // simulated heap is just a big array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    // if this returns null, test01 returns FAILURE (==0)
    return (hl_alloc(heap, HEAP_SIZE/2) != NULL);
}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * If there is not enough room in the heap for a request for a block,
 * hl_alloc should return NULL.
 *
 * MANIFESTATION OF ERROR:
 * This test is designed to request a block that is definitely too big
 * for the library to find. If hl_alloc returns a pointer, the library is flawed.
 *
 * Notice that heaplame's hl_alloc does NOT return NULL. (This is one
 * of many bugs in heaplame.)
 *
 * Note: this shows you how to create a test that should fail.
 *
 * Surely it would be a good idea to test this SPEC with more than
 * just 1 call to alloc, no?
 */
int test02() {

    // simulated heap is just an array
    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    // if this returns NULL, test02 returns SUCCESS (==1)
    return !hl_alloc(heap, HEAP_SIZE*2);

}

/* THIS TEST IS COMPLETE AND WILL NOT BE INCOPORATED INTO YOUR GRADE.
 *
 * FUNCTIONS BEING TESTED: init & alloc
 * SPECIFICATION BEING TESTED:
 * There should be no hard-coded limit to the number of blocks heaplib
 * can support. So if the heap gets larger, so should the number of
 * supported blocks.
 *
 * MANIFESTATION OF ERROR:
 * See how many blocks are supported with heap size N. This number should
 * increase with heap size 2N. Otherwise fail!
 *
 * Note: unless it is fundamentally changed, heaplame will always fail
 * this test. That is fine. The test will have more meaning when run on
 * your heaplib.c
 */
int test03() {

    // now we simulate 2 heaps, once 2x size of the first
    char heap[HEAP_SIZE], heap2[HEAP_SIZE*2];
    int num_blocks = 0;
    int num_blocks2 = 0;

    hl_init(heap, HEAP_SIZE);

    while(true) {
        int *array = hl_alloc(heap, 8);
        if (array)
            num_blocks++;
        else
            break;
    }

    hl_init(heap2, HEAP_SIZE*2);

    while(true) {
        int *array = hl_alloc(heap2, 8);
        if (array)
            num_blocks2++;
        else
            break;
    }
#ifdef PRINT_DEBUG
    printf("%d blocks (n), then %d blocks (2n) allocated\n", num_blocks, num_blocks2);
#endif

    // hoping to return SUCCESS (==1)
    return (num_blocks2 > num_blocks);
}

/* ------------------ YOUR SPEC TESTS ------------------------- */

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: alloc
 * SPECIFICATION BEING TESTED:
 * The returned pointer address must be divisible by 8
 *
 * MANIFESTATION OF ERROR:
 * If it's not divisible by 8, should fail and return 0
 */
int test04() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    uintptr_t addr = (uintptr_t) hl_alloc(heap, 15); 
    return (addr % 8 == 0);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: release
 * SPECIFICATION BEING TESTED:
 * After a block has been released, can be reallocated 
 *
 * MANIFESTATION OF ERROR:
 * If hl_release doesn't successfully release the block, hl_alloc will fail
 */
int test05() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    hl_release(heap, hl_alloc(heap, HEAP_SIZE / 2));
    return (hl_alloc(heap, HEAP_SIZE / 2) != 0);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: resize
 * SPECIFICATION BEING TESTED:
 * hl_resize should return an address that is divisible by 8
 *
 * MANIFESTATION OF ERROR:
 * If it is not divisible by 8, should fail and return 0
 */
int test06() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    void *block = hl_alloc(heap, HEAP_SIZE / 2);
    uintptr_t addr = (uintptr_t) hl_resize(heap, block, HEAP_SIZE / 2 + 5);
    return (addr % 8 == 0);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize 
 * SPECIFICATION BEING TESTED: 
 * When resizing down, the pointer to the block shouldn't change
 * 
 *
 * MANIFESTATION OF ERROR:
 * If the pointer changes, should fail and return 0
 * 
 */
int test07() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    void *block1 = hl_alloc(heap, HEAP_SIZE / 2);
    void *block2 = hl_resize(heap, block1, HEAP_SIZE / 2 - 1);
    return (block1 == block2);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED:
 * When resizing down, should always be successful
 * 
 *
 * MANIFESTATION OF ERROR:
 * Resizing down should always be successful, so it should never return 0.
 * 
 */
int test08() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    void *block = hl_alloc(heap, HEAP_SIZE / 2);
    return (hl_resize(heap, block, HEAP_SIZE / 2 - 1) != 0);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED:
 * If can't resize, should return 0
 *
 *
 * MANIFESTATION OF ERROR:
 * If tries to resize the block to be larger than the heap, should fail and return 0
 *
 */
int test09() {
    char heap[HEAP_SIZE];
    hl_init(heap, HEAP_SIZE);
    void *block = hl_alloc(heap, HEAP_SIZE / 2);
    return !hl_resize(heap, block, HEAP_SIZE + 1);
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED:
 * 
 *
 * MANIFESTATION OF ERROR:
 * 
 *
 */
int test10() {
    return FAILURE;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: hl_resize
 * SPECIFICATION BEING TESTED:
 * 
 *
 * MANIFESTATION OF ERROR:
 * If after resizing the content changes, should fail
 *
 */
int test11() {
    return FAILURE;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test12() {

    return FAILURE;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED: 420 * 69 ;))
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test13() {
    //int hey_bb = 420 * 69;
    return FAILURE;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test14() {

    return FAILURE;
}

/* Find something that you think heaplame does wrong. Make a test
 * for that thing!
 *
 * FUNCTIONS BEING TESTED:
 * SPECIFICATION BEING TESTED:
 *
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test15() {

    return FAILURE;
}

/* ------------------ STRESS TESTS ------------------------- */

/* THIS TEST IS NOT FINISHED. It is a stress test, but it does not
 * actually test to see whether the library or the user writes
 * past the heap. You are encouraged to complete this test.
 *
 * FUNCTIONS BEING TESTED: alloc, free
 * SPECIFICATION BEING TESTED:
 * The library should not give user "permission" to write off the end
 * of the heap. Nor should the library ever write off the end of the heap.
 *
 * MANIFESTATION OF ERROR:
 * If we track the data on each end of the heap, it should never be
 * written to by the library or a good user.
 *
 */
int test16() {
    int n_tries    = 10000;
    int block_size = 16;

    // 1024 bytes of padding
    // --------------------
    // 1024 bytes of "heap"
    // --------------------  <-- heap_start
    // 1024 bytes of "padding"
    char memarea[HEAP_SIZE*3];

    char *heap_start = &memarea[1024]; // heap will start 1024 bytes in
    char *pointers[NPOINTERS];

    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));

    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        if (pointers[index] == 0) {
            pointers[index] = hl_alloc(heap_start, block_size);
            if (pointers[index] > 0 && (pointers[index] < heap_start || pointers[index] > &memarea[2048])){
                return FAILURE;
            }
        }
        else{
            hl_release(heap_start, pointers[index]);
            pointers[index] = 0;
        }
    }

    return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: alloc, resize, release
 * INTEGRITY OR DATA CORRUPTION?
 * Integrity
 *
 * MANIFESTATION OF ERROR:
 * Returned pointer should never be outside of the heap.
 *
 */
int test17() {
    int n_tries    = 10000;
    int block_size = 16;

    // 1024 bytes of padding
    // --------------------
    // 1024 bytes of "heap"
    // --------------------  <-- heap_start
    // 1024 bytes of "padding"
    char memarea[HEAP_SIZE*3];

    char *heap_start = &memarea[1024]; // heap will start 1024 bytes in
    char *pointers[NPOINTERS];

    // zero out the pointer array
    memset(pointers, 0, NPOINTERS*sizeof(char *));

    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        int resize = random() % 2;
        int new_block_size = random() % 100;
        if (pointers[index] == 0) {
            pointers[index] = hl_alloc(heap_start,  block_size);
            if (pointers[index] > 0 && (pointers[index] < heap_start || pointers[index] > &memarea[2048])){
                return FAILURE;
            }
        } else if (resize == 0){
            uintptr_t resize_block = (uintptr_t) hl_resize(heap_start, pointers[index], new_block_size);
            if (resize_block > 0) {
                pointers[index] = (char *) resize_block;
                if (pointers[index] < heap_start || pointers[index] > &memarea[2048]){
                    return FAILURE;
                }
            }
        } else {
            hl_release(heap_start, pointers[index]);
            pointers[index] = 0;
        }
    }

    return SUCCESS;
}

/* Stress the heap library and see if you can break it!
 * 
 * FUNCTIONS BEING TESTED: alloc, release
 * INTEGRITY OR DATA CORRUPTION? data integrity
 * 
 * MANIFESTATION OF ERROR:
 * if we save data before the start of heap or after 
 * the end of heap, return failure
 * 
 */
int test18() {

    int n_tries    = 10000;
    int block_size = 20;

    char memareal[HEAP_SIZE*3];
    memset(memareal, '0', HEAP_SIZE*3*sizeof(char));

    char *heap_start = &memareal[1024];
    char *pointers[NPOINTERS];

    int data[4] = {1,2,3,4};
    int data_in_pointers[NPOINTERS];

    memset(pointers, 0, NPOINTERS*sizeof(char *));
    memset(data_in_pointers, 0, NPOINTERS*sizeof(int));
    
    hl_init(heap_start, HEAP_SIZE);
    srandom(0);

    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        int resize = random() % 2;
        int new_block_size = random() % 100;
        if (pointers[index] == 0) {
            pointers[index] = hl_alloc(heap_start,  block_size);
            if (pointers[index] > 0) {
                for (int j = 0; j< 20 / 8; j++) {
                    *(pointers[index] + (j * sizeof(int))) = data[j % 4];
                }
                data_in_pointers[index] = 2; 
                uintptr_t data_addr = (uintptr_t) pointers[index] + block_size;
                if (data_addr > (uintptr_t) &memareal[2048] || data_addr < (uintptr_t) heap_start){
                    return FAILURE;
                }
            }
        } else if (resize == 0){
            uintptr_t resize_block = (uintptr_t) hl_resize(heap_start, pointers[index], new_block_size);
            if (resize_block > 0) {
                pointers[index] = (char *) resize_block;
                int new_array_size = new_block_size / 8;
                for (int j = 0; j < new_array_size; j++) {
                    *(pointers[index] + (j * sizeof(int))) = data[j % 4];
                }
                uintptr_t data_addr = (uintptr_t) pointers[index] + new_block_size;
                if (data_addr > (uintptr_t) &memareal[2048]){
                    return FAILURE;
                }
            }
        } else {
            hl_release(heap_start, pointers[index]);
            pointers[index] = 0;
        }
    }
        
    for (int i = 2048; i < 3072; i++){
        if (memareal[i] != '0'){
            return FAILURE;
        }
    }

    for (int i = 0; i < 1024; i++){
        if (memareal[i] != '0') {
            return FAILURE;
        }
    }

    return SUCCESS;

}

/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: alloc, release, resize
 * INTEGRITY OR DATA CORRUPTION? data integrity
 *
 * MANIFESTATION OF ERROR:
 * If between each function, the data isnt' preserved,
 * return failure (initial block size is not dividible
 * by 8) 
 * 
 */
int test19(){             

    int block_size = 57;
    int n_tries = 10000;

    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    char *pointers[NPOINTERS];
    int data[4] = {1,2,3,4};
    int data_in_pointers[NPOINTERS];
    memset(pointers, 0, NPOINTERS*sizeof(char *));
    memset(data_in_pointers, 0, NPOINTERS*sizeof(int));
    srandom(0);

    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        int resize = random() % 2;
        int new_block_size = random() % 128;
        if (pointers[index] == 0) {
            pointers[index] = hl_alloc(heap, block_size);
            if (pointers[index] > 0) {
                for (int j = 0; j < 57 / 8; j++){
                    *(pointers[index] + (j * sizeof(int))) = data[j % 4];
                }
                data_in_pointers[index] = 7;
            }
            for (int i = 0; i < NPOINTERS; i++){
                if (pointers[i] > 0 && data_in_pointers[i] > 0){
                    for (int j = 0; j < data_in_pointers[i]; j++){
                        if (*(pointers[i] + (j * sizeof(int))) != data[j % 4]){
                            return FAILURE;
                        }
                    }
                }
            }
        } else if (resize == 0){
            uintptr_t resize_block = (uintptr_t) hl_resize(heap, pointers[index], new_block_size);
            if (resize_block > 0){
                pointers[index] = (char *)resize_block;
                int new_array_size = new_block_size / 8;
                data_in_pointers[index] = new_array_size;
                for (int j = 0; j < new_array_size; j++){
                    *(pointers[index] + (j * sizeof(int))) = data[j % 4];
                }
            }
            for (int i = 0; i < NPOINTERS; i++){
                if (pointers[i] > 0 && data_in_pointers[i] > 0){
                    for (int j = 0; j < data_in_pointers[i]; j++){
                        if (*(pointers[i] + (j * sizeof(int))) != data[j % 4]){
                            return FAILURE;
                        }
                    }
                }
            }
        } else{
            hl_release(heap, pointers[index]);
            pointers[index] = 0;
        }
    }
    for (int i = 0; i < NPOINTERS; i++){
        if (pointers[i] > 0 && data_in_pointers[i] > 0){
             for (int j = 0; j < data_in_pointers[i]; j++){
                 if (*(pointers[i] + (j * sizeof(int))) != data[j % 4]){
                      return FAILURE;
                 }
             }
        }
    }

    return SUCCESS;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: alloc, resize, release
 * INTEGRITY OR DATA CORRUPTION? data integrity
 *
 * MANIFESTATION OF ERROR:
 * * after repetitive calls to the function, the allocated position
 * for both alloc and resize should not be smaller than the beginning
 * of the heap or larger than end of the heap
 * (resize block is always smaller than block_size)
 */
int test20() {

    int n_tries    = 10000;
    int block_size = 128;

    char memarea[HEAP_SIZE*3];

    char *heap_start = &memarea[1024]; 
    char *pointers[NPOINTERS];

    memset(pointers, 0, NPOINTERS*sizeof(char *));

    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        int resize = random() % 2;
        int new_block_size = random() % 128;
        if (pointers[index] == 0) {
            pointers[index] = hl_alloc(heap_start,  block_size);
            if (pointers[index] > 0 && (pointers[index] < heap_start || pointers[index] > &memarea[2048])){
                return FAILURE;
            }
        } else if (resize == 0){
            intptr_t resize_block = (uintptr_t) hl_resize(heap_start, pointers[index], new_block_size);
            if (resize_block > 0){
                pointers[index] = (char *)resize_block;
                if (pointers[index] < heap_start || pointers[index] > &memarea[2048]){
                    return FAILURE;
                }
            }
        } else {
            hl_release(heap_start, pointers[index]);
            pointers[index] = 0;
        }
    }

    return SUCCESS;
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: alloc, resize, release
 * INTEGRITY OR DATA CORRUPTION? data integrity
 *
 * MANIFESTATION OF ERROR:
 * after repetitive calls to the function, the allocated position
 * for both alloc and resize should not be smaller than the beginning
 * of the heap or larger than end of the heap
 * (resize block might be larger than block_size)
 */
int test21() {

    int n_tries    = 10000;
    int block_size = 8;

    char memarea[HEAP_SIZE*3];

    char *heap_start = &memarea[1024];
    char *pointers[NPOINTERS];

    memset(pointers, 0, NPOINTERS*sizeof(char *));

    hl_init(heap_start, HEAP_SIZE);
    srandom(0);
    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        int resize = random() % 2;
        int new_block_size = random() % 64;
        if (pointers[index] == 0) {
            pointers[index] = hl_alloc(heap_start,  block_size);
            if (pointers[index] > 0 && (pointers[index] < heap_start || pointers[index] > &memarea[2048])){
                return FAILURE;
            }
        } else if (resize == 0){
            uintptr_t resize_block = (uintptr_t) hl_resize(heap_start, pointers[index], new_block_size);
            if (resize_block > 0){
                pointers[index] = (char *)resize_block;
                if (pointers[index] < heap_start || pointers[index] > &memarea[2048]){
                    return FAILURE;
                }
            }
        } else {
            hl_release(heap_start, pointers[index]);
            pointers[index] = 0;
        }
    }

    return SUCCESS;

}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: 
 * INTEGRITY OR DATA CORRUPTION? 
 *
 * MANIFESTATION OF ERROR:
 *
 */
int test22() {
    
    int n_tries = 10000;
    
    char heap[HEAP_SIZE];
    
    hl_init(heap, HEAP_SIZE);
    
    char *pointers[NPOINTERS];
    int data[5] = {1,2,3,4,5};
    int data_in_pointers[NPOINTERS];
    memset(pointers, 0, NPOINTERS*sizeof(char *));
    memset(data_in_pointers, 0, NPOINTERS*sizeof(int));
    

    for (int i = 0; i < n_tries; i++) {
        int block_size = random() % 300;
        int index = random() % NPOINTERS;
        int resize = random() % 2;
        int new_block_size = random() % 500;
        if (pointers[index] == 0) {
            pointers[index] = hl_alloc(heap, block_size);
            if (pointers[index] > 0) {
                for (int j = 0; j < block_size / 8; j++){
                    *(pointers[index] + (j * sizeof(int))) = data[j % 5];
                }   
                data_in_pointers[index] = 8;
            }   
        } else if (resize == 0){
            uintptr_t resize_block = (uintptr_t) hl_resize(heap, pointers[index], new_block_size);
            if (resize_block > 0){ 
                pointers[index] = (char *)resize_block;
                int new_array_size = new_block_size / 8;
                data_in_pointers[index] = new_array_size;
                for (int j = 0; j < new_array_size; j++){
                    *(pointers[index] + (j * sizeof(int))) = data[j % 5];
                }   
            }   
        } else{
            hl_release(heap, pointers[index]);
            pointers[index] = 0;
        }   
    }   
    for (int i = 0; i < NPOINTERS; i++){
        if (pointers[i] > 0 && data_in_pointers[i] > 0){
             for (int j = 0; j < data_in_pointers[i]; j++){
                 if (*(pointers[i] + (j * sizeof(int))) != data[j % 5]){
                      return FAILURE;
                 }    
             }   
        }    
    }   
    

    return SUCCESS;

   
}


/* Stress the heap library and see if you can break it!
 *
 * FUNCTIONS BEING TESTED: alloc, release, resize
 * INTEGRITY OR DATA CORRUPTION? data corruption
 *
 * MANIFESTATION OF ERROR:
 * After repetitive calls to the functions, if the stored data
 * changes, return failure.
 * (resize block size might be larger than original block size)
 */
int test23() {

    int block_size = 64;
    int n_tries = 10000;

    char heap[HEAP_SIZE];

    hl_init(heap, HEAP_SIZE);

    char *pointers[NPOINTERS];
    int data[4] = {1,2,3,4};
    int data_in_pointers[NPOINTERS];
    memset(pointers, 0, NPOINTERS*sizeof(char *));
    memset(data_in_pointers, 0, NPOINTERS*sizeof(int));
    srandom(0);

    for (int i = 0; i < n_tries; i++) {
        int index = random() % NPOINTERS;
        int resize = random() % 2;
        int new_block_size = random() % 128;
        if (pointers[index] == 0) {
            pointers[index] = hl_alloc(heap, block_size);
            if (pointers[index] > 0) {
                for (int j = 0; j < 64 / 8; j++){
                    *(pointers[index] + (j * sizeof(int))) = data[j % 4];
                }
                data_in_pointers[index] = 8;
            }
        } else if (resize == 0){
            uintptr_t resize_block = (uintptr_t) hl_resize(heap, pointers[index], new_block_size);
            if (resize_block > 0){
                pointers[index] = (char *)resize_block;
                int new_array_size = new_block_size / 8;
                data_in_pointers[index] = new_array_size;
                for (int j = 0; j < new_array_size; j++){
                    *(pointers[index] + (j * sizeof(int))) = data[j % 4];
                }
            }
        } else{
            hl_release(heap, pointers[index]);
            pointers[index] = 0;
        }
    }
    for (int i = 0; i < NPOINTERS; i++){
        if (pointers[i] > 0 && data_in_pointers[i] > 0){
             for (int j = 0; j < data_in_pointers[i]; j++){
                 if (*(pointers[i] + (j * sizeof(int))) != data[j % 4]){
                      return FAILURE;
                 }
             }
        }
    }


    return SUCCESS;
}
