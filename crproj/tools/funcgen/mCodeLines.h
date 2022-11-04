#ifndef __M_CODE_LINES__
#define __M_CODE_LINES__

#define MCODELINE_FLAG_NO_TRASH 0x1

class mCodeLines
{
	PMCODE_BLOCK	parents[128]; // max C++ inner blocks
	DWORD			current_parent;
	PMCODE_BLOCK	last_block;
	BOOL			push_was;
	DWORD			trash_flags;

	mCodeGenerator*	generator;

	void gen_trash();
public:
	mCodeLines(mCodeGenerator* generator, PMCODE_BLOCK parent);
	~mCodeLines();

	void add_first(DWORD flags, PCHAR format, ...);
	PMCODE_BLOCK add_ex(DWORD flags, PCHAR format, ...);
	PMCODE_BLOCK add(PCHAR format, ...);
	void add_random(PCHAR format, ...);
	void set_trash_flags(DWORD flags);
	void push();
	void pop();
};

#endif