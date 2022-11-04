#ifndef __GLOBALS__
#define __GLOBALS__

#include "funcs.h"

#include "mCode.h"
#include "mArray.h"
#include "mConfig.h"
#include "mRandom.h"
#include "mImport.h"

extern mConfig	config;
extern mRandom	random;
extern mRandom	random2;
extern mImport	import;
extern DWORD	max_build_procs;

#include "mRandomTypes.h"
#include "mVars.h"
#include "mFunction.h"

extern mChunkUnitRandom<mFunction> all_functions;

#endif