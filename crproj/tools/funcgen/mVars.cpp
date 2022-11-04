#include <windows.h>
#include <tchar.h>
#include <intrin.h>
#include <Strsafe.h>
#include <Shlobj.h>

#include "globals.h"

MVAR_TYPE_INFO VarTypeInfo[] = {
	{"INT8", sizeof(INT8)},
	{"INT16", sizeof(INT16)},
	{"INT32", sizeof(INT32)},
	{"INT64", sizeof(INT64)},
	{"UINT8", sizeof(UINT8)},
	{"UINT16", sizeof(UINT16)},
	{"UINT32", sizeof(UINT32)},
	{"UINT64", sizeof(UINT64)},

	{"PINT8", sizeof(PINT8)},
	{"PINT16", sizeof(PINT16)},
	{"PINT32", sizeof(PINT32)},
	{"PINT64", sizeof(PINT64)},
	{"PUINT8", sizeof(PUINT8)},
	{"PUINT16", sizeof(PUINT16)},
	{"PUINT32", sizeof(PUINT32)},
	{"PUINT64", sizeof(PUINT64)},

	{"HINSTANCE", sizeof(HINSTANCE)},
	{"PCHAR", sizeof(PCHAR)},
	{"PWCHAR", sizeof(PWCHAR)},
	{"PTCHAR", sizeof(PTCHAR)},
	{"PCHAR*", sizeof(PCHAR*)},
	{"PWCHAR*", sizeof(PWCHAR*)},
	{"PTCHAR*", sizeof(PTCHAR*)},
	{"CHAR", sizeof(PCHAR)},
	{"WCHAR", sizeof(PWCHAR)},
	{"TCHAR", sizeof(PTCHAR)},
	{"", 0},

	// apis data
	{"SYSTEM_INFO", sizeof(SYSTEM_INFO)},
	{"LARGE_INTEGER", sizeof(LARGE_INTEGER)},
	{"ULARGE_INTEGER", sizeof(ULARGE_INTEGER)},
	{"HANDLE", sizeof(HANDLE)},
	{"HANDLE", sizeof(HANDLE)},
	{"HANDLE", sizeof(HANDLE)},
	{"HANDLE", sizeof(HANDLE)},
	{"HANDLE", sizeof(HANDLE)},
	{"FILE_SEGMENT_ELEMENT",sizeof(FILE_SEGMENT_ELEMENT)},
	{"BY_HANDLE_FILE_INFORMATION",sizeof(BY_HANDLE_FILE_INFORMATION)},
	{"BOOL", sizeof(BOOL)},
	{"VOID", 0},
	{"CRITICAL_SECTION", sizeof(CRITICAL_SECTION)},
	{"HANDLE", sizeof(HANDLE)},
	{"HANDLE", sizeof(HANDLE)},
	{"ATOM", sizeof(ATOM)},
	{"HKL", sizeof(HKL)},
	{"HANDLE", sizeof(HANDLE)},
	{"HDC", sizeof(HDC)},
	{"HRESULT", sizeof(HRESULT)},
	{"LRESULT", sizeof(LRESULT)},
	{"HWND", sizeof(HWND)},
	{"HDWP", sizeof(HDWP)},
	{"POINT", sizeof(POINT)},
	{"HENHMETAFILE", sizeof(HENHMETAFILE)},
	{"HMETAFILE", sizeof(HMETAFILE)},
	{"HICON", sizeof(HICON)},
	{"LPVOID", sizeof(LPVOID)},
	{"HACCEL", sizeof(HACCEL)},
	{"HBITMAP", sizeof(HBITMAP)},
	{"HCURSOR", sizeof(HCURSOR)},
	{"HBRUSH", sizeof(HBRUSH)},
	{"HRGN", sizeof(HRGN)},
	{"HFONT", sizeof(HFONT)},
	{"HPALETTE", sizeof(HPALETTE)},
	{"HPEN", sizeof(HPEN)},
	{"HPROPSHEETPAGE", sizeof(HPROPSHEETPAGE)},
	{"HMENU", sizeof(HMENU)},
	{"HGDIOBJ", sizeof(HGDIOBJ)},
	{"PIDLIST_RELATIVE", sizeof(PVOID)},
	{"PITEMID_CHILD", sizeof(PVOID)},
	{"PIDLIST_ABSOLUTE", sizeof(PVOID)},
	{"PUIDLIST_RELATIVE", sizeof(PVOID)},
	{"PUITEMID_CHILD", sizeof(PVOID)},
	{"HGLOBAL", sizeof(HGLOBAL)},
	{"HFILE", sizeof(PVOID)},
	{"HDROP", sizeof(PVOID)},
	{"HKEY", sizeof(PVOID)},
	{"RPC_AUTH_IDENTITY_HANDLE", sizeof(PVOID)},
	{"BLENDFUNCTION", sizeof(BLENDFUNCTION)},
	{"COORD", sizeof(COORD)},
	{"IID", sizeof(IID)},
	{"CLSID", sizeof(CLSID)},
	{"GUID", sizeof(GUID)},

	{NULL, 0},

	{"PROPSHEETHEADER", sizeof(PROPSHEETHEADER)},
	{"PROPSHEETHEADERA", sizeof(PROPSHEETHEADERA)},
	{"PROPSHEETHEADERW", sizeof(PROPSHEETHEADERW)},
	{"PROPENUMPROCEX", sizeof(PROPENUMPROCEX)},
	{"PROPENUMPROCEXA", sizeof(PROPENUMPROCEXA)},
	{"PROPENUMPROCEXW", sizeof(PROPENUMPROCEXW)},
	{"DISPLAY_DEVICE", sizeof(DISPLAY_DEVICE)},
	{"DISPLAY_DEVICEA", sizeof(DISPLAY_DEVICEA)},
	{"DISPLAY_DEVICEW", sizeof(DISPLAY_DEVICEW)},
	{"PROPENUMPROC", sizeof(PROPENUMPROC)},
	{"PROPENUMPROCA", sizeof(PROPENUMPROCA)},
	{"PROPENUMPROCW", sizeof(PROPENUMPROCW)},
	{"UILANGUAGE_ENUMPROC", sizeof(UILANGUAGE_ENUMPROC)},
	{"UILANGUAGE_ENUMPROCA", sizeof(UILANGUAGE_ENUMPROCA)},
	{"UILANGUAGE_ENUMPROCW", sizeof(UILANGUAGE_ENUMPROCW)},
	{"PRINTDLGEX", sizeof(PRINTDLGEX)},
	{"PRINTDLGEXA", sizeof(PRINTDLGEXA)},
	{"PRINTDLGEXW", sizeof(PRINTDLGEXW)},
	{"NOTIFYICONDATA", sizeof(NOTIFYICONDATA)},
	{"NOTIFYICONDATAA", sizeof(NOTIFYICONDATAA)},
	{"NOTIFYICONDATAW", sizeof(NOTIFYICONDATAW)},
	{"PAGESETUPDLG", sizeof(PAGESETUPDLG)},
	{"PAGESETUPDLGA", sizeof(PAGESETUPDLGA)},
	{"PAGESETUPDLGW", sizeof(PAGESETUPDLGW)},
	{"GCP_RESULTS", sizeof(GCP_RESULTS)},
	{"GCP_RESULTSA", sizeof(GCP_RESULTSA)},
	{"GCP_RESULTSW", sizeof(GCP_RESULTSW)},
	{"WNDCLASS", sizeof(WNDCLASS)},
	{"WNDCLASSA", sizeof(WNDCLASSA)},
	{"WNDCLASSW", sizeof(WNDCLASSW)},
	{"PRINTDLG", sizeof(PRINTDLG)},
	{"PRINTDLGA", sizeof(PRINTDLGA)},
	{"PRINTDLGW", sizeof(PRINTDLGW)},
	{"OSVERSIONINFO", sizeof(OSVERSIONINFO)},
	{"OSVERSIONINFOA", sizeof(OSVERSIONINFOA)},
	{"OSVERSIONINFOW", sizeof(OSVERSIONINFOW)},
	{"OSVERSIONINFOEX", sizeof(OSVERSIONINFOEX)},
	{"OSVERSIONINFOEXA", sizeof(OSVERSIONINFOEXA)},
	{"OSVERSIONINFOEXW", sizeof(OSVERSIONINFOEXW)},
	{"FINDREPLACE", sizeof(FINDREPLACE)},
	{"FINDREPLACEA", sizeof(FINDREPLACEA)},
	{"FINDREPLACEW", sizeof(FINDREPLACEW)},
	{"PRINTER_DEFAULTS", sizeof(PRINTER_DEFAULTS)},
	{"PRINTER_DEFAULTSA", sizeof(PRINTER_DEFAULTSA)},
	{"PRINTER_DEFAULTSW", sizeof(PRINTER_DEFAULTSW)},
	{"OUTLINETEXTMETRIC", sizeof(OUTLINETEXTMETRIC)},
	{"OUTLINETEXTMETRICA", sizeof(OUTLINETEXTMETRICA)},
	{"OUTLINETEXTMETRICW", sizeof(OUTLINETEXTMETRICW)},
	{"DEVMODE", sizeof(DEVMODE)},
	{"DEVMODEA", sizeof(DEVMODEA)},
	{"DEVMODEW", sizeof(DEVMODEW)},
	{"POLYTEXT", sizeof(POLYTEXT)},
	{"POLYTEXTA", sizeof(POLYTEXTA)},
	{"POLYTEXTW", sizeof(POLYTEXTW)},
	{"TEXTMETRIC", sizeof(TEXTMETRIC)},
	{"TEXTMETRICA", sizeof(TEXTMETRICA)},
	{"TEXTMETRICW", sizeof(TEXTMETRICW)},
	{"LOGFONT", sizeof(LOGFONT)},
	{"LOGFONTA", sizeof(LOGFONTA)},
	{"LOGFONTW", sizeof(LOGFONTW)},
	{"OPENFILENAME", sizeof(OPENFILENAME)},
	{"OPENFILENAMEA", sizeof(OPENFILENAMEA)},
	{"OPENFILENAMEW", sizeof(OPENFILENAMEW)},
	{"CHOOSECOLOR", sizeof(CHOOSECOLOR)},
	{"CHOOSECOLORA", sizeof(CHOOSECOLORA)},
	{"CHOOSECOLORW", sizeof(CHOOSECOLORW)},
	{"WNDCLASSEX", sizeof(WNDCLASSEX)},
	{"WNDCLASSEXA", sizeof(WNDCLASSEXA)},
	{"WNDCLASSEXW", sizeof(WNDCLASSEXW)},
	{"MENUITEMINFO", sizeof(MENUITEMINFO)},
	{"MENUITEMINFOA", sizeof(MENUITEMINFOA)},
	{"MENUITEMINFOW", sizeof(MENUITEMINFOW)},
	{"CHOOSEFONT", sizeof(CHOOSEFONT)},
	{"CHOOSEFONTA", sizeof(CHOOSEFONTA)},
	{"CHOOSEFONTW", sizeof(CHOOSEFONTW)},
	{"MSGBOXPARAMS", sizeof(MSGBOXPARAMS)},
	{"MSGBOXPARAMSA", sizeof(MSGBOXPARAMSA)},
	{"MSGBOXPARAMSW", sizeof(MSGBOXPARAMSW)},
	{"SHFILEINFO", sizeof(SHFILEINFO)},
	{"SHFILEINFOA", sizeof(SHFILEINFOA)},
	{"SHFILEINFOW", sizeof(SHFILEINFOW)},
	{"BROWSEINFO", sizeof(BROWSEINFO)},
	{"BROWSEINFOA", sizeof(BROWSEINFOA)},
	{"BROWSEINFOW", sizeof(BROWSEINFOW)},
	{"SHFILEOPSTRUCT", sizeof(SHFILEOPSTRUCT)},
	{"SHFILEOPSTRUCTA", sizeof(SHFILEOPSTRUCTA)},
	{"SHFILEOPSTRUCTW", sizeof(SHFILEOPSTRUCTW)},
	{"SHELLEXECUTEINFO", sizeof(SHELLEXECUTEINFO)},
	{"SHELLEXECUTEINFOA", sizeof(SHELLEXECUTEINFOA)},
	{"SHELLEXECUTEINFOW", sizeof(SHELLEXECUTEINFOW)},
	{"STARTUPINFO", sizeof(STARTUPINFO)},
	{"STARTUPINFOA", sizeof(STARTUPINFOA)},
	{"STARTUPINFOW", sizeof(STARTUPINFOW)},
	{"CPINFOEX", sizeof(CPINFOEX)},
	{"CPINFOEXA", sizeof(CPINFOEXA)},
	{"CPINFOEXW", sizeof(CPINFOEXW)},
	{"WIN32_FIND_DATA", sizeof(WIN32_FIND_DATA)},
	{"WIN32_FIND_DATAA", sizeof(WIN32_FIND_DATAA)},
	{"WIN32_FIND_DATAW", sizeof(WIN32_FIND_DATAW)},
	{"CURRENCYFMT", sizeof(CURRENCYFMT)},
	{"CURRENCYFMTA", sizeof(CURRENCYFMTA)},
	{"CURRENCYFMTW", sizeof(CURRENCYFMTW)},
	{"NUMBERFMT", sizeof(NUMBERFMT)},
	{"NUMBERFMTA", sizeof(NUMBERFMTA)},
	{"NUMBERFMTW", sizeof(NUMBERFMTW)},
	{"FONTENUMPROC", sizeof(FONTENUMPROC)},
	{"FONTENUMPROCA", sizeof(FONTENUMPROCA)},
	{"FONTENUMPROCW", sizeof(FONTENUMPROCW)},

	{NULL, 0},
};

PCHAR VarsTypePrefixStrings[] = {
	"glob",
	"loc",
	"arg"
};

PCHAR mVars::get_type_name(MVAR_TYPE type)
{
	return VarTypeInfo[type].name;
}

DWORD mVars::get_type_size(MVAR_TYPE type)
{
	return VarTypeInfo[type].size;
}

DWORD mVars::get_types_count(MVARS_GROUP group, PDWORD types)
{
	DWORD total = 0;

	mem_zero(types, VARS_COUNT*sizeof(DWORD));

	for(int i = 0; i < VARS_COUNT; i++)
	{
		types[i] = search_groups[group].types[i]->get_count();
		total	 += types[i];
	}
	return total;
}

mVars::mVars(MVARS_TYPE Type, DWORD ChunkEntries) : units(ChunkEntries), value_units(ChunkEntries)
{
	type = Type;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < VARS_COUNT; j++)
		{
			search_groups[i].types[j] = new mChunkUnitRandom<MVAR_INFO>(ChunkEntries);
		}
	}

	switch( Type )
	{
		case VARS_TYPE_FORMALS:
			// no supported
			break;
		case VARS_TYPE_GLOBALS:
			init.add(0,config.vars.initialization.globals.no);
			init.add(1,config.vars.initialization.globals.yes);
			getoradd.add(0,config.vars.getoradd.code.globals.add);
			getoradd.add(1,config.vars.getoradd.code.globals.get);
			break;
		case VARS_TYPE_LOCALS:
			init.add(0,config.vars.initialization.locals.no);
			init.add(1,config.vars.initialization.locals.yes);
			getoradd.add(0,config.vars.getoradd.code.locals.add);
			getoradd.add(1,config.vars.getoradd.code.locals.get);
			break;
	}
}

DWORD mVars::length()
{
	return units.get_count();
}

MVAR_INFO* mVars::first()
{
	return units.get_first_chunk();
}

MVAR_INFO* mVars::next()
{
	return units.get_next_chunk();
}

MVAR_INFO* mVars::add_custom(PCHAR type, PCHAR name)
{
	return add_custom_ex(type, name, NULL);
}

MVAR_INFO* mVars::add_custom_ex(PCHAR type, PCHAR name, PCHAR init_string)
{
	MVAR_INFO* result = units.get_new_chunk();

	lstrcpyA(result->name, name);
	result->type = MVT_CUSTOM;

	if( !init_string )
	{
		wsprintfA(result->string, "%s %s", type, name);
	}else{
		wsprintfA(result->string, "%s %s = %s", type, name, init_string);
	}

	return result;
}

MVAR_INFO* mVars::add(MVARS_GROUP Group, MVAR_TYPE Type, PCHAR Name, DWORD Flags, PVOID Value)
{
	MVAR_INFO* result = units.get_new_chunk();

	mem_zero(result,sizeof(MVAR_INFO));

	// our engine do not use variables with unknown value, may be only for trash but i do not need trash
	if( !(Flags & MVF_UNKNOWN_VALUE) && !(Flags & MVF_RESERVED) )
	{
		search_groups[Group].types[Type]->add_reference(result);
	}

	result->id		  = units.get_count() - 1;
	result->type	  = Type;
	result->type_str  = VarTypeInfo[Type].name;
	result->flags	  = Flags;
	result->vars_type = type;

	switch( result->type )
	{
		case MVT_PINT8:
		case MVT_PINT16:
		case MVT_PINT32:
		case MVT_PINT64:
		case MVT_PUINT8:
		case MVT_PUINT16:
		case MVT_PUINT32:
		case MVT_PUINT64:
			break;
		default:
			result->value = value_units.get_new_chunk();
			break;
	}

	if( !Name )
	{
		wsprintfA(result->name,"%s%d",VarsTypePrefixStrings[type], result->id);
	}else{
		lstrcpyA(result->name,Name);
	}

	switch( Type )
	{
		case MVT_CHAR_ARRAY:
		case MVT_WCHAR_ARRAY:
		case MVT_TCHAR_ARRAY:
			result->array_size = random.get_equal(config.string_array_size.min, config.string_array_size.max);

			wsprintfA(result->string, "%s %s[%d]", result->type_str, result->name, result->array_size);
			break;
		default:
			result->array_size = 1;

			if( Flags & MVF_INITIALIZED )
			{
				if( !(Flags & MVF_UNKNOWN_VALUE) )
				{
					if( !result->value )
						result->value = value_units.get_new_chunk();

					mem_copy(result->value, Value, VarTypeInfo[Type].size);

					switch( Type )
					{
					case MVT_INT8: 
					case MVT_UINT8:
						wsprintfA(result->string, "%s %s = 0x%0.2X", result->type_str, result->name, result->value->ui8); 
						break;
					case MVT_INT16: 
					case MVT_UINT16:
						wsprintfA(result->string, "%s %s = 0x%0.4X", result->type_str, result->name, result->value->ui16); 
						break;
					case MVT_INT32: 
					case MVT_UINT32:
						wsprintfA(result->string, "%s %s = 0x%0.8X", result->type_str, result->name, result->value->ui32); 
						break;
					case MVT_INT64: 
					case MVT_UINT64:
						wsprintfA(result->string, "%s %s = 0x%0.8X%0.8X", result->type_str, result->name, (DWORD)(result->value->i64>>32), (DWORD)result->value->i64); 
						break;
					}
				}else{
					wsprintfA(result->string, "%s %s", result->type_str, result->name);
				}
			}else{
				wsprintfA(result->string, "%s %s", result->type_str, result->name);
			}
			break;
	}

	return result;
}

MVAR_INFO* mVars::get_random(MVARS_GROUP Group, MVAR_TYPE Type, DWORD except_count, MVAR_INFO* excepts[])
{
	return search_groups[Group].types[Type]->get_random(except_count, excepts);	
}


void mVars::get_imm_by_type(PINT64 Value, MVAR_TYPE Type)
{
	*Value = 0;

	// Set checks (value%2==0) to prevent be zero in loops

	switch( Type )
	{
		case MVT_PINT8:
		case MVT_INT8:	
			*(PINT8)Value = random.get_less(config.vars.initialization.types.t_int8.min,config.vars.initialization.types.t_int8.max); 
			if( *(PINT8)Value%2==0 )
				*(PINT8)Value++;
			break;
		case MVT_PINT16:
		case MVT_INT16:	
			*(PINT16)Value = random.get_less(config.vars.initialization.types.t_int16.min,config.vars.initialization.types.t_int16.max); 
			if( *(PINT16)Value%2==0 )
				*(PINT16)Value++;
			break;
		case MVT_PINT32:
		case MVT_INT32:	
			*(PINT32)Value = random.get_less(config.vars.initialization.types.t_int32.min,config.vars.initialization.types.t_int32.max);
			if( *(PINT32)Value%2==0 )
				*(PINT32)Value++;
			break;
		case MVT_PINT64:
		case MVT_INT64:	
			*(PINT64)Value = random.get_less(config.vars.initialization.types.t_int64.min,config.vars.initialization.types.t_int64.max); 
			if( *(PINT64)Value%2==0 )
				*(PINT64)Value++;
			break;

		case MVT_PUINT8:
		case MVT_UINT8:	
			*(PUINT8)Value = random.get_less(config.vars.initialization.types.t_uint8.min,config.vars.initialization.types.t_uint8.max); 
			if( *(PUINT8)Value%2==0 )
				*(PUINT8)Value++;
			break;
		case MVT_PUINT16:
		case MVT_UINT16: 
			*(PUINT16)Value = random.get_less(config.vars.initialization.types.t_uint16.min,config.vars.initialization.types.t_uint16.max); 
			if( *(PUINT16)Value%2==0 )
				*(PUINT16)Value++;
			break;
		case MVT_PUINT32:
		case MVT_UINT32:	
			*(PUINT32)Value = random.get_less(config.vars.initialization.types.t_uint32.min,config.vars.initialization.types.t_uint32.max); 
			if( *(PUINT32)Value%2==0 )
				*(PUINT32)Value++;
			break;
		case MVT_PUINT64:
		case MVT_UINT64:	
			*(PUINT64)Value = random.get_less(config.vars.initialization.types.t_uint64.min,config.vars.initialization.types.t_uint64.max); 
			if( *(PUINT64)Value%2==0 )
				*(PUINT64)Value++;
			break;
	}
}

MVAR_INFO* mVars::add_or_get_random(MVARS_GROUP Group,MVAR_TYPE Type,DWORD p_flags)
{
	return add_or_get_random_ex(Group,Type,p_flags,&getoradd);
}

MVAR_INFO* mVars::add_or_get_random_ex(MVARS_GROUP Group, MVAR_TYPE Type, DWORD p_flags, mRandomTypes* GetOrAdd)
{
	MVAR_INFO* result = NULL;

	// if not vars always create, to prevent deadlocks
	if( search_groups[Group].types[Type]->get_count() > 2 && GetOrAdd->get() )
	{
		result = get_random(Group, Type, NULL, NULL);
	}

	if( !result )
	{
		DWORD		Flags = p_flags;
		MVAR_VALUE	Value;

		if( !(p_flags & MVF_DO_NOT_INITIALIZE) )
		{
			if( !MVAR_TYPE_IS_PTR(Type) )
			{
				if( init.get() )
				{
					Flags |= MVF_INITIALIZED;

					get_imm_by_type(&Value.i64, Type);
				}
			}
		}

		result = add(Group, Type, NULL, Flags, &Value);
	}

	return result;
}
