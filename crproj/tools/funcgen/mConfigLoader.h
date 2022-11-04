#ifndef _M_CONFIG_LOADER_
#define _M_CONFIG_LOADER_

#include "mConfig.h"

enum MCONFIG_OPTION_TYPE
{
	COT_DIGIT8,
	COT_DIGIT32,
	COT_DIGIT64,
	COT_RANGE,
	COT_RANGE64
};

typedef struct _MCONFIG_OPTION
{
	PCHAR				name;
	DWORD				offset;
	MCONFIG_OPTION_TYPE	type;
}MCONFIG_OPTION,*PMCONFIG_OPTION;

class mConfigLoader
{
	mConfig *			config;
	DWORD				line_number;
	TCHAR				error[MAX_PATH];
	PIMAGE_NT_HEADERS	nt;

	PCHAR skip_spaces(PCHAR current, PCHAR end);
	PCHAR get_digit(PCHAR current, PCHAR end, PUINT64 value);
public:
	mConfigLoader(mConfig* config, PIMAGE_DOS_HEADER dos);

	bool load_from_file(PTCHAR file_path);
	bool load_default();
	PTCHAR get_error();
};

#endif