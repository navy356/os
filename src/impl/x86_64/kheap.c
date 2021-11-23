#include "kheap.h"
#include "utility.h"

void init_kheap()
{
  placement_address = kernel_end_virtual+1;
}
uint64_t kmalloc(uint64_t sz)
{
  uint64_t tmp = placement_address;
  placement_address += sz;
  return tmp;
}

uint64_t kmalloc_a(uint64_t sz, int align)
{
  if (align == 1 && (placement_address & 0xFFFFF000)) // If the address is not already page-aligned
  {
    // Align it.
    placement_address &= 0xFFFFF000;
    placement_address += 0x1000;
  }
  uint64_t tmp = placement_address;
  placement_address += sz;
  return tmp;
}

uint64_t kmalloc_ap(uint64_t sz, int align, uint64_t *phys)
{
  if (align == 1 && (placement_address & 0xFFFFF000)) // If the address is not already page-aligned
  {
    // Align it.
    placement_address &= 0xFFFFF000;
    placement_address += 0x1000;
  }
  if (phys)
  {
    *phys = getPhysicalKernelOffset(placement_address);
  }
  uint32_t tmp = placement_address;
  placement_address += sz;
  return tmp;
}
