#include <windows.h>
#include <tchar.h>
#include <intrin.h>
#include <Strsafe.h>

#include "globals.h"

// globals
mChunkUnitRandom<mFunction> all_functions(NULL);

PCHAR ConvetionStrings[] = {
	"",
	"__stdcall",
	"__cdecl",
	"__fastcall",
};



mFunction::mFunction(mVars* Globals) 
	: locals(VARS_TYPE_LOCALS, NULL)
	, formals(VARS_TYPE_FORMALS, NULL)
	, code_generator(Globals, &locals, &formals)
{
	globals = Globals;

	id = all_functions.get_count();

	all_functions.add_reference(this);

	set_return(MVT_INT32); // MVT_VOID
	set_name(NULL);
	set_convention(FC_STDCALL);

	user_data = NULL;
}

mFunction::~mFunction()
{
}

void mFunction::set_user_data(DWORD data)
{
	user_data = data;
}

DWORD mFunction::get_user_data()
{
	return user_data;
}

PMCODE_BLOCK mFunction::generate(DWORD min, DWORD max, DWORD flags)
{
	return code_generator.generate(NULL, min, max, flags);
}

void mFunction::set_name(PCHAR format, ...)
{
	if( format )
	{
		va_list argptr;

		va_start(argptr, format);
		wvsprintfA(name, format, argptr);
		va_end(argptr);
	}else{
		wsprintfA(name, "function_trash_%d", id);
	}
}

PCHAR mFunction::get_name()
{
	return name;
}

void mFunction::set_convention(FUNCTION_CONVENTION Convention)
{
	if( Convention==FC_RANDOM )
	{
		Convention = (FUNCTION_CONVENTION)random.get_equal(1, REAL_CONVETIONS);
	}

	convention = Convention;
}

void mFunction::set_return(MVAR_TYPE ret_type)
{
	mem_zero(&ret_exp, sizeof(ret_exp));

	ret_mvar_type = ret_type;

	/*if( ret_type!=MVT_VOID )
	{
		mCodeExpression exp(&ret_exp);

		PMVAR_INFO var = code_generator.get_random_var_by_type(VG_CODE, ret_type, 0, NULL);
		if( var )
		{
			exp.add_var(var);
		}else{
			exp.add_imm( random.get_less(0,255) );
		}
	}*/

	if (ret_type != MVT_VOID)
	{
		mCodeExpression exp(&ret_exp);

		MVAR_TYPE types[] = {
			MVT_INT8,
			MVT_INT16,
			MVT_INT32
		};
		int start;

		switch (ret_type)
		{
		case MVT_INT8: start = 0;
		case MVT_INT16: start = 1;
		case MVT_INT32: start = 2;
		}

		PMVAR_INFO var = NULL;
		for (int i = start; i >= 0 && !var; i--)
		{
			var = code_generator.get_random_var_by_type(IS_PROC | IS_TRASH, types[i], 0, NULL);
		}

		if (var)
		{
			exp.add_var(var);
		}
		else{
			exp.add_imm(random.get_less(0, 255));
		}
	}
}

INT64 mFunction::get_return_value()
{
	mCodeExpression exp(&ret_exp);

	return exp.emulate(ret_mvar_type);
}

MVAR_INFO* mFunction::add_formal(MVAR_TYPE Type, PCHAR Name, DWORD Flags, PVOID Value)
{
	return formals.add(VG_CODE, Type, Name, Flags, Value);
}

mVars* mFunction::get_formals()
{
	return &formals;
}

mVars* mFunction::get_locals()
{
	return &locals;
}

mCodeGenerator* mFunction::get_code_generator()
{
	return &code_generator;
}

void mFunction::get_prototype_string(mCode &out)
{
	out.add( mVars::get_type_name(ret_mvar_type) );
	out.add(" ");
	out.add(ConvetionStrings[convention]);
	out.add(" ");	
	out.add(name);
	out.add("(");

	// add formal variables
	MVAR_INFO *var;
	INT i;

	for(var = formals.first(), i = 0; var ; var = formals.next(), i++)
	{
		if( i )
			out.add(", ");

		out << var->type_str << " " << var->name;
	}
	 
	out.add(");\r\n");
}

void mFunction::get_string(mCode &out)
{
	out.add( mVars::get_type_name(ret_mvar_type) );
	out.add(" ");
	out.add(ConvetionStrings[convention]);
	out.add(" ");	
	out.add(name);
	out.add("(");

	// add formal variables
	MVAR_INFO *var;
	INT i;

	for(var = formals.first(), i = 0; var ; var = formals.next(), i++)
	{
		if( i )
			out.add(", ");

		out << var->type_str << " " << var->name;
	}

	out.add(")\r\n{\r\n");

	PMVAR_INFO *random_locals = (PMVAR_INFO*)halloc( locals.length()*sizeof(PMVAR_INFO) ); 
	for(i = 0, var = locals.first(); var ; var = locals.next())
	{
		random_locals[i++] = var;
	}

	shuffle<PMVAR_INFO>(random_locals, locals.length());

	for(i = 0; i < locals.length(); i++)
	{
		out.add("\t");
		out.add( random_locals[i]->string );
		out.add(";\r\n");
	}

	

	out.add("\r\n");
	code_generator.get_string(out);

	if( ret_mvar_type!=MVT_VOID )
	{
		out << "\treturn ";

		mCodeExpression exp(&ret_exp);
		exp.get_string(out);

		/*for (i = 0; i < locals.length(); i++)
		{
			if (random_locals[i]->vars_type == MVT_INT8)
			{
				out.add(random_locals[1]->name);
			}
		}*/

		
		out << ";\r\n";
	}

	hfree(random_locals);

	out.add("}\r\n\r\n");
}
