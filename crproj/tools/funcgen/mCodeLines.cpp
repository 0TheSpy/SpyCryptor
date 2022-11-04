#include <windows.h>
#include <tchar.h>
#include <intrin.h>
#include <Strsafe.h>

#include "globals.h"
#include "mCodeLines.h"

mCodeLines::mCodeLines(mCodeGenerator* generator, PMCODE_BLOCK parent)
{
	current_parent = 0;
	parents[current_parent] = parent;

	last_block = NULL;
	this->generator = generator;
	push_was = false;

	trash_flags = NULL;
}

mCodeLines::~mCodeLines()
{

}

void mCodeLines::set_trash_flags(DWORD flags)
{
	trash_flags = flags;
}

void mCodeLines::gen_trash()
{
	DWORD flags = trash_flags;

	PMCODE_BLOCK new_parent = generator->generate(parents[current_parent],config.vars.decryptor.trash_between.min,config.vars.decryptor.trash_between.max, flags);
	if( new_parent!=parents[current_parent] )
	{
		if( current_parent < sizeof(parents)/sizeof(PMCODE_BLOCK) )
		{
			current_parent++;
			parents[current_parent] = new_parent;

			push_was = true;
		}
	}
}

PMCODE_BLOCK mCodeLines::add_ex(DWORD flags, PCHAR format, ...)
{
	if( push_was )
	{
		if( !(flags & MCODELINE_FLAG_NO_TRASH) )
		{
			gen_trash();
		}

		push_was = false;
	}

	if( (last_block = generator->get_graph()->allocate(parents[current_parent], CEBT_CUSTOM)) )
	{
		va_list argptr;

		va_start(argptr, format);
		wvsprintfA(last_block->custom.string, format, argptr);
		va_end(argptr);

		if( !(flags & MCODELINE_FLAG_NO_TRASH) )
		{
			gen_trash();
		}
	}

	return last_block;
}

PMCODE_BLOCK mCodeLines::add(PCHAR format, ...)
{
	if( push_was )
	{
		gen_trash();
		push_was = false;
	}

	if( (last_block = generator->get_graph()->allocate(parents[current_parent], CEBT_CUSTOM)) )
	{
		va_list argptr;

		va_start(argptr, format);
		wvsprintfA(last_block->custom.string, format, argptr);
		va_end(argptr);

		gen_trash();
	}

	return last_block;
}

void mCodeLines::add_first(DWORD flags, PCHAR format, ...)
{
	if( (last_block = generator->get_graph()->allocate_ex(parents[current_parent], CEBT_CUSTOM, FALSE)) )
	{
		va_list argptr;

		va_start(argptr, format);
		wvsprintfA(last_block->custom.string, format, argptr);
		va_end(argptr);

		PMCODE_BLOCK block = (PMCODE_BLOCK)generator->get_graph()->get_inner(parents[current_parent]);
		InsertTailList(&block->entry, &last_block->entry);

		if( !(flags & MCODELINE_FLAG_NO_TRASH) )
		{
			gen_trash();
		}
	}
}

void mCodeLines::add_random(PCHAR format, ...)
{
	if( (last_block = generator->get_graph()->allocate_ex(parents[current_parent], CEBT_CUSTOM, FALSE)) )
	{
		va_list argptr;

		va_start(argptr, format);
		wvsprintfA(last_block->custom.string, format, argptr);
		va_end(argptr);

		DWORD count = generator->get_graph()->get_inner_count(parents[current_parent]);
		DWORD pos = count ? random.get_less(0, count) : 0;

		PMCODE_BLOCK block = (PMCODE_BLOCK)generator->get_graph()->get_inner(parents[current_parent]);
		while( pos )
		{
			block = (PMCODE_BLOCK)block->entry.Flink;
			pos--;
		}

		InsertHeadList(&block->entry, &last_block->entry);
	}
}

void mCodeLines::push()
{
	if( current_parent < sizeof(parents)/sizeof(PMCODE_BLOCK) )
	{
		current_parent++;
		parents[current_parent] = last_block;

		push_was = true;
	}
}

void mCodeLines::pop()
{
	if( current_parent )
	{
		current_parent--;
		push_was = true;
	}
}

