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
	{
		type->Kind = STRUCTURE;
		return type;
	}
	else
	{
		printf("Specifier does not have other.\n");
		return NULL;
	}
}
void VarDec(Tree *node)
{
	if (node->Para->type == NULL)
		node->Para->type = node->type;
	if (node->num == 1)
	{
		strcpy(node->Para->name, node->child->name);
		return;
	}
	else
	{
		assert(node->num == 4);
		Type type = (Type)malloc(sizeof(struct TYPE));
		type->Kind = ARRAY;
		type->Array.size = atoi(node->child->brother->brother->value);
		type->Array.element = node->Para->type;
		node->Para->type = type;
		node->child->type = node->type;
		node->child->Para = node->Para;
		VarDec(node->child);
		node->type = node->child->type;
		node->Para = node->child->Para;
		return;
	}
}
void ParamDec(Tree* node, func_def_table func)
{
	assert(strcmp(node->name, "ParamDec") == 0);
	
	struct para* Para = (struct para*)malloc(sizeof(struct para));
	Para->type = NULL;
	
	Type type = Specifier(node->child);
	node->child->type = type;
	node->child->Para = Para;
	VarDec(node->child);
	
	struct para* p = node->child->Para;
	func->num_para++;
	if (func->list_para == NULL)
	{
		func->list_para = p;
		p->next_para = NULL;
	}
	else
	{
		p->next_para = func->list_para;
		func->list_para = p;
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
	func->num_para = 0;
	
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
void DecList(Type type, Tree* node)
{
	
}
void Def(Tree* node)
{
	if (node->struct_def)
	{
		node->child->struct_def = node->struct_def;
		node->child->st = node->st;
		Type type = (Type)malloc(sizeof(struct TYPE));
		type = Specifier(node->child);
	}
	else
	{
		Type type = (Type)malloc(sizeof(struct TYPE));
		type = Specifier(node->child);
		DecList(type, node->child->brother);
	}
}
void DefList(Tree* node)
{
	if (!node->num) return;
	assert(node->num == 2);
	if (node->struct_def)
	{
		Def(node);
		node->child->brother->struct_def = 1;
		node->child->brother->st = node->st;
		DefList(node->child);
		node->st = node->child->st;
	}
	else
	{
		Def(node->child);
		DefList(node->child->brother);
	}
}
struct_table StructSpecifier(Type type, Tree* node)
{
	assert(strcmp(node->name, "StructSpecifier") == 0);
	struct_table st = (struct_table)malloc(sizeof(struct StructTableNode));
	st->type = type;
	if (node->num != 2)
	{
		assert(node->num == 5);
		st->Kind = Definition;
		Tree* children = node->child->brother;
		if (children->num != 0)
		{
			assert(children->num == 1);
			strcpy(st->name, children->child->value);
		}
		children = children->brother->brother;
		children->st = st;
		children->struct_def = 1;
		DefList(children);
		return children->st;
	}
	else
	{
		st->Kind = Declaration;
		printf("The struct name is %s\n", node->child->brother->child->value);
		strcpy(st->name, node->child->brother->child->value);
		return st; 
	}
	return st;
}
void Compst(Tree* node)
{
	DefList(node->child->brother);
}
void search(Tree* node, int blank)
{
	if (strcmp(node->name, "ExtDef") == 0)
	{
		
		if (strcmp(node->child->brother->name, "FunDec") == 0)
		{
			Type type = Specifier(node->child);
			func_def_table func = FunDec(node->child->brother, type);
			int return_val = insert_function_def_table(func);
			printf("%d\n", return_val);
			Compst(node->child->brother->brother);
			
		}
		else
		if (strcmp(node->child->brother->name, "SEMI") == 0)
		{
			Type type = Specifier(node->child);
			struct_table st = StructSpecifier(type, node->child->child);
		}
		else
		if (strcmp(node->child->brother->name, "ExtDecList") == 0)
		{}
	}
	
	if (node->child != NULL) search(node->child, blank + 2);
	if (node->brother != NULL) search(node->brother, blank);
}
void check_semantic(Tree *root)
{
	printf("We are checking!\n");
	init_hash();
	search(root, 0);
	check_function_table();
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
						while (type->Kind != BASIC)
						{
							printf("		The size of array is: %d\n", type->Array.size);
							type = type->Array.element;
						}
						if (type->Kind == BASIC)
							printf("		The basic type is %d\n", type->Kind);
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
