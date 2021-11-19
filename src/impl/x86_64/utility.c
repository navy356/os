#include "utility.h"

uint64_t getVirtualKernelOffset(uint64_t address)
{
    return address+KERNEL_OFFSET;
}

uint64_t getPhysicalKernelOffset(uint64_t address)
{
    return address-KERNEL_OFFSET;
}