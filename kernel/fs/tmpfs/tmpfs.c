#include "fs/tmpfs/tmpfs.h"
#include "fs/include/vfs.h"
#include "klibc/include/kstring.h"
#include "klibc/include/kstdlib.h"

// spec_data unused but required
VFSNode *tmpfs_mount(void *spec_data)
{
    VFSNode *root = vfs_create_node("/", VFS_DIR, NULL);
    return root;
}

void tmpfs_unmount(VFSNode *node)
{
    if (!node)
        return;

    if (node->type == VFS_DIR)
    {
        for (size_t i = 0; i < node->child_count; i++)
            tmpfs_unmount(node->children[i]);

        if (node->children)
            kfree(node->children);
    }

    if (node->fs_data)
    {
        FileData *data = (FileData *)node->fs_data;
        if (data->data)
            kfree(data->data);
        kfree(data);
        node->fs_data = NULL;
    }

    kfree(node->name);
    kfree(node);
}

VFSNode *tmpfs_lookup(VFSNode *dir, const char *name)
{
    if (!dir || dir->type != VFS_DIR || !name)
        return NULL;

    for (size_t i = 0; i < dir->child_count; i++)
        if (kstrcmp(dir->children[i]->name, name) == 0)
            return dir->children[i];

    return NULL;
}

int tmpfs_readdir(VFSNode *dir, VFSNode **out_entries, size_t max_entries)
{
    if (!dir || dir->type != VFS_DIR || !out_entries)
        return -1;

    size_t count = dir->child_count < max_entries ? dir->child_count : max_entries;

    for (size_t i = 0; i < count; i++)
        out_entries[i] = dir->children[i];

    return (int)count;
}

VFSNode *tmpfs_create(VFSNode *dir, const char *name, VFSType type)
{
    if (!dir || dir->type != VFS_DIR || !name)
        return NULL;

    VFSNode *node = vfs_create_node(name, type, dir);
    if (!node)
        return NULL;
    node->driver = &tmpfs_driver;

    if (vfs_add_child(dir, node) != 0)
        return NULL;

    if (type == VFS_FILE)
    {
        FileData *data = kmalloc(sizeof(FileData));
        if (!data)
            return NULL;

        data->data = NULL;
        data->size = 0;
        data->capacity = 0;
        node->fs_data = data;
    }

    return node;
}

ssize_t tmpfs_read(VFSNode *node, void *buffer, size_t size, size_t offset)
{
    if (!node || node->type != VFS_FILE || !buffer)
        return -1;

    FileData *data = node->fs_data;
    if (!data || offset >= data->size)
        return 0;

    size_t bytes = size;
    if (offset + size > data->size)
        bytes = data->size - offset;

    kmemmove(buffer, data->data + offset, bytes);
    return bytes;
}

ssize_t tmpfs_write(VFSNode *node, const void *buffer, size_t size, size_t offset)
{
    if (!node || node->type != VFS_FILE || !buffer)
        return -1;

    FileData *data = node->fs_data;
    if (!data)
    {
        data = kmalloc(sizeof(FileData));
        if (!data)
            return -2;

        data->data = NULL;
        data->size = 0;
        data->capacity = 0;
        node->fs_data = data;
    }

    size_t needed = offset + size;

    if (needed > data->capacity)
    {
        size_t new_capacity = data->capacity ? data->capacity * 2 : 128;
        while (new_capacity < needed)
            new_capacity *= 2;

        uint8_t *new_data = kmalloc(new_capacity);
        if (!new_data)
            return -3;

        if (data->data)
        {
            kmemmove(new_data, data->data, data->size);
            kfree(data->data);
        }

        data->data = new_data;
        data->capacity = new_capacity;
    }

    kmemmove(data->data + offset, buffer, size);

    if (offset + size > data->size)
        data->size = offset + size;

    return size;
}

FSDriver tmpfs_driver = {
    .name = "tmpfs",
    .mount = tmpfs_mount,
    .read = tmpfs_read,
    .write = tmpfs_write,
    .readdir = tmpfs_readdir,
    .lookup = tmpfs_lookup,
    .create = tmpfs_create
};