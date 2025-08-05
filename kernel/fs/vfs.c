#include "fs/include/vfs.h"
#include "klibc/include/kstdlib.h"
#include "klibc/include/kstring.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

VFSNode *vfs_root = NULL;

int vfs_init()
{
    vfs_root = vfs_create_node("/", VFS_DIR, NULL);
    if (!vfs_root)
        return -1;

    vfs_create("/bin", VFS_DIR);
    vfs_create("/usr", VFS_DIR);
    vfs_create("/usr/bin", VFS_DIR);
    vfs_create("/rott", VFS_DIR);

    vfs_create("/README.txt", VFS_FILE);

    return 0;
}

VFSNode *vfs_create_node(const char *name, VFSType type, VFSNode *parent)
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

void vfs_remove_node(VFSNode *node)
{
    if (!node)
        return;

    if (node->driver && node->driver->unmount)
        node->driver->unmount(node);

    if (node->type == VFS_DIR)
    {
        for (size_t i = 0; i < node->child_count; i++)
            vfs_remove_node(node->children[i]);

        if (node->children)
            kfree(node->children);
    }

    if (node->fs_data)
        kfree(node->fs_data);

    kfree(node->name);
    kfree(node);
}

void vfs_cleanup_node(VFSNode *node)
{
    if (!node)
        return;

    if (node->type == VFS_DIR)
    {
        for (size_t i = 0; i < node->child_count; i++)
            vfs_cleanup_node(node->children[i]);

        if (node->children)
            kfree(node->children);
    }

    if (node->fs_data)
        kfree(node->fs_data);

    kfree(node->name);
    kfree(node);
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
        size_t new_capacity = parent->child_capacity * 2;
        VFSNode **new_children = kmalloc(sizeof(VFSNode *) * new_capacity);
        if (!new_children)
            return -4;

        kmemmove(new_children, parent->children, sizeof(VFSNode *) * parent->child_count);
        kfree(parent->children);
        parent->children = new_children;
        parent->child_capacity = new_capacity;
    }

    parent->children[parent->child_count++] = child;
    child->parent = parent;

    return 0;
}

int vfs_mount(const char *path, FSDriver *driver, void *spec_data)
{
    if (!path || !driver)
        return -1;

    VFSNode *mount_point = vfs_lookup(path);
    if (!mount_point)
        return -2;

    if (mount_point->type != VFS_DIR)
        return -3;

    VFSNode *fs_root = driver->mount(spec_data);
    if (!fs_root)
        return -4;

    mount_point->driver = driver;
    mount_point->fs_data = fs_root->fs_data;

    if (mount_point->children)
        kfree(mount_point->children);

    mount_point->children = fs_root->children;
    mount_point->child_count = fs_root->child_count;
    mount_point->child_capacity = fs_root->child_capacity;

    kfree(fs_root->name);
    kfree(fs_root);

    return 0;
}

VFSNode *vfs_lookup(const char *path)
{
    if (!path || path[0] != '/' || !vfs_root)
        return NULL;

    if (kstrcmp(path, "/") == 0)
        return vfs_root;

    VFSNode *current = vfs_root;

    char *path_copy = kmalloc(kstrlen(path) + 1);
    if (!path_copy)
        return NULL;

    kstrncpy(path_copy, path, kstrlen(path) + 1);

    char *token = kstrtok(path_copy + 1, "/");
    while (token && current)
    {
        if (current->driver && current->driver->lookup)
            current = current->driver->lookup(current, token);
        else
        {
            VFSNode *next = NULL;
            if (current->type != VFS_DIR)
            {
                current = NULL;
                break;
            }
            for (size_t i = 0; i < current->child_count; i++)
                if (kstrcmp(current->children[i]->name, token) == 0)
                {
                    next = current->children[i];
                    break;
                }
            current = next;
        }
        token = kstrtok(NULL, "/");
    }

    kfree(path_copy);

    return current;
}

int vfs_create(const char *path, VFSType type)
{
    if (!path || path[0] != '/')
        return -1;

    char *path_copy = kmalloc(kstrlen(path) + 1);
    if (!path_copy)
        return -2;

    kstrncpy(path_copy, path, kstrlen(path) + 1);

    VFSNode *current = vfs_root;
    char *token = kstrtok(path_copy + 1, "/");
    char *next_token = NULL;

    while (token)
    {
        next_token = kstrtok(NULL, "/");
        VFSNode *child = NULL;

        if (current->driver && current->driver->lookup)
            child = current->driver->lookup(current, token);
        else
        {
            for (size_t i = 0; i < current->child_count; i++)
                if (kstrcmp(current->children[i]->name, token) == 0)
                {
                    child = current->children[i];
                    break;
                }
        }

        if (!child)
        {
            if (current->driver && current->driver->create)
                child = current->driver->create(current, token, next_token ? VFS_DIR : type);
            else
            {
                child = vfs_create_node(token, next_token ? VFS_DIR : type, current);
                if (!child)
                {
                    kfree(path_copy);
                    return -3;
                }
                if (vfs_add_child(current, child) != 0)
                {
                    kfree(path_copy);
                    return -4;
                }
            }
        }
        current = child;
        token = next_token;
    }

    kfree(path_copy);

    return 0;
}

int vfs_remove(const char *path)
{
    VFSNode *node = vfs_lookup(path);
    if (!node)
        return -1;

    if (node == vfs_root)
        return -2;

    VFSNode *parent = node->parent;
    if (!parent)
        return -3;

    size_t idx = 0;
    for (; idx < node->child_count; idx++)
        if (parent->children[idx] == node)
            break;

    if (idx == parent->child_count)
        return -4;

    for (size_t i = idx; i < parent->child_count - 1; i++)
        parent->children[i] = parent->children[i + 1];

    parent->child_count--;

    vfs_remove_node(node);

    return 0;
}

int vfs_cleanup()
{
    if (!vfs_root)
        return -1;

    vfs_cleanup_node(vfs_root);
    vfs_root = NULL;

    return 0;
}

ssize_t vfs_read(VFSNode *node, void *buffer, size_t size, size_t offset)
{
    if (!node || node->type != VFS_FILE || !buffer)
        return -1;

    if (node->driver && node->driver->read)
        return node->driver->read(node, buffer, size, offset);

    return -2;
}

ssize_t vfs_write(VFSNode *node, const void *buffer, size_t size, size_t offset)
{
    if (!node || node->type != VFS_FILE || !buffer)
        return -1;

    if (node->driver && node->driver->write)
        return node->driver->write(node, buffer, size, offset);

    return -2;
}