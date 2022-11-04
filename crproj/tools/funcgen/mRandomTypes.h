#ifndef _M_RANDOMTYPES_
#define _M_RANDOMTYPES_

#include "funcs.h"
#include "mRandom.h"
#include "mArray.h"

typedef struct _MRANDOM_TYPE
{
	INT		now_percents; // actual %
	DWORD	type;
	INT		percents; // not less than
	DWORD	count;
}MRANDOM_TYPE,*PMRANDOM_TYPE;

int __cdecl mRandomTypes_cmp(PMRANDOM_TYPE a, PMRANDOM_TYPE b);

class mRandomTypes
{
	mArray<MRANDOM_TYPE> array;
	DWORD	calls;
public:
	mRandomTypes();
	~mRandomTypes();
	void clear();
	void add(DWORD Type,DWORD Percents);
	BOOL exists(DWORD Type);
	DWORD get();
};


#endif