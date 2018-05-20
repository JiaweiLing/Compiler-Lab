#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<assert.h>
#include"semantic.h"

Type Specifier(Tree* node)
{
	Type type = (Type)malloc(sizeof(struct TYPE));
	if (strcmp(node->child->name, "TYPE") == 0)
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
	if (strcmp(node->child->name, "StructSpecifier") == 0)
	{;}
	else
	{
		printf("Specifier does not have other.\n");
		return NULL;
	}
}
void VarDec(Tree *node, Type type_pre, struct para* Para)
{
	if (Para->type == NULL)
		Para->type = type_pre;
	if (node->num == 1)
	{
		strcpy(Para->name, node->child->name);
		return;
	}
	else
	{
		assert(node->num == 4);
		Type type = (Type)malloc(sizeof(struct TYPE));
		type->Kind = ARRAY;
		type->Array.size = atoi(node->child->brother->brother->name);
		type->Array.element = Para->type;
		Para->type = type;
		
		VarDec(node->child, type_pre, Para);
		return;
	}
}
void ParamDec(Tree* node, func_def_table func)
{
	assert(strcmp(node->name, "ParamDec") == 0);
	
	struct para* Para = (struct para*)malloc(sizeof(struct para));
	Para->type = NULL;
	
	Type type = Specifier(node->child);
	VarDec(node->child->brother, type, Para);
	
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

void VarList(Tree* node, func_def_table func)
{
	assert(strcmp(node->name, "VarList") == 0);
	switch (node->num)
	{
		case 1:
			ParamDec(node->child, func);
			return;
			break;
		case 3:
			ParamDec(node->child, func);
			VarList(node->child->brother->brother, func);
			return;
			break;
		default:
			break;
	}
}

func_def_table FunDec(Tree* node, Type type)
{
	func_def_table func = (func_def_table)malloc(sizeof(struct FunctionDefTableNode));
	func->return_type = type->Basic;
	
	strcpy(func->name, node->child->value);
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
void search(Tree* node, int blank)
{
	if (strcmp(node->name, "ExtDef") == 0)
	{
		Type type = Specifier(node->child);
		if (strcmp(node->child->brother->name, "FunDec") == 0)
		{
			if (strcmp(node->child->brother->brother->name, "Compst") == 0)
			{
				func_def_table func = FunDec(node->child->brother, type);
				int return_value = insert_function_def_table(func);
				printf("%d\n", return_value);
			}
			
		}
	}
	else
	if (node->child != NULL) search(node->child, blank + 2);
	if (node->brother != NULL) search(node->brother, blank);
}
void check_semantic(Tree *root)
{
	printf("We are checking!\n");
	init_hash();
	search(root, 0);
	printf("\n\n");
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
int insert_function_def_table(func_def_table node)
{
	printf("The function name is %s\n", node->name);
	unsigned index = hash_table(node->name);
	printf("%d\n", index);
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
