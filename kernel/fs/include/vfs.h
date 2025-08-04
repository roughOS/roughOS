#pragma once

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

typedef struct VFSNode
{
    char *name;
    enum
    {
        VFS_DIR,
        VFS_FILE
    } type;
    struct VFSNode *parent;
    struct VFSNode **children;
    size_t child_count;
    size_t child_capacity;
    struct FSDriver *driver;
    void *fs_data;
} VFSNode;

typedef struct FSDriver
{
    const char *name;
    VFSNode* (*mount)(void * fs_spec_data);
    ssize_t (*read)(VFSNode *node, void *buffer, size_t size, size_t offset);
    ssize_t (*write)(VFSNode *node, const void *buffer, size_t size, size_t offset);
    int (*readdir)(VFSNode *dir, VFSNode **out_entries, size_t max_entries);
    VFSNode *(*lookup)(VFSNode *dir, const char *name);
} FSDriver;

typedef struct
{
    uint8_t *data;
    size_t size;
    size_t capacity;
} FileData;

VFSNode *vfs_create_node(const char *name, int type, VFSNode *parent);
int vfs_add_child(VFSNode *parent, VFSNode *child);
void vfs_init();
VFSNode *vfs_lookup(const char *name);
void vfs_mkdir(const char *name);
char **vfs_listdir(const char *name, size_t *output_size);
VFSNode *vfs_newfile(const char *name);
ssize_t vfs_read(VFSNode *file, void *buffer, size_t size, size_t offset);
ssize_t vfs_write(VFSNode *file, const void *buffer, size_t size, size_t offset);