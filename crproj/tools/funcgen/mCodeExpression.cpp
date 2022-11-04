#include <windows.h>
#include <tchar.h>
#include <intrin.h>
#include <Strsafe.h>

#include "globals.h"
#include "mCodeExpression.h"

PCHAR CodeExpressionOperators[] = {
	" = ",
	" ( ",
	" ) ",
	" + ",
	" - ",
	" * ",
	" / ",
	" % ",
	" & ",
	" ^ ",
	" | ",
	" << ",
	" >> "
};

PCHAR CodeExpressionCmpOperators[] = {
	" == ",
	" != ",
	" < ",
	" >= ",
	" > ",
	" <= "
};

PCHAR op2str(CODE_EXPRESSION_OPERATORS op)
{
	return CodeExpressionOperators[op];
}

PCHAR cop2str(CODE_EXPRESSION_CMP_OPERATORS op)
{
	return CodeExpressionCmpOperators[op];
}

void EM_CALLING emulate_int8_int8(PINT8 left, CODE_EXPRESSION_OPERATORS Operator, PINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int8_int16(PINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int8_int32(PINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int8_int64(PINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int8_uint8(PINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int8_uint16(PINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int8_uint32(PINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int8_uint64(PINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int16_int8(PINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int16_int16(PINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int16_int32(PINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int16_int64(PINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int16_uint8(PINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int16_uint16(PINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int16_uint32(PINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int16_uint64(PINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int32_int8(PINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int32_int16(PINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int32_int32(PINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= *right; break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int32_int64(PINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int32_uint8(PINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int32_uint16(PINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int32_uint32(PINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int32_uint64(PINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int64_int8(PINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int64_int16(PINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int64_int32(PINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int64_int64(PINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int64_uint8(PINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int64_uint16(PINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int64_uint32(PINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_int64_uint64(PINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint8_int8(PUINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint8_int16(PUINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint8_int32(PUINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint8_int64(PUINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint8_uint8(PUINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint8_uint16(PUINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint8_uint32(PUINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint8_uint64(PUINT8 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint16_int8(PUINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint16_int16(PUINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint16_int32(PUINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint16_int64(PUINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint16_uint8(PUINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint16_uint16(PUINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint16_uint32(PUINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint16_uint64(PUINT16 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint32_int8(PUINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint32_int16(PUINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint32_int32(PUINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint32_int64(PUINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint32_uint8(PUINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint32_uint16(PUINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint32_uint32(PUINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint32_uint64(PUINT32 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint64_int8(PUINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint64_int16(PUINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint64_int32(PUINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint64_int64(PUINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint64_uint8(PUINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT8 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint64_uint16(PUINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT16 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint64_uint32(PUINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT32 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}
void EM_CALLING emulate_uint64_uint64(PUINT64 left, CODE_EXPRESSION_OPERATORS  Operator, PUINT64 right)
{
	switch( Operator )
	{
	case CEO_SUB: *left -= *right; break;
	case CEO_ADD: *left += *right; break;
	case CEO_MUL: *left *= (*right); break;
	case CEO_DIV: *left /= (*right); break;
	case CEO_MOD: *left %= (*right); break;
	case CEO_XOR: *left ^= *right; break;
	case CEO_OR: *left |= *right; break;
	case CEO_AND: *left &= *right; break;
	case CEO_SAL: *left <<= *right; break;
	case CEO_SAR: *left >>= *right; break;
	case CEO_EQUAL: *left = *right; break;
	}
}


TD_emulate_operation CodeStackEmulators[8][8] = {
	{(TD_emulate_operation)emulate_int8_int8,(TD_emulate_operation)emulate_int8_int16,(TD_emulate_operation)emulate_int8_int32,(TD_emulate_operation)emulate_int8_int64,(TD_emulate_operation)emulate_int8_uint8,(TD_emulate_operation)emulate_int8_uint16,(TD_emulate_operation)emulate_int8_uint32,(TD_emulate_operation)emulate_int8_uint64,},
	{(TD_emulate_operation)emulate_int16_int8,(TD_emulate_operation)emulate_int16_int16,(TD_emulate_operation)emulate_int16_int32,(TD_emulate_operation)emulate_int16_int64,(TD_emulate_operation)emulate_int16_uint8,(TD_emulate_operation)emulate_int16_uint16,(TD_emulate_operation)emulate_int16_uint32,(TD_emulate_operation)emulate_int16_uint64,},
	{(TD_emulate_operation)emulate_int32_int8,(TD_emulate_operation)emulate_int32_int16,(TD_emulate_operation)emulate_int32_int32,(TD_emulate_operation)emulate_int32_int64,(TD_emulate_operation)emulate_int32_uint8,(TD_emulate_operation)emulate_int32_uint16,(TD_emulate_operation)emulate_int32_uint32,(TD_emulate_operation)emulate_int32_uint64,},
	{(TD_emulate_operation)emulate_int64_int8,(TD_emulate_operation)emulate_int64_int16,(TD_emulate_operation)emulate_int64_int32,(TD_emulate_operation)emulate_int64_int64,(TD_emulate_operation)emulate_int64_uint8,(TD_emulate_operation)emulate_int64_uint16,(TD_emulate_operation)emulate_int64_uint32,(TD_emulate_operation)emulate_int64_uint64,},
	{(TD_emulate_operation)emulate_uint8_int8,(TD_emulate_operation)emulate_uint8_int16,(TD_emulate_operation)emulate_uint8_int32,(TD_emulate_operation)emulate_uint8_int64,(TD_emulate_operation)emulate_uint8_uint8,(TD_emulate_operation)emulate_uint8_uint16,(TD_emulate_operation)emulate_uint8_uint32,(TD_emulate_operation)emulate_uint8_uint64,},
	{(TD_emulate_operation)emulate_uint16_int8,(TD_emulate_operation)emulate_uint16_int16,(TD_emulate_operation)emulate_uint16_int32,(TD_emulate_operation)emulate_uint16_int64,(TD_emulate_operation)emulate_uint16_uint8,(TD_emulate_operation)emulate_uint16_uint16,(TD_emulate_operation)emulate_uint16_uint32,(TD_emulate_operation)emulate_uint16_uint64,},
	{(TD_emulate_operation)emulate_uint32_int8,(TD_emulate_operation)emulate_uint32_int16,(TD_emulate_operation)emulate_uint32_int32,(TD_emulate_operation)emulate_uint32_int64,(TD_emulate_operation)emulate_uint32_uint8,(TD_emulate_operation)emulate_uint32_uint16,(TD_emulate_operation)emulate_uint32_uint32,(TD_emulate_operation)emulate_uint32_uint64,},
	{(TD_emulate_operation)emulate_uint64_int8,(TD_emulate_operation)emulate_uint64_int16,(TD_emulate_operation)emulate_uint64_int32,(TD_emulate_operation)emulate_uint64_int64,(TD_emulate_operation)emulate_uint64_uint8,(TD_emulate_operation)emulate_uint64_uint16,(TD_emulate_operation)emulate_uint64_uint32,(TD_emulate_operation)emulate_uint64_uint64,},
};

int mCodeExpressionEmulator::compare(PVOID left, MVAR_TYPE right_type, PVOID right)
{
	switch( right_type )
	{
	case MVT_UINT8:
	case MVT_UINT16:
	case MVT_PUINT8:
	case MVT_PUINT16:
	case MVT_PINT8:
	case MVT_PINT16:
	case MVT_PINT32:
	case MVT_INT8:
	case MVT_INT16:
	case MVT_INT32:
		if( *(PINT32)left==*(PINT32)right )
			return 0;
		if( *(PINT32)left > *(PINT32)right )
			return 1;
		return -1;
	case MVT_PUINT32:
	case MVT_UINT32:
		if( *(PUINT32)left==*(PUINT32)right )
			return 0;
		if( *(PUINT32)left > *(PUINT32)right )
			return 1;
		return -1;
	case MVT_PINT64:
	case MVT_INT64:
		if( *(PINT64)left==*(PINT64)right )
			return 0;
		if( *(PINT64)left > *(PINT64)right )
			return 1;
		return -1;
	case MVT_PUINT64:
	case MVT_UINT64:
		if( *(PUINT64)left==*(PUINT64)right )
			return 0;
		if( *(PUINT64)left > *(PUINT64)right )
			return 1;
		return -1;
	}
}

void mCodeExpressionEmulator::emulate(PVOID left, CODE_EXPRESSION_OPERATORS  op, MVAR_TYPE VarTypeRight, PVOID right)
{
	if( op==CEO_EQUAL )
	{
		emulate_begin(left, VarTypeRight, right);
	}else{
		MVAR_TYPE use;

		/*
		add, sub, and, xor, or   - poh na tip
		signed - shl, sar 
		unsigned - shl, sar < 4, shl, shr >= 4
		signed - idiv, imul
		unsigned - idiv, imul < 4, imul,div >= 4
		*/

		switch( VarTypeRight )
		{
			case MVT_INT8:	
			case MVT_INT16:
			case MVT_INT32:
			case MVT_PINT8:
			case MVT_PINT16:
			case MVT_PINT32:
				use = MVT_INT32; 
				break;
			case MVT_UINT8:	
			case MVT_UINT16:
			case MVT_UINT32:
			case MVT_PUINT8:	
			case MVT_PUINT16:
			case MVT_PUINT32:
				use = MVT_UINT32; 
				break;
			case MVT_PINT64:
			case MVT_INT64:
				use = MVT_INT64;
				break;
			case MVT_PUINT64:
			case MVT_UINT64:
				use = MVT_UINT64;
				break;
		}

		if( MVAR_TYPE_IS_PTR(VarTypeRight) )
		{
			VarTypeRight = MVAR_PTR_TO_TYPE(VarTypeRight);
		}

		((TD_emulate_operation)CodeStackEmulators[ use ][VarTypeRight])(left, op, right);
	}
}

void mCodeExpressionEmulator::emulate_begin(PVOID result, MVAR_TYPE type, PVOID right)
{
	*(PUINT64)result = 0;

	/*
	byte - 		movzx eax, byte ptr [var]
	char - 		movsx eax, byte ptr [var]
	short - 	movsx eax, word ptr [var]
	ushort - 	movzx eax, word ptr [var]
	int		- 	mov   eax, dword ptr [var]
	uint	-  	mov   eax, dword ptr [var]
	*/

	switch( type )
	{
		case MVT_PINT8:
		case MVT_INT8:	
			*(PINT32)result = *(PINT8)right;  
			break;
		case MVT_PUINT8:
		case MVT_UINT8:	
			*(PUINT32)result = *(PUINT8)right;  
			break;
		case MVT_PINT16:
		case MVT_INT16:	
			*(PINT32)result = *(PINT16)right;  
			break;
		case MVT_PUINT16:
		case MVT_UINT16:
			*(PUINT32)result = *(PUINT16)right;  
			break;
		case MVT_PINT32:
		case MVT_INT32:	
			*(PINT32)result = *(PINT32)right;  
			break;
		case MVT_PUINT32:
		case MVT_UINT32:
			*(PUINT32)result = *(PUINT32)right;  
			break;
		case MVT_PINT64:
		case MVT_INT64:	
			*(PINT64)result = *(PINT64)right;  
			break;
		case MVT_PUINT64:
		case MVT_UINT64:
			*(PUINT64)result = *(PUINT64)right;  
			break;
	}
}

void mCodeExpressionEmulator::emulate_end(PVOID result, MVAR_TYPE type, PVOID right)
{
	*(PUINT64)result = 0;

	/*
	byte - 		mov byte ptr [var], al
	char - 		mov byte ptr [var], al
	short - 	mov word ptr [var], ax
	ushort - 	mov word ptr [var], ax
	int		- 	mov dword ptr [var], eax
	uint	-   mov dword ptr [var], eax
	*/

	switch( type )
	{
		case MVT_INT8:
		case MVT_UINT8:	
		case MVT_PINT8:
		case MVT_PUINT8:
			*(PUINT8)result = *(PUINT8)right;  
			break;
		case MVT_INT16:	
		case MVT_UINT16:
		case MVT_PINT16:	
		case MVT_PUINT16:
			*(PUINT16)result = *(PUINT16)right;  
			break;
		case MVT_PINT32:
		case MVT_PUINT32:
		case MVT_INT32:
		case MVT_UINT32:
			*(PUINT32)result = *(PUINT32)right;  
			break;
		case MVT_PINT64:	
		case MVT_PUINT64:
		case MVT_INT64:	
		case MVT_UINT64:
			*(PUINT64)result = *(PUINT64)right;  
			break;
	}
}


mCodeExpression::mCodeExpression(PMCODE_EXPRESSION	expression)
{
	set(expression);
}

mCodeExpression::mCodeExpression()
{
	set(NULL);
}

void mCodeExpression::clear()
{
	mem_zero(expression,sizeof(MCODE_EXPRESSION));
}

int mCodeExpression::add_var(MVAR_INFO* var)
{
	if( expression->count < CONFIG_MCODE_EXPRESSION_MAX_ARGUMENTS )
	{
		expression->arguments[ expression->count ].type = CET_VAR;
		expression->arguments[ expression->count ].var  = var;

		expression->count++;

		return expression->count - 1;
	}

	return -1;
}

int mCodeExpression::add_var_addr(MVAR_INFO* var)
{
	if( expression->count < CONFIG_MCODE_EXPRESSION_MAX_ARGUMENTS )
	{
		expression->arguments[ expression->count ].type = CET_VAR_ADDR;
		expression->arguments[ expression->count ].var  = var;

		expression->count++;

		return expression->count - 1;
	}

	return -1;
}

int mCodeExpression::add_imm(INT64 imm)
{
	if( expression->count < CONFIG_MCODE_EXPRESSION_MAX_ARGUMENTS )
	{
		expression->arguments[ expression->count ].type = CET_IMM;
		expression->arguments[ expression->count ].imm  = imm;

		expression->count++;

		return expression->count - 1;
	}

	return -1;
}

int mCodeExpression::add_op(CODE_EXPRESSION_OPERATORS  op)
{
	if( expression->count < CONFIG_MCODE_EXPRESSION_MAX_ARGUMENTS )
	{
		expression->arguments[ expression->count ].type = CET_OP;
		expression->arguments[ expression->count ].op  = op;

		expression->count++;

		return expression->count - 1;
	}

	return -1;
}

void mCodeExpression::set(PMCODE_EXPRESSION expression)
{
	this->expression = expression;
}

void mCodeExpression::get_string(mCode &str)
{
	if( expression->count > 1 )
		str << "( ";

	for(int i = 0; i < expression->count; i++)
	{
		switch( expression->arguments[i].type )
		{
			case CET_IMM:
				str.add_digit(16,expression->arguments[i].imm);
				break;
			case CET_VAR:
				if( MVAR_IS_PTR(expression->arguments[i].var) )
				{
					str << "*" << expression->arguments[i].var->name;
				}else{
					str << expression->arguments[i].var->name;
				}
				break;
			case CET_VAR_ADDR:
				str << "&" << expression->arguments[i].var->name;
				break;
			case CET_OP:
				str << op2str(expression->arguments[i].op);
				break;
		}
	}

	if( expression->count > 1 )
		str << " )";
}

PMVAR_INFO mCodeExpression::get_var_addr()
{
	if( expression->count==1 && expression->arguments[0].type==CET_VAR_ADDR )
		return expression->arguments[0].var;

	return NULL;
}

INT64 mCodeExpression::emulate(MVAR_TYPE type)
{
	INT64 result = 0;
	
	for(int i = 0; i < expression->count; )
	{
		if( !i )
		{
			switch( expression->arguments[i].type )
			{
				case CET_IMM:
					mCodeExpressionEmulator::emulate_begin(&result, type, &expression->arguments[i].imm);
					break;
				case CET_VAR:
					mCodeExpressionEmulator::emulate_begin(&result, type, &expression->arguments[i].var->value->i64);
					break;
				case CET_VAR_ADDR:

					break;
			}

			i++;
		}else{
			switch( expression->arguments[i + 1].type )
			{
				case CET_IMM:
					mCodeExpressionEmulator::emulate(&result, expression->arguments[i].op, type, &expression->arguments[i + 1].imm);
					break;
				case CET_VAR:
					mCodeExpressionEmulator::emulate(&result, expression->arguments[i].op, type, &expression->arguments[i + 1].var->value->i64);
					break;
			}

			i += 2;
		}
	}

	return result;
}

#ifdef CONFIG_DEBUG_EMULATOR

void mCodeExpression::get_debug_string(PCHAR string, PMCODE_EXPRESSION expression)
{
	for(int i = 0; i < expression->count; i++)
	{
		switch( expression->arguments[i].type )
		{
		case CET_IMM:
			wsprintfA(&string[lstrlenA(string)], "0x%X%0.8X", (DWORD)(expression->arguments[i].imm>>32),(DWORD)expression->arguments[i].imm);
			break;
		case CET_VAR:
			wsprintfA(&string[lstrlenA(string)], "0x%X%0.8X", (DWORD)(expression->arguments[i].var->value->i64>>32),(DWORD)expression->arguments[i].var->value->i64);
			break;
		case CET_OP:
			lstrcatA(string, op2str(expression->arguments[i].op));
			break;
		}
	}
}

#endif