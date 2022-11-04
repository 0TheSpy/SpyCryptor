#ifndef __M_CODE_EXPRESSION__
#define __M_CODE_EXPRESSION__

#include "config.h"

enum CODE_EXPRESSION_TYPE
{
	CET_VAR,
	CET_VAR_ADDR,
	CET_OP,
	CET_IMM
};

enum CODE_EXPRESSION_CMP_OPERATORS
{
	CECO_EQUAL,
	CECO_NOT_EQUAL,
	CECO_LESS,
	CECO_GREAT_OR_EQUAL,
	CECO_GREAT,
	CECO_LESS_OR_EQUAL,
};

enum CODE_EXPRESSION_OPERATORS
{
	CEO_EQUAL,
	CEO_LGROUP,
	CEO_RGROUP,

	CEO_ADD,
	CEO_SUB,
	CEO_MUL,
	CEO_DIV,
	CEO_MOD,
	CEO_AND,
	CEO_XOR,
	CEO_OR,
	CEO_SAL,
	CEO_SAR,
};

#define CEO_FOR_INT_FROM 3
#define CEO_FOR_INT_TO	3 + 10

#define OP_COUNT (sizeof(CodeExpressionOperators)/sizeof(PCHAR))

typedef struct _MCODE_EXPRESSION_ARGUMENT
{
	BYTE type;
	union {
		PMVAR_INFO					var;
		INT64						imm;
		CODE_EXPRESSION_OPERATORS	op;
	};
}MCODE_EXPRESSION_ARGUMENT,*PMCODE_EXPRESSION_ARGUMENT;

typedef struct _MCODE_EXPRESSION
{
	DWORD						count;
	MCODE_EXPRESSION_ARGUMENT	arguments[CONFIG_MCODE_EXPRESSION_MAX_ARGUMENTS];
}MCODE_EXPRESSION,*PMCODE_EXPRESSION;

#define EM_CALLING __fastcall

typedef void (__fastcall *TD_emulate_operation)(PVOID Left,CODE_EXPRESSION_OPERATORS Operator,PVOID Right);

void EM_CALLING emulate_int8_int8(PINT8 left, CODE_EXPRESSION_OPERATORS Operator, PINT8 right);
void EM_CALLING emulate_int8_int16(PINT8 left, CODE_EXPRESSION_OPERATORS Operator, PINT16 right);
void EM_CALLING emulate_int8_int32(PINT8 left, CODE_EXPRESSION_OPERATORS Operator, PINT32 right);
void EM_CALLING emulate_int8_int64(PINT8 left, CODE_EXPRESSION_OPERATORS Operator, PINT64 right);
void EM_CALLING emulate_int8_uint8(PINT8 left, CODE_EXPRESSION_OPERATORS Operator, PUINT8 right);
void EM_CALLING emulate_int8_uint16(PINT8 left, CODE_EXPRESSION_OPERATORS Operator, PUINT16 right);
void EM_CALLING emulate_int8_uint32(PINT8 left, CODE_EXPRESSION_OPERATORS Operator, PUINT32 right);
void EM_CALLING emulate_int8_uint64(PINT8 left, CODE_EXPRESSION_OPERATORS Operator, PUINT64 right);
void EM_CALLING emulate_int16_int8(PINT16 left, CODE_EXPRESSION_OPERATORS Operator, PINT8 right);
void EM_CALLING emulate_int16_int16(PINT16 left, CODE_EXPRESSION_OPERATORS Operator, PINT16 right);
void EM_CALLING emulate_int16_int32(PINT16 left, CODE_EXPRESSION_OPERATORS Operator, PINT32 right);
void EM_CALLING emulate_int16_int64(PINT16 left, CODE_EXPRESSION_OPERATORS Operator, PINT64 right);
void EM_CALLING emulate_int16_uint8(PINT16 left, CODE_EXPRESSION_OPERATORS Operator, PUINT8 right);
void EM_CALLING emulate_int16_uint16(PINT16 left, CODE_EXPRESSION_OPERATORS Operator, PUINT16 right);
void EM_CALLING emulate_int16_uint32(PINT16 left, CODE_EXPRESSION_OPERATORS Operator, PUINT32 right);
void EM_CALLING emulate_int16_uint64(PINT16 left, CODE_EXPRESSION_OPERATORS Operator, PUINT64 right);
void EM_CALLING emulate_int32_int8(PINT32 left, CODE_EXPRESSION_OPERATORS Operator, PINT8 right);
void EM_CALLING emulate_int32_int16(PINT32 left, CODE_EXPRESSION_OPERATORS Operator, PINT16 right);
void EM_CALLING emulate_int32_int32(PINT32 left, CODE_EXPRESSION_OPERATORS Operator, PINT32 right);
void EM_CALLING emulate_int32_int64(PINT32 left, CODE_EXPRESSION_OPERATORS Operator, PINT64 right);
void EM_CALLING emulate_int32_uint8(PINT32 left, CODE_EXPRESSION_OPERATORS Operator, PUINT8 right);
void EM_CALLING emulate_int32_uint16(PINT32 left, CODE_EXPRESSION_OPERATORS Operator, PUINT16 right);
void EM_CALLING emulate_int32_uint32(PINT32 left, CODE_EXPRESSION_OPERATORS Operator, PUINT32 right);
void EM_CALLING emulate_int32_uint64(PINT32 left, CODE_EXPRESSION_OPERATORS Operator, PUINT64 right);
void EM_CALLING emulate_int64_int8(PINT64 left, CODE_EXPRESSION_OPERATORS Operator, PINT8 right);
void EM_CALLING emulate_int64_int16(PINT64 left, CODE_EXPRESSION_OPERATORS Operator, PINT16 right);
void EM_CALLING emulate_int64_int32(PINT64 left, CODE_EXPRESSION_OPERATORS Operator, PINT32 right);
void EM_CALLING emulate_int64_int64(PINT64 left, CODE_EXPRESSION_OPERATORS Operator, PINT64 right);
void EM_CALLING emulate_int64_uint8(PINT64 left, CODE_EXPRESSION_OPERATORS Operator, PUINT8 right);
void EM_CALLING emulate_int64_uint16(PINT64 left, CODE_EXPRESSION_OPERATORS Operator, PUINT16 right);
void EM_CALLING emulate_int64_uint32(PINT64 left, CODE_EXPRESSION_OPERATORS Operator, PUINT32 right);
void EM_CALLING emulate_int64_uint64(PINT64 left, CODE_EXPRESSION_OPERATORS Operator, PUINT64 right);
void EM_CALLING emulate_uint8_int8(PUINT8 left, CODE_EXPRESSION_OPERATORS Operator, PINT8 right);
void EM_CALLING emulate_uint8_int16(PUINT8 left, CODE_EXPRESSION_OPERATORS Operator, PINT16 right);
void EM_CALLING emulate_uint8_int32(PUINT8 left, CODE_EXPRESSION_OPERATORS Operator, PINT32 right);
void EM_CALLING emulate_uint8_int64(PUINT8 left, CODE_EXPRESSION_OPERATORS Operator, PINT64 right);
void EM_CALLING emulate_uint8_uint8(PUINT8 left, CODE_EXPRESSION_OPERATORS Operator, PUINT8 right);
void EM_CALLING emulate_uint8_uint16(PUINT8 left, CODE_EXPRESSION_OPERATORS Operator, PUINT16 right);
void EM_CALLING emulate_uint8_uint32(PUINT8 left, CODE_EXPRESSION_OPERATORS Operator, PUINT32 right);
void EM_CALLING emulate_uint8_uint64(PUINT8 left, CODE_EXPRESSION_OPERATORS Operator, PUINT64 right);
void EM_CALLING emulate_uint16_int8(PUINT16 left, CODE_EXPRESSION_OPERATORS Operator, PINT8 right);
void EM_CALLING emulate_uint16_int16(PUINT16 left, CODE_EXPRESSION_OPERATORS Operator, PINT16 right);
void EM_CALLING emulate_uint16_int32(PUINT16 left, CODE_EXPRESSION_OPERATORS Operator, PINT32 right);
void EM_CALLING emulate_uint16_int64(PUINT16 left, CODE_EXPRESSION_OPERATORS Operator, PINT64 right);
void EM_CALLING emulate_uint16_uint8(PUINT16 left, CODE_EXPRESSION_OPERATORS Operator, PUINT8 right);
void EM_CALLING emulate_uint16_uint16(PUINT16 left, CODE_EXPRESSION_OPERATORS Operator, PUINT16 right);
void EM_CALLING emulate_uint16_uint32(PUINT16 left, CODE_EXPRESSION_OPERATORS Operator, PUINT32 right);
void EM_CALLING emulate_uint16_uint64(PUINT16 left, CODE_EXPRESSION_OPERATORS Operator, PUINT64 right);
void EM_CALLING emulate_uint32_int8(PUINT32 left, CODE_EXPRESSION_OPERATORS Operator, PINT8 right);
void EM_CALLING emulate_uint32_int16(PUINT32 left, CODE_EXPRESSION_OPERATORS Operator, PINT16 right);
void EM_CALLING emulate_uint32_int32(PUINT32 left, CODE_EXPRESSION_OPERATORS Operator, PINT32 right);
void EM_CALLING emulate_uint32_int64(PUINT32 left, CODE_EXPRESSION_OPERATORS Operator, PINT64 right);
void EM_CALLING emulate_uint32_uint8(PUINT32 left, CODE_EXPRESSION_OPERATORS Operator, PUINT8 right);
void EM_CALLING emulate_uint32_uint16(PUINT32 left, CODE_EXPRESSION_OPERATORS Operator, PUINT16 right);
void EM_CALLING emulate_uint32_uint32(PUINT32 left, CODE_EXPRESSION_OPERATORS Operator, PUINT32 right);
void EM_CALLING emulate_uint32_uint64(PUINT32 left, CODE_EXPRESSION_OPERATORS Operator, PUINT64 right);
void EM_CALLING emulate_uint64_int8(PUINT64 left, CODE_EXPRESSION_OPERATORS Operator, PINT8 right);
void EM_CALLING emulate_uint64_int16(PUINT64 left, CODE_EXPRESSION_OPERATORS Operator, PINT16 right);
void EM_CALLING emulate_uint64_int32(PUINT64 left, CODE_EXPRESSION_OPERATORS Operator, PINT32 right);
void EM_CALLING emulate_uint64_int64(PUINT64 left, CODE_EXPRESSION_OPERATORS Operator, PINT64 right);
void EM_CALLING emulate_uint64_uint8(PUINT64 left, CODE_EXPRESSION_OPERATORS Operator, PUINT8 right);
void EM_CALLING emulate_uint64_uint16(PUINT64 left, CODE_EXPRESSION_OPERATORS Operator, PUINT16 right);
void EM_CALLING emulate_uint64_uint32(PUINT64 left, CODE_EXPRESSION_OPERATORS Operator, PUINT32 right);
void EM_CALLING emulate_uint64_uint64(PUINT64 left, CODE_EXPRESSION_OPERATORS Operator, PUINT64 right);

PCHAR op2str(CODE_EXPRESSION_OPERATORS op);
PCHAR cop2str(CODE_EXPRESSION_CMP_OPERATORS op);

class mCodeExpressionEmulator
{
public:
	static int	compare(PVOID left, MVAR_TYPE right_type, PVOID right);
	static void	emulate(PVOID left, CODE_EXPRESSION_OPERATORS op,MVAR_TYPE VarTypeRight, PVOID right);
	static void	emulate_begin(PVOID result, MVAR_TYPE type, PVOID right);
	static void emulate_end(PVOID result, MVAR_TYPE type, PVOID right);
};

class mCodeExpression
{
	PMCODE_EXPRESSION	expression;
public:
	mCodeExpression(PMCODE_EXPRESSION expression);
	mCodeExpression();

	void		set(PMCODE_EXPRESSION expression);

	void		clear();

	int			add_var(MVAR_INFO* pvar);
	int			add_var_addr(MVAR_INFO* var);
	int			add_imm(INT64 Imm);
	int			add_op(CODE_EXPRESSION_OPERATORS Op);

	void		get_string(mCode &str);
	INT64		emulate(MVAR_TYPE type);
	PMVAR_INFO	get_var_addr();
#ifdef CONFIG_DEBUG_EMULATOR
	static void	get_debug_string(PCHAR string, PMCODE_EXPRESSION expression);
#endif
};

#endif