#include "memoryAllocationService.h"

int main() {
    char memoryPool[LENGTH];
    MemoryAllocator *memory;
    memory = MemoryAllocator_init(memoryPool, sizeof(memoryPool));
    MemoryAllocator_allocate(memory, 4);
    MemoryAllocator_allocate(memory, 8);
    MemoryAllocator_allocate(memory, 4);
    MemoryAllocator_allocate(memory, 8);
    MemoryAllocator_allocate(memory, 4);
    MemoryAllocator_allocate(memory, 8);
    MemoryAllocator_allocate(memory, 8);
    MemoryAllocator_free(memory, (memory->memoryPool) + 16);
    MemoryAllocator_free(memory, (memory->memoryPool) + 22);
    MemoryAllocator_free(memory, (memory->memoryPool) + 32);
    MemoryAllocator_free(memory, (memory->memoryPool) + 38);
    size_t maxBlockAfterOptimize = MemoryAllocator_optimize(memory);
    printf("%zu\n", maxBlockAfterOptimize);
    MemoryAllocator_release(memory);

    unsigned char *memoryCast = (unsigned char *) memory->memoryPool;
    for (int i = 0; i < memory->size;) {
        if ((char) memoryCast[i] == FREE || (char) memoryCast[i] == USED) {
            printf("%c", memoryCast[i]);
            printf("%zu", (size_t) memoryCast[i + 1]);
            i += 2;
        } else {
            printf("%c", memoryCast[i]);
            i += 1;
        }
    }
    return 0;
}