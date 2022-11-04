#ifndef _M_IMPORT_H_
#define _M_IMPORT_H_

#include "mChunkUnitAllocator.h"
#include "mVars.h"

enum API_PARAMETER_TYPE
{
	PT_NONE,
	PT_NULL,
	PT_RAND,
	PT_RAND_FLOAT,
	PT_DIGIT,
	PT_SHIFT,
	PT_VAR_SIZE,
	PT_PTR_ARG,
	PT_ARG,
	PT_ANY,
	PT_STRING,
	PT_PTR_ANY,
	PT_GETVAR_OR_NULL,
	PT_PTR_GETVAR_OR_NULL,
	PT_ADDVAR,
	PT_PTR_ADDVAR,
	PT_PROC,
	PT_PROC_AW,
	PT_CUSTOM,
	PT_ZERO,
};

enum IMPORT_PROCS_TYPE{
	IPT_RANDOM_A_OR_W,
	IPT_W,
	IPT_A
};

typedef struct _API_PARAMETER
{
	PCHAR				convert;
	API_PARAMETER_TYPE	type;
	union
	{
		struct 
		{
			DWORD		from;
			DWORD		to;
		}rand;
		BYTE			shift;
		BYTE			arg_pos;
		struct  
		{
			MVAR_TYPE	type;
		}var;
		DWORD			digit;
		MVAR_TYPE		proc_aw;
	}value;
}API_PARAMETER,*PAPI_PARAMETER;

class mImport;
struct _API_TEMPLATE;

typedef void (*TD_api_constructor)(mImport* apis, mCode &str, _API_TEMPLATE *tmpl, PMVAR_INFO arg, bool is_trash);

#define ATF_A_OR_W	0x1
#define MAX_API_PARAMETERS 16

typedef struct _API_TEMPLATE
{
	DWORD				flags;
	TD_api_constructor	constructor;
	API_PARAMETER		ret;
	PCHAR				name;
	API_PARAMETER		arguments[MAX_API_PARAMETERS];
}API_TEMPLATE,*PAPI_TEMPLATE;

#define MIMPORT_MAX_LIBRARIES	200
#define MIMPORT_MAX_GROUPS		20

class mImportLibraryGroup
{
protected:
	mImport*	procs;

	// total apis in group
	WORD		api_count;

	// reserved apis per group
	DWORD		api_reserved_pos;
	PWORD		api_reserved;

	void add_path(mCode &str, DWORD count);
	void add_string(mCode &str, DWORD count);
	void add_shift(mCode &str, DWORD count, INT from,INT to);
	void add_rand(mCode &str, DWORD count, INT from, INT to);
	void add_null(mCode &str, DWORD count);
	void add_bits(mCode &str, DWORD count, DWORD Bits, PCHAR *Strings);
	void add_path(mCode &str, DWORD count, CHAR type);
public:
	mImportLibraryGroup(mImport* import, DWORD api_count);
	~mImportLibraryGroup();

	void	prepare_reservation();
	WORD	reserve_api();
	WORD	get_api_count();

	virtual bool	get_api(mCode& code, WORD api_id, bool is_loop, bool is_trash) = 0;
};

#pragma pack(push,1)

typedef struct _MIMPORT_RESERVED_API
{
	mImportLibraryGroup*	group;
	WORD					api_id;
}MIMPORT_RESERVED_API,*PMIMPORT_RESERVED_API;

typedef struct _MIMPORT_RESERVED_IMPORT
{
	DWORD					count;
	MIMPORT_RESERVED_API	apis[0];
}MIMPORT_RESERVED_IMPORT,*PMIMPORT_RESERVED_IMPORT;

#pragma pack(pop)

class mImportLibrary
{
protected:
	mImport*				procs;
	DWORD					api_count;

	// all groups database
	DWORD					groups_count;
	mImportLibraryGroup*	groups[MIMPORT_MAX_GROUPS];

	// reserved groups
	DWORD					groupsres_count;
	mImportLibraryGroup*	groupsres[MIMPORT_MAX_GROUPS];

	void	update_api_count();
public:
	mImportLibrary(mImport* import);

	DWORD	reserve_apis(PMIMPORT_RESERVED_API apis_save, DWORD apis_per_library);
	DWORD	get_api_count();
};

class mImport
{
	mVars*			globals;
	mVars*			locals;
	mVars*			formals;

	IMPORT_PROCS_TYPE	procs_type;
	MVARS_GROUP			vars_group;

	DWORD			debug_counter;

	// all libraries database
	DWORD			libs_count;
	mImportLibrary*	libs[MIMPORT_MAX_LIBRARIES];

	DWORD			trash_libs_count;
	mImportLibrary*	trash_libs[MIMPORT_MAX_LIBRARIES];

	// reserved libraries for current build
	PMIMPORT_RESERVED_IMPORT	apis_reserved;
	DWORD						apis_pos;
	DWORD						apis_last;

	// reserved libraries for current build
	PMIMPORT_RESERVED_IMPORT	trash_apis_reserved;
	DWORD						trash_apis_pos;
	DWORD						trash_apis_last;

	mRandomTypes	vusage;

	mRandomTypes getoradd_globals;
	mRandomTypes getoradd_locals;

	PMBUF  word_buffer;
	PCHAR* word_list;
	DWORD  word_list_size;

	bool	get_api_from_reserved(MVARS_GROUP group, PMIMPORT_RESERVED_IMPORT reserved, PDWORD position, PDWORD last_position, mCode& code, bool is_loop,bool is_trash);
public:
	mImport();

	PMIMPORT_RESERVED_IMPORT reserve_libraries(DWORD dlls_min, DWORD dlls_max, DWORD apis_min, DWORD apis_max, DWORD total_libs_count, mImportLibrary** libraries);
	void		load_config();

	PMVAR_INFO	get_any();
	PMVAR_INFO	get_or_add_var(MVAR_TYPE p_type);
	PMVAR_INFO	get_var(MVAR_TYPE p_type);
	void		get_path(mCode& str, IMPORT_PROCS_TYPE type);
	void		get_string(mCode& str, IMPORT_PROCS_TYPE type);

	MVAR_TYPE	parse_get_var_type(DWORD flags, MVAR_TYPE type);
	void		parse(mCode &str, API_TEMPLATE *tmpl, PMVAR_INFO arg, bool is_trash);

	void					set_procs_type(IMPORT_PROCS_TYPE type);
	IMPORT_PROCS_TYPE		get_procs_type();
	PCHAR					get_procs_string();

	bool		get_api(mVars* globals,mVars* locals, mVars* formals, mCode &code, bool is_loop, bool is_trash);
};

#endif