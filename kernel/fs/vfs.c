#include "fs/include/vfs.h"
#include "klibc/include/kstdlib.h"
#include "klibc/include/kstring.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>

static VFSNode *vfs_root = NULL;

VFSNode *vfs_create_node(const char *name, int type, VFSNode *parent)
{
    if (!name)
        return NULL;

    VFSNode *node = kmalloc(sizeof(VFSNode));
    if (!node)
        return NULL;

    size_t name_length = kstrlen(name) + 1;
    node->name = kmalloc(name_length);
    if (!node->name)
    {
        kfree(node);
        return NULL;
    }

    kmemmove(node->name, name, name_length);

    node->type = type;
    node->parent = parent;
    node->driver = NULL;
    node->fs_data = NULL;
    node->children = NULL;
    node->child_count = 0;
    node->child_capacity = 0;

    return node;
}

int vfs_add_child(VFSNode *parent, VFSNode *child)
{
    if (!parent || !child)
        return -1;

    if (parent->type != VFS_DIR)
        return -2;

    if (parent->child_capacity == 0)
    {
        parent->child_capacity = 4;
        parent->children = kmalloc(sizeof(VFSNode *) * parent->child_capacity);
        if (!parent->children)
            return -3;
    }
    else if (parent->child_count >= parent->child_capacity)
    {
        size_t new_child_capacity = parent->child_capacity * 2;
        VFSNode **new_children = kmalloc(sizeof(VFSNode *) * new_child_capacity);
        if (!new_children)
            return -4;

        kmemmove(new_children, parent->children, sizeof(VFSNode *) * parent->child_count);

        kfree(parent->children);

        parent->children = new_children;
        parent->child_capacity = new_child_capacity;
    }

    parent->children[parent->child_count++] = child;
    child->parent = parent;

    return 0;
}

void vfs_init()
{
    vfs_root = vfs_create_node("/", VFS_DIR, NULL);

    vfs_mkdir("/bin");
    vfs_mkdir("/root");
    vfs_mkdir("/usr");
    vfs_mkdir("/usr/bin");

    vfs_newfile("README.txt");
    VFSNode *readme = vfs_lookup("/README.txt");
    vfs_write(readme, "Welcome on RoughOS!", 20, 0);
}

VFSNode *vfs_lookup(const char *name)
{
    if (!name || name[0] != '/' || !vfs_root)
        return NULL;

    if (kstrcmp(name, "/") == 0)
        return vfs_root;
    
    VFSNode *current = vfs_root;

    char *name_copy = kmalloc(kstrlen(name) + 1);
    if (!name_copy)
        return NULL;

    kstrncpy(name_copy, name, kstrlen(name) + 1);

    char *token = kstrtok(name_copy + 1, "/");
    if (!token) {
        kfree(name_copy);
        return NULL;
    }

    while (token && current)
    {
        if (current->type != VFS_DIR) {
            kfree(name_copy);
            return NULL;
        }

        VFSNode *next = NULL;
        for (size_t i = 0; i < current->child_count; i++) {
            if (kstrcmp(current->children[i]->name, token) == 0) {
                next = current->children[i];
                break;
            }
        }

        if (!next) {
            kfree(name_copy);
            return NULL;
        }

        current = next;
        token = kstrtok(NULL, "/");
    }

    kfree(name_copy);
    return current;
}

void vfs_mkdir(const char *name)
{
    if (!name || name[0] != '/')
        return;

    char *name_copy = kmalloc(kstrlen(name) + 1);
    if (!name_copy)
        return;

    kstrncpy(name_copy, name, kstrlen(name) + 1);

    VFSNode *current = vfs_root;
    char *token = kstrtok(name_copy + 1, "/");

    while (token)
    {
        VFSNode *next = NULL;

        for (size_t i = 0; i < current->child_count; ++i)
            if (kstrcmp(current->children[i]->name, token) == 0)
            {
                next = current->children[i];
                break;
            }

        if (!next)
        {
            next = vfs_create_node(token, VFS_DIR, NULL);
            if (vfs_add_child(current, next) != 0)
            {
                kfree(name_copy);
                return;
            }
        }

        current = next;
        token = kstrtok(NULL, "/");
    }

    kfree(name_copy);
}

char **vfs_listdir(const char *name, size_t *output_size)
{
    if (output_size)
        *output_size = 0;

    VFSNode *node = vfs_lookup(name);
    if (!node || node->type != VFS_DIR)
        return NULL;
    if (node->child_count == 0)
        return NULL;

    char **result = kmalloc(sizeof(char *) * node->child_count);
    if (!result)
        return NULL;

    for (size_t i = 0; i < node->child_count; i++)
    {
        size_t length = kstrlen(node->children[i]->name) + 1;
        result[i] = kmalloc(length);

        if (!result[i])
        {
            for (size_t j = 0; j < i; j++)
                kfree(result[j]);

            kfree(result);
            
            return NULL;
        }

        kstrncpy(result[i], node->children[i]->name, length);
    }

    if (output_size)
        *output_size = node->child_count;

    return result;
}

VFSNode *vfs_newfile(const char *name)
{
    if (!name || name[0] != '/')
        return NULL;

    char *name_copy = kmalloc(kstrlen(name) + 1);
    if (!name_copy)
        return NULL;

    kstrncpy(name_copy, name, kstrlen(name) + 1);

    VFSNode *current = vfs_root;
    char *token = kstrtok(name_copy + 1, "/");
    VFSNode *last_created = NULL;

    while (token)
    {
        VFSNode *next = NULL;

        for (size_t i = 0; i < current->child_count; i++)
            if (kstrcmp(current->children[i]->name, token) == 0)
            {
                next = current->children[i];
                break;
            }

        if (!next)
        {
            char *next_tok = kstrtok(NULL, "/");
            kstrtok(NULL, "");
            kstrtok(name_copy + 1, "/");

            if (next_tok)
                next = vfs_create_node(token, VFS_DIR, NULL);
            else
                next = vfs_create_node(token, VFS_FILE, NULL);

            if (!next || vfs_add_child(current, next) != 0)
            {
                kfree(name_copy);
                return NULL;
            }

            last_created = next;
        }
        else
            if (token && token[0] && next->type != VFS_DIR && kstrtok(NULL, "/") != NULL)
            {
                kfree(name_copy);
                return NULL;
            }

        current = next;
        token = kstrtok(NULL, "/");
    }

    kfree(name_copy);

    return current;
}

ssize_t vfs_read(VFSNode *file, void *buffer, size_t size, size_t offset)
{
    if (!file || file->type != VFS_FILE || !buffer)
        return -1;

    FileData *data = (FileData *)file->fs_data;
    if (!data || offset >= data->size)
        return -2;

    size_t bytes = size;

    if (offset + size > data->size)
        bytes = data->size - offset;

    kmemmove(buffer, data->data + offset, bytes);

    return bytes;
}

ssize_t vfs_write(VFSNode *file, const void *buffer, size_t size, size_t offset)
{
    if (!file || file->type != VFS_FILE || !buffer)
        return -1;

    FileData *data = (FileData *)file->fs_data;
    if (!data)
    {
        data = kmalloc(sizeof(FileData));
        if (!data)
            return -2;

        data->data = NULL;
        data->size = 0;
        data->capacity = 0;

        file->fs_data = data;
    }

    size_t needed_size = offset + size;

    if (data->capacity < needed_size)
    {
        size_t new_capacity = data->capacity ? data->capacity * 2 : 128;

        while (new_capacity < needed_size)
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