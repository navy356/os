#pragma once
#include <stdint.h>

#define FS_FILE        0x01
#define FS_DIRECTORY   0x02
#define FS_CHARDEVICE  0x03
#define FS_BLOCKDEVICE 0x04
#define FS_PIPE        0x05
#define FS_SYMLINK     0x06
#define FS_MOUNTPOINT  0x08

typedef uint64_t (*read_type_t)(struct fs_node*,uint64_t,uint64_t,uint64_t*);
typedef uint64_t (*write_type_t)(struct fs_node*,uint64_t,uint64_t,uint64_t*);
typedef void (*open_type_t)(struct fs_node*, uint8_t read, uint8_t write);
typedef void (*close_type_t)(struct fs_node*);
typedef struct dirent * (*readdir_type_t)(struct fs_node*,uint64_t);
typedef struct fs_node * (*finddir_type_t)(struct fs_node*,char *name);

typedef struct fs_node
{
   char name[128];     // The filename.
   uint64_t mask;        // The permissions mask.
   uint64_t uid;         // The owning user.
   uint64_t gid;         // The owning group.
   uint64_t flags;       // Includes the node type. See #defines above.
   uint64_t inode;       // This is device-specific - provides a way for a filesystem to identify files.
   uint64_t length;      // Size of the file, in bytes.
   uint64_t impl;        // An implementation-defined number.
   read_type_t read;
   write_type_t write;
   open_type_t open;
   close_type_t close;
   readdir_type_t readdir;
   finddir_type_t finddir;
   struct fs_node *ptr; // Used by mountpoints and symlinks.
} fs_node_t;

struct dirent // One of these is returned by the readdir call, according to POSIX.
{
  char name[128]; // Filename.
  uint64_t ino;     // Inode number. Required by POSIX.
};

fs_node_t *fs_root;

uint64_t read_fs(fs_node_t *node, uint64_t offset, uint64_t size, uint64_t *buffer);
uint64_t write_fs(fs_node_t *node, uint64_t offset, uint64_t size, uint64_t *buffer);
void open_fs(fs_node_t *node, uint8_t read, uint8_t write);
void close_fs(fs_node_t *node);
struct dirent *readdir_fs(fs_node_t *node, uint64_t index);
fs_node_t *finddir_fs(fs_node_t *node, char *name);