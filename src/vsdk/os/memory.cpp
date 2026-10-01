// Allocator recovered from Sum.exe vsdk/os/memory.cpp.
// VA 0x5124C0-0x512580. Not byte-matched.

#if defined(_WIN64) || defined(__x86_64__) || defined(__amd64__)
#error Sum.exe is a 32-bit image
#endif

#include "vfs.h"

#include <cstdio>

extern void* vfs_block_alloc(void* allocator, int size);
extern void* crt_malloc(unsigned int size);
extern void crt_free(void* block);
extern void vfs_error(const char* file, int line, const char* message);

void* g_vfs_allocator;

void* vfs_heap_alloc3(int size, int unused_a, int unused_b) {
    (void)unused_a;
    (void)unused_b;
    if (g_vfs_allocator) {
        return vfs_block_alloc(g_vfs_allocator, size);
    }
    void* block = crt_malloc((unsigned int)size);
    if (block) {
        return block;
    }
    char message[0x80];
    for (;;) {
        sprintf(message, "Failed to allocate %d bytes\n", size);
        vfs_error("D:\\projects\\Summoner\\pccode\\vsdk\\os\\memory.cpp", 0xDF, message);
    }
}

void* vfs_heap_forward(int size, int unused_a, int unused_b) {
    return vfs_heap_alloc3(size, unused_a, unused_b);
}

void* vfs_heap_alloc(int size) {
    return vfs_heap_alloc3(size, 0, 0);
}

void vfs_heap_free(void* block) {
    crt_free(block);
}
