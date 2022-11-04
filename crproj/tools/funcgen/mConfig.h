#ifndef _M_CONFIG_
#define _M_CONFIG_

typedef struct _MINMAX
{
	DWORD min;
	DWORD max;
}MINMAX,*PMINMAX;

typedef struct _MINMAX64
{
	DWORD64 min;
	DWORD64 max;
}MINMAX64,*PMINMAX64;

struct mConfig
{
	struct {
		struct {
			BYTE	loop;
			BYTE	_if;
			BYTE	code;
			BYTE	ptrcode;
			BYTE	api;
			BYTE	_switch;
			BYTE	proc;
		}usage;

		struct {
			BYTE	add;
			BYTE	sub;
			BYTE	mul;
			BYTE	div;
			BYTE	mod;
			BYTE	and;
			BYTE	xor;
			BYTE	or;
			BYTE	shl;
			BYTE	shr;
		}block_code;

		struct 
		{
			MINMAX	interations;
			BYTE	max_levels;
			MINMAX	inner_blocks;
		}block_loop;

		struct  
		{
			MINMAX	chains;
			BYTE	max_levels;
			MINMAX	inner_blocks;
			MINMAX	trash_blocks;
		}block_if;

		struct  
		{
			MINMAX	chains;
			BYTE	max_levels;
			MINMAX	inner_blocks;
			MINMAX	trash_blocks;
		}block_switch;

		struct  
		{
			MINMAX	count;
			MINMAX	parameters;
			MINMAX	inner_blocks;
		}block_call;

		struct
		{
			MINMAX	count;
		}blocks;
	}code;

	struct {
		MINMAX	trash_before;
		MINMAX	trash_after;
	}shellcode;

	struct  
	{
		MINMAX	dlls;
		MINMAX	apis;

		MINMAX	trash_dlls;
		MINMAX	trash_apis;
	}import;

	struct {
		struct {
			BYTE locals;
			BYTE globals;
			BYTE formals;
		}usage_left;

		struct {
			BYTE locals;
			BYTE globals;
			BYTE formals;
		}usage_right;

		struct {
			BYTE locals;
			BYTE globals;
		}usage_api;

		BYTE types[8];

		struct {
			struct {
				BYTE yes;
				BYTE no;
			}locals;
			struct {
				BYTE yes;
				BYTE no;
			}globals;
			struct {
				MINMAX t_int8;
				MINMAX t_int16;
				MINMAX t_int32;
				MINMAX64 t_int64;
				MINMAX t_uint8;
				MINMAX t_uint16;
				MINMAX t_uint32;
				MINMAX64 t_uint64;
			}types;
		}initialization;

		struct {
			struct {
				struct {
					BYTE get;
					BYTE add;
				}locals;
				struct {
					BYTE get;
					BYTE add;
				}globals;
			}code;

			struct {
				struct {
					BYTE get;
					BYTE add;
				}locals;
				struct {
					BYTE get;
					BYTE add;
				}globals;
			}api;
		}getoradd;

		struct {
			MINMAX length;
		}path;

		struct {
			MINMAX trash_after;
			MINMAX trash_between;
		}decryptor;
	}vars;

	struct {
		MINMAX	extend;
		BYTE	rdata;
		BYTE	data;
	}image;

	MINMAX	string_array_size;

	BYTE	not_use_crt_stub;
};

#endif