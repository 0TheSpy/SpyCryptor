#ifndef _M_FUNCTION_
#define _M_FUNCTION_

#include "mVars.h"
#include "mRandom.h"
#include "mConfig.h"
#include "mCodeGenerator.h"
#include "mChunkUnitRandom.h"

#define REAL_CONVETIONS 3

enum FUNCTION_CONVENTION
{
	FC_RANDOM, // always NULL

	FC_STDCALL,
	FC_CDECL,
	FC_FASTCALL
};

class mFunction
{
	mVars	locals;
	mVars	formals;
	mVars*	globals;

	DWORD	user_data;

	mCodeGenerator code_generator;

	FUNCTION_CONVENTION convention;

	CHAR				name[120];
	MCODE_EXPRESSION	ret_exp;
	MVAR_TYPE			ret_mvar_type;

	DWORD id;
public:
	mFunction(mVars* Globals);
	~mFunction();
	void				set_name(PCHAR format, ...);
	PCHAR				get_name();
	void				set_convention(FUNCTION_CONVENTION Convention);
	void				set_return(MVAR_TYPE Returns);
	MVAR_INFO*			add_formal(MVAR_TYPE Type, PCHAR Name, DWORD Flags, PVOID Value);
	void				get_prototype_string(mCode &out);
	void				get_string(mCode &out);
	INT64				get_return_value();
	PMCODE_BLOCK		generate(DWORD min, DWORD max, DWORD flags);
	mCodeGenerator*		get_code_generator();
	mVars*				get_locals();
	mVars*				get_formals();

	void				set_user_data(DWORD data);
	DWORD				get_user_data();
};

#endif