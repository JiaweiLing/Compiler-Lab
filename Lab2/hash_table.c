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
	unsigned index = hash_table(node->name);
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
	struct_table strt = StructDefHash[index];
	while (strt != NULL)
	{
		if (strcmp(node->name, strt->name) == 0)
		{
			errorprint(3, node->line, node->name);
			return -1;
		}
		strt = strt->next;
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

int insert_struct_table(struct_table st)
{
	unsigned index = hash_table(st->name);
	struct_table s_t = StructDefHash[index];
	while (s_t != NULL)
	{
		if (strcmp(st->name, s_t->name) == 0)
		{
			errorprint(16, st->line, st->name);
			return -1;
		}
		s_t = s_t->next;
	}
	if (StructDefHash[index] == NULL)
	{
		StructDefHash[index] = st;
		st->next = NULL;
		return 0;
	}
	else
	{
		st->next = StructDefHash[index];
		StructDefHash[index] = st;
		return 1;
	}
}

symbol_table search_symbol(Tree* node)
{
	unsigned index = hash_table(node->value);
	symbol_table symt;
	symbol_table st = SymbolTableHash[index];
	
	while (st != NULL)
	{
		if (strcmp(node->value, st->name) == 0)
		{
			//strcpy(node->struct_name, st->struct_name);
			//node->type = st->type;
			//return 1;
			symt = st;
			return symt;
		}
		st = st->next;
	}
	return NULL;
}

func_def_table search_func(Tree* node)
{
	func_def_table ft;
	unsigned index = hash_table(node->value);
	func_def_table fdt = FunctionDefHash[index];
	while (fdt != NULL)
	{
		if (strcmp(node->value, fdt->name) == 0)
		{
			//return 1;
			ft = fdt;
			return ft;
		}
		fdt = fdt->next;
	}
	return NULL;
}

struct_table search_struct(Tree* node)
{
	unsigned index = hash_table(node->struct_name);
	struct_table strt;
	struct_table st = StructDefHash[index];
	while (st != NULL)
	{
		if (strcmp(node->struct_name, st->name) == 0)
		{
			strt = st;
			return strt;
		}
		st = st->next;
	}
	return NULL;
}
