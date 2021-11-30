#pragma once
#include "file.h"
#include <stdint.h>

typedef struct
{
   uint64_t nfiles; // The number of files in the ramdisk.
} initrd_header_t;

typedef struct
{
   uint8_t magic;     // Magic number, for error checking.
   char name[64];  // Filename.
   uint64_t offset;   // Offset in the initrd that the file starts.
   uint64_t length;   // Length of the file.
} initrd_file_header_t;

// Initialises the initial ramdisk. It gets passed the address of the multiboot module,
// and returns a completed filesystem node.
fs_node_t *initialise_initrd(uint64_t location);