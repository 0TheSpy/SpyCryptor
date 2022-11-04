#include <windows.h>
#include <tchar.h>
#include <intrin.h>
#include <Strsafe.h>

#include "globals.h"

int __cdecl mRandomTypes_cmp(PMRANDOM_TYPE a, PMRANDOM_TYPE b)
{
	BOOL isMustA = a->percents ? a->now_percents <= a->percents : 0;
	BOOL isMustB = b->percents ? b->now_percents <= b->percents : 0;

	if( isMustA < isMustB )
		return 1;

	if( isMustA > isMustB )
		return -1;

	return a->now_percents - b->now_percents;
}

int __cdecl mRandomTypes_cmp_ex(PMRANDOM_TYPE a, PMRANDOM_TYPE b)
{
	if( a->percents > b->percents )
		return -1;

	if( a->percents < b->percents )
		return 1;

	return random.get_less(0,2)==0 ? 1 : -1;
}

	mRandomTypes::mRandomTypes()
	{
		calls = 0;
	}
	mRandomTypes::~mRandomTypes()
	{
		clear();
	}

	void mRandomTypes::clear()
	{
		array.clear();
		calls = 0;
	}

	void mRandomTypes::add(DWORD Type,DWORD Percents)
	{
		MRANDOM_TYPE mt;

		mem_zero(&mt,sizeof(MRANDOM_TYPE));

		mt.type = Type;
		mt.percents = Percents;
		mt.now_percents = calls;

		array.add(mt);
	}

	BOOL mRandomTypes::exists(DWORD Type)
	{
		for(int i = 0; i < array.size(); i++)
		{
			if( array[i]->type==Type )
				return TRUE;
		}

		return FALSE;
	}

	DWORD mRandomTypes::get()
	{
		if( array.size() )
		{
			INT i;

			// do not move end of proc this cycle, because may be first call, on first call all now_percents==0
			for(i = 0; i < array.size(); i++)
			{
				array[i]->now_percents = array[i]->count ? array[i]->count*100/calls : 0;
			}

			// sort by now_percents
			array.qsort((TD_qsort)mRandomTypes_cmp);

			// check how much equal elements
			for(i = 1; i < array.size(); i++)
			{
				if( array[i]->now_percents!=array[0]->now_percents ) 
					break;
				if( array[i]->percents==0 )
					break;
				if( array[i]->now_percents > array[i]->percents )
					break;
			}

			if( i > 1 )
			{
				// in some situations we have elements with equal now_percents, then we must choose
				// top percent element, or if equal mix random
				array.qsort_ex((TD_qsort)mRandomTypes_cmp_ex, i);
			}

			DWORD idx = 0;

			array[idx]->count++;
			calls++;

			return array[idx]->type;
		}

		return -1;
	}
