#include"symbol_table.h"
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
		VarTableHash[i] = NULL;
		FunctionDefHash[i] = NULL;
		StructDefHash[i] = NULL;
	}
}
int insert_function_def_table(func_def_table node)
{
	unsigned index = hash_table(node->name);
	func_def_table fdt = FunctionDefHash[index];
	for (; fdt; fdt = fdt->next)
		if (strcmp(fdt->name, node->name) == 0)
		{
			errorprint(4, node->line, node->name);
			return -1;
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


int insert_var_table(var_table node)
{
	unsigned index = hash_table(node->name);
	var_table vt = VarTableHash[index];
	for (; vt; vt = vt->next)
		if (strcmp(node->name, vt->name) == 0)
		{
			errorprint(3, node->line, node->name);
			return -1;
		}
	if (VarTableHash[index] == NULL)
	{
		VarTableHash[index] = node;
		node->next = NULL;
		return 0;
	}
	else
	{
		node->next = VarTableHash[index];
		VarTableHash[index] = node;
		return 1;
	}
}

int insert_struct_table(struct_table st)
{
	unsigned index = hash_table(st->name);
	struct_table s = StructDefHash[index];
	for (; s; s = s->next)
		if (strcmp(st->name, s->name) == 0)
		{
			errorprint(16, st->line, st->name);
			return -1;
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

var_table search_variable(Tree* node)
{
	var_table vt = VarTableHash[hash_table(node->value)];
	for (; vt; vt = vt->next)
		if (strcmp(node->value, vt->name) == 0)
			return vt;
	return NULL;
}

func_def_table search_func(Tree* node)
{
	func_def_table fdt = FunctionDefHash[hash_table(node->value)];
	for (; fdt; fdt = fdt->next)
		if (strcmp(node->value, fdt->name) == 0)
			return fdt;
	return NULL;
}

struct_table search_struct(Tree* node)
{
	struct_table st = StructDefHash[hash_table(node->struct_name)];
	for (; st; st = st->next)
		if (strcmp(node->struct_name, st->name) == 0)
			return st;
	return NULL;
}
