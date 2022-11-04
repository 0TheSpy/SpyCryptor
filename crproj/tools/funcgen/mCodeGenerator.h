#ifndef _M_CODEGENERATOR_
#define _M_CODEGENERATOR_

#include "mVars.h"
#include "mRandom.h"
#include "mRandomTypes.h"
#include "mCodeExpression.h"
#include "mCodeEmulator.h"

enum VAR_USAGE
{
	VAR_USE_LOCAL,
	VAR_USE_GLOBAL,
	VAR_USE_FORMAL,
};

enum VAR_INIT
{
	VAR_INIT_NOT_ANY,
	VAR_INIT_LOCAL,
	VAR_INIT_GLOBAL,
};

#define IS_IF		0x1
#define IS_LOOP		0x2
#define IS_TRASH	0x4
#define DO_NOT_CHANGE_PARENT	0x8
#define IS_PROC		0x10
#define IS_CASE		0x20
#define IS_START_EP	0x10000

#define CLEAR_STARTUP_FLAGS(flags) (flags & 0xFFFF)

typedef struct _MCODE_GENERATOR_MAP
{
	DWORD	count;
	DWORD	position;
	BYTE	types[0];
}MCODE_GENERATOR_MAP,*PMCODE_GENERATOR_MAP;

class mCodeGenerator
{
	mVars* locals;
	mVars* globals;
	mVars* formals;

	mRandomTypes vusage_left;
	mRandomTypes vusage_right;
	mRandomTypes vtypes;
	mRandomTypes code_type;
	mRandomTypes cblock_ops;

	MVAR_INFO*		if_last_vars[4];

	mChunkUnitRandom<MCODE_BLOCK>	safe_random;

	mCodeEmulator	graph;

	// first we get any types, only then randomtypes
	DWORD			locals_first_pos;
	DWORD			locals_first_count;
	MVAR_TYPE		locals_first[8 + 8]; // min 2 vars on each type

	DWORD			globals_first_pos;
	DWORD			globals_first_count;
	MVAR_TYPE		globals_first[8 + 8]; // min 2 vars on each type

	CHAR			tabs[0xFF];
	int				calls_idx;
	int				loop_idx;

	MVAR_INFO*		add_or_get_random_var(DWORD flags, BOOL isPtr);
	MVAR_INFO*		get_random_var_or_any(DWORD flags, BOOL isPtr);
	DWORD64			get_imm_by_operation(MVAR_INFO* left_var,CODE_EXPRESSION_OPERATORS op);
	BOOL			is_null(PMVAR_INFO var);
	CODE_EXPRESSION_OPERATORS get_op_by_value(INT64 result, DWORD flags);
	PMCODE_BLOCK	generate_code(PMCODE_BLOCK parent, DWORD interations, DWORD flags, BOOL isPtr);
	PMCODE_BLOCK	generate_api(PMCODE_BLOCK parent, DWORD interations, DWORD flags);
	DWORD			generate_if(PMCODE_BLOCK parent, PMCODE_BLOCK chain[], PMCODE_BLOCK *new_parent, DWORD level, DWORD left, DWORD interations, DWORD flags);
	DWORD			generate_switch(PMCODE_BLOCK parent, PMCODE_BLOCK chain[], PMCODE_BLOCK *new_parent, PMCODE_BLOCK *save_switch_block, DWORD level, DWORD left, DWORD interations, DWORD flags);
	PMCODE_BLOCK	generate_loop(PMCODE_BLOCK parent, DWORD level, DWORD left, DWORD interations);
	mFunction*		generate_proc(PMCODE_BLOCK parent, DWORD level, DWORD left, DWORD interations, DWORD flags, PDWORD icount);
	PMCODE_BLOCK	generate_blocks(PMCODE_GENERATOR_MAP map, PMCODE_BLOCK parent, INT left, DWORD level, DWORD interations, DWORD flags);
public:
	mCodeGenerator(mVars *Globals,mVars *Locals, mVars *Formals);
	mCodeEmulator*	get_graph();

	PMVAR_INFO		get_random_var_by_random_type(DWORD flags, MVAR_TYPE type, DWORD except, MVAR_INFO* except_vars[]);
	PMVAR_INFO		get_random_var_by_type(DWORD flags, MVAR_TYPE Type, DWORD except, MVAR_INFO* except_vars[]);

	void			emulate_block(PMCODE_BLOCK block, BOOL is_first);
	PMCODE_BLOCK	generate(PMCODE_BLOCK parent, DWORD min, DWORD max, DWORD flags);

	mVars*			get_locals();
	mVars*			get_globals();
	mVars*			get_formals();

	void			get_string(mCode &str);
};

#endif