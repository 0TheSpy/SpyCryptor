#ifndef __M_CODE_EMULATOR__
#define __M_CODE_EMULATOR__

#include "config.h"

#include "mCodeExpression.h"
#include "mChunkUnitAllocator.h"
#include "mCode.h"
#include "mFunction.h"
#include "mCodeGenerator.h"

class mFunction;
class mCodeGenerator;

enum CODE_EMULATOR_BLOCK_TYPES
{
	CEBT_NONE,
	CEBT_LINE,
	CEBT_PTR_LINE,
	CEBT_IF,
	CEBT_ELSEIF,
	CEBT_ELSE,
	CEBT_API,
	CEBT_LOOP,
	CEBT_SWITCH,
	CEBT_PROC,
	CEBT_CASE,
	CEBT_DEFAULT,
	CEBT_CUSTOM,
};

struct _MCODE_BLOCK;

#define MBLOCK_EXTRA_MAX 20

typedef struct _MBLOCK_EXTRA
{
	DWORD			count;
	_MCODE_BLOCK*	blocks[MBLOCK_EXTRA_MAX];
}MBLOCK_EXTRA,*PMBLOCK_EXTRA;

#define MCP_TYPE_VAR	0
#define MCP_TYPE_IMM	1

typedef struct _MCODE_BLOCK
{
	LIST_ENTRY			entry;
	MBLOCK_EXTRA		extra_blocks;

	CODE_EMULATOR_BLOCK_TYPES	type;

	DWORD				inner_count;
	LIST_ENTRY			inner;

	CHAR comment[CONFIG_COMMENT_MAX_LENGTH];

	union{
		struct 
		{
			PMVAR_INFO			left_var;
			MCODE_EXPRESSION	expression;
		}code;

		struct  
		{
			PMVAR_INFO	left_var;
		}code_ptr;

		struct  
		{
			MCODE_EXPRESSION				left;
			CODE_EXPRESSION_CMP_OPERATORS	op;
			BOOL							is_trash;
			MCODE_EXPRESSION				right;
			MVAR_TYPE						type;
		}compare;

		struct  
		{
			PMVAR_INFO	var;
		}block_switch;

		struct 
		{
			INT64	value;
			BOOL	is_trash;
		}block_case;

		struct  
		{
			DWORD	count;
			DWORD	start;
			DWORD	step;
			PMVAR_INFO	index;
			_MCODE_BLOCK*	index_start;
			_MCODE_BLOCK*	index_update;
			BYTE	type; // 0 - for, 1 - while
			CODE_EXPRESSION_CMP_OPERATORS	op;
		}loop;

		struct  
		{
			CHAR	string[CONFIG_CODE_LINE_MAX_LENGTH];
		}custom;

		struct 
		{
			mFunction		*proc;
			DWORD			params_count;
			struct  
			{
				BYTE		type;
				union{
					PMVAR_INFO	var;
					INT64		imm;
				};
			}params[8];
		}block_proc;
	};
}MCODE_BLOCK,*PMCODE_BLOCK;

class mCodeEmulator
{
	DWORD								count;
	LIST_ENTRY							entries;
	mChunkUnitAllocator<MCODE_BLOCK>	units;

	mVars* locals;
	mVars* globals;
	mVars* formals;

	void get_block_string(mCode &str, DWORD level, PMCODE_BLOCK block);
	void get_blocks_string(mCode &str, DWORD level, PLIST_ENTRY root);
	void add(PMCODE_BLOCK parent, PMCODE_BLOCK new_block);
public:
	mCodeEmulator(mVars *Globals,mVars *Locals, mVars *Formals);
	~mCodeEmulator();

	PMCODE_BLOCK allocate(PMCODE_BLOCK parent, CODE_EMULATOR_BLOCK_TYPES type);;
	PMCODE_BLOCK allocate_ex(PMCODE_BLOCK parent, CODE_EMULATOR_BLOCK_TYPES type, BOOL add_to_parent);

	DWORD get_inner_count(PMCODE_BLOCK parent);
	PMCODE_BLOCK get_inner(PMCODE_BLOCK parent);

	PMCODE_BLOCK	get_first();
	PMCODE_BLOCK	get_end();

	void emulate();
	void get_string(mCode& str);
};

#endif