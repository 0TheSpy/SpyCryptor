#include <windows.h>
#include <tchar.h>
#include <intrin.h>
#include <stdio.h>
#include <math.h>
#include <conio.h>

#include "globals.h"
#include "funcs.h"
#include "mPayloadChunks.h"

mPayloadEntropy::mPayloadEntropy()
{
	total = 0;
	mem_zero(bytes,sizeof(bytes));
}

mPayloadEntropy::mPayloadEntropy(PVOID data, DWORD data_size)
{
	set_data(data, data_size);
}

void mPayloadEntropy::set_data(PVOID data, DWORD data_size)
{
	total = data_size;
	mem_zero(bytes,sizeof(bytes));

	for(int i = 0; i < data_size; i++)
	{
		bytes[ *((PBYTE)data + i) ]++;
	}
}

void mPayloadEntropy::add_byte(BYTE data)
{
	bytes[ data ]++;
	total++;
}

BYTE mPayloadEntropy::get_top_byte()
{
	BYTE id = 1;
	// skip 0xFF and 0x00
	for(int i = 2; i < 255; i++)
	{
		if( bytes[id] < bytes[i] )
		{
			id = i;
		}
	}
	return id; // was return id;
}

double mPayloadEntropy::get()
{
	double entr = 0;

	if( total )
	{
		double ln2 = log(2.0);
		for(int i = 0; i < 256; i++)
		{
			double probability = (double)bytes[i]/(double)total; 
			if( probability )
				entr += -probability * log(probability) / ln2;
		}
	}

	return entr;
}

mPayloadChunks::mPayloadChunks()
{
	total = 0;
}

mPayloadChunks::~mPayloadChunks()
{
	clear();
}

void mPayloadChunks::clear()
{
	total = 0;
}

POPERATION_SEQUENCE mPayloadChunks::get_entr_ops()
{
	return entropy_ops;
}

DWORD mPayloadChunks::get_entr_ops_count()
{
	return entropy_ops_count;
}

POPERATION_SEQUENCE mPayloadChunks::get_chunk_ops()
{
	return chunk_ops;
}

DWORD mPayloadChunks::get_chunk_ops_count()
{
	return chunk_ops_count;
}

void mPayloadChunks::get_string(mCode& string, MPAYLOAD_STRING_TYPE type, DWORD chunk_id)
{
	PMPAYLOAD_CHUNK current = get_chunk(chunk_id);

	if( current )
	{
		PBYTE buffer = (PBYTE)current->entr_data;
		if( type==PST_CPP )
		{
			for(int i = 0, p = 0; i < current->entr_size; i++)
			{
				if( p==16 )
				{
					string << "\r\n";
					p = 0;
				}

				string.add_digit(16, *buffer);
				string << ", ";

				buffer++;
				p++;
			}
		}else{
			for(int i = 0; i < current->entr_size; i++)
			{
				if( i%16==0 )
				{
					if( i )
						string << "\r\ndb ";
					else
						string << "db ";
				}else{
					string << ", ";
				}

				string.add_digit(10, *buffer);

				buffer++;
			}
		}

		string << "\r\n";
	}
}

PMPAYLOAD_CHUNK mPayloadChunks::get_chunks()
{
	return chunks;
}

PMPAYLOAD_CHUNK mPayloadChunks::get_chunk(DWORD chunk_id)
{
	for(int i = 0; i < total; i++)
	{
		if( chunks[i].id == chunk_id )
			return &chunks[i];
	}
	return NULL;
}

DWORD mPayloadChunks::add_chunk(DWORD id, DWORD offset, PVOID data, DWORD size, BYTE need_byte, double need_entr, BOOL use_entr)
{
	if( total < sizeof(chunks)/sizeof(MPAYLOAD_CHUNK) )
	{
		if( data && size )
		{
			PMPAYLOAD_CHUNK new_chunk = &chunks[total];

			mem_zero(new_chunk, sizeof(MPAYLOAD_CHUNK));

			new_chunk->offset    = offset;
			new_chunk->orig_size = size;
			new_chunk->orig_data = data;
			new_chunk->id		 = id;
			new_chunk->need_byte = need_byte;
			new_chunk->need_entr = need_entr;
			new_chunk->use_entr  = use_entr;

			total++;

			return offset + size;
		}
	}

	return 0;
}

DWORD mPayloadChunks::get_entr_buffer_size(PVOID orig_data, DWORD orig_size, DWORD entr_hash, mPayloadEntropy* entropy, BYTE need_byte, double need_entr)
{
	entropy->set_data(orig_data, orig_size);

	DWORD count = 0;
	double r = entropy->get();

	if( r > need_entr )
	{
		// разбавляем энтропию
		while( (r = entropy->get()) > need_entr )
		{
			entropy->add_byte(need_byte);
			count++;
		}
	}else{
		// увеличиваем энтропию
		while( (r = entropy->get()) < need_entr )
		{
			entropy->add_byte( random.get_less(1,255) );
			count++;
		}
	}

	return orig_size + count;
}

void mPayloadChunks::get_entr_buffer(PBYTE entr_data, DWORD entr_size, PBYTE orig_data, DWORD orig_size, DWORD entr_hash,mPayloadEntropy* entropy, BYTE need_byte, double need_entr)
{
	PBYTE pdata = (PBYTE)halloc(entr_size);
	DWORD pos;

	mem_zero(pdata, entr_size);

	DWORD max_step = entr_size/(entr_size - orig_size);
	DWORD offset = 0;
	for(int i = orig_size; i < entr_size; )
	{
		pos = entr_hash%max_step;

		pdata[offset + pos] = 1;
		i++;

		offset += max_step;

		entr_hash = eval_operations(entropy_ops, entropy_ops_count, entr_hash);
	}

	for(int i = 0, j = 0; i < entr_size; i++)
	{
		if( pdata[i]==0 )
		{
			entr_data[i] = orig_data[j++];
#ifdef _DEBUG
			if( j > orig_size )
				__debugbreak();
#endif
		}else{
			entr_data[i] = need_byte;
		}
	}

	hfree(pdata);
}

void mPayloadChunks::prepare()
{
	if( total > 1 )
	{
		for(int i = 0; i < total >> 1; i++)
		{
			DWORD a;

			do 
			{
				a = random.get_less(0, total);
			} while (a==i);


			MPAYLOAD_CHUNK tmp;
			
			tmp		  = chunks[i];
			chunks[i] = chunks[a];
			chunks[a] = tmp;
		}
	}

	DWORD start = 0;
	for(int i = 0; i < 8; i++)
	{
		start <<= 4;
		start |= random.get_less(1,0xF);
	}

	chunk_ops_count = create_operations(chunk_ops, 2, 3, TRUE);
	entropy_ops_count = create_operations(entropy_ops, 2, 3, TRUE);

	for(int i = 0; i < total; i++)
	{
		chunks[i].seed = start;

		if( chunks[i].use_entr )
		{
			chunks[i].entr_size = get_entr_buffer_size(chunks[i].orig_data, chunks[i].orig_size, start, &chunks[i].entropy, chunks[i].need_byte, chunks[i].need_entr);
		}else{
			chunks[i].entr_size = chunks[i].orig_size;
		}

		chunks[i].entr_data = halloc(chunks[i].entr_size);
		if( !chunks[i].entr_data )
		{
#ifdef _DEBUG
			__debugbreak();
#endif
			return;
		}

		if( chunks[i].use_entr )
		{
			get_entr_buffer((PBYTE)chunks[i].entr_data, chunks[i].entr_size, (PBYTE)chunks[i].orig_data, chunks[i].orig_size, start, &chunks[i].entropy, chunks[i].need_byte, chunks[i].need_entr);
		}else{
			mem_copy(chunks[i].entr_data, chunks[i].orig_data, chunks[i].entr_size);
		}

		start = eval_operations(chunk_ops, chunk_ops_count, start);
	}
}
DWORD mPayloadChunks::get_count()
{
	return total;
}
DWORD mPayloadChunks::eval_operations(POPERATION_SEQUENCE operations, DWORD count, DWORD hresult)
{
	for(int j = 0; j < count; j++)
	{
		switch( operations[j].type )
		{
			case DOT_ADD: hresult += operations[j].imm; break;
			case DOT_SUB: hresult -= operations[j].imm; break;
			case DOT_MUL: hresult *= operations[j].imm; break;
			case DOT_DIV: hresult /= operations[j].imm; break;
			case DOT_ROR: hresult = _rotr(hresult, operations[j].imm); break;
			case DOT_ROL: hresult = _rotl(hresult, operations[j].imm); break;
			case DOT_XOR: hresult ^= operations[j].imm; break;
		}
	}
	return hresult;
}

void mPayloadChunks::operations2str(PCHAR dest, PCHAR name, POPERATION_SEQUENCE operations, DWORD count)
{
	CHAR format[128];
	CHAR string[128];
	CHAR string2[128];

	wsprintfA(format, "%s = %%s;\r\n", name);

	for(int i = count - 1; i >=0 ; i--)
	{
		switch( operations[i].type )
		{
		case DOT_ADD: wsprintfA(string, "(%%s + 0x%0.8X)", operations[i].imm); break;
		case DOT_SUB: wsprintfA(string, "(%%s - 0x%0.8X)", operations[i].imm); break;
		case DOT_MUL: wsprintfA(string, "(%%s * 0x%0.8X)", operations[i].imm); break;
		case DOT_DIV: wsprintfA(string, "(%%s / 0x%0.8X)", operations[i].imm); break;
		case DOT_ROR: wsprintfA(string, "_rotr(%%s,0x%0.8X)", operations[i].imm); break;
		case DOT_ROL: wsprintfA(string, "_rotl(%%s,0x%0.8X)", operations[i].imm); break;
		case DOT_XOR: wsprintfA(string, "(%%s ^ 0x%0.8X)", operations[i].imm); break;
		}
		wsprintfA(string2, format, string);
		lstrcpyA(format, string2);
	}

	wsprintfA(dest, format, name);
}

DWORD mPayloadChunks::get_max_entr_size()
{
	DWORD max = 0;

	for(int i = 0; i < total; i++)
	{
		if( max < chunks[i].entr_size )
			max = chunks[i].entr_size;
	}

	return max;
}

DWORD mPayloadChunks::get_first_seed()
{
	if( total )
	{
		return chunks[0].seed;
	}
	return 0;
}

DWORD mPayloadChunks::create_operations(POPERATION_SEQUENCE operations, DWORD min, DWORD max, BOOL is_hash)
{
	DWORD count = random.get_less(min,max);

	DWORD	imm;

	OPERATION_SEQUENCE_TYPE hash_ops[4][2] = {
		{DOT_ADD,DOT_SUB},
		{DOT_ROR,DOT_ROL},
		{DOT_MUL,DOT_MUL}, // do not use DIV, it can create 0 with hash
	};

	OPERATION_SEQUENCE_TYPE decr_ops[4][3] = {
		{DOT_ADD,DOT_SUB,DOT_XOR}, //bugfix: byte ^ hash - hash == byte, thats because xor is in one group add,sub
		{DOT_ROR,DOT_ROL,DOT_ROR},
	};

	INT group, last_group = -1;

	for(int i = 0; i < count; i++)
	{
		do 
		{
			if( is_hash )
			{
				group = random.get_equal(0,2);
			}else{
				group = random.get_equal(0,1);
			}
		} while ( group==last_group );

		last_group = group;

		if( is_hash )
		{
			operations[i].type = hash_ops[group][ random.get_equal(0,1) ];
		}else{
			if( group==0 )
			{
				operations[i].type = decr_ops[group][ random.get_equal(0,2) ];
			}else{
				operations[i].type = decr_ops[group][ random.get_equal(0,1) ];
			}
		}

		switch( operations[i].type )
		{
		case DOT_ADD: // add
		case DOT_SUB: // sub
		case DOT_MUL:
		case DOT_DIV:
			if( is_hash )
			{
				operations[i].imm  = random.get_less(1,0xFFFF);
				if( operations[i].imm%2==0 ) //bugfix: stobi ne bilo vsegda kruglo cislo inace posle hash odin i totze
					operations[i].imm++;
			}
			break;
		case DOT_XOR:
			if( is_hash )
			{
				operations[i].imm  = random.get_less(0xFFFF,0xFFFFFFFF);
				if( operations[i].imm%2==0 ) //bugfix: stobi ne bilo vsegda kruglo cislo inace posle hash odin i totze
					operations[i].imm++;
			}
			break;
		case DOT_ROL:
		case DOT_ROR:
			if( is_hash )
			{
				operations[i].imm = random.get_less(1,31);
				if( operations[i].imm%2==0 ) 
					operations[i].imm++;
			}
			break;
		}
	}

	return count;
}
