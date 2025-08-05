#pragma once

#include <stdint.h>
#include <sys/types.h>
#include <stddef.h>

typedef struct VFSNode VFSNode;
typedef struct FSDriver FSDriver;

typedef enum
{
    VFS_DIR,
    VFS_FILE
} VFSType;

struct VFSNode
{
    char *name;
    VFSType type;
    VFSNode *parent;
    VFSNode **children;
    size_t child_count;
    size_t child_capacity;

    FSDriver *driver;
    void *fs_data;
};

struct FSDriver
{
    const char *name;

    VFSNode *(*mount)(void *fs_spec_data);
    ssize_t  (*read)(VFSNode *node, void *buffer, size_t size, size_t offset);
    ssize_t  (*write)(VFSNode *node, const void *buffer, size_t size, size_t offset);
    int (*readdir)(VFSNode *dir, VFSNode **out_entries, size_t max_entries);
    VFSNode *(*lookup)(VFSNode *dir, const char *name);
    VFSNode *(*create)(VFSNode *dir, const char *name, VFSType type);
    VFSNode *(*remove)(VFSNode *dir, const char *name, VFSType type);
    void (*unmount)(VFSNode *node);
};

typedef struct
{
    uint8_t *data;
    size_t size;
    size_t capacity;
} FileData;

int vfs_init();

VFSNode *vfs_lookup(const char *path);

ssize_t vfs_read(VFSNode *node, void *buffer, size_t size, size_t offset);
ssize_t vfs_write(VFSNode *node, const void *buffer, size_t size, size_t offset);

int vfs_create(const char *path, VFSType type);
int vfs_remove(const char *path);
int vfs_cleanup();

int vfs_mount(const char *path, FSDriver *driver, void *spec_data);

VFSNode *vfs_create_node(const char *name, VFSType type, VFSNode *parent);
void vfs_remove_node(VFSNode *node);
void vfs_cleanup_node(VFSNode *node);

int vfs_add_child(VFSNode *parent, VFSNode *child);