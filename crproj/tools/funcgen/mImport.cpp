#include <windows.h>
#include <tchar.h>
#include <intrin.h>
#include <Strsafe.h>

#include "globals.h"
#include "mImport.h"

#include "lib_kernel32.h"
#include "lib_trash.h"
#include "lib_db_trash.h"

mImportLibraryGroup::mImportLibraryGroup(mImport* import, DWORD api_count)
{
	procs = import;
	this->api_count = api_count;

	api_reserved = NULL;
	api_reserved_pos = 0;
}

mImportLibraryGroup::~mImportLibraryGroup()
{
	if( api_reserved )
		hfree(api_reserved);
}

WORD mImportLibraryGroup::get_api_count()
{
	return api_count;
}

void mImportLibraryGroup::prepare_reservation()
{
	api_reserved = NULL;
	api_reserved_pos = 0;

	if( api_count )
	{
		api_reserved = (PWORD)halloc(sizeof(WORD)*api_count);
		if( api_reserved )
		{
			for(int i = 0; i < api_count; i++ )
			{
				api_reserved[i] = i;
			}
		}
	}
}

WORD mImportLibraryGroup::reserve_api()
{
	if( api_reserved)
	{
		if( api_reserved_pos < api_count )
		{
			WORD pos = random.get_less(api_reserved_pos, api_count);
			WORD id  = api_reserved[pos];

			api_reserved[pos] = api_reserved[api_reserved_pos];

			api_reserved_pos++;

			return id;
		}
	}

	return NULL;
}

void mImportLibraryGroup::add_shift(mCode &str, DWORD count, INT from,INT to)
{
	if( count )
	{
		str << ", ";
	}

	str << (1 << random.get_equal(from,to));
}

void mImportLibraryGroup::add_rand(mCode &str, DWORD count, INT from, INT to)
{
	if( count )
	{
		str << ", ";
	}

	str << random.get_equal(from,to);
}

void mImportLibraryGroup::add_null(mCode &str, DWORD count)
{
	if( count )
	{
		str << ", NULL";
	}else{
		str << "NULL";
	}
}

void mImportLibraryGroup::add_bits(mCode &str, DWORD count, DWORD Bits, PCHAR *Strings)
{
	if( count )
	{
		str << ", ";
	}

	int params = 0;
	for(int i = 1, pos = 0; Bits; i<<=1, pos++)
	{
		if( Bits & i)
		{
			if( params )
				str << " | ";

			str << Strings[pos];
			params++;
		}

		Bits &= ~i;
	}
}

void mImportLibraryGroup::add_path(mCode &str, DWORD count)
{
	if( count )
	{
		str << ", ";
	}

	procs->get_path(str, procs->get_procs_type());
}

void mImportLibraryGroup::add_string(mCode &str, DWORD count)
{
	if( count )
	{
		str << ", ";
	}

	procs->get_string(str, procs->get_procs_type());
}

mImportLibrary::mImportLibrary(mImport *import)
{
	procs = import;
	api_count = 0;
}

DWORD mImportLibrary::get_api_count()
{
	return api_count;
}

void mImportLibrary::update_api_count()
{
	api_count = 0;

	for(int i = 0; i < groups_count; i++)
	{
		api_count += groups[i]->get_api_count();
	}
}

DWORD mImportLibrary::reserve_apis(PMIMPORT_RESERVED_API apis_save, DWORD apis_per_library)
{	
	DWORD group_apis_count[MIMPORT_MAX_GROUPS];
	DWORD group_ids[MIMPORT_MAX_GROUPS];

	if( !apis_per_library )
		return 0;

	if( apis_per_library > api_count )
		apis_per_library = api_count;

	mem_zero(group_apis_count,sizeof(DWORD)*groups_count);

	// 0 1 2 3 ...
	for(int i = 0; i < groups_count; i++)
	{
		group_ids[i] = i;
		groups[i]->prepare_reservation();
	}

	DWORD upos = 0; // used special algo, to ignore which was used
	for(int i = 0; i < apis_per_library; )
	{
		DWORD group_pos = random.get_less(upos, groups_count);
		DWORD group_id = group_ids[ group_pos ];

		if( group_apis_count[group_id] < groups[group_id]->get_api_count() )
		{
			group_apis_count[group_id]++;

			apis_save[i].group = groups[group_id];
			apis_save[i].api_id = groups[group_id]->reserve_api();

			i++;
		}else{ 
			// max api reaches ? move to left riched group
			group_ids[group_pos] = group_ids[upos];
			upos++;
		}
	}

	return apis_per_library;
}

mImport::mImport()
{
	//word_list = get_wordlist(&word_list_size);

	globals = NULL;
	locals  = NULL;
	formals = NULL;

	procs_type = IPT_RANDOM_A_OR_W;

	apis_reserved = NULL;

	debug_counter = 0;

	libs_count = 0;
	libs[libs_count++] = new mImportKernel32(this);

	trash_libs_count = sizeof(db_trash_dlls)/sizeof(TRASH_IMPORT_DLL_REFERENCE);
	for(int i = 0; i < trash_libs_count; i++)
	{
		trash_libs[i] = new mImportTrashLibrary(this, db_trash_dlls[i].templates, db_trash_dlls[i].count);
	}
}

void mImport::load_config()
{
	vusage.add(0, config.vars.usage_api.locals);
	vusage.add(1, config.vars.usage_api.globals);

	getoradd_locals.add(0,config.vars.getoradd.api.locals.add);
	getoradd_locals.add(1,config.vars.getoradd.api.locals.get);

	getoradd_globals.add(0,config.vars.getoradd.api.globals.add);
	getoradd_globals.add(1,config.vars.getoradd.api.globals.get);

	apis_pos = 0;
	apis_reserved = reserve_libraries(config.import.dlls.min, config.import.dlls.max, config.import.apis.min, config.import.apis.max, libs_count, libs);

	trash_apis_pos = 0;
	trash_apis_reserved = reserve_libraries(config.import.trash_dlls.min, config.import.trash_dlls.max, config.import.trash_apis.min, config.import.trash_apis.max, trash_libs_count, trash_libs);
}

PMIMPORT_RESERVED_IMPORT mImport::reserve_libraries(DWORD dlls_min, DWORD dlls_max, DWORD apis_min, DWORD apis_max, DWORD total_libs_count, mImportLibrary** libraries)
{
	DWORD libres_count = random.get_equal(dlls_min, dlls_max);

	if( libres_count > total_libs_count)
		libres_count = total_libs_count;

	WORD libs_ids[MIMPORT_MAX_LIBRARIES];
	WORD libs_apis[MIMPORT_MAX_LIBRARIES];

	apis_last = -1;

	for(int i = 0; i < total_libs_count; i++)
	{
		libs_ids[i] = i;
	}

	// собераем уник ид ли
	DWORD apis_total = 0;
	for(int i = 0; i < libres_count; i++)
	{
		DWORD pos = random.get_less(i, total_libs_count);

		WORD tmp = libs_ids[i];
		libs_ids[i] = libs_ids[pos];
		libs_ids[pos] = tmp;

		DWORD lib_id = libs_ids[i];

		// Ќе даем привысить размеры базы
		DWORD min = apis_min > libraries[lib_id]->get_api_count() ? libraries[lib_id]->get_api_count() : apis_min;
		DWORD max = apis_max > libraries[lib_id]->get_api_count() ? libraries[lib_id]->get_api_count() : apis_max;

		libs_apis[i] = random.get_equal(min, max);
		apis_total += libs_apis[i];
	}

	PMIMPORT_RESERVED_IMPORT reserved = (PMIMPORT_RESERVED_IMPORT)halloc(sizeof(MIMPORT_RESERVED_IMPORT) + sizeof(MIMPORT_RESERVED_API)*apis_total);
	if( reserved )
	{
		reserved->count = 0;

		for(int i = 0; i < libres_count; i++)
		{
			reserved->count += libraries[ libs_ids[i] ]->reserve_apis(&reserved->apis[reserved->count],libs_apis[i]);
		}
	}

	return reserved;
}

PMVAR_INFO mImport::get_any()
{
	DWORD	use = vusage.get();
	mVars*	vars = (use==0 ? locals : globals);

	DWORD counts[ VARS_COUNT ];

	// may be first call no types
	if( !vars->get_types_count(vars_group, counts) )
	{
		vars = (use==0 ? globals : locals);
		if( !vars->get_types_count(vars_group,counts) )
		{
			// if not other types then api is first, call before any code
			return NULL;
		}
	}
		
	DWORD	count = random.get_less(1, VARS_COUNT); // bugfix: from 1 start, becaus need find index
	INT		i;

	for(i = 0; 1 ; )
	{
		if( i >= VARS_COUNT ) 
			i = 0;
	
		if( counts[i] )
		{
			count--;

			if( !count ) 
				break;
		}

		i++;
	}

	return vars->get_random(vars_group,(MVAR_TYPE)i,NULL,NULL);
}

PMVAR_INFO mImport::get_or_add_var(MVAR_TYPE p_type)
{
	switch( vusage.get() )
	{
		case 1: return globals->add_or_get_random_ex(vars_group, p_type, MVF_DO_NOT_INITIALIZE, &getoradd_globals); break;
		case 0:	return locals->add_or_get_random_ex(vars_group, p_type, MVF_DO_NOT_INITIALIZE, &getoradd_locals); break;
	}

	return NULL;
}

PMVAR_INFO mImport::get_var(MVAR_TYPE p_type)
{
	switch( vusage.get() )
	{
		case 1:	return globals->get_random(vars_group, p_type, NULL, NULL);
		case 0:	return locals->get_random(vars_group, p_type, NULL, NULL);
	}

	return NULL;
}


void mImport::get_path(mCode& str,IMPORT_PROCS_TYPE type)
{
	if ( type==IPT_W )
		str << "L\"";
	else
		str << "\"";

	CHAR alpha[] = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890";

	DWORD count = random.get_less(2,5);
	for(int i = 0; i < count; i++)
	{
		if( i )
		{	
			str << "\\\\";
		}

		DWORD word_length = random.get_less(2,12);

		for(int j = 0; j < word_length; j++)
		{
			str.add_char( alpha[ random.get_less(0, sizeof(alpha) - 1) ] );
		}
	}

	str << "\"";
}

void mImport::get_string(mCode& str,IMPORT_PROCS_TYPE type)
{
	if ( type==IPT_W )
		str << "L\"";
	else
		str << "\"";

	CHAR upper[] = "QWERTYUIOPASDFGHJKLZXCVBNM";
	CHAR alpha[] = "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm1234567890";
	CHAR div[]   = "_-$@#~., ";

	DWORD count = random.get_less(1,5);
	for(int i = 0; i < count; i++)
	{
		if( i )
		{	
			str.add_char( div[ random.get_less(0, sizeof(div) - 1) ] );
		}

		DWORD word_length = random.get_less(1,8);

		if( random.get_less(0,2) )
		{
			str.add_char( upper[ random.get_less(0, sizeof(upper) - 1) ] );
		}

		for(int j = 0; j < word_length; j++)
		{
			str.add_char( alpha[ random.get_less(0, sizeof(alpha) - 1) ] );
		}
	}

	str << "\"";
}

void mImport::set_procs_type(IMPORT_PROCS_TYPE type)
{
	procs_type = type;
}

IMPORT_PROCS_TYPE mImport::get_procs_type()
{
	return procs_type;
}

PCHAR mImport::get_procs_string()
{
	switch( procs_type )
	{
		case IPT_A:	return "A"; break;
		case IPT_W:	return "W"; break;
		default:
			if( random.get_less(0,1) )
			{
				return "W";
			}
			return "A";
			break;
	}
	return "W";
}

MVAR_TYPE mImport::parse_get_var_type(DWORD flags, MVAR_TYPE type)
{
	if( flags & ATF_A_OR_W )
	{
		if( type==MVT_PTCHAR )
			return get_procs_type()==IPT_W ? MVT_PWCHAR : MVT_PCHAR;

		if( type==MVT_PTCHAR_PTR )
			return get_procs_type()==IPT_W ? MVT_PWCHAR_PTR : MVT_PCHAR_PTR;

		if( type > MVT_TSTRUCTS_BEGIN && type < MVT_TSTRUCTS_END )
		{
			return get_procs_type()==IPT_W ? MVAR_TCHAR_TO_UNICODE(type) : MVAR_TCHAR_TO_ASCII(type);
		}
	}

	return type;
}

void mImport::parse(mCode &str, API_TEMPLATE *tmpl, PMVAR_INFO arg, bool is_trash)
{
	PMVAR_INFO var;

	if( tmpl->constructor )
	{
		tmpl->constructor(this, str, tmpl, arg, is_trash);
		return;
	}

	if( tmpl->ret.type!=PT_NONE )
	{
		switch( tmpl->ret.type )
		{
			case PT_ARG:
				str << arg->name << " = ";
				break;
			default:
				PMVAR_INFO rvar = get_or_add_var( parse_get_var_type(tmpl->flags, tmpl->ret.value.var.type) );
				if( rvar )
				{
					str << rvar->name << " = ";
				}
				break;
		}
	}
		
	if( tmpl->flags & ATF_A_OR_W )
	{
		if( get_procs_type()==IPT_A )
		{
			str << tmpl->name << "A(";
		}else{
			str << tmpl->name << "W(";
		}
	}else{
		str << tmpl->name << "(";
	}

	PMVAR_INFO	vars[MAX_API_PARAMETERS];
	DWORD		vars_sizes[MAX_API_PARAMETERS];

	if( !lstrcmpA(tmpl->name,"GetCurrencyFormatEx") )
		__debugbreak();

	for(int i = 0; tmpl->arguments[i].type!=PT_NONE; i++)
	{
		vars[i] = NULL;
		vars_sizes[i] = 0;

		switch( tmpl->arguments[i].type )
		{
			case PT_ADDVAR:
			case PT_PTR_ADDVAR:
				vars[i] = get_or_add_var( parse_get_var_type(tmpl->flags,tmpl->arguments[i].value.var.type) );
				break;
			case PT_GETVAR_OR_NULL:
			case PT_PTR_GETVAR_OR_NULL:
				vars[i] = get_var( parse_get_var_type(tmpl->flags,tmpl->arguments[i].value.var.type) );
				break;
			case PT_ANY:
			case PT_PTR_ANY:
				vars[i] = get_any();
				break;
		}

		if( vars[i] )
		{
			if( vars[i]->array_size )
			{
				vars_sizes[i] = mVars::get_type_size( parse_get_var_type(tmpl->flags,vars[i]->type) )*vars[i]->array_size;	
			}else{
				vars_sizes[i] = mVars::get_type_size( parse_get_var_type(tmpl->flags,vars[i]->type) );
			}
		}
	}
	
	mFunction* proc;

	for(int i = 0; tmpl->arguments[i].type!=PT_NONE; i++)
	{
		if( i > 0 )
			str << ", ";
	
		str << tmpl->arguments[i].convert;

		switch( tmpl->arguments[i].type )
		{
			case PT_CUSTOM:
				// .convert is custom string, do nothing.
				break;
			case PT_STRING:
				get_string(str, get_procs_type());
				break;
			case PT_PTR_ARG:
				str << "&" << arg->name;
				break;
			case PT_ARG:
				str << arg->name;
				break;
			case PT_PROC:
				if( all_functions.get_count()==1 )
				{
					str << "NULL";
				}else{
					do 
					{
						proc = all_functions.get_random(NULL, NULL);
					} while ( !lstrcmpA(proc->get_name(), "DllMain") || !lstrcmpA(proc->get_name(), "WinMain") || !lstrcmpA(proc->get_name(), "wWinMain") );

					str << proc->get_name();
				}
				break;
			case PT_PROC_AW:
				str << "(" << mVars::get_type_name(parse_get_var_type(tmpl->flags, tmpl->arguments[i].value.proc_aw)) << ")";

				if( all_functions.get_count()==1 )
				{
					str << "NULL";
				}else{
					do 
					{
						proc = all_functions.get_random(NULL, NULL);
					} while ( !lstrcmpA(proc->get_name(), "DllMain") || !lstrcmpA(proc->get_name(), "WinMain") || !lstrcmpA(proc->get_name(), "wWinMain") );

					str << proc->get_name();
				}
				break;
			case PT_DIGIT:
				if( is_trash && random.get_less(0,2) )
				{
					var = get_any();
					if( var )
					{
						str << var->name;
					}else{
						str << "NULL";
					}
				}else{
					str << tmpl->arguments[i].value.digit;
				}
				break;
			case PT_RAND:
				str << random.get_equal(tmpl->arguments[i].value.rand.from,tmpl->arguments[i].value.rand.to);
				break;
			case PT_RAND_FLOAT:
				str.add_digit(10, random.get_equal(tmpl->arguments[i].value.rand.from,tmpl->arguments[i].value.rand.to));
				str << ".";
				str.add_digit(10, random.get_equal(0,0xFFFFFFFF));
				break;
			case PT_SHIFT:
				str << (1 << random.get_equal(tmpl->arguments[i].value.rand.from,tmpl->arguments[i].value.rand.to));
				break;
			case PT_NULL:
				str << "NULL";
				break;
			case PT_ADDVAR:
			case PT_PTR_ADDVAR:
				if( vars[i] )
				{
					if( tmpl->arguments[i].type==PT_PTR_ADDVAR )
						str << "&";

					str << vars[i]->name;
				}else{
					str << "NULL";
				}
				break;
			case PT_GETVAR_OR_NULL:
			case PT_PTR_GETVAR_OR_NULL:
				if( vars[i] )
				{
					if( tmpl->arguments[i].type==PT_PTR_GETVAR_OR_NULL )
						str << "&";

					str << vars[i]->name;
				}else{
					str << "NULL";
				}
				break;
			case PT_ANY:
			case PT_PTR_ANY:
				if( vars[i] )
				{
					if( tmpl->arguments[i].type==PT_PTR_ANY )
						str << "&";

					str << vars[i]->name;
				}else{
					str << "NULL";
				}
				break;
			case PT_VAR_SIZE:
				str << vars_sizes[i];
				break;
			case PT_ZERO:
				str << "0";
				break;
		}
	}

	str << ");";
}

bool mImport::get_api_from_reserved(MVARS_GROUP group, PMIMPORT_RESERVED_IMPORT reserved, PDWORD position, PDWORD last_position, mCode& code, bool is_loop, bool is_trash)
{
	bool result = false;

	if( reserved && reserved->count )
	{
		vars_group = group;

		if( *position < reserved->count )
		{
			result = reserved->apis[*position].group->get_api(code, reserved->apis[*position].api_id, is_loop, is_trash);

			*position = *position + 1;
		}else{
			DWORD pos;

			if( reserved->count > 1 )
			{
				do 
				{
					pos = random.get_less(0, reserved->count);
				} while (pos==*last_position);
			}else{
				pos = random.get_less(0, reserved->count);
			}

			*last_position = pos;

			result = reserved->apis[pos].group->get_api(code, reserved->apis[pos].api_id, is_loop, is_trash);
		}
	}

	return result;
}

bool mImport::get_api(mVars* globals,mVars* locals, mVars* formals, mCode &code, bool is_loop, bool is_trash)
{
	this->globals = globals;
	this->locals  = locals;
	this->formals = formals;

	bool result = false;

	if( is_trash )
	{
		result = get_api_from_reserved(VG_CODE, trash_apis_reserved, &trash_apis_pos, &trash_apis_last, code, is_loop, is_trash);
	}else{
		result = get_api_from_reserved(VG_API, apis_reserved, &apis_pos, &apis_last, code, is_loop, is_trash);
	}

	return result;
}
