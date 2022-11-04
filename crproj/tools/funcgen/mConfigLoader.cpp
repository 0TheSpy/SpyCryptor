#include <windows.h>
#include <tchar.h>
#include <intrin.h>

#include "mConfigLoader.h"
#include "mVars.h"
#include "funcs.h"

MCONFIG_OPTION options[] = {
	{"config.not_use_crt_stub",FIELD_OFFSET(mConfig,not_use_crt_stub),COT_DIGIT8},
	{"config.code.blocks.count.min",FIELD_OFFSET(mConfig,code.blocks.count.min),COT_DIGIT32},
	{"config.code.blocks.count.max",FIELD_OFFSET(mConfig,code.blocks.count.max),COT_DIGIT32},
	{"config.vars.decryptor.trash_between.min",FIELD_OFFSET(mConfig,vars.decryptor.trash_between.min),COT_DIGIT32},
	{"config.vars.decryptor.trash_between.max",FIELD_OFFSET(mConfig,vars.decryptor.trash_between.max),COT_DIGIT32},
	{"config.shellcode.trash_after.min",FIELD_OFFSET(mConfig,shellcode.trash_after.min),COT_DIGIT32},
	{"config.shellcode.trash_after.max",FIELD_OFFSET(mConfig,shellcode.trash_after.max),COT_DIGIT32},
	{"config.shellcode.trash_before.min",FIELD_OFFSET(mConfig,shellcode.trash_before.min),COT_DIGIT32},
	{"config.shellcode.trash_before.max",FIELD_OFFSET(mConfig,shellcode.trash_before.max),COT_DIGIT32},
	{"config.code.block_loop.max_levels",FIELD_OFFSET(mConfig,code.block_loop.max_levels),COT_DIGIT8},
	{"config.code.block_loop.interations.min",FIELD_OFFSET(mConfig,code.block_loop.interations.min),COT_DIGIT32},
	{"config.code.block_loop.interations.max",FIELD_OFFSET(mConfig,code.block_loop.interations.max),COT_DIGIT32},
	{"config.code.block_loop.inner_blocks.min",FIELD_OFFSET(mConfig,code.block_loop.inner_blocks.min),COT_DIGIT32},
	{"config.code.block_loop.inner_blocks.max",FIELD_OFFSET(mConfig,code.block_loop.inner_blocks.max),COT_DIGIT32},
	{"config.code.block_if.inner_blocks.min",FIELD_OFFSET(mConfig,code.block_if.inner_blocks.min),COT_DIGIT32},
	{"config.code.block_if.inner_blocks.max",FIELD_OFFSET(mConfig,code.block_if.inner_blocks.max),COT_DIGIT32},
	{"config.code.block_if.max_levels",FIELD_OFFSET(mConfig,code.block_if.max_levels),COT_DIGIT8},
	{"config.image.extend.min",FIELD_OFFSET(mConfig,image.extend.min),COT_DIGIT32},
	{"config.image.extend.max",FIELD_OFFSET(mConfig,image.extend.max),COT_DIGIT32},
	{"config.image.rdata",FIELD_OFFSET(mConfig,image.rdata),COT_DIGIT8},
	{"config.image.data",FIELD_OFFSET(mConfig,image.data),COT_DIGIT8},
	{"config.code.usage.papi",FIELD_OFFSET(mConfig,code.usage.api),COT_DIGIT8},
	{"config.code.usage.pcode",FIELD_OFFSET(mConfig,code.usage.code),COT_DIGIT8},
	{"config.code.usage.ploop",FIELD_OFFSET(mConfig,code.usage.loop),COT_DIGIT8},
	{"config.code.usage.pif",FIELD_OFFSET(mConfig,code.usage._if),COT_DIGIT8},
	{"config.code.block_code.add",FIELD_OFFSET(mConfig,code.block_code.add),COT_DIGIT8},
	{"config.code.block_code.sub",FIELD_OFFSET(mConfig,code.block_code.sub),COT_DIGIT8},
	{"config.code.block_code.mul",FIELD_OFFSET(mConfig,code.block_code.mul),COT_DIGIT8},
	{"config.code.block_code.div",FIELD_OFFSET(mConfig,code.block_code.div),COT_DIGIT8},
	{"config.code.block_code.mod",FIELD_OFFSET(mConfig,code.block_code.mod),COT_DIGIT8},
	{"config.code.block_code.shl",FIELD_OFFSET(mConfig,code.block_code.shl),COT_DIGIT8},
	{"config.code.block_code.shr",FIELD_OFFSET(mConfig,code.block_code.shr),COT_DIGIT8},
	{"config.code.block_code.and",FIELD_OFFSET(mConfig,code.block_code.and),COT_DIGIT8},
	{"config.code.block_code.xor",FIELD_OFFSET(mConfig,code.block_code.xor),COT_DIGIT8},
	{"config.code.block_code.or",FIELD_OFFSET(mConfig,code.block_code.or),COT_DIGIT8},
	{"config.vars.usage_left.globals",FIELD_OFFSET(mConfig,vars.usage_left.globals),COT_DIGIT8},
	{"config.vars.usage_left.locals",FIELD_OFFSET(mConfig,vars.usage_left.locals),COT_DIGIT8},
	{"config.vars.usage_left.formals",FIELD_OFFSET(mConfig,vars.usage_left.formals),COT_DIGIT8},
	{"config.vars.usage_right.globals",FIELD_OFFSET(mConfig,vars.usage_right.globals),COT_DIGIT8},
	{"config.vars.usage_right.locals",FIELD_OFFSET(mConfig,vars.usage_right.locals),COT_DIGIT8},
	{"config.vars.usage_right.formals",FIELD_OFFSET(mConfig,vars.usage_right.formals),COT_DIGIT8},
	{"config.vars.usage_api.globals",FIELD_OFFSET(mConfig,vars.usage_api.globals),COT_DIGIT8},
	{"config.vars.usage_api.locals",FIELD_OFFSET(mConfig,vars.usage_api.locals),COT_DIGIT8},
	{"config.vars.types.int8",FIELD_OFFSET(mConfig,vars.types[MVT_INT8]),COT_DIGIT8},
	{"config.vars.types.int16",FIELD_OFFSET(mConfig,vars.types[MVT_INT16]),COT_DIGIT8},
	{"config.vars.types.int32",FIELD_OFFSET(mConfig,vars.types[MVT_INT32]),COT_DIGIT8},
	{"config.vars.types.int64",FIELD_OFFSET(mConfig,vars.types[MVT_INT64]),COT_DIGIT8},
	{"config.vars.types.uint8",FIELD_OFFSET(mConfig,vars.types[MVT_UINT8]),COT_DIGIT8},
	{"config.vars.types.uint16",FIELD_OFFSET(mConfig,vars.types[MVT_UINT16]),COT_DIGIT8},
	{"config.vars.types.uint32",FIELD_OFFSET(mConfig,vars.types[MVT_UINT32]),COT_DIGIT8},
	{"config.vars.types.uint64",FIELD_OFFSET(mConfig,vars.types[MVT_UINT64]),COT_DIGIT8},
	{"config.vars.getoradd.code.globals.add",FIELD_OFFSET(mConfig,vars.getoradd.code.globals.add),COT_DIGIT8},
	{"config.vars.getoradd.code.globals.get",FIELD_OFFSET(mConfig,vars.getoradd.code.globals.get),COT_DIGIT8},
	{"config.vars.getoradd.code.locals.add",FIELD_OFFSET(mConfig,vars.getoradd.code.locals.add),COT_DIGIT8},
	{"config.vars.getoradd.code.locals.get",FIELD_OFFSET(mConfig,vars.getoradd.code.locals.get),COT_DIGIT8},
	{"config.vars.getoradd.api.globals.add",FIELD_OFFSET(mConfig,vars.getoradd.api.globals.add),COT_DIGIT8},
	{"config.vars.getoradd.api.globals.get",FIELD_OFFSET(mConfig,vars.getoradd.api.globals.get),COT_DIGIT8},
	{"config.vars.getoradd.api.locals.add",FIELD_OFFSET(mConfig,vars.getoradd.api.locals.add),COT_DIGIT8},
	{"config.vars.getoradd.api.locals.get",FIELD_OFFSET(mConfig,vars.getoradd.api.locals.get),COT_DIGIT8},
	{"config.vars.initialization.globals.yes",FIELD_OFFSET(mConfig,vars.initialization.globals.yes),COT_DIGIT8},
	{"config.vars.initialization.globals.no",FIELD_OFFSET(mConfig,vars.initialization.globals.no),COT_DIGIT8},
	{"config.vars.initialization.locals.yes",FIELD_OFFSET(mConfig,vars.initialization.locals.yes),COT_DIGIT8},
	{"config.vars.initialization.locals.no",FIELD_OFFSET(mConfig,vars.initialization.locals.no),COT_DIGIT8},
	{"config.vars.initialization.types.t_int8.min",FIELD_OFFSET(mConfig,vars.initialization.types.t_int8.min),COT_DIGIT32},
	{"config.vars.initialization.types.t_int8.max",FIELD_OFFSET(mConfig,vars.initialization.types.t_int8.max),COT_DIGIT32},
	{"config.vars.initialization.types.t_int16.min",FIELD_OFFSET(mConfig,vars.initialization.types.t_int16.min),COT_DIGIT32},
	{"config.vars.initialization.types.t_int16.max",FIELD_OFFSET(mConfig,vars.initialization.types.t_int16.max),COT_DIGIT32},
	{"config.vars.initialization.types.t_int32.min",FIELD_OFFSET(mConfig,vars.initialization.types.t_int32.min),COT_DIGIT32},
	{"config.vars.initialization.types.t_int32.max",FIELD_OFFSET(mConfig,vars.initialization.types.t_int32.max),COT_DIGIT32},
	{"config.vars.initialization.types.t_int64.min",FIELD_OFFSET(mConfig,vars.initialization.types.t_int64.min),COT_DIGIT64},
	{"config.vars.initialization.types.t_int64.max",FIELD_OFFSET(mConfig,vars.initialization.types.t_int64.max),COT_DIGIT64},
	{"config.vars.initialization.types.t_uint8.min",FIELD_OFFSET(mConfig,vars.initialization.types.t_uint8.min),COT_DIGIT32},
	{"config.vars.initialization.types.t_uint8.max",FIELD_OFFSET(mConfig,vars.initialization.types.t_uint8.max),COT_DIGIT32},
	{"config.vars.initialization.types.t_uint16.min",FIELD_OFFSET(mConfig,vars.initialization.types.t_uint16.min),COT_DIGIT32},
	{"config.vars.initialization.types.t_uint16.max",FIELD_OFFSET(mConfig,vars.initialization.types.t_uint16.max),COT_DIGIT32},
	{"config.vars.initialization.types.t_uint32.min",FIELD_OFFSET(mConfig,vars.initialization.types.t_uint32.min),COT_DIGIT32},
	{"config.vars.initialization.types.t_uint32.max",FIELD_OFFSET(mConfig,vars.initialization.types.t_uint32.max),COT_DIGIT32},
	{"config.vars.initialization.types.t_uint64.min",FIELD_OFFSET(mConfig,vars.initialization.types.t_uint64.min),COT_DIGIT64},
	{"config.vars.initialization.types.t_uint64.max",FIELD_OFFSET(mConfig,vars.initialization.types.t_uint64.max),COT_DIGIT64},
	{"config.vars.path.length.min",FIELD_OFFSET(mConfig,vars.path.length.min),COT_DIGIT32},
	{"config.vars.path.length.max",FIELD_OFFSET(mConfig,vars.path.length.max),COT_DIGIT32},
};

mConfigLoader::mConfigLoader(mConfig* config, PIMAGE_DOS_HEADER dos)
{
	this->config = config;

	mem_zero(config,sizeof(mConfig));
	*error = 0;

	nt = (PIMAGE_NT_HEADERS)((DWORD_PTR)dos + dos->e_lfanew);
}

PTCHAR mConfigLoader::get_error()
{
	return error;
}

PCHAR mConfigLoader::skip_spaces(PCHAR current, PCHAR end)
{
	while( current < end && *current <= ' ' )
	{
		if( *current=='\n' )
			line_number++;

		current++;
	}

	return current;
}

PCHAR mConfigLoader::get_digit(PCHAR current, PCHAR end, PUINT64 value)
{
	UINT64	result = 0;

	if( current < end )
	{
		BYTE	base = 10;

		if( *current=='0' )
		{
			if( current + 1 < end && *(current + 1)=='x' )
			{
				base = 16;
				current = current + 2;
			}
		}

		if( base==16 )
		{
			while( current < end )
			{
				if( *current >= '0' && *current<='9' )
				{
					result <<= 4;
					result |= (*current - '0');
				}else if( *current >= 'a' && *current<='f' )
				{
					result <<= 4;
					result |= 0xA + (*current - 'a');
				}else if( *current >= 'A' && *current<='F' )
				{
					result <<= 4;
					result |= 0xA + (*current - 'A');
				}else{
					break;
				}

				current++;
			}
		}else if( base==10 )
		{
			CHAR sign = '+';
			if( current < end && (*current=='-' || *current=='+') )
			{
				sign = *current++;
			}

			while( current < end )
			{
				if( *current >= '0' && *current<='9' )
				{
					result = result*10 + (*current - '0');
				}else{
					break;
				}

				current++;
			}

			if( sign=='-' )
				result = -result;
		}
	}

	if( value )
	{
		*value = result;
	}

	return current;
}

/*
bool mConfigLoader::load_from_file(PTCHAR file_path)
{
	*error = 0;

	PMBUF file = file_get_contents(file_path);
	if( file && file->size)
	{
		PCHAR current = (PCHAR)file->data;
		PCHAR end = (PCHAR)(file->data + file->size);

		CHAR option_name[MAX_PATH];
		INT  i;

		line_number = 1;
		//__debugbreak();
		while( current < end )
		{
			current = skip_spaces(current, end);

			// check comment
			if( *current=='#' || (current + 1 < end && *current=='/' && *(current + 1)=='/') )
			{
				while( current < end && *current!='\n' )
				{
					current++;
				}

				current++;
				line_number++;

				continue;
			}

			if( current >= end )
				break;

			// allow all, but not \r \n \t space
			for(i = 0; i < MAX_PATH - 1 && current < end; i++)
			{
				if( *current <= ' ' )
					break;

				option_name[i] = *current;
				current++;
			}

			option_name[i] = 0;

			if( *option_name==0 )
			{
				wsprintf(error, _T("Option name is empty on line %d!"), line_number);
				break;
			}

#ifdef _DEBUG
			CHAR info[256];

			wsprintfA(info,"line: %d, option: %s\n", line_number, option_name);

			OutputDebugStringA(info);
#endif

			current = skip_spaces(current, end);
			if( current < end && *current == '=' )
			{
				current++;

				current = skip_spaces(current, end);

				UINT64 value;
				current = get_digit(current, end, &value);

				if( current < end )
				{
					BOOL is_finded = FALSE;

					for(int i = 0; i < sizeof(options)/sizeof(MCONFIG_OPTION); i++)
					{
						if( !lstrcmpA(options[i].name,option_name) )
						{
							PBYTE on_offset = (PBYTE)config + options[i].offset;

							is_finded = true;

							switch( options[i].type )
							{
								case COT_DIGIT8:
									*(PBYTE)on_offset = (BYTE)value;
									break;
								case COT_DIGIT32:
									*(PDWORD)on_offset = (DWORD)value;
									break;
								case COT_DIGIT64:
									*(PDWORD64)on_offset = (DWORD64)value;
									break;
							}
							break;
						}
					}

					if( !is_finded )
					{
						wsprintf(error, _T("Option name \"%s\" invalid!"), option_name);
						break;
					}

					current = skip_spaces(current, end);
					if( current < end && *current==';' )
					{
						current++;
					}else{
						wsprintf(error, _T("Can not find \";\" on line %d!"), line_number);
						break;
					}	
				}else{
					wsprintf(error, _T("No end delimiter (\";\") on line %d!"), line_number);
					break;
				}
			}else{
				wsprintf(error, _T("Can not find \"=\" on line %d!"), line_number);
				break;
			}
		}
	}else{
		lstrcpy(error, _T("Can not open config file or file is empty!"));
	}

	if( *error==0 )
		return true;

	return false;
}
*/

bool mConfigLoader::load_default()
{
	config->not_use_crt_stub = false;
	// max blocks count range
	config->code.blocks.count.min = 5000; 
	config->code.blocks.count.max = 20000;

	config->vars.decryptor.trash_between.min = 2;
	config->vars.decryptor.trash_between.max = 6;

	config->shellcode.trash_after.min = 5;
	config->shellcode.trash_after.max = 15;
	config->shellcode.trash_before.min = 3;
	config->shellcode.trash_before.max = 10;

	config->code.block_call.parameters.min = 0;
	config->code.block_call.parameters.max = 5;
	config->code.block_call.inner_blocks.min = 2;
	config->code.block_call.inner_blocks.max = 100;
	config->code.block_call.count.min = 5;
	config->code.block_call.count.max = 15;

	config->code.block_loop.max_levels = 30;
	config->code.block_loop.interations.min = 10;
	config->code.block_loop.interations.max = 100;
	config->code.block_loop.inner_blocks.min = 10;
	config->code.block_loop.inner_blocks.max = 50;

	config->code.block_if.inner_blocks.min = 3;
	config->code.block_if.inner_blocks.max = 10;
	config->code.block_if.trash_blocks.min = 1;
	config->code.block_if.trash_blocks.max = 10;
	config->code.block_if.max_levels = 30;
	config->code.block_if.chains.min = 1;
	config->code.block_if.chains.max = 3;

	config->code.block_switch.inner_blocks.min = 3;
	config->code.block_switch.inner_blocks.max = 10;
	config->code.block_switch.trash_blocks.min = 1;
	config->code.block_switch.trash_blocks.max = 10;
	config->code.block_switch.max_levels = 30;
	config->code.block_switch.chains.min = 2;
	config->code.block_switch.chains.max = 3;

	// if crypted_sizeofimage < orig_sizeofimage then add fake_data + fake_rdata + config->image.extend
	config->image.extend.min = 0;
	config->image.extend.max = 0;
	config->image.rdata = 40;
	config->image.data  = 60;

	// blocks percentage
	config->code.usage.api = 3;
	config->code.usage.code = 35;
	config->code.usage.ptrcode = 15;
	config->code.usage.loop = 5;
	config->code.usage._if = 10;
	config->code.usage._switch = 5;
	config->code.usage.proc = 20;

	/*config->code.usage.api = 50;
	config->code.usage.code = 50;
	config->code.usage.ptrcode = 0;
	config->code.usage.loop = 0;
	config->code.usage._if = 0;
	config->code.usage._switch = 0;
	config->code.usage.proc = 0;*/

	/*config->code.blocks.count.min = 200; 
	config->code.blocks.count.max = 200;

	config->code.usage.api = 0;
	config->code.usage.code = 50;
	config->code.usage.ptrcode = 0;
	config->code.usage.loop = 0;
	config->code.usage._if = 0;
	config->code.usage._switch = 0;
	config->code.usage.proc = 50;

	config->code.block_loop.interations.min = 3;
	config->code.block_loop.interations.max = 5;
	config->code.block_loop.inner_blocks.min = 1;
	config->code.block_loop.inner_blocks.max = 5;*/

	config->code.block_code.add = 30;
	config->code.block_code.sub = 30;
	config->code.block_code.mul = 10;
	config->code.block_code.div = 5;
	config->code.block_code.mod = 0;  // MUSTFIX: DO NOT USE, EMULATION ERRORS
	config->code.block_code.shl = 5;
	config->code.block_code.shr = 5;
	config->code.block_code.and = 10;
	config->code.block_code.xor = 5;
	config->code.block_code.or  = 5;

	config->import.dlls.min = 1;
	config->import.dlls.max = 1;
	config->import.apis.min = 5;
	config->import.apis.max = 30;

	config->import.trash_dlls.min = 1;
	config->import.trash_dlls.max = 3;
	config->import.trash_apis.min = 1;
	config->import.trash_apis.max = 10;

	// which types of variables will be created
	config->vars.usage_left.globals = 10; // ќ  ”–ј“Ќќ!!! ав не люб€т когда глобалс используют с операци€ми только запись
	config->vars.usage_left.locals  = 70;
	config->vars.usage_left.formals = 20;

	config->vars.usage_right.globals = 0; // не включать, ав не люб€т когда глобалс используют с операци€ми только запись
	config->vars.usage_right.locals  = 80;
	config->vars.usage_right.formals = 20;

	config->vars.usage_api.locals = 100; 
	config->vars.usage_api.globals = 0; // не включать, ав не люб€т когда глобалс используют с операци€ми только запись

	// which types of variables will be generated BYTE, WORD, DWORD, DWORD64 ...
	if( nt->FileHeader.Machine==IMAGE_FILE_MACHINE_I386 )
	{
		config->vars.types[MVT_INT8]	= 5;
		config->vars.types[MVT_INT16]   = 5;
		config->vars.types[MVT_INT32]   = 50;
		config->vars.types[MVT_INT64]   = 0;
		config->vars.types[MVT_UINT8]	= 15;
		config->vars.types[MVT_UINT16]  = 15;
		config->vars.types[MVT_UINT32]  = 20;
		config->vars.types[MVT_UINT64]  = 0;
	}else{
		config->vars.types[MVT_INT8]	= 15;
		config->vars.types[MVT_INT16]   = 15;
		config->vars.types[MVT_INT32]   = 10;
		config->vars.types[MVT_INT64]   = 10;
		config->vars.types[MVT_UINT8]	= 15;
		config->vars.types[MVT_UINT16]  = 15;
		config->vars.types[MVT_UINT32]  = 10;
		config->vars.types[MVT_UINT64]  = 10;
	}

	// vars get or add variable, if get fails with type then auto add
	config->vars.getoradd.code.globals.add = 50;
	config->vars.getoradd.code.globals.get = 50;

	config->vars.getoradd.code.locals.add = 40;
	config->vars.getoradd.code.locals.get = 60;

	config->vars.getoradd.api.globals.add = 20;
	config->vars.getoradd.api.globals.get = 80;

	config->vars.getoradd.api.locals.add = 20;
	config->vars.getoradd.api.locals.get = 80;

	// initialize globals
	config->vars.initialization.globals.yes = 70;
	config->vars.initialization.globals.no = 30;

	// initialize locals
	config->vars.initialization.locals.yes = 0;
	config->vars.initialization.locals.no = 100;

	// initialization value ranges, may be for entropy
	config->vars.initialization.types.t_int8.min = 0;
	config->vars.initialization.types.t_int8.max = 0x7F; // max 0x7F
	config->vars.initialization.types.t_int16.min = 0;
	config->vars.initialization.types.t_int16.max = 0x7FF; // max 0x7FFF
	config->vars.initialization.types.t_int32.min = 0;
	config->vars.initialization.types.t_int32.max = 0x7FFF; // max 0x7FFFFFFF
	config->vars.initialization.types.t_int64.min = 0x70000000000000;
	config->vars.initialization.types.t_int64.max = 0x7F000000000000; // max 0x7FFFFFFFFFFFFFFF

	config->vars.initialization.types.t_uint8.min = 0;
	config->vars.initialization.types.t_uint8.max = 0xFF;
	config->vars.initialization.types.t_uint16.min = 0;
	config->vars.initialization.types.t_uint16.max = 0xFFF;
	config->vars.initialization.types.t_uint32.min = 0;
	config->vars.initialization.types.t_uint32.max = 0xFFFF;
	config->vars.initialization.types.t_uint64.min = 0x70000000000000;
	config->vars.initialization.types.t_uint64.max = 0x7F000000000000;

	config->vars.path.length.min = 2;
	config->vars.path.length.max = 6;

	config->string_array_size.min = 16;
	config->string_array_size.max = 64;

	return true;
}