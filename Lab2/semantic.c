#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include"tree.h"
#include"semantic.h"

Type Specifier(tree* node)
{
	if (node == NULL) return NULL;
	Type type = (Type)malloc(sizeof(struct TYPE));
	if (strcmp(node->child->value, "TYPE") == 0)
	{
		if (strcmp(node->child->value, "int") == 0)
		{
			type->Kind = BASIC;
			type->Basic = INT_;
			return type;
		}
		else
		if (strcmp(node->child->value, "float") == 0)
		{
			type->Kind = BASIC;
			type->Basic = FLOAT_;
			return type;
		}
		else
		{
			printf("%s\n", node->child->value);
			printf("TYPE does not have other.\n");
			return NULL;
		}
	}
	else
	if (strcmp(node->child->name, "StructSpecifier") != 0)
	{
		printf("Specifier does not have other.\n");
		return NULL;
	}
}

void ParamDec(tree* node, func_def_table func)
{
	assert(strcmp(node->name, "ParamDec") == 0);
	
	struct para* Para = (struct para*)malloc(sizeof(struct para));
	Type type = Specifier(node->child);
	
	if (func->list_para == NULL)
	{
		func->list_para = Para;
		Para->next_para = NULL;
	}
	else
	{
		Para->next_para = func->list_para;
		func->list_para = Para;
	}
}

void VarList(tree* node, func_def_table func)
{
	assert(strcmp(node->name, "VarList") == 0);
	switch (node->num)
	{
		case 1:
			ParamDec(node->child, func);
			return;
			break;
		case 3:
			break;
	}
}

func_def_table FunDec(tree* node, Type type)
{
	func_def_table func = (func_def_table)malloc(sizeof(FunctionDefTableNode));
	func->return_type = type->Basic;
	
	strcpy(func->name, node->child->name);
	if (strcmp(node->child->brother->brother->name, "RP") == 0)
	{
		func->num_para = 0;
		func->list_para = NULL;
		return func;
	}
	else
	if (strcmp(node->child->brother->brother->name, "VarList") == 0)
	{
		VarList(node->child->brother->brother, func);
		return func;
	}
	else
	{
		printf("FunDec does not have other.\n");
		return NULL;
	}
}
void search(tree* node)
{
	if (strcmp(node->name, "ExtDef") == 0)
	{
		Type type = Specifier(node->child);
		if (strcmp(node->child->brother->name, "FunDec") == 0)
		{
			if (strcmp(node->child->brother->brother->name, "Compst") == 0)
				func_def_table = FunDec(node->child->brother, type);
		}
	}
	else
	if (node->child != NULL) search(node->child);
	if (node->brother != NULL) search(node->brother);
}
void check_semantic(tree *root)
{
	init_hash();
	search(root);
	printf("We are checking!\n");
}

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

void insert_symbol_table(symbol_table node)
{
	unsigned index = hash_table(node->name);
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
