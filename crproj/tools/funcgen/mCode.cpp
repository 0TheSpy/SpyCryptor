#include <windows.h>
#include <tchar.h>
#include <intrin.h>
#include <Strsafe.h>

#include "globals.h"

PCHAR HexAlpha256[] = {
	"00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "0A", "0B", "0C", "0D", "0E", "0F", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "1A", "1B", "1C", "1D", "1E", "1F", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "2A", "2B", "2C", "2D", "2E", "2F", "30", "31", "32", "33", "34", "35", "36", "37", "38", "39", "3A", "3B", "3C", "3D", "3E", "3F", "40", "41", "42", "43", "44", "45", "46", "47", "48", "49", "4A", "4B", "4C", "4D", "4E", "4F", "50", "51", "52", "53", "54", "55", "56", "57", "58", "59", "5A", "5B", "5C", "5D", "5E", "5F", "60", "61", "62", "63", "64", "65", "66", "67", "68", "69", "6A", "6B", "6C", "6D", "6E", "6F", "70", "71", "72", "73", "74", "75", "76", "77", "78", "79", "7A", "7B", "7C", "7D", "7E", "7F", "80", "81", "82", "83", "84", "85", "86", "87", "88", "89", "8A", "8B", "8C", "8D", "8E", "8F", "90", "91", "92", "93", "94", "95", "96", "97", "98", "99", "9A", "9B", "9C", "9D", "9E", "9F", "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "A8", "A9", "AA", "AB", "AC", "AD", "AE", "AF", "B0", "B1", "B2", "B3", "B4", "B5", "B6", "B7", "B8", "B9", "BA", "BB", "BC", "BD", "BE", "BF", "C0", "C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "CA", "CB", "CC", "CD", "CE", "CF", "D0", "D1", "D2", "D3", "D4", "D5", "D6", "D7", "D8", "D9", "DA", "DB", "DC", "DD", "DE", "DF", "E0", "E1", "E2", "E3", "E4", "E5", "E6", "E7", "E8", "E9", "EA", "EB", "EC", "ED", "EE", "EF", "F0", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "FA", "FB", "FC", "FD", "FE", "FF", 
};

	mCode::mCode(DWORD chunk_size)
	{
		InitializeListHead(&chunks);
		total = 0;
		last = NULL;
		get_result = NULL;
		plus_size = ALIGN_UP(chunk_size,0x1000);
	}

	mCode::~mCode()
	{
		clear();
	}

	DWORD mCode::length()
	{
		return total;
	}

	VOID mCode::clear()
	{
		for(PMSTRING_CHUNK current = (PMSTRING_CHUNK)chunks.Flink; current!=(PMSTRING_CHUNK)&chunks; )
		{
			PMSTRING_CHUNK next = (PMSTRING_CHUNK)current->entry.Flink;

			VirtualFree(current, NULL, MEM_RELEASE);

			current = next;
		}

		InitializeListHead(&chunks);
		total = 0;
		last  = NULL;

		if( get_result )
		{
			VirtualFree(get_result, NULL, MEM_RELEASE);
		}
	}

	void mCode::add(PCHAR string, DWORD slength)
	{
		if( !last || last->free < slength )
		{
			DWORD calc_size = ALIGN_UP(sizeof(MSTRING_CHUNK) + slength + plus_size,0x1000);
			PMSTRING_CHUNK chunk = (PMSTRING_CHUNK)VirtualAlloc(NULL,calc_size,MEM_COMMIT,PAGE_READWRITE);
			if( !chunk )
				return;

			InsertTailList(&chunks,&chunk->entry);

			chunk->length = 0;
			chunk->free = calc_size - sizeof(MSTRING_CHUNK);

			last = chunk;
		}

		mem_copy(&last->string[last->length],string,slength);

		last->length += slength;
		last->free   -= slength;

		total += slength;
	}

	void mCode::add_char(CHAR chr)
	{
		add(&chr, 1);
	}

	void mCode::add(PCHAR String)
	{
		add(String,lstrlenA(String));
	}

	PMSTRA mCode::convert_ascii(PWCHAR string, INT length)
	{
		INT		need_size;
		PMSTRA	ascii = NULL;
		
		need_size = WideCharToMultiByte(CP_ACP, 0, string, length, 0, 0, NULL, NULL);
		if( need_size > 0 )
		{
			if( (ascii = (PMSTRA)halloc( sizeof(MSTRA) + need_size)) )
			{
				if( !(ascii->length = WideCharToMultiByte(CP_ACP, 0, string, length, (PCHAR)ascii->string, need_size, NULL, NULL)) )
				{
					hfree( ascii);
					ascii = NULL;
				}
			}
		}

		return ascii;
	}

	PMSTRW mCode::convert_unicode(PCHAR string, INT length)
	{
		INT		need_size;
		PMSTRW	uni = NULL;

		need_size = MultiByteToWideChar(CP_ACP, 0, string, length,0,0);
		if( need_size > 0 )
		{
			if( (uni = (PMSTRW)halloc( sizeof(MSTRW) + (need_size << 1))) )
			{
				if( !(uni->length = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)string, length, (LPWSTR)uni->string, need_size)) )
				{
					hfree( uni);
					uni = NULL;
				}
			}
		}

		return uni;
	}

	void mCode::add(PWCHAR string)
	{
		PMSTRA ascii = convert_ascii(string, -1);
		if( ascii )
		{
			add(ascii->string, ascii->length - 1); // without null byte
		}
	}

	void mCode::add(mCode& code)
	{
		for(PMSTRING_CHUNK current = (PMSTRING_CHUNK)code.chunks.Flink; current!=(PMSTRING_CHUNK)&code.chunks; current = (PMSTRING_CHUNK)current->entry.Flink )
		{
			add(current->string,current->length);
		}
	}

	void mCode::set(PCHAR String)
	{
		clear();
		add(String);
	}

	PCHAR mCode::get()
	{
		if( !total )
			return NULL;

		if( get_result )
			VirtualFree(get_result,NULL,MEM_RELEASE);

		PCHAR get_result = (PCHAR)VirtualAlloc(NULL,total + 1,MEM_COMMIT,PAGE_READWRITE);
		if( get_result )
		{
			PMSTRING_CHUNK current = (PMSTRING_CHUNK)chunks.Flink;
			for(int pos = 0; current!=(PMSTRING_CHUNK)&chunks; )
			{
				mem_copy(&get_result[pos], current->string, current->length);
				pos += current->length;

				current = (PMSTRING_CHUNK)current->entry.Flink;
			}

			get_result[total] = 0;
		}

		return get_result;
	}

	void mCode::add_digit(DWORD Base, DWORD64 Digit)
	{
		CHAR	String[56];
		PCHAR	pString;

		if( Base==16 )
		{
			pString = String;

			*pString++ = '0';
			*pString++ = 'x';

			int count = 0;
			for(DWORD64 current = Digit; current ; current >>= 8)
			{
				count++;
			}

			if( !count )
			{
				*pString++ = HexAlpha256[ 0 ][0];
				*pString++ = HexAlpha256[ 0 ][1];
			}else{
				for(PBYTE pBytes = (PBYTE)&Digit + count - 1; count; count--, pBytes--)
				{
					*pString++ = HexAlpha256[ *pBytes ][0];
					*pString++ = HexAlpha256[ *pBytes ][1];
				}
			}

			*pString++ = 0;
			add(String);
		}else{
			pString = String + sizeof(String) - 1;   // Set psz to last char
			*pString = 0;  // Set terminating null

			PCHAR alpha = "0123456789";

			do
			{
				--pString;                     // Move back
				*pString = alpha[ Digit % 10]; // Place the digit

				Digit /= 10;
			} while(Digit != 0);

			add(pString);
		}
	}

mCode& operator << (mCode& code,WCHAR* str)
{
	code.add(str);
	return code;
}

mCode& operator << (mCode& code,CHAR* str) 
{
	code.add(str);
	return code;
}

mCode& operator << (mCode& code,DWORD64 Digit) 
{
	code.add_digit(16,Digit);
	return code;
}

