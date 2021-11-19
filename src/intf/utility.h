#pragma once

#include "constants.h"
#include <stdint.h>

uint64_t getVirtualKernelOffset(uint64_t address);
uint64_t getPhysicalKernelOffset(uint64_t address);