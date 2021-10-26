# Memory Allocation Service

## GOAL ##
The goal of this project is to create memory allocator in C, which is using a big chunk of memory pool which is set at initialization.

## DESCRIPTION ##
1. Each block keeps its size and indication whether it is free or occupied in a compressed manner (LSB).
2. When allocation is requested, the algorithm should find the first free block and do the following:
- Check if it is big enough. If it is, and it is the exact same size, mark it as allocated and return it. Otherwise, split it to the requested size + the rest, and mark the first block as allocated and the second as free.
- If it is not big enough, check if the next block is free. If it is, merge them and check the size again.
- If it is not big enough, look for the next free block.
- If the algorithm reached the end of the memory pool, it return NULL.

## FLOW ##
The MemoryAllocator is a struct that save size, and a pointer to the memory pool.

### functions

#### MemoryAllocator_init
Get the pointer to memoryPool and the size to initialize. returns the initialized memoryAllocator.
define a new MemoryAllocator: its size is given, and its memoryPool is defined as follows: memoryPool[0] is FREE, memoryPool[1] is size-2, and the rest of the memoryPool is NULL.

#### MemoryAllocator_release
Realese given MemoryAllocator, and return it.

#### MemoryAllocator_allocate
Get the pointer to MemoryAllocator and the size to allocate. returns the new memoryAllocator.

#### MemoryAllocator_free
Merge the next adjacent block is free

MemoryAllocator_optimize
Merges all adjacent free blocks, and returns the size of largest free block
