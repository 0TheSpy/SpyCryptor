#ifndef _M_CHUNK_UNIT_RANDOM_
#define _M_CHUNK_UNIT_RANDOM_

template<class T>
struct MCHUNK_UNIT_RANDOM_CHUNK
{
	DWORD		count;
	T			items[0];
};

#define MCHUNK_REFS_MAX_TABLES 30

template<class T>
class mChunkUnitRandom
{
	DWORD							need_memory;
	DWORD							units_per_chunk;

	DWORD							refs_count;

	DWORD							refs_tables_current;
	MCHUNK_UNIT_RANDOM_CHUNK<T*>*	refs_tables[MCHUNK_REFS_MAX_TABLES];

	T**		get_by_index(DWORD index);
	void	swap_by_index(DWORD index_a, DWORD index_b);
public:
	mChunkUnitRandom(DWORD UnitPerChunk);
	~mChunkUnitRandom();

	void	clear_all();
	BOOL	add_reference(T* referece);
	DWORD	get_count();
	T*		get_random(DWORD except_count, T* excepts[]);
	T*		get_value(DWORD index);
};

template< class T>
mChunkUnitRandom<T>::mChunkUnitRandom(DWORD UnitPerChunk)
{
	if( UnitPerChunk < 0x400 )
		UnitPerChunk = 0x400;

	need_memory		= ALIGN_UP((UnitPerChunk*sizeof(T*)) + sizeof(MCHUNK_UNIT_RANDOM_CHUNK<T*>),0x1000); // ekonomim pamatj :)
	units_per_chunk = (need_memory - sizeof(MCHUNK_UNIT_RANDOM_CHUNK<T*>))/sizeof(T*);

	refs_tables_current = 0;
	refs_count = 0;

	mem_zero(refs_tables, sizeof(refs_tables));
}

template< class T>
mChunkUnitRandom<T>::~mChunkUnitRandom()
{
	clear_all();
}

template< class T>
void mChunkUnitRandom<T>::clear_all()
{
	for(int i = 0; i < MCHUNK_REFS_MAX_TABLES; i++)
	{
		if( refs_tables[i] )
		{
			VirtualFree(refs_tables[i],0,MEM_RELEASE);
			refs_tables[i] = NULL;
		}
	}
}

template<class T>
DWORD mChunkUnitRandom<T>::get_count()
{
	return refs_count;
}

template<class T>
T** mChunkUnitRandom<T>::get_by_index(DWORD index)
{
	DWORD table = index/units_per_chunk;
	DWORD id	= index%units_per_chunk;

	return &refs_tables[table]->items[id];
}

template<class T>
T*	mChunkUnitRandom<T>::get_value(DWORD index)
{
	DWORD table = index/units_per_chunk;
	DWORD id	= index%units_per_chunk;

	return refs_tables[table]->items[id]; 
}

template<class T>
void mChunkUnitRandom<T>::swap_by_index(DWORD index_a, DWORD index_b)
{
	T**	var_a = get_by_index(index_a);
	T** var_b = get_by_index(index_b);

	T* tmp = *var_a;
	*var_a = *var_b;
	*var_b = tmp;
}

template<class T>
BOOL mChunkUnitRandom<T>::add_reference(T* reference)
{
	if( refs_tables_current < sizeof(refs_tables)/sizeof(MCHUNK_UNIT_RANDOM_CHUNK<T*>*) )
	{
		MCHUNK_UNIT_RANDOM_CHUNK<T*>* last_table = refs_tables[refs_tables_current];;

		if( !last_table )
		{
			last_table = (MCHUNK_UNIT_RANDOM_CHUNK<T*>*)VirtualAlloc(NULL,need_memory,MEM_COMMIT,PAGE_READWRITE);
			if( !last_table ) 
			{
				err_critical("mChunkUnitRandom can not allocate new chunk");
				__debugbreak();
				return false;
			}

			last_table->count = 0;

			refs_tables[refs_tables_current] = last_table;
		}

		last_table->items[ last_table->count++ ] = reference;

		// on add mix
		if( refs_count > 2 )
		{
			DWORD pos = random.get_less(0, refs_count);

			swap_by_index(refs_count, pos);
		}

		refs_count++;

		if( last_table->count >= units_per_chunk )
			refs_tables_current++;

		return true;
	}

	return false;
}

template<class T>
T* mChunkUnitRandom<T>::get_random(DWORD except_count,T* excepts[])
{
	T* result = NULL;
	int e;

	DWORD upos = 0;
	for(int i = 0; i < refs_count; i++)
	{
		DWORD pos = random.get_less(upos, refs_count);
		T**   var = get_by_index(pos);

		e = 0;
		while( e < except_count && excepts[e]!=*var )
		{
			e++;
		}

		if( e >= except_count )
		{
			result = *var;
			break;
		}

		swap_by_index(upos, pos);
		upos++;
	}

	return result;
}

template<class T>
void shuffle(T array[], int size)
{
	if( array && size )
	{
		for(int i = 0; i < size; i++)
		{
			int pos = random.get_less(0, size);

			T tmp = array[i];
			array[i] = array[pos];
			array[pos] = tmp;
		}
	}
}

#endif