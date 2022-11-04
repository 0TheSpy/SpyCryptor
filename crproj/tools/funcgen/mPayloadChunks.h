#ifndef __M_PAYLOAD_CHUNKS__
#define __M_PAYLOAD_CHUNKS__

#include "mCode.h"
#include "mRandom.h"

enum MPAYLOAD_STRING_TYPE
{
	PST_ASM,
	PST_CPP,
};

enum OPERATION_SEQUENCE_TYPE
{
	DOT_NONE,

	DOT_ADD,
	DOT_SUB,
	DOT_MUL,
	DOT_DIV,
	DOT_XOR,
	DOT_ROR,
	DOT_ROL,
};

typedef struct _OPERATION_SEQUENCE
{
	OPERATION_SEQUENCE_TYPE type;
	DWORD imm;
}OPERATION_SEQUENCE,*POPERATION_SEQUENCE;

class mPayloadEntropy
{
	DWORD	total;
	DWORD	bytes[256];
public:
	mPayloadEntropy(PVOID data, DWORD data_size);
	mPayloadEntropy();

	void	set_data(PVOID data, DWORD data_size);
	void	add_byte(BYTE data);
	BYTE	get_top_byte();
	double	get();
};

#pragma pack(push,1)

typedef struct _MPAYLOAD_CHUNK
{
	DWORD			offset;
	DWORD			entr_size;
	PVOID			entr_data;
	DWORD			orig_size;
	PVOID			orig_data;
	DWORD			seed;
	DWORD			id;
	double			need_entr;
	BYTE			need_byte;
	mPayloadEntropy	entropy;
	BYTE			use_entr;
	DWORD			user_data;
}MPAYLOAD_CHUNK,*PMPAYLOAD_CHUNK;
#pragma pack(pop)

class mPayloadChunks
{
	DWORD				total;
	MPAYLOAD_CHUNK		chunks[16];
	DWORD				chunk_ops_count;
	OPERATION_SEQUENCE	chunk_ops[5];
	DWORD				entropy_ops_count;
	OPERATION_SEQUENCE	entropy_ops[5];
public:
	mPayloadChunks();
	~mPayloadChunks();

	void			clear();
	DWORD			add_chunk(DWORD id, DWORD offset, PVOID data, DWORD size, BYTE need_byte, double need_entr,BOOL use_entr);
	void			get_string(mCode& string, MPAYLOAD_STRING_TYPE type, DWORD chunk_id);
	PMPAYLOAD_CHUNK	get_chunk(DWORD chunk_id);
	PMPAYLOAD_CHUNK	get_chunks();
	DWORD			get_count();
	DWORD			get_max_entr_size();
	DWORD			get_first_seed();
	void			prepare();

	DWORD			get_entr_buffer_size(PVOID orig_data, DWORD orig_size, DWORD entr_hash, mPayloadEntropy* entropy, BYTE need_byte, double need_entr);
	void			get_entr_buffer(PBYTE entr_data, DWORD entr_size, PBYTE orig_data, DWORD orig_size, DWORD entr_hash,mPayloadEntropy* entropy, BYTE need_byte, double need_entr);

	POPERATION_SEQUENCE get_entr_ops();
	DWORD				get_entr_ops_count();
	POPERATION_SEQUENCE get_chunk_ops();
	DWORD				get_chunk_ops_count();

	static DWORD	create_operations(POPERATION_SEQUENCE operations, DWORD min, DWORD max, BOOL is_hash);
	static DWORD	eval_operations(POPERATION_SEQUENCE operations, DWORD count, DWORD hresult);
	static void		operations2str(PCHAR dest, PCHAR name, POPERATION_SEQUENCE operations, DWORD count);
};

#endif