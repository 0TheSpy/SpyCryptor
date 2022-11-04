#ifndef _M_CHUNK_UNIT_ALLOCATOR_H_
#define _M_CHUNK_UNIT_ALLOCATOR_H_

template<class T>
struct MCHUNK_UNIT_ALLOCATOR_CHUNK
{
	LIST_ENTRY	entry;
	DWORD		count;
	T			items[0];
};

template<class T>
class mChunkUnitAllocator
{
	DWORD					need_memory;
	DWORD					units_per_chunk;

	LIST_ENTRY				units_chunks;
	MCHUNK_UNIT_ALLOCATOR_CHUNK<T>*	units_last;
	DWORD					units_count;

	MCHUNK_UNIT_ALLOCATOR_CHUNK<T>*	units_current_chunk;
	DWORD					units_current_index;
public:
	mChunkUnitAllocator(DWORD UnitPerChunk);
	~mChunkUnitAllocator();

	T*		get_new_chunk();
	void	clear_all_chunks();
	DWORD	get_count();
	T*		get_first_chunk();
	T*		get_next_chunk();
};


template< class T>
mChunkUnitAllocator<T>::mChunkUnitAllocator(DWORD UnitPerChunk)
{
	if( UnitPerChunk < 0x400 )
		UnitPerChunk = 0x400;

	need_memory		= ALIGN_UP((UnitPerChunk*sizeof(T)) + sizeof(MCHUNK_UNIT_ALLOCATOR_CHUNK<T>),0x1000); // ekonomim pamatj :)
	units_per_chunk = (need_memory - sizeof(MCHUNK_UNIT_ALLOCATOR_CHUNK<T>))/sizeof(T);

	units_last = NULL;
	units_count = 0;

	InitializeListHead(&units_chunks);
}

template<class T>
mChunkUnitAllocator<T>::~mChunkUnitAllocator()
{
	clear_all_chunks();
}

template<class T>
DWORD mChunkUnitAllocator<T>::get_count()
{
	return units_count;
}

template<class T>
void mChunkUnitAllocator<T>::clear_all_chunks()
{
	for(MCHUNK_UNIT_ALLOCATOR_CHUNK<T>* current = (MCHUNK_UNIT_ALLOCATOR_CHUNK<T>*)units_chunks.Flink; current!=(MCHUNK_UNIT_ALLOCATOR_CHUNK<T>*)&units_chunks; )
	{
		MCHUNK_UNIT_ALLOCATOR_CHUNK<T>* next = (MCHUNK_UNIT_ALLOCATOR_CHUNK<T>*)current->entry.Flink;

		VirtualFree(current,NULL,MEM_RELEASE);

		current = next;
	}

	units_last = NULL;
	units_count = 0;

	InitializeListHead(&units_chunks);
}

template<class T>
T* mChunkUnitAllocator<T>::get_new_chunk()
{
	if( !units_last )
	{
		MCHUNK_UNIT_ALLOCATOR_CHUNK<T>* nchunk = (MCHUNK_UNIT_ALLOCATOR_CHUNK<T>*)VirtualAlloc(NULL,need_memory,MEM_COMMIT,PAGE_READWRITE);
		if( !nchunk ) 
		{
			err_critical("mChunkUnitAllocator can not allocate new chunk");
			__debugbreak();
			return NULL;
		}

		InsertTailList(&units_chunks, &nchunk->entry);

		nchunk->count = 0;

		units_last = nchunk;
	}

	units_count++;

	T* result = &units_last->items[units_last->count++];

	mem_zero(result,sizeof(T));

	if( units_last->count >= units_per_chunk )
		units_last = NULL;

	return result;
}

template<class T>
T* mChunkUnitAllocator<T>::get_first_chunk()
{
	if( IsListEmpty(&units_chunks) )
		return NULL;

	units_current_chunk = (MCHUNK_UNIT_ALLOCATOR_CHUNK<T>*)units_chunks.Flink;
	units_current_index = 1;

	return &units_current_chunk->items[0];
}

template<class T>
T* mChunkUnitAllocator<T>::get_next_chunk()
{
	if( units_current_index >= units_current_chunk->count )
	{
		units_current_chunk = (MCHUNK_UNIT_ALLOCATOR_CHUNK<T>*)units_current_chunk->entry.Flink;

		if( units_current_chunk==(MCHUNK_UNIT_ALLOCATOR_CHUNK<T>*)&units_chunks )
		{
			return NULL;
		}

		units_current_index = 0;
	}

	return &units_current_chunk->items[units_current_index++];
}

#endif