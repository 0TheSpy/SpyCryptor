#include <windows.h>
#include <tchar.h>
#include <intrin.h>
#include <Strsafe.h>

#include "globals.h"
#include "mCodeEmulator.h"

mCodeEmulator::mCodeEmulator(mVars *Globals,mVars *Locals, mVars *Formals) : units(1024)
{
	count = 0;
	InitializeListHead(&entries);

	globals	= Globals;
	locals	= Locals;
	formals = Formals;
}

mCodeEmulator::~mCodeEmulator()
{

}

void mCodeEmulator::add(PMCODE_BLOCK parent, PMCODE_BLOCK new_block)
{
	if( !parent )
	{
		count++;
		InsertTailList(&entries, &new_block->entry);
	}else{
		parent->inner_count++;
		InsertTailList(&parent->inner, &new_block->entry);
	}
}

DWORD mCodeEmulator::get_inner_count(PMCODE_BLOCK parent)
{
	if( !parent )
		return count;

	return parent->inner_count;
}

PMCODE_BLOCK mCodeEmulator::get_inner(PMCODE_BLOCK parent)
{
	if( !parent )
		return (PMCODE_BLOCK)entries.Flink;

	return (PMCODE_BLOCK)parent->inner.Flink;
}

PMCODE_BLOCK mCodeEmulator::allocate(PMCODE_BLOCK parent, CODE_EMULATOR_BLOCK_TYPES type)
{
	return allocate_ex(parent, type, TRUE);
}

PMCODE_BLOCK mCodeEmulator::allocate_ex(PMCODE_BLOCK parent, CODE_EMULATOR_BLOCK_TYPES type, BOOL add_to_parent)
{
	PMCODE_BLOCK block = units.get_new_chunk();

	if( block )
	{
		InitializeListHead(&block->inner);
		
		block->type = type;

		if( add_to_parent )
		{
			add(parent,block);
		}
	}else{
		__debugbreak();
	}

	return block;
}

void mCodeEmulator::get_block_string(mCode &str, DWORD level, PMCODE_BLOCK block)
{
	mCodeExpression expression;
	CHAR tabs[256];

	if( level >= sizeof(tabs) - 1 )
		level = sizeof(tabs) - 1;

	mem_set(tabs,'\t',level);
	tabs[level] = 0;

	DWORD i,j;

	switch( block->type )
	{
		case CEBT_PTR_LINE:
		case CEBT_LINE:
			expression.set(&block->code.expression);

			if( expression.get_var_addr() )
			{
				str << tabs << block->code.left_var->name << " = ";

			}else{
				if( MVAR_IS_PTR(block->code.left_var) )
				{
					str << tabs << "*" << block->code.left_var->name << " = ";
				}else{
					str << tabs << block->code.left_var->name << " = ";
				}
			}

			expression.get_string(str);

			if( *block->comment )
			{
				str << "; // " << block->comment << "\r\n";
			}else{
				str << ";\r\n";
			}
			break;
		case CEBT_ELSEIF:
		case CEBT_IF:
			if( block->type==CEBT_IF )
			{
				str << tabs << "if( ";
			}else{
				str << tabs << "else if( ";
			}

			expression.set(&block->compare.left);
			expression.get_string(str);

			str << cop2str(block->compare.op);

			expression.set(&block->compare.right);
			expression.get_string(str);

			if( *block->comment )
			{
				str << " ) // " << block->comment << "\r\n";
			}else{
				str << " )\r\n";
			}

			str << tabs << "{\r\n";

			get_blocks_string(str, level + 1, &block->inner);

			str << tabs << "}\r\n";
			break;
		case CEBT_ELSE:
			str << tabs << "else{\r\n";

			get_blocks_string(str, level + 1, &block->inner);

			str << tabs << "}\r\n";
			break;
		case CEBT_SWITCH:
			if( MVAR_IS_PTR(block->block_switch.var) )
			{
				str << tabs << "switch( *" << block->block_switch.var->name << " )\r\n";
			}else{
				str << tabs << "switch( " << block->block_switch.var->name << " )\r\n";
			}
			str << tabs << "{\r\n";

			get_blocks_string(str, level + 1, &block->inner);

			str << tabs << "}\r\n";
			break;
		case CEBT_LOOP:
			switch( block->loop.type )
			{
				case 0: // for
					/*str << tabs << "for(" << block->loop.index->name << " = " << block->loop.start 
						<< "; " << block->loop.index->name << " < " << (block->loop.start + block->loop.count*block->loop.step)
						<< "; " << block->loop.index->name << " += " << block->loop.step;

					if( *block->comment )
					{
						str << ") // " << block->comment << "\r\n";
					}else{
						str << ")\r\n";
					}

					str << tabs << "{\r\n";

					get_blocks_string(str, level + 1, &block->inner);

					str << tabs << "}\r\n";*/
					break;
				case 1: // while
					switch( block->loop.op )
					{
						case CECO_LESS:
						case CECO_NOT_EQUAL:
							str << tabs << "while( " << block->loop.index->name << cop2str(block->loop.op) << (block->loop.start + block->loop.count*block->loop.step);
							break;
						case CECO_LESS_OR_EQUAL:
							str << tabs << "while( " << block->loop.index->name << cop2str(block->loop.op) << (block->loop.start + (block->loop.count - 1)*block->loop.step);
							break;
						case CECO_GREAT:
						case CECO_GREAT_OR_EQUAL:
							str << tabs << "while( " << block->loop.index->name << cop2str(block->loop.op) << block->loop.start;
							break;
					}

					if( *block->comment )
					{
						str << " ) // " << block->comment << "\r\n";
					}else{
						str << " )\r\n";
					}

					str << tabs << "{\r\n";

					get_blocks_string(str, level + 1, &block->inner);

					str << tabs << "}\r\n";
					break;
				case 2:
					str << tabs;

					if( *block->comment )
					{
						str << "do // " << block->comment << "\r\n";
					}else{
						str << "do\r\n";
					}

					str << tabs << "{\r\n";

					get_blocks_string(str, level + 1, &block->inner);

					switch( block->loop.op )
					{
						case CECO_LESS:
						case CECO_NOT_EQUAL:
							str << tabs << "}while( " << block->loop.index->name << cop2str(block->loop.op) << (block->loop.start + block->loop.count*block->loop.step)<< ");\r\n";
							break;
						case CECO_LESS_OR_EQUAL:
							str << tabs << "}while( " << block->loop.index->name << cop2str(block->loop.op) << (block->loop.start + (block->loop.count - 1)*block->loop.step)<< ");\r\n";
							break;
						case CECO_GREAT:
						case CECO_GREAT_OR_EQUAL:
							str << tabs << "}while( " << block->loop.index->name << cop2str(block->loop.op) << block->loop.start<< ");\r\n";
							break;
					}

					break;
			}
			
			break;
		case CEBT_DEFAULT:
			str << tabs << "default:\r\n";

			get_blocks_string(str, level + 1, &block->inner);

			str << tabs << "break;\r\n";
			break;
		case CEBT_CASE:
			str << tabs << "case " << block->block_case.value << ":\r\n";

			get_blocks_string(str, level + 1, &block->inner);

			str << tabs << "break;\r\n";
			break;
		case CEBT_PROC:
			str << tabs << block->block_proc.proc->get_name() << "(";

			i = 0;
			for(PMVAR_INFO var_param = block->block_proc.proc->get_formals()->first(); var_param; var_param = block->block_proc.proc->get_formals()->next() )
			{
				if( i > 0 )
					str << ", ";

				if( block->block_proc.params[i].type==MCP_TYPE_VAR )
				{
					if( MVAR_TYPE_IS_PTR(var_param->type) )
					{
						if( MVAR_TYPE_IS_PTR(block->block_proc.params[i].var->type) )
						{
							str << block->block_proc.params[i].var->name;
						}else{
							str << "&" << block->block_proc.params[i].var->name;
						}
					}else{
						if( MVAR_TYPE_IS_PTR(block->block_proc.params[i].var->type) )
						{
							str << "*" << block->block_proc.params[i].var->name;
						}else{
							str << block->block_proc.params[i].var->name;
						}
					}
				}else{
					str << block->block_proc.params[i].imm;
				}

				i++;
			}

			str << ");\r\n";
			break;
		case CEBT_CUSTOM:
			str << tabs << block->custom.string;

			if( *block->comment )
			{
				str << " // " << block->comment << "\r\n";
			}else{
				str << "\r\n";
			}

			if( !IsListEmpty(&block->inner) )
			{
				str << tabs << "{\r\n";

				get_blocks_string(str, level + 1, &block->inner);

				str << tabs << "}\r\n";
			}
			break;
	}
}

PMCODE_BLOCK mCodeEmulator::get_first()
{
	return (PMCODE_BLOCK)entries.Flink;
}

PMCODE_BLOCK mCodeEmulator::get_end()
{
	return (PMCODE_BLOCK)&entries;
}

void mCodeEmulator::get_blocks_string(mCode &str, DWORD level, PLIST_ENTRY root)
{
	PMCODE_BLOCK block = (PMCODE_BLOCK)root->Flink;
	while( block!=(PMCODE_BLOCK)root )
	{
		get_block_string(str, level, block);

		for(int i = 0; i < block->extra_blocks.count; i++)
		{
			get_block_string(str, level, block->extra_blocks.blocks[i]);
		}

		block = (PMCODE_BLOCK)block->entry.Flink;
	}
}

void mCodeEmulator::get_string(mCode &str)
{
	get_blocks_string(str, 1, &entries);
}

