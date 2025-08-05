#pragma once

#include "fs/include/vfs.h"
#include <stddef.h>
#include <sys/types.h>

VFSNode *tmpfs_mount(void *spec_data);
ssize_t tmpfs_read(VFSNode *node, void *buffer, size_t size, size_t offset);
ssize_t tmpfs_write(VFSNode *node, const void *buffer, size_t size, size_t offset);
int tmpfs_readdir(VFSNode *dir, VFSNode **out_entries, size_t max_entries);
VFSNode *tmpfs_lookup(VFSNode *dir, const char *name);
VFSNode *tmpfs_create(VFSNode *dir, const char *name, VFSType type);
void tmpfs_unmount(VFSNode *node);

extern FSDriver tmpfs_driver;