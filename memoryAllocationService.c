#include "memoryAllocationService.h"


MemoryAllocator *MemoryAllocator_init(void *memoryPool, size_t size) {
    MemoryAllocator *memoryAllocator = (MemoryAllocator *) malloc(sizeof(MemoryAllocator));
    unsigned char *allocateMemoryPool = (unsigned char *) memoryPool;
    allocateMemoryPool[0] = FREE;
    allocateMemoryPool[1] = (unsigned char) (size - 2);
    for (int i = 2; i < size; i++) {
        allocateMemoryPool[i] = '\0';
    }
    memoryAllocator->size = size;
    memoryAllocator->memoryPool = allocateMemoryPool;
    return memoryAllocator;
}

void *MemoryAllocator_release(MemoryAllocator *allocator) {
    unsigned char *memoryPool = (unsigned char *) allocator->memoryPool;
    memoryPool[0] = FREE;
    memoryPool[1] = allocator->size - 2;
    for (int i = 2; i < allocator->size; i++) {
        memoryPool[i] = '\0';
    }
    return (void *) memoryPool;
}

void *MemoryAllocator_allocate(MemoryAllocator *allocator, size_t size) {
    unsigned char *memoryPool = (unsigned char *) allocator->memoryPool;
    int sizeOfNextBlock = memoryPool[1], currentIndex = 0;
    unsigned char currentState = memoryPool[currentIndex];
    while ((currentState == USED || sizeOfNextBlock < size + 2) && currentIndex < allocator->size) {
        currentIndex = currentIndex + sizeOfNextBlock + 2;
        sizeOfNextBlock = memoryPool[currentIndex + 1];
        currentState = memoryPool[currentIndex];
    }
    size_t prevBlockSize = memoryPool[currentIndex + 1];
    memoryPool[currentIndex] = USED;
    memoryPool[currentIndex + 1] = (unsigned char) size;
    memoryPool[currentIndex + size + 2] = FREE;
    memoryPool[currentIndex + size + 3] = (unsigned char) (prevBlockSize - size - 2);
    return (void *) &memoryPool[currentIndex];
}

void *MemoryAllocator_free(MemoryAllocator *allocator, void *ptr) {
    char *ptrCast = (char *) ptr;
    if (*ptrCast == 'u') {
        *ptrCast = 'f';
    }
    int size = (int) *(ptrCast + 1);
    for (int i = 0; i < size; i++)
        *(ptrCast + i + 2) = '\0';
    return NULL;
}

size_t MemoryAllocator_optimize(MemoryAllocator *allocator) {
    unsigned char *memoryPool = (unsigned char *) allocator->memoryPool;
    int index = 0, sumOfFree = 0, firstFree = 0, maxBlock = 0;
    while (index <= allocator->size) {
        if (memoryPool[index] == USED || index == allocator->size) {
            if (sumOfFree) {
                *(memoryPool + 1 + firstFree) = sumOfFree - 2;
                for (int i = 2; i < sumOfFree; i++)
                    *(memoryPool + i + firstFree) = '\0';
            }
            if (maxBlock < sumOfFree) {
                maxBlock = sumOfFree;
            }
            if (index != allocator->size) {
                firstFree = memoryPool[index + 1] + index + 2;
                sumOfFree = 0;
            }
        } else {
            if (memoryPool[index] == FREE) {
                sumOfFree += memoryPool[index + 1] + 2;
            }
        }
        index += memoryPool[index + 1] + 2;
    }
    return maxBlock-2;
}

