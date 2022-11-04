#ifndef _M_ARRAY_
#define _M_ARRAY_

#define MARRAY_GRANULARITY 1

template<typename T>
struct MARRAY
{
	DWORD	count;
	DWORD	free;
	T		elements[0];
};

typedef int (__cdecl *TD_qsort)(const void *, const void *);

template<class T>
class mArray
{
	MARRAY<T>*	m_data;
public:
	mArray()
	{
		m_data = NULL;
	}
	~mArray()
	{
		clear();
	}
	void clear()
	{
		LocalFree(m_data);
		m_data = NULL;
	}

	VOID qsort(TD_qsort cmpfunc)
	{
		if( m_data )
		{
			::qsort(m_data->elements,m_data->count,sizeof(T),cmpfunc);
		}
	}

	VOID qsort_ex(TD_qsort cmpfunc,DWORD count)
	{
		if( m_data )
		{
			::qsort(m_data->elements,count,sizeof(T),cmpfunc);
		}
	}

	DWORD size()
	{
		return (m_data ? m_data->count : 0);
	}

	T* add(T &element)
	{
		DWORD Need = MARRAY_GRANULARITY;

		if( m_data )
		{
			if( m_data->free )
			{
				m_data->elements[ m_data->count ] = element;

				m_data->count++;
				m_data->free--;

				return &m_data->elements[ m_data->count - 1 ];
			}

			Need += m_data->count;
		}

		MARRAY<T>* n_data = (MARRAY<T>*)LocalAlloc(LMEM_FIXED,sizeof(MARRAY<T>) + Need*sizeof(T));
		if( n_data )
		{
			if( m_data )
			{
				n_data->count = m_data->count;

				mem_copy(n_data->elements,m_data->elements,m_data->count*sizeof(T));
			}else{
				n_data->count = 0;
			}

			n_data->free = MARRAY_GRANULARITY;

			LocalFree(m_data);
			m_data = n_data;

			return add(element);
		}

		return NULL;
	}

	void remove(int FromIndex,int Count)
	{
		if( m_data && FromIndex < m_data->count )
		{
			if( FromIndex + Count > m_data->count )
			{
				Count = m_data->count - FromIndex;
			}

			for(int i = 0, rep = (m_data->count - (FromIndex + Count)); i < rep; i++)
			{
				m_data->elements[FromIndex + i] = m_data->elements[FromIndex + Count + i];
			}

			m_data->count -= Count;
			m_data->free += Count;
		}
	}

	T* add()
	{
		T el;

		mem_zero(&el,sizeof(T));

		if( add(el) )
		{
			return get(m_data->count - 1);
		}

		return NULL;
	}

	T* get(DWORD index)
	{
		return &m_data->elements[index];
	}

	T* operator [] (DWORD index)
	{
		return get(index);
	}

};

template<class T>
void shuffle(mArray<T> &arr)
{
	//if( arr )
	{
		for(DWORD i = 0; i < arr.size()>>1 ; i++)
		{
			DWORD a;

			do 
			{
				a = randrange(0, arr.size() - 1);
			} while (a==i);


			T tmp = *arr[i];
			*arr[i] = *arr[a];
			*arr[a] = tmp;
		}
	}
}

#endif