#include <windows.h>
#include <tchar.h>
#include <intrin.h>
#include <Strsafe.h>

#include "globals.h"
#include "mCodeGenerator.h"

DWORD debug_counter = 0;

mCodeGenerator::mCodeGenerator(mVars *Globals,mVars *Locals, mVars *Formals) 
	: graph(Globals,Locals,Formals)
	, safe_random(NULL)
{
	globals = Globals;
	locals = Locals;
	formals = Formals;

	mem_set(tabs,'\t', 0xFF);

	calls_idx = 0;
	loop_idx = 0;

	// setup random block generator
	code_type.add(CEBT_IF, config.code.usage._if);
	code_type.add(CEBT_LOOP, config.code.usage.loop);
	code_type.add(CEBT_LINE, config.code.usage.code);
	code_type.add(CEBT_PTR_LINE, config.code.usage.ptrcode);
	code_type.add(CEBT_API, config.code.usage.api);
	code_type.add(CEBT_SWITCH, config.code.usage._switch);
	code_type.add(CEBT_PROC, config.code.usage.proc);

	vusage_left.add(VAR_USE_LOCAL,  config.vars.usage_left.locals);
	vusage_left.add(VAR_USE_GLOBAL, config.vars.usage_left.globals);
	vusage_left.add(VAR_USE_FORMAL, config.vars.usage_left.formals);

	vusage_right.add(VAR_USE_LOCAL,  config.vars.usage_right.locals);
	vusage_right.add(VAR_USE_GLOBAL, config.vars.usage_right.globals);
	vusage_right.add(VAR_USE_FORMAL, config.vars.usage_right.formals);

	locals_first_pos = globals_first_pos = 0;
	locals_first_count = globals_first_count = 0;

	for(int i = 0; i < 8; i++)
	{
		vtypes.add((MVAR_TYPE)i, config.vars.types[i]);

		if( config.vars.types[i] )
		{
			locals_first[locals_first_count++] = (MVAR_TYPE)i;
			globals_first[globals_first_count++] = (MVAR_TYPE)i;
		}
	}

	mem_copy(&locals_first[locals_first_count],locals_first, locals_first_count*sizeof(MVAR_TYPE));
	mem_copy(&globals_first[globals_first_count],globals_first, globals_first_count*sizeof(MVAR_TYPE));

	locals_first_count += locals_first_count;
	globals_first_count += globals_first_count;

	// shuffle 
	for(int i = 0; i < locals_first_count; i++)
	{
		DWORD pos = random.get_less(0, locals_first_count);

		MVAR_TYPE tmp = locals_first[i];
		locals_first[i] = locals_first[pos];
		locals_first[pos] = tmp;
	}

	for(int i = 0; i < globals_first_count; i++)
	{
		DWORD pos = random.get_less(0, globals_first_count);

		MVAR_TYPE tmp = globals_first[i];
		globals_first[i] = globals_first[pos];
		globals_first[pos] = tmp;
	}

	cblock_ops.add(CEO_ADD, config.code.block_code.add);
	cblock_ops.add(CEO_SUB, config.code.block_code.sub);
	cblock_ops.add(CEO_MUL, config.code.block_code.mul);
	cblock_ops.add(CEO_DIV, config.code.block_code.div);
	cblock_ops.add(CEO_MOD, config.code.block_code.mod);
	cblock_ops.add(CEO_SAL, config.code.block_code.shl);
	cblock_ops.add(CEO_SAR, config.code.block_code.shr);
	cblock_ops.add(CEO_AND, config.code.block_code.and);
	cblock_ops.add(CEO_XOR, config.code.block_code.xor);
	cblock_ops.add(CEO_OR,  config.code.block_code.or);
}

mCodeEmulator* mCodeGenerator::get_graph()
{
	return &graph;
}

mVars* mCodeGenerator::get_locals()
{
	return locals;
}

mVars* mCodeGenerator::get_formals()
{
	return formals;
}

mVars* mCodeGenerator::get_globals()
{
	return globals;
}

PMVAR_INFO mCodeGenerator::get_random_var_by_random_type(DWORD flags, MVAR_TYPE type, DWORD except, MVAR_INFO* except_vars[])
{
	MVAR_TYPE	types[2];

	types[0] = type;

	if( MVAR_TYPE_IS_PTR(type) )
	{
		types[1] = MVAR_PTR_TO_TYPE(type);
	}else{
		types[1] = MVAR_TYPE_TO_PTR(type);
	}

	for(int i = 0; i < sizeof(types)/sizeof(MVAR_TYPE); i++)
	{
		DWORD		pos = random.get_less(i, sizeof(types)/sizeof(MVAR_TYPE));
		PMVAR_INFO	var = get_random_var_by_type(flags, types[pos], except, except_vars);
		if( var )
			return var;

		// move not need anymore to left side
		types[pos] = types[i];
	}

	return NULL;
}

MVAR_INFO* mCodeGenerator::get_random_var_by_type(DWORD flags, MVAR_TYPE Type, DWORD except, MVAR_INFO* except_vars[])
{
	MVARS_GROUP group;
	MVAR_INFO *var = NULL;

	// дл€ того чтобы небыло больгого стэка, мы вначале пробуем вз€ть какие то перменые из VG_CODE, и если нет, обычно в начале, тогда из VG_CODE_TRASH
	if (flags & IS_TRASH)
	{
		DWORD new_flags = flags & ~IS_TRASH;
		if ((var = get_random_var_by_type(new_flags, Type, except, except_vars)))
			return var;

		// нет переменных в коде берем из треше
		group = VG_CODE_TRASH;
	}
	else{
		group = VG_CODE;
	}

	VAR_USAGE use = (VAR_USAGE)vusage_right.get();

	if ((flags & IS_PROC) && use == VAR_USE_GLOBAL)
		use = VAR_USE_LOCAL;

	switch (use)
	{
	case VAR_USE_GLOBAL:
		var = globals->get_random(group, Type, except, except_vars);
		break;
	case VAR_USE_LOCAL:
		var = locals->get_random(group, Type, except, except_vars);
		break;
	case VAR_USE_FORMAL:
		var = formals->get_random(group, Type, except, except_vars);
		if (!var)
		{
			if ((flags & IS_PROC) || random.get_less(0, 100) < 50)
			{
				var = locals->get_random(group, Type, except, except_vars);
			}
			else{
				var = globals->get_random(group, Type, except, except_vars);
			}
		}
		break;
	}

	if (!var) // try find in other arrays
	{
		for (int i = 0; i < 3; i++)
		{
			if (i == use) continue;

			switch (i)
			{
			case VAR_USE_LOCAL:
				var = locals->get_random(group, Type, except, except_vars);
				break;
			case VAR_USE_GLOBAL:
				var = globals->get_random(group, Type, except, except_vars);
				break;
			case VAR_USE_FORMAL:
				var = formals->get_random(group, Type, except, except_vars);
			}

			if (var) break;
		}
	}

	return var;
}

MVAR_INFO* mCodeGenerator::add_or_get_random_var(DWORD flags, BOOL isPtr)
{
	MVARS_GROUP group;
	MVAR_INFO *var = NULL;

	// дл€ того чтобы небыло больгого стэка, мы вначале пробуем вз€ть какие то перменые из VG_CODE, и если нет, обычно в начале, тогда из VG_CODE_TRASH
	if( flags & IS_TRASH )
	{
		DWORD new_flags = flags & ~IS_TRASH;
		if( (var = get_random_var_or_any(new_flags, isPtr)) )
			return var;

		// нет переменных в коде берем из треше
		group = VG_CODE_TRASH;
	}else{
		group = VG_CODE;
	}

	VAR_USAGE use = (VAR_USAGE)vusage_left.get();
	MVAR_TYPE  vtype;

	// TODO: support formals
	if( use==VAR_USE_FORMAL )
		use = VAR_USE_LOCAL;

	if ((flags & IS_PROC) && use == VAR_USE_GLOBAL) use = VAR_USE_LOCAL;

	switch( use )
	{
		case VAR_USE_GLOBAL:
			if( globals_first_pos < globals_first_count )
			{
				vtype = globals_first[globals_first_pos++];
			}else{
				vtype = (MVAR_TYPE)vtypes.get();
			}

			if( isPtr )
			{
				// in ptr first check exists var for ptr ?
				if( !globals->get_random(group, vtype, 0, NULL) )
					return NULL;

				vtype = MVAR_TYPE_TO_PTR(vtype);
			}
				
			return globals->add_or_get_random(group, vtype, NULL);
		case VAR_USE_LOCAL:
			if( locals_first_pos < locals_first_count )
			{
				vtype = locals_first[locals_first_pos++];
			}else{
				vtype = (MVAR_TYPE)vtypes.get();
			}

			if( isPtr )
			{
				// in ptr first check exists var for ptr ?
				if( !locals->get_random(group, vtype, 0, NULL) )
					return NULL;

				vtype = MVAR_TYPE_TO_PTR(vtype);
			}

			return locals->add_or_get_random(group, vtype, NULL);
	}

	return NULL;
}

MVAR_INFO* mCodeGenerator::get_random_var_or_any(DWORD flags, BOOL isPtr)
{
	MVARS_GROUP group;
	MVAR_INFO *var = NULL;

	// дл€ того чтобы небыло больгого стэка, мы вначале пробуем вз€ть какие то перменые из VG_CODE, и если нет, обычно в начале, тогда из VG_CODE_TRASH
	if( flags & IS_TRASH )
	{
		DWORD new_flags = flags & ~IS_TRASH;
		if( (var = get_random_var_or_any(new_flags, isPtr)) )
			return var;

		// нет переменных в коде берем из треше
		group = VG_CODE_TRASH;
	}else{
		group = VG_CODE;
	}

	DWORD use = vusage_left.get();
	MVAR_TYPE  vtype = (MVAR_TYPE)vtypes.get();

	if( isPtr )
		vtype = MVAR_TYPE_TO_PTR(vtype);

	BYTE	used[3];
	mVars*	used_vars[] = {
		locals,
		globals,
		formals
	};

	mem_set(used, 0, sizeof(used));

	if (flags & IS_PROC)
		used[1] = 1;

	for(int i = 0; i < sizeof(used); i++)
	{
		for(int j = 0; j < 3 && used[use]; j++)
		{
			if( use < sizeof(used) - 1)
			{
				use++;
			}else{
				use = 0;
			}
		}

		if( used[use] )
			return NULL;

		DWORD types[VARS_COUNT];
		DWORD pos, total;

		mVars* vars = used_vars[use];

		var = vars->get_random(group, vtype, NULL, NULL); 
		if( !var )
		{
			if( (total = vars->get_types_count(group, types)) )		
			{
				DWORD begin, end;

				if( isPtr )
				{
					pos = random.get_equal(MVT_PINT8, MVT_PUINT64);

					begin = (DWORD)MVT_PINT8;
					end   = (DWORD)MVT_PUINT64 + 1;
				}else{
					pos = random.get_equal(MVT_INT8, MVT_UINT64);

					begin = (DWORD)MVT_INT8;
					end   = (DWORD)MVT_UINT64 + 1;
				}

				for(int t = begin; types[pos]==0 && t < end; t++)
				{
					if( pos < end - 1 )
					{
						pos++;
					}else{
						pos = begin;
					}
				}
			
				if( types[pos]!=0 )
					var = vars->get_random(group, (MVAR_TYPE)pos, NULL, NULL);
			}
		}

		if( var )
			return var;

		used[ use ] = 1;
	}

	return NULL;
}

DWORD64 mCodeGenerator::get_imm_by_operation(MVAR_INFO* left_var,CODE_EXPRESSION_OPERATORS op)
{
	INT64 imm;

	mVars::get_imm_by_type(&imm,left_var->type);

	// can not imm==0 on *,/,%
	while( imm==0 && (op==CEO_DIV || op==CEO_MUL || op==CEO_MOD) )
	{
		mVars::get_imm_by_type(&imm,left_var->type);
	}

	if( op==CEO_SAL || op==CEO_SAR )
	{
		switch( left_var->type )
		{
			case MVT_PINT8:
			case MVT_PUINT8:
			case MVT_INT8:
			case MVT_UINT8:
				imm &= 0x7;
				break;
			case MVT_PINT16:
			case MVT_PUINT16:
			case MVT_INT16:
			case MVT_UINT16:
				imm &= 0xF;
				break;
			case MVT_PINT32:
			case MVT_PUINT32:
			case MVT_INT32:
			case MVT_UINT32:
				imm &= 0x1F;
				break;
			case MVT_PINT64:
			case MVT_PUINT64:
			case MVT_INT64:
			case MVT_UINT64:
				imm &= 0x3F;
				break;
		}

		if( !imm )
			imm = 1;
	}

	return imm;
}

BOOL mCodeGenerator::is_null(PMVAR_INFO var)
{
	if( var )
	{
		switch( var->type )
		{
			case MVT_PINT8:
			case MVT_PUINT8:
			case MVT_INT8:	
			case MVT_UINT8:
				return var->value->i8==0;
			case MVT_PINT16:
			case MVT_PUINT16:
			case MVT_INT16:
			case MVT_UINT16:
				return var->value->i16==0;
			case MVT_PINT32:
			case MVT_PUINT32:
			case MVT_INT32:
			case MVT_UINT32:
				return var->value->i32==0;
			case MVT_PINT64:
			case MVT_PUINT64:
			case MVT_INT64:
			case MVT_UINT64:
				return var->value->i64==0;
		}
	}

	return FALSE;
}

CODE_EXPRESSION_OPERATORS mCodeGenerator::get_op_by_value(INT64 result, DWORD flags)
{
	CODE_EXPRESSION_OPERATORS sg_op = (CODE_EXPRESSION_OPERATORS)cblock_ops.get();

	if( flags & IS_LOOP )
	{
		// prevent *,%,/ in loop, because may be div by zero
		while( sg_op==CEO_DIV || sg_op==CEO_MUL || sg_op==CEO_MOD )
		{
			sg_op = (CODE_EXPRESSION_OPERATORS)cblock_ops.get();
		}
	}else{
		// bugfix: prevent /,*,% to null, prevent integer overflow, division by zero
		while( ( !result || (result & 0x80000000) ) && (sg_op==CEO_DIV || sg_op==CEO_MUL || sg_op==CEO_MOD) )
		{
			sg_op = (CODE_EXPRESSION_OPERATORS)cblock_ops.get();
		}
	}

	return sg_op;
}

PMCODE_BLOCK mCodeGenerator::generate_code(PMCODE_BLOCK parent, DWORD interations, DWORD flags, BOOL isPtr)
{
	PMVAR_INFO	var;
	MVAR_INFO*	var_a;

	//if( isPtr ) __debugbreak();

	if( !(flags & IS_LOOP) ) // allow all except loop
	{
		var = add_or_get_random_var(flags, isPtr);
	}else{
		var = get_random_var_or_any(flags, isPtr);
	}

	if( !var )
		return NULL;

	PMVAR_INFO	not_vars[2];
	MVAR_TYPE	types[2];

	if( isPtr )
	{
		if( !(var->flags & MVF_INITIALIZED) )
		{
			// if not initialized and no var with this type, exit
			var_a = get_random_var_by_type(flags, MVAR_PTR_TO_TYPE(var->type), 0, NULL);
			if( !var_a )
				return NULL;		
		}else{
			var_a = get_random_var_by_random_type(flags, var->type, 0, NULL);
		}
	}else{
		var_a = get_random_var_by_random_type(flags, var->type, 1, &var);
	}

	PMCODE_BLOCK block = graph.allocate(parent, CEBT_LINE);
	if( !block )
		return FALSE;

	//if( !lstrcmpA(var->name,"loc159") && !lstrcmpA(var_a->name,"loc154") )
	//	__debugbreak();

	block->code.left_var = var;

	mCodeExpression expression(&block->code.expression);

	expression.clear();

	MVAR_INFO* var_b;
	INT64 group_result = 0;
	int cur;

	// var = imm;
	// var = var; // any
	// var = var + imm; // any
	// var = var + var; // any

	if( !(block->code.left_var->flags & MVF_INITIALIZED) )
	{
		if( isPtr )
		{
			block->code.left_var->value = var_a->value;

			expression.add_var_addr(var_a);
		}else{
			if( var_a ) // var = var_a
			{
				if( !(flags & IS_TRASH) )
				{
					mCodeExpressionEmulator::emulate(&group_result, CEO_EQUAL, var_a->type, &var_a->value->i64);
				}

				expression.add_var(var_a);
			}else{ // var = imm
				mVars::get_imm_by_type(&group_result,block->code.left_var->type);

				expression.add_imm(group_result);
			}
		}
	}else{

		if( !var_a )
		{
			var_a = block->code.left_var;
		}

		expression.add_var(var_a);

		if( !(flags & IS_TRASH) )
		{
			mCodeExpressionEmulator::emulate(&group_result, CEO_EQUAL, var_a->type, &var_a->value->i64);
		}

		CODE_EXPRESSION_OPERATORS sg_op = get_op_by_value(group_result, flags);

		not_vars[0] = block->code.left_var;
		not_vars[1] = var_a;

		var_b = get_random_var_by_random_type(flags, var_a->type, 2, not_vars);

		if( var_b ) // check for var
		{
			if( var_a==var_b ) // prevent var1 [-=/%*&] var1
			{
				var_b = NULL; 
			}else{
				if( sg_op==CEO_DIV || sg_op==CEO_MUL || sg_op==CEO_MOD )
				{
					// prevent *,/,% to NULL
					if( is_null(var_b) )
					{
						var_b = NULL;
					}
				}
			}
		}

		expression.add_op(sg_op);

		INT64 imm;

		if( !var_b ) // check try imm ?
		{
			imm = get_imm_by_operation(block->code.left_var,sg_op);

			// bugfix: to imm set MVT_INT64 because if it be less MVT_INT32, then in emulator_... proc it will grow to MVT_INT32 and result will be not correct
			if( !(flags & IS_TRASH) )
			{
				mCodeExpressionEmulator::emulate(&group_result, sg_op, block->code.left_var->type, &imm);
			}

			expression.add_imm(imm);
		}else{
			if( !(flags & IS_TRASH) )
			{
				mCodeExpressionEmulator::emulate(&group_result, sg_op, var_b->type, &var_b->value->i64);
			}

			expression.add_var(var_b);
		}

	}

	if( !(flags & IS_TRASH) )
	{
		if( isPtr )
		{
			// if initilized
			if( (block->code.left_var->flags & MVF_INITIALIZED) )
			{
				mCodeExpressionEmulator::emulate_end(&block->code.left_var->value->i64, block->code.left_var->type, &group_result);

#ifdef _DEBUG
				wsprintfA(block->comment, "ptr code %s == 0x%0.8X", block->code.left_var->name, block->code.left_var->value->ui32);
#endif
#ifdef CONFIG_DEBUG_EMULATOR
				if( !(flags & IS_LOOP) && !(flags & IS_TRASH) )
				{
					PCHAR tostr;
					switch( block->code.left_var->type )
					{
						case MVT_PINT8:
						case MVT_PUINT8:
							tostr = "PUINT8";
							break;
						case MVT_PINT16:
						case MVT_PUINT16:
							tostr = "PUINT16";
							break;
						case MVT_PINT32:
						case MVT_PUINT32:
							tostr = "PUINT32";
							break;
					}
					PMCODE_BLOCK dblock = graph.allocate(parent,CEBT_CUSTOM);
					wsprintfA(dblock->custom.string, "if( *(%s)%s!=0x%X ) __debugbreak();\r\n", tostr, block->code.left_var->name, block->code.left_var->value->ui32);
				}
#endif
			}else{
#ifdef CONFIG_DEBUG_EMULATOR
				if( !(flags & IS_LOOP) && !(flags & IS_TRASH) )
				{

					PCHAR tostr;
					switch( block->code.left_var->type )
					{
						case MVT_PINT8:
						case MVT_PUINT8:
							tostr = "UINT8";
							break;
						case MVT_PINT16:
						case MVT_PUINT16:
							tostr = "UINT16";
							break;
						case MVT_PINT32:
						case MVT_PUINT32:
							tostr = "UINT32";
							break;
					}

					PMCODE_BLOCK dblock = graph.allocate(parent,CEBT_CUSTOM);
					wsprintfA(dblock->custom.string, "if( (%s)*%s!=0x%X ) __debugbreak();\r\n", tostr, block->code.left_var->name, (DWORD)block->code.left_var->value->ui32);
				}
#endif
				// do nothing
#ifdef _DEBUG
				lstrcpyA(block->comment, "ptr init");
#endif
			}
		}else{
			mCodeExpressionEmulator::emulate_end(&block->code.left_var->value->i64, block->code.left_var->type, &group_result);

#ifdef CONFIG_DEBUG_EMULATOR
			if( !(flags & IS_LOOP) && !(flags & IS_TRASH) )
			{
				PCHAR tostr;
				switch( block->code.left_var->type )
				{
				case MVT_INT8:
				case MVT_UINT8:
					tostr = "UINT8";
					break;
				case MVT_INT16:
				case MVT_UINT16:
					tostr = "UINT16";
					break;
				case MVT_INT32:
				case MVT_UINT32:
					tostr = "UINT32";
					break;
				}
				PMCODE_BLOCK dblock = graph.allocate(parent,CEBT_CUSTOM);
				wsprintfA(dblock->custom.string, "if( (%s)%s!=0x%X ) __debugbreak();\r\n", tostr, block->code.left_var->name, block->code.left_var->value->ui32);
			}
#endif

#ifdef _DEBUG
			wsprintfA(block->comment, "code %s == 0x%0.8X", block->code.left_var->name, block->code.left_var->value->ui32);
#endif
		}

		block->code.left_var->flags |= MVF_INITIALIZED;
	}else{
#ifdef _DEBUG
		lstrcpyA(block->comment, " trash");
#endif
	}

	if( !(flags & IS_LOOP) && !(flags & IS_TRASH) )
	{
		safe_random.add_reference(block);
	}

	return block;
}


PMCODE_BLOCK mCodeGenerator::generate_api(PMCODE_BLOCK parent, DWORD interations, DWORD flags)
{
	mCode str(128);

	if (!import.get_api(globals, locals, formals, str, (interations == 1 ? false : true), (flags & IS_TRASH ? true : false)))
		return NULL;
	
	PMCODE_BLOCK block = graph.allocate(parent, CEBT_CUSTOM);
	PCHAR out = str.get();

	StringCbCopyA(block->custom.string, sizeof(block->custom.string), out);

#ifdef _DEBUG

	if( flags & IS_TRASH )
	{
		lstrcpyA(block->comment, " trash");
	}else{
		lstrcpyA(block->comment, " code");
	}

#endif

	if( !(flags & IS_LOOP) && !(flags & IS_TRASH) )
	{
		safe_random.add_reference(block);
	}

	return block;
}

DWORD mCodeGenerator::generate_if(PMCODE_BLOCK parent, PMCODE_BLOCK chain[], PMCODE_BLOCK *new_parent, DWORD level, DWORD left, DWORD interations, DWORD flags)
{
	if( (flags & IS_LOOP) || level > config.code.block_if.max_levels || left < (config.code.block_if.inner_blocks.min + 1)*interations ) 
		return NULL;

	DWORD		vars_count;
	MVAR_INFO*	vars[4];

	if( !locals_first_pos )
		return NULL;

	DWORD		 chain_count = 0;

	*new_parent = NULL;

	DWORD chain_size = random.get_less(config.code.block_if.chains.min,config.code.block_if.chains.max);
	DWORD chain_id;
	BOOL  is_trash;
	BOOL  is_equal = false; // finded new if statement, in which we can in

	for(chain_id = 0; chain_id < chain_size; chain_id++)
	{
		MVAR_TYPE type = locals_first[ random.get_less(0,locals_first_pos) ];

		vars_count = 0;

		vars[vars_count] = get_random_var_by_random_type(flags, type, NULL, NULL);
		if( !vars[vars_count] )
			break;

		vars_count++;

		INT64	result = 0;

		while( vars_count < 4)
		{
			vars[vars_count] = get_random_var_by_random_type(flags, type, vars_count, vars);
			if( !vars[vars_count] )
				break;

			vars_count++;
		}

		CODE_EMULATOR_BLOCK_TYPES block_type;
		if( chain_id==0 )
		{
			block_type = CEBT_IF;
		}else if( chain_id + 1 == chain_size )
		{
			if( random.get_less(0,2) )
			{
				block_type = CEBT_ELSEIF;
			}else{
				block_type = CEBT_ELSE;
			}
		}else{
			block_type = CEBT_ELSEIF;
		}

		PMCODE_BLOCK block = graph.allocate(parent, block_type);
		if( !block )
			break;

		mCodeExpression exp_left(&block->compare.left);
		mCodeExpression exp_right(&block->compare.right);

		INT64	val_left, val_right, imm;

		CODE_EXPRESSION_OPERATORS op_left, op_right;

		block->compare.type = vars[0]->type;

		switch( vars_count )
		{
			case 1:
				mCodeExpressionEmulator::emulate(&val_left, CEO_EQUAL, vars[0]->type, &vars[0]->value->i64);

				// только в начала может быть 1 тип, и чтобы сбить сигнатру, добавим числа
				// если не хватка переменных, то хот€бы разбавим это числами
				if( if_last_vars[0]==vars[0] )
				{
					do 
					{
						mVars::get_imm_by_type(&imm,vars[0]->type);
						op_left = get_op_by_value(val_left, flags);
					} while ( !imm && (op_left==CEO_DIV || op_left==CEO_MUL || op_left==CEO_MOD) );

					mCodeExpressionEmulator::emulate(&val_left, op_left, vars[0]->type, &imm);
				}

				mVars::get_imm_by_type(&val_right,vars[0]->type);

				exp_left.add_var(vars[0]);

				if( if_last_vars[0]==vars[0] )
				{
					exp_left.add_op(op_left);
					exp_left.add_imm(imm);
				}

				exp_right.add_imm(val_right);
				break;
			case 2:
				mCodeExpressionEmulator::emulate(&val_left, CEO_EQUAL, vars[0]->type, &vars[0]->value->i64);
					
				do 
				{
					mVars::get_imm_by_type(&imm,vars[0]->type);
					op_left = get_op_by_value(val_left, flags);
				} while ( !imm && (op_left==CEO_DIV || op_left==CEO_MUL || op_left==CEO_MOD) );

				mCodeExpressionEmulator::emulate(&val_left, op_left, vars[0]->type, &imm);

				exp_left.add_var(vars[0]);
				exp_left.add_op(op_left);
				exp_left.add_imm(imm);
					
				mCodeExpressionEmulator::emulate(&val_right, CEO_EQUAL, vars[1]->type, &vars[1]->value->i64);

				exp_right.add_var(vars[1]);
				break;
			case 3:
				mCodeExpressionEmulator::emulate(&val_left, CEO_EQUAL, vars[0]->type, &vars[0]->value->i64);

				do 
				{
					op_left = get_op_by_value(val_left, flags);
				} while ( !vars[1]->value->i64 && (op_left==CEO_DIV || op_left==CEO_MUL || op_left==CEO_MOD) );

				mCodeExpressionEmulator::emulate(&val_left, op_left, vars[1]->type, &vars[1]->value->i64);

				mCodeExpressionEmulator::emulate(&val_right, CEO_EQUAL, vars[2]->type, &vars[2]->value->i64);

				exp_left.add_var(vars[0]);
				exp_left.add_op(op_left);
				exp_left.add_var(vars[1]);

				exp_right.add_var(vars[2]);
				break;
			case 4:
				mCodeExpressionEmulator::emulate(&val_left, CEO_EQUAL, vars[0]->type, &vars[0]->value->i64);
					
				do 
				{
					op_left = get_op_by_value(val_left, flags);
				} while ( !vars[1]->value->i64 && (op_left==CEO_DIV || op_left==CEO_MUL || op_left==CEO_MOD) );

				mCodeExpressionEmulator::emulate(&val_left, op_left, vars[1]->type, &vars[1]->value->i64);
				
				mCodeExpressionEmulator::emulate(&val_right, CEO_EQUAL, vars[2]->type, &vars[2]->value->i64);

				do 
				{
					op_right = get_op_by_value(val_right, flags);
				} while ( !vars[3]->value->i64 && (op_right==CEO_DIV || op_right==CEO_MUL || op_right==CEO_MOD) );

				mCodeExpressionEmulator::emulate(&val_right, op_right, vars[3]->type, &vars[3]->value->i64);

				exp_left.add_var(vars[0]);
				exp_left.add_op(op_left);
				exp_left.add_var(vars[1]);

				exp_right.add_var(vars[2]);
				exp_right.add_op(op_right);
				exp_right.add_var(vars[3]);
				break;
		}

		mem_copy(if_last_vars, vars, vars_count*sizeof(MVAR_INFO*));
		if( vars_count < 3 )
		{
			mem_zero(&if_last_vars[vars_count], (4 - vars_count)*sizeof(MVAR_INFO*));
		}

		CODE_EXPRESSION_CMP_OPERATORS ops[2];

		switch( random.get_less(0,3) )
		{
			case 0: // ==, !=
				if( mCodeExpressionEmulator::compare(&val_left, vars[0]->type, &val_right)==0 )
				{
					ops[0] = CECO_EQUAL;
				}else{
					ops[0] = CECO_NOT_EQUAL;
				}

				ops[1] = (ops[0]==CECO_EQUAL ? CECO_NOT_EQUAL : CECO_EQUAL);
				break;
			case 1: // <, >=
				if( mCodeExpressionEmulator::compare(&val_left, vars[0]->type, &val_right)==-1 )
				{
					ops[0] = CECO_LESS;
				}else{
					ops[0] = CECO_GREAT_OR_EQUAL;
				}

				ops[1] = (ops[0]==CECO_LESS ? CECO_GREAT_OR_EQUAL : CECO_LESS);
				break;
			case 2: // > , <=
				if( mCodeExpressionEmulator::compare(&val_left, vars[0]->type, &val_right)==1 )
				{
					ops[0] = CECO_GREAT;
				}else{
					ops[0] = CECO_LESS_OR_EQUAL;
				}

				ops[1] = (ops[0]==CECO_GREAT ? CECO_LESS_OR_EQUAL : CECO_GREAT);
				break;
		}

		// is first or other calls was trash
		if( !is_equal )
		{
			if( block_type==CEBT_ELSE )
			{
				is_trash = false;
			}else{
				is_trash = random.get_less(0,2);
			}

			if( !is_trash )
			{
				is_equal = true;
				*new_parent = block;
			}
		}else{
			is_trash = true;
		}

		if( flags & IS_TRASH )
		{
			block->compare.is_trash = true;
			block->compare.op = ops[ random.get_less(0,2) ];
		}else{
			block->compare.is_trash = is_trash;
			block->compare.op = ( is_trash ? ops[1] : ops[0] );
		}

		chain[chain_count++] = block;
	}

	return chain_count;
}

	DWORD mCodeGenerator::generate_switch(PMCODE_BLOCK parent, PMCODE_BLOCK chain[], PMCODE_BLOCK *new_parent, PMCODE_BLOCK *save_switch_block, DWORD level, DWORD left, DWORD interations, DWORD flags)
	{
		if( (flags & IS_LOOP) || level > config.code.block_switch.max_levels || left < (config.code.block_switch.inner_blocks.min + 1)*interations ) 
			return NULL;

		DWORD		vars_count;
		MVAR_INFO*	vars[4];

		if( !locals_first_pos )
			return NULL;

		DWORD		 chain_count = 0;

		*new_parent = NULL;

		DWORD chain_size = random.get_less(config.code.block_switch.chains.min,config.code.block_switch.chains.max);
		DWORD chain_id;
		BOOL  is_trash;
		BOOL  is_equal = false; // finded new if statement, in which we can in

		MVAR_TYPE	type = locals_first[ random.get_less(0,locals_first_pos) ];

		PMVAR_INFO	switch_var = get_random_var_by_random_type(flags, type, 0, NULL);

		if( !switch_var )
			return NULL;

		PMCODE_BLOCK switch_block = graph.allocate(parent, CEBT_SWITCH);
		if( !switch_block )
			return NULL;

		//if( !lstrcmpA(switch_var->name,"loc154") )
		//	__debugbreak();

		*save_switch_block = switch_block;

		switch_block->block_switch.var = switch_var;

		INT64 val_left;

		mCodeExpressionEmulator::emulate(&val_left, CEO_EQUAL, switch_var->type, &switch_var->value->i64);

		for(chain_id = 0; chain_id < chain_size; chain_id++)
		{
			CODE_EMULATOR_BLOCK_TYPES block_type;
			if( chain_id==0 )
			{
				block_type = CEBT_CASE;
			}else if( chain_id + 1 == chain_size )
			{
				if( random.get_less(0,2) )
				{
					block_type = CEBT_CASE;
				}else{
					block_type = CEBT_DEFAULT;
				}
			}else{
				block_type = CEBT_CASE;
			}

			PMCODE_BLOCK block = graph.allocate(switch_block, block_type);
			if( !block )
				break;

			// is first or other calls was trash
			if( !is_equal )
			{
				if( block_type==CEBT_DEFAULT )
				{
					is_trash = false;
				}else{
					is_trash = random.get_less(0,2);
				}

				if( !is_trash )
				{
					is_equal = true;
					*new_parent = block;
				}
			}else{
				is_trash = true;
			}

			BOOL is_exists;

			if( flags & IS_TRASH )
			{
				block->block_case.is_trash = true;

				do 
				{
					mVars::get_imm_by_type(&block->block_case.value, switch_var->type);

					// проверка чтобы не было повтор€ющихс€ значений в других кейсах, или не будет компилации
					is_exists = false;
					for(int cp = 0; cp < chain_count; cp++)
					{
						if( chain[cp]->block_case.value==block->block_case.value)
						{
							is_exists = true;
							break;
						}
					}

				}while ( is_exists || mCodeExpressionEmulator::compare(&val_left, switch_var->type, &block->block_case.value)==0 );
			}else{
				block->block_case.is_trash = is_trash;
				
				// фэйковый case ?
				if( is_trash )
				{
					do 
					{
						mVars::get_imm_by_type(&block->block_case.value, switch_var->type);

						// проверка чтобы не было повтор€ющихс€ значений в других кейсах, или не будет компилации
						is_exists = false;
						for(int cp = 0; cp < chain_count; cp++)
						{
							if( chain[cp]->block_case.value==block->block_case.value)
							{
								is_exists = true;
								break;
							}
						}
					}while ( is_exists || mCodeExpressionEmulator::compare(&val_left, switch_var->type, &block->block_case.value)==0 );
				}else{
					// ¬нимание: мы берем именно val_left, на как signed 0xFF98!=0xFF98, а 0xFFFFFF98
					block->block_case.value = val_left;
				}
			}

			chain[chain_count++] = block;
		}

		return chain_count;
	}

	PMCODE_BLOCK mCodeGenerator::generate_loop(PMCODE_BLOCK parent, DWORD level, DWORD left, DWORD interations)
	{
		if( level > config.code.block_loop.max_levels || left < (config.code.block_loop.inner_blocks.min + 2)*interations*config.code.block_loop.interations.min ) 
			return NULL;

		if( safe_random.get_count()==0 )
			return NULL;

		PMCODE_BLOCK block = graph.allocate(parent, CEBT_LOOP);

		if( !block )
			return NULL;

		// for - inc,check,job,jmp to check
		// while - check,job,inc,jmp to check
		// do-while if i > 1,  job, inc, check, jmp to job

		block->loop.count = random.get_less(config.code.block_loop.interations.min,config.code.block_loop.interations.max);
		block->loop.step  = random.get_less(1, 0xFF);
		block->loop.start = random.get_less(0, 0xFFFF);

		CODE_EXPRESSION_CMP_OPERATORS ops[] = {
			CECO_NOT_EQUAL,
			CECO_LESS,
			CECO_LESS_OR_EQUAL,
			CECO_GREAT,
			CECO_GREAT_OR_EQUAL,
		};

		block->loop.op    = ops[ random.get_less(0,sizeof(ops)/sizeof(CODE_EXPRESSION_CMP_OPERATORS)) ];

		CHAR name[50];

		wsprintfA(name, "idx%d", loop_idx++);


		block->loop.index = locals->add(VG_CODE, random.get_less(0,1) ? MVT_INT32 : MVT_UINT32, name, MVF_RESERVED, NULL);
		if( block->loop.index )
		{
			PMCODE_BLOCK after_block;
			
			do 
			{
				after_block = safe_random.get_random(0,NULL);
			} while ( after_block->extra_blocks.count >= MBLOCK_EXTRA_MAX );

			if( after_block )
			{
				block->loop.index_start	 = graph.allocate_ex(NULL, CEBT_CUSTOM, FALSE);
				block->loop.index_update = graph.allocate_ex(NULL, CEBT_CUSTOM, FALSE);

				switch( block->loop.op )
				{
					case CECO_LESS:
					case CECO_LESS_OR_EQUAL:
					case CECO_NOT_EQUAL:
						wsprintfA(block->loop.index_start->custom.string, "%s = 0x%.8X;\r\n", block->loop.index->name, block->loop.start);
						wsprintfA(block->loop.index_update->custom.string, "%s += 0x%0.8X;\r\n", block->loop.index->name, block->loop.step);
						break;
					case CECO_GREAT:
						wsprintfA(block->loop.index_start->custom.string, "%s = 0x%.8X;\r\n", block->loop.index->name, block->loop.start + block->loop.count*block->loop.step);
						wsprintfA(block->loop.index_update->custom.string, "%s -= 0x%0.8X;\r\n", block->loop.index->name, block->loop.step);
						break;
					case CECO_GREAT_OR_EQUAL:
						wsprintfA(block->loop.index_start->custom.string, "%s = 0x%.8X;\r\n", block->loop.index->name, block->loop.start + (block->loop.count - 1)*block->loop.step);
						wsprintfA(block->loop.index_update->custom.string, "%s -= 0x%0.8X;\r\n", block->loop.index->name, block->loop.step);
						break;
				}
				
				// mixer :)
				after_block->extra_blocks.blocks[ after_block->extra_blocks.count ] = block->loop.index_start;

				DWORD pos;

				pos = random.get_equal(0, after_block->extra_blocks.count);

				PMCODE_BLOCK pos_block = after_block->extra_blocks.blocks[pos];
				after_block->extra_blocks.blocks[ pos ] = block->loop.index_start;
				after_block->extra_blocks.blocks[ after_block->extra_blocks.count ] = pos_block;

				after_block->extra_blocks.count++;
			}
		}

		// проверка дл€ do-while мы же 1 интерацию так и так делаем, следовательно надо чтобы было > 1
		// HEURFIX: убераем for чтобы комодо алерт не детектил
		if( block->loop.count > 1 )
		{
			block->loop.type = random.get_less(1,3); // 0,1,2
		}else{
			block->loop.type = random.get_less(1,2); // 0,1
		}


		return block;
	}

	mFunction* mCodeGenerator::generate_proc(PMCODE_BLOCK parent, DWORD level, DWORD left, DWORD interations, DWORD flags, PDWORD icount)
	{
		if( max_build_procs <= all_functions.get_count() || left < interations*config.code.block_call.inner_blocks.min ) 
			return NULL;

		mFunction *func = new mFunction(globals);
		func->set_convention(FC_RANDOM);

		DWORD count = random.get_equal(config.code.block_call.parameters.min, config.code.block_call.parameters.max);
		for(int i = 0; i < count; i++)
		{
			MVAR_TYPE formal_type;

			formal_type = locals_first_pos ? locals_first[ random.get_less(0, locals_first_pos) ] : MVT_INT32;
			
			/* ¬Ќ»ћјЌ»≈: надо установить value дл€ PTR
			if( random.get_less(0,2) )
			{
				formal_type = MVAR_TYPE_TO_PTR(formal_type);
			}*/

			func->add_formal(formal_type, NULL, MVF_DO_NOT_INITIALIZE, NULL);
		}

		*icount = random.get_less(config.code.block_call.inner_blocks.min*interations, config.code.block_call.inner_blocks.max*interations);
		if( *icount > left )
		{
			*icount = left;
		}

		func->generate(*icount,*icount, flags | IS_PROC);

		return func;
	}

	void mCodeGenerator::emulate_block(PMCODE_BLOCK block, BOOL is_first)
	{
		mCodeExpression exp_left, exp_right;
		INT64			result_left, result_right;

		DWORD start, end;
		BOOL  is_true;

		PMCODE_BLOCK	start_block;
		PMCODE_BLOCK	end_block;
		PMVAR_INFO		tmp_var;

		if( block )
		{
			if( IsListEmpty(&block->inner) )
				return;

			start_block = (PMCODE_BLOCK)block->inner.Flink;
			end_block = (PMCODE_BLOCK)&block->inner;

			switch( block->type )
			{
				case CEBT_LOOP:
					start = (is_first ? 1 : 0); // в процессе создани€ кода, первый цикл уже эмулируетс€, но все внутрение циклы мы должны еще эмулировать с 0
					end   = block->loop.count;
					break;
				default:
					start = 0;
					end   = 1;
					break;
			}
		}else{
			start = 0;
			end = 1;

			start_block = graph.get_first();
			end_block = graph.get_end();
		}

		//if( block->type==CEBT_LOOP && block->loop.index==1 )
		{
		//	__debugbreak();
		}

		for(int i = start; i < end; i++)
		{
			PMCODE_BLOCK current = start_block;

			while( current!=end_block )
			{
				switch( current->type )
				{
				case CEBT_API:
					// skip
					break;
				case CEBT_PTR_LINE:
				case CEBT_LINE:
					exp_left.set(&current->code.expression);

					if( (tmp_var = exp_left.get_var_addr()) )
					{
						current->code.left_var->value = tmp_var->value;
					}else{
						result_left = exp_left.emulate(current->code.left_var->type);
						mCodeExpressionEmulator::emulate_end(&current->code.left_var->value->i64, current->code.left_var->type, &result_left);
					}
					break;
				case CEBT_IF:
					exp_left.set(&current->compare.left);
					exp_right.set(&current->compare.right);

					result_left = exp_left.emulate(current->compare.type);
					result_right = exp_right.emulate(current->compare.type);

					is_true = false;

					switch( current->compare.op )
					{
					case CECO_EQUAL: is_true = mCodeExpressionEmulator::compare(&result_left, current->compare.type, &result_right)==0; break;
					case CECO_NOT_EQUAL: is_true = mCodeExpressionEmulator::compare(&result_left, current->compare.type, &result_right)!=0; break;
					case CECO_LESS: is_true = mCodeExpressionEmulator::compare(&result_left, current->compare.type, &result_right)==-1; break;
					case CECO_GREAT_OR_EQUAL: is_true = mCodeExpressionEmulator::compare(&result_left, current->compare.type, &result_right)!=-1; break;
					case CECO_GREAT: is_true = mCodeExpressionEmulator::compare(&result_left, current->compare.type, &result_right)==1; break;
					case CECO_LESS_OR_EQUAL: is_true = mCodeExpressionEmulator::compare(&result_left, current->compare.type, &result_right)!=1; break;
					}

					if( is_true )
					{
						emulate_block(current, false);
					}

					break;
				case CEBT_LOOP:
					emulate_block(current, false);
					break;

				}

				current = (PMCODE_BLOCK)current->entry.Flink;
			}
		}
	}

	PMCODE_BLOCK mCodeGenerator::generate_blocks(PMCODE_GENERATOR_MAP map, PMCODE_BLOCK parent, INT left, DWORD level, DWORD interations, DWORD flags)
	{
		PMCODE_BLOCK block;
		PMCODE_BLOCK chain[10];
		PMCODE_BLOCK chain_parent, new_parent;
		DWORD new_level;
		DWORD chain_count;
		DWORD sflags = flags;

		flags = CLEAR_STARTUP_FLAGS(flags);

		BYTE last_type = -1;

		while( left > 0 )
		{
			DWORD ctype;
			
			if( flags & IS_TRASH )
			{
				do 
				{
					ctype = code_type.get();
				} while ( ctype==CEBT_LOOP || ctype==CEBT_IF || ctype==CEBT_SWITCH ); // запрещаем кучу ифов и т.д
			}else{
				ctype = (map->position < map->count ? map->types[map->position++] : code_type.get());
			}
			
			if( ctype==CEBT_LINE )
			{
				if( (block = generate_code(parent, interations, flags, false)) )
				{
					mCodeExpression exp(&block->code.expression);
					mCode cd(24);

					cd << block->code.left_var->name << " = ";

					exp.get_string(cd);

					cd << "\r\n";

					//if (flags & IS_PROC)
					//{
					//	for (int z = 0; z < 10; z++)
					//	{
					//		generate_code(parent, interations, flags, false);
					//	}
					//}

					OutputDebugStringA(cd.get());

					left -= interations;
					last_type = ctype;
				}
			}else if( ctype==CEBT_PTR_LINE )
			{
				if( (block = generate_code(parent, interations, flags, true)) )
				{
					mCodeExpression exp(&block->code.expression);
					mCode cd(24);

					cd << "*" << block->code.left_var->name << " = ";

					exp.get_string(cd);

					cd << "\r\n";

					OutputDebugStringA(cd.get());

					left -= interations;
					last_type = ctype;
				}
			}else if( ctype==CEBT_API )
			{
				if( (block = generate_api(parent, interations, flags)) )
				{
					left -= interations;
					last_type = ctype;
				}
			}else if( ctype==CEBT_IF )
			{
				if( (chain_count = generate_if(parent, chain, &chain_parent, level, left, interations, flags)) )
				{
					new_parent = parent;
					new_level  = level;

					BOOL is_changed = false;

#ifdef CONFIG_DEBUG_IF

					mCode c(0xFF);
					for(int k = 0; k < chain_count; k++)
					{
						mCodeExpression dexp(&chain[k]->compare.left);

						c.clear();

						if( chain[k]->type==CEBT_IF || chain[k]->type==CEBT_ELSEIF )
						{
							if(  chain[k]->type==CEBT_IF )
							c << k << " == if( ";
							else
								c << k << " == elseif( ";
							dexp.get_string(c);
							c << cop2str(chain[k]->compare.op);
							dexp.set(&chain[k]->compare.right);
							dexp.get_string(c);
							c << " ) " << (chain[k]->compare.is_trash ? "trash" : "code" ) << "\r\n";
						}else if( chain[k]->type==CEBT_ELSE)
						{
							c << k << " == else " << (chain[k]->compare.is_trash ? "trash" : "code" ) << "\r\n";
						}

						OutputDebugStringA(c.get());
					}
#endif

					// we can change parent to new chunk
					if( !(flags & DO_NOT_CHANGE_PARENT) && chain_parent && random.get_less(0,2)==1 )
					{
						is_changed = true;

						new_parent = chain_parent;
						new_level += 1;

						DWORD icount = random.get_less(config.code.block_if.trash_blocks.min*interations, config.code.block_if.trash_blocks.max*interations);
						if( left > 1 )
						{
							if( icount > left )
							{
								icount = left - 1;
							}
						}else{
							icount = 1;
						}

#ifdef CONFIG_DEBUG_EMULATOR
						PMCODE_BLOCK int_block = graph.allocate(parent,CEBT_CUSTOM);
						wsprintfA(int_block->custom.string, "__debugbreak(); // icount: %d, left: %d\r\n", icount, left);
#endif

						generate_blocks(map, parent, icount, level + 1, interations, flags | IS_TRASH);

						left -= (icount + 1)*interations;
					}else{
						if( !(flags & IS_LOOP) && !(flags & IS_TRASH) )
						{
							safe_random.add_reference(chain[chain_count - 1]);
						}
					}
					
					for(int w = 0; w < chain_count; w++)
					{
						if( new_parent==chain[w] )
							continue;

						DWORD icount;

						if( chain[w]->compare.is_trash )
						{
							icount = random.get_less(config.code.block_if.trash_blocks.min*interations, config.code.block_if.trash_blocks.max*interations);
							if( left > 1 )
							{
								if( icount > left )
								{
									icount = left - 1;
								}
							}else{
								icount = 1;
							}

#ifdef CONFIG_DEBUG_EMULATOR
							PMCODE_BLOCK int_block = graph.allocate(chain[w],CEBT_CUSTOM);
							wsprintfA(int_block->custom.string, "__debugbreak(); // icount: %d, left: %d\r\n", icount, left);
#endif

							generate_blocks(map, chain[w], icount, level + 1, interations, flags | IS_IF | IS_TRASH | (!is_changed ? DO_NOT_CHANGE_PARENT : 0));

							left -= (icount + 1)*interations;
						}else{
							icount = random.get_less(config.code.block_if.inner_blocks.min*interations, config.code.block_if.inner_blocks.max*interations);
							if( icount > left )
							{
								icount = left;
							}

							generate_blocks(map, chain[w], icount, level + 1, interations, flags | IS_IF | (!is_changed ? DO_NOT_CHANGE_PARENT : 0));

							left -= (icount + 1)*interations;
							last_type = ctype;
						}
					}

					parent = new_parent;
					level  = new_level;
				}
			}else if( ctype==CEBT_SWITCH )
			{
				PMCODE_BLOCK switch_block;

				if( (chain_count = generate_switch(parent, chain, &chain_parent, &switch_block, level, left, interations, flags)) )
				{
					new_parent = parent;
					new_level  = level;

					BOOL is_changed = false;

					// we can change parent to new chunk
					if( !(flags & DO_NOT_CHANGE_PARENT) && chain_parent && random.get_less(0,2)==1 )
					{
						is_changed = true;

						new_parent = chain_parent;
						new_level += 2;

						DWORD icount = random.get_less(config.code.block_switch.trash_blocks.min*interations, config.code.block_switch.trash_blocks.max*interations);
						if( left > 1 )
						{
							if( icount > left )
							{
								icount = left - 1;
							}
						}else{
							icount = 1;
						}

#ifdef CONFIG_DEBUG_EMULATOR
						PMCODE_BLOCK int_block = graph.allocate(parent,CEBT_CUSTOM);
						wsprintfA(int_block->custom.string, "__debugbreak(); // icount: %d, left: %d\r\n", icount, left);
#endif

						generate_blocks(map, parent, icount, level + 1, interations, flags | IS_TRASH);

						left -= (icount + 1)*interations;
					}else{
						if( !(flags & IS_LOOP) && !(flags & IS_TRASH) )
						{
							safe_random.add_reference(switch_block);
						}
					}

					for(int w = 0; w < chain_count; w++)
					{
						if( new_parent==chain[w] )
							continue;

						DWORD icount;

						if( chain[w]->block_case.is_trash )
						{
							icount = random.get_less(config.code.block_switch.trash_blocks.min*interations, config.code.block_switch.trash_blocks.max*interations);
							if( left > 1 )
							{
								if( icount > left )
								{
									icount = left - 1;
								}
							}else{
								icount = 1;
							}

#ifdef CONFIG_DEBUG_EMULATOR
							PMCODE_BLOCK int_block = graph.allocate(chain[w],CEBT_CUSTOM);
							wsprintfA(int_block->custom.string, "__debugbreak(); // case icount: %d, left: %d\r\n", icount, left);
#endif

							generate_blocks(map, chain[w], icount, level + 2, interations, flags | IS_CASE | IS_TRASH | (!is_changed ? DO_NOT_CHANGE_PARENT : 0));

							left -= (icount + 1)*interations;
						}else{
							icount = random.get_less(config.code.block_switch.inner_blocks.min*interations, config.code.block_switch.inner_blocks.max*interations);
							if( icount > left )
							{
								icount = left;
							}

							generate_blocks(map, chain[w], icount, level + 2, interations, flags | IS_CASE | (!is_changed ? DO_NOT_CHANGE_PARENT : 0));

							left -= (icount + 1)*interations;
							last_type = ctype;
						}
					}

					parent = new_parent;
					level  = new_level;
				}
			}else if( ctype==CEBT_LOOP )
			{
				if( (block = generate_loop(parent, level, left, interations)) )
				{
					//if( loop_idx==24 )
					//	__debugbreak();
					DWORD icount = random.get_less(config.code.block_loop.inner_blocks.min*interations*block->loop.count, config.code.block_loop.inner_blocks.max*interations*block->loop.count);
					if( icount > left )
					{
						icount = left;
					}

					generate_blocks(map, block, icount, level + 1, interations*block->loop.count, flags | IS_LOOP);

					if( !(flags & IS_TRASH) )
					{
						emulate_block(block, true);
					}

					DWORD incr_pos = block->inner_count > 1 ? random.get_less(0, block->inner_count - 1) : 0;
					PMCODE_BLOCK current = (PMCODE_BLOCK)&block->inner.Flink;

					while( incr_pos )
					{
						incr_pos--;
						current = (PMCODE_BLOCK)current->entry.Flink;
					}

					InsertHeadList(&current->entry, &block->loop.index_update->entry);

					//wsprintfA(block->comment, "left: %d, inner: %d, after-left: %d", left, icount, (icount + 2)*interations);

					left -= (icount + 2)*interations;
					last_type = ctype;
				}
			}else if(ctype==CEBT_PROC)
			{
				if (flags & IS_TRASH & ~IS_PROC)
				{
					mFunction*	proc;
					DWORD		icount;

					if( all_functions.get_count() < 2 || random.get_less(0,2) )
					{
						if( (proc = generate_proc(parent, level, left, interations, flags, &icount)) )
						{
							proc->set_user_data(icount);
						}
					}else{
						do 
						{
							proc = all_functions.get_random(0, NULL);
						} while ( !lstrcmpA(proc->get_name(),"DllMain") || !lstrcmpA(proc->get_name(),"WinMain") || !lstrcmpA(proc->get_name(),"wWinMain"));
						icount = proc->get_user_data();
					}

					if( proc )
					{
						left -= (icount)*interations;

						block = graph.allocate(parent, CEBT_PROC);
						if( block )
						{
							block->block_proc.proc = proc;

							for(PMVAR_INFO var_param = proc->get_formals()->first(); var_param; var_param = proc->get_formals()->next() )
							{
								PMVAR_INFO in_param = get_random_var_by_random_type(flags, var_param->type, NULL, NULL);

								if( MVAR_TYPE_IS_PTR(var_param->type) )
								{
									if( in_param )
									{
										block->block_proc.params[block->block_proc.params_count].type = MCP_TYPE_VAR;
										block->block_proc.params[block->block_proc.params_count].var  = in_param;
									}else{
										block->block_proc.params[block->block_proc.params_count].type = MCP_TYPE_IMM;
										block->block_proc.params[block->block_proc.params_count].imm  = 0;
									}
								}else{
									if( in_param )
									{
										block->block_proc.params[block->block_proc.params_count].type = MCP_TYPE_VAR;
										block->block_proc.params[block->block_proc.params_count].var  = in_param;
									}else{
										block->block_proc.params[block->block_proc.params_count].type = MCP_TYPE_IMM;
										block->block_proc.params[block->block_proc.params_count].imm  = random.get_less(0,255);
									}
								}

								block->block_proc.params_count++;
							}
						}
					}
				}
			}
		}

		return parent;
	}

	PMCODE_BLOCK mCodeGenerator::generate(PMCODE_BLOCK parent, DWORD min, DWORD max, DWORD flags)
	{
		INT total = random.get_equal(min,max);

		if( !total )
			return parent;

		PMCODE_BLOCK new_parent = parent;

		PMCODE_GENERATOR_MAP map = (PMCODE_GENERATOR_MAP)halloc(sizeof(MCODE_GENERATOR_MAP) + total*2); // set extra, because we can add special code to prevent call api1/call api2/...
		if( map )
		{
			map->count = total;
			map->position = 0;

			DWORD codes = 0;
			if( total )
			{
				map->types[0] = code_type.get();
			}

			for(int i = 1, j = 0; i < total; i++)
			{
				DWORD pos = random.get_less(0,i); // for mix

				BYTE tmp = map->types[pos];
				map->types[pos] = code_type.get();
				map->types[i] = tmp;
			}

			// if code exists and not all locals initialized
			if( (flags & IS_START_EP) && locals_first_pos < locals_first_count )
			{
				// to generate normal loops, we need local/global variables. This check prevent loop, while not all variable types initialized
				DWORD codes = 0;
				for(int i = 0; i < map->count && i < locals_first_count; i++)
				{
					if( map->types[i]==CEBT_LOOP /*|| map->types[i]==CEBT_CALL*/)
					{
						INT j ;
						for(j = i + 1; j < map->count; j++)
						{
							if( map->types[j] != map->types[i] )
							{
								map->types[i] = map->types[j];
								map->types[j] = map->types[i];

								break;
							}
						}

						// we can not find variables to exchange, then do not any more checks, no vars free left
						if( j >= map->count )
							break;
					}
				}
			}

			// check prevent call api1 / call api2 / ... set code between them 1-2


			new_parent = generate_blocks(map, parent, total, 1, 1, flags);

			hfree(map);
		}

		return new_parent;
	}

	void mCodeGenerator::get_string(mCode &str)
	{
		graph.get_string(str);
	}


