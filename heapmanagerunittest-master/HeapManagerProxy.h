#ifndef __HEAP_MANAGER_PROXY_H
#define __HEAP_MANAGER_PROXY_H

#include <stdint.h>



namespace HeapManagerProxy
{
	struct Descriptor {
		void* offset;
		unsigned int size;
	};

	class HeapManager {
	public:
		void* startPointer;
		void * endPointer;
		size_t memorySize = 0;
		size_t _memoryAlloc = 0;
		size_t memoryUsed = 0;
		unsigned int nDescriptors = 0;
		unsigned int nUsedDesc = 0;
		unsigned int nFreeDesc = 0;
		Descriptor * usedDesc;
		Descriptor * freeDesc;
	};

HeapManager *	CreateHeapManager( void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors );
void			Destroy( HeapManager * i_pManager );

void *			alloc( HeapManager * i_pManager, size_t i_size );
void *			alloc( HeapManager * i_pManager, size_t i_size, unsigned int i_alignment );
bool			free( HeapManager * i_pManager, void * i_ptr );

void			Collect( HeapManager * i_pManager );

bool			Contains(const HeapManager * i_pManager, void * i_ptr );
bool			IsAllocated( const HeapManager * i_pManager, void * i_ptr );

size_t			GetLargestFreeBlock( const HeapManager * i_pManager );
size_t			GetTotalFreeMemory( const HeapManager * i_pManager );

void			ShowFreeBlocks( const HeapManager * i_pManager );
void			ShowOutstandingAllocations( const HeapManager * i_pManager );

} // namespace HeapManagerProxy

#endif // __HEAP_MANAGER_PROXY_H