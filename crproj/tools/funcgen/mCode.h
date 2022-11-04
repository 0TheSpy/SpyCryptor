#ifndef _M_CODE_
#define _M_CODE_

typedef struct _MSTRING_CHUNK
{
	LIST_ENTRY	entry;
	DWORD		length;
	DWORD		free;
	CHAR		string[0];
}MSTRING_CHUNK,*PMSTRING_CHUNK;

typedef struct  
{
	DWORD	length;
	CHAR	string[0];
}MSTRA,*PMSTRA;

typedef struct  
{
	DWORD	length;
	WCHAR	string[0];
}MSTRW,*PMSTRW;

class mCode
{
	LIST_ENTRY		chunks;
	DWORD			total;
	PMSTRING_CHUNK	last;
	PCHAR			get_result;
	PCHAR			Alpha[256];
	DWORD			plus_size;

public:
	mCode(DWORD chunk_size);
	~mCode();

	DWORD length();
	PCHAR get();

	VOID clear();

	static PMSTRA convert_ascii(PWCHAR string, INT length);
	static PMSTRW convert_unicode(PCHAR string, INT length);

	void add(PCHAR string, DWORD slength);
	void add(PCHAR String);
	void add_char(CHAR chr);
	void add(PWCHAR String);
	void add(mCode& code);
	void set(PCHAR String);
	void add_digit(DWORD Base, DWORD64 Digit);
};

mCode& operator << (mCode& code, WCHAR* str);
mCode& operator << (mCode& code, CHAR* str);
mCode& operator << (mCode& code, DWORD64 Digit);


#endif