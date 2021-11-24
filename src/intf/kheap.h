#pragma once
#include <stdint.h>
#include "constants.h"
#include "list.h"

typedef struct Chunk Chunk;
struct Chunk
{
    DList all;
    uint64_t used;
    union
    {
        char* data;
        DList free;
    };
};

#define NUM_SIZES 32
#define ALIGN 4
#define PAGE_ALIGN 0xFFFFFFFFFFFFF000
#define MIN_SIZE sizeof(DList)
#define HEADER_SIZE OFFSETOF(Chunk, data)
#define SIZE 0x2000

void free(void *mem);
void *malloc(size_t size,int align);
void init_memory(void *mem, size_t size);
uint64_t placement_address;

uint64_t kmalloc(uint64_t sz);
uint64_t kmalloc_a(uint64_t sz, int align);
uint64_t kmalloc_ap(uint64_t sz, int align, uint64_t *phys);
void init_kheap();
uint64_t page_align_offset(uint64_t addr);
void kfree(void * addr);