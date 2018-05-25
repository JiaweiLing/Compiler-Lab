#include"hash_table.h"
#include"semantic.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<assert.h>

unsigned hash_table(char *name)
{
	unsigned int val = 0, i;
	for (; *name; ++name)
	{
		val = (val << 2) + *name;
		if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & 0x3fff;
	}
	return val;
}

void init_hash()
{
	int i;
	for (i = 0; i < hash_size; i++)
	{
		SymbolTableHash[i] = NULL;
		FunctionDefHash[i] = NULL;
		FunctionDecHash[i] = NULL;
		StructDefHash[i] = NULL;
	}
}
int insert_function_def_table(func_def_table node)
{
	//printf("The function name is %s\n", node->name);
	unsigned index = hash_table(node->name);
	//printf("%d\n", index);
	func_def_table fdt = FunctionDefHash[index];
	while (fdt != NULL)
	{
		if (strcmp(fdt->name, node->name) == 0)
		{
			errorprint(4, node->line, node->name);
			return -1;
		}
		fdt = fdt->next;
	}
	if (FunctionDefHash[index] == NULL)
	{
		FunctionDefHash[index] = node;
		node->next = NULL;
		return 0;
	}
	else
	{
		node->next = FunctionDefHash[index];
		FunctionDefHash[index] = node;
		return 1;
	}
}
void check_function_table()
{
	int i;
	for (i = 0; i < hash_size; i++)
	{
		if (FunctionDefHash[i] != NULL)
		{
			func_def_table p = FunctionDefHash[i];
			while (p != NULL)
			{
				printf("The function name is: %s\n", p->name);
				printf("The return type is: %d\n", p->return_type);
				printf("The number of parameters is: %d\n", p->num_para);
				if (p->num_para != 0)
				{
					printf("The parameters are: \n");
					struct para* t = p->list_para;
					while (t != NULL)
					{
						printf("	The name of parameter is: %s\n", t->name);
						Type type = t->type;
						printf("	The type of parameter is %d\n", type->Kind);
						while (type->Kind != BASIC && type->Kind != STRUCTURE)
						{
							printf("		The size of array is: %d\n", type->Array.size);
							type = type->Array.element;
						}
						if (type->Kind == BASIC)
							printf("		The basic type is %d\n", type->Basic);
						if (type->Kind == STRUCTURE)
							printf("		The struct type is %s\n", t->struct_name);
						t = t->next_para;
					}
				}
				p = p->next;
			}
		}
	}
}

int insert_symbol_table(symbol_table node)
{
	unsigned index = hash_table(node->name);
	symbol_table st = SymbolTableHash[index];
	while (st != NULL)
	{
		if (strcmp(node->name, st->name) == 0)
		{
			errorprint(3, node->line, node->name);
			return -1;
		}
		st = st->next;
	}
	if (SymbolTableHash[index] == NULL)
	{
		SymbolTableHash[index] = node;
		node->next = NULL;
		return 0;
	}
	else
	{
		node->next = SymbolTableHash[index];
		SymbolTableHash[index] = node;
		return 1;
	}
}
void check_symbol_table()
{
	int i;
	for (i = 0; i < hash_size; i++)
		if (SymbolTableHash[i] != NULL)
		{
			symbol_table st = SymbolTableHash[i];
			while (st != NULL)
			{
				//if (st->type->Kind == BASIC)
				//	printf("symbol_table type: %d ", st->type->Basic);
				//else
				//if (st->type->Kind == STRUCTURE)
				//{
				//	printf("symbol table type: struct %s ", st->struct_name);
				//}
				//printf("symbol_table name is %s\n", st->name);
				//st = st->next;
				printf("symbol table name: %s\n", st->name);
				Type type = st->type;
				while (type->Kind != BASIC && type->Kind != STRUCTURE)
				{
					printf("size: %d\n", type->Array.size);
					type = type->Array.element;
				}
				if (type->Kind == BASIC)
					printf("type: %d\n", type->Basic);
				else
					printf("type: %d\n", type->Kind);
				if (type->Kind == STRUCTURE)
					printf("struct name: %s\n", st->struct_name);
				st = st->next;
			}
		}
}
