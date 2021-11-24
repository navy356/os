#include "kheap.h"
#include "utility.h"
#include "paging.h"
#include "print.h"
#include "string.h"
#include "io.h"

Chunk *free_chunk[NUM_SIZES] = {NULL};
size_t mem_free = 0;
size_t mem_used = 0;
size_t mem_meta = 0;
Chunk *first = NULL;
Chunk *last = NULL;

void init_kheap()
{
  placement_address = kernel_end_virtual + 1;
  init_memory((void *)placement_address, 0x1000000);
}

static void memory_chunk_init(Chunk *chunk)
{
  DLIST_INIT(chunk, all);
  chunk->used = 0;
  DLIST_INIT(chunk, free);
}

static size_t memory_chunk_size(const Chunk *chunk)
{
  char *end = (char *)(chunk->all.next);
  char *start = (char *)(&chunk->all);
  return (end - start) - HEADER_SIZE;
}

static int memory_chunk_slot(size_t size)
{
  int n = -1;
  while (size > 0)
  {
    ++n;
    size /= 2;
  }
  return n;
}

void init_memory(void *mem, size_t size)
{
  char *mem_start = (char *)((((intptr_t)mem) & PAGE_ALIGN) + (~PAGE_ALIGN + 1));
  char *mem_end = (char *)((((intptr_t)mem + size) & PAGE_ALIGN) + (~PAGE_ALIGN + 1));

  first = (Chunk *)mem_start;
  Chunk *second = first + 1;
  last = ((Chunk *)mem_end) - 1;
  memory_chunk_init(first);
  memory_chunk_init(second);
  memory_chunk_init(last);

  dlist_insert_after(&second->all, &last->all);
  dlist_insert_after(&first->all, &second->all);
  first->used = 1;
  last->used = 1;

  size_t len = memory_chunk_size(second);
  int n = memory_chunk_slot(len);
  DLIST_PUSH(&free_chunk[n], second, free);
  mem_free = len - HEADER_SIZE;
  mem_meta = sizeof(Chunk) * 2 + HEADER_SIZE;
}

void *malloc(size_t size, int align)
{
  size = (size + ALIGN - 1) & (~(ALIGN - 1));
  if (size < MIN_SIZE)
    size = MIN_SIZE;

  int n = memory_chunk_slot(size - 1) + 1;

  if (n >= NUM_SIZES)
    return NULL;

  Chunk *chunk;
  size_t size2;
  size_t len;
  if (!align)
  {
    while (!free_chunk[n])
    {
      ++n;
      if (n >= NUM_SIZES)
        return NULL;
    }

    chunk = DLIST_POP(&free_chunk[n], free);
    size2 = memory_chunk_size(chunk);
    len = 0;

    if (size + sizeof(Chunk) <= size2)
    {
      Chunk *chunk2 = (Chunk *)(((char *)chunk) + HEADER_SIZE + size);
      memory_chunk_init(chunk2);
      dlist_insert_after(&chunk->all, &chunk2->all);
      len = memory_chunk_size(chunk2);
      int n = memory_chunk_slot(len);
      DLIST_PUSH(&free_chunk[n], chunk2, free);
      mem_meta += HEADER_SIZE;
      mem_free += len - HEADER_SIZE;
    }
  }
  else
  {
    uint8_t flag = 0;
    uint64_t offset;
    while (!flag)
    {
      while (!free_chunk[n])
      {
        ++n;
        if (n >= NUM_SIZES)
          return NULL;
      }

      chunk = DLIST_POP(&free_chunk[n], free);
      size2 = memory_chunk_size(chunk);
      uint64_t addr = ((uint64_t)chunk + HEADER_SIZE);
      offset = page_align_offset(addr);
      if (offset == 0 || (((offset + size) <= size2) && (offset >= sizeof(Chunk))))
      {
        flag = 1;
      }
      else
      {
        DLIST_PUSH(&free_chunk[n], chunk, free);
      }
    }

    if (offset != 0)
    {
      DLIST_PUSH(&free_chunk[n], chunk, free);
      Chunk *chunk2 = (Chunk *)(((char *)chunk) + offset);
      memory_chunk_init(chunk2);
      dlist_insert_after(&chunk->all, &chunk2->all);
      len = memory_chunk_size(chunk2);
      int n = memory_chunk_slot(len);
      DLIST_PUSH(&free_chunk[n], chunk2, free);
      mem_meta += HEADER_SIZE;
      mem_free += len - HEADER_SIZE;
      chunk = DLIST_POP(&free_chunk[n], free);
      size2 = memory_chunk_size(chunk2);
      if ((size + sizeof(Chunk)) <= (size2 - offset))
      {
        Chunk *chunk3 = (Chunk *)(((char *)chunk2) + HEADER_SIZE + size);
        memory_chunk_init(chunk3);
        dlist_insert_after(&chunk2->all, &chunk3->all);
        len = memory_chunk_size(chunk3);
        int n = memory_chunk_slot(len);
        DLIST_PUSH(&free_chunk[n], chunk3, free);
        mem_meta += HEADER_SIZE;
        mem_free += len - HEADER_SIZE;
      }
    }
    else if (offset == 0 && (size + sizeof(Chunk) <= size2))
    {
      Chunk *chunk2 = (Chunk *)(((char *)chunk) + HEADER_SIZE + size);
      memory_chunk_init(chunk2);
      dlist_insert_after(&chunk->all, &chunk2->all);
      len = memory_chunk_size(chunk2);
      int n = memory_chunk_slot(len);
      DLIST_PUSH(&free_chunk[n], chunk2, free);
      mem_meta += HEADER_SIZE;
      mem_free += len - HEADER_SIZE;
    }
  }

  chunk->used = 1;
  mem_free -= size2;
  mem_used += size2 - len - HEADER_SIZE;
  return chunk->data;
}

uint64_t page_align_offset(uint64_t addr)
{
  if ((addr & 0xFFF) == 0)
  {
    return 0;
  }
  uint64_t aligned = addr & PAGE_ALIGN;
  aligned += 0x1000;
  return (aligned - addr);
}

void remove_free(Chunk *chunk)
{
  size_t len = memory_chunk_size(chunk);
  int n = memory_chunk_slot(len);
  DLIST_REMOVE_FROM(&free_chunk[n], chunk, free);
  mem_free -= len - HEADER_SIZE;
}

void push_free(Chunk *chunk)
{
  size_t len = memory_chunk_size(chunk);
  int n = memory_chunk_slot(len);
  DLIST_PUSH(&free_chunk[n], chunk, free);
  mem_free += len - HEADER_SIZE;
}

void free(void *mem)
{
  Chunk *chunk = (Chunk *)((char *)mem - HEADER_SIZE);
  Chunk *next = CONTAINER(Chunk, all, chunk->all.next);
  Chunk *prev = CONTAINER(Chunk, all, chunk->all.prev);

  mem_used -= memory_chunk_size(chunk);

  if (next->used == 0)
  {
    remove_free(next);
    dlist_remove(&next->all);
    memset(next, 0xDD, sizeof(Chunk));
    mem_meta -= HEADER_SIZE;
    mem_free += HEADER_SIZE;
  }
  if (prev->used == 0)
  {
    remove_free(prev);
    dlist_remove(&chunk->all);
    memset(chunk, 0xDD, sizeof(Chunk));
    push_free(prev);
    mem_meta -= HEADER_SIZE;
    mem_free += HEADER_SIZE;
  }
  else
  {
    chunk->used = 0;
    DLIST_INIT(chunk, free);
    push_free(chunk);
  }
}

uint64_t kmalloc(uint64_t sz)
{
  uint64_t tmp = malloc(sz, 0);
  write(hexToString(tmp));
  write("\n");
  write(hexToString(getPhysicalKernelOffset((uint64_t)tmp)));
  write("\n");
  uint64_t v_addr = mapPage(getPhysicalKernelOffset(tmp), sz);
  return v_addr;
}

uint64_t kmalloc_a(uint64_t sz, int align)
{
  uint64_t tmp = malloc(sz, 1);
  uint64_t v_addr = mapPage(getPhysicalKernelOffset(tmp), sz);
  return v_addr;
}

uint64_t kmalloc_ap(uint64_t sz, int align, uint64_t *phys)
{
  uint64_t tmp = malloc(sz, align);
  if (phys)
  {
    *phys = getPhysicalKernelOffset(tmp);
  }
  return tmp;
}
