#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<assert.h>
#include"semantic.h"
#include"hash_table.h"



void search(Tree* node, int blank);

Type Specifier(Tree* node);
void VarDec(Tree *node);
void ParamDec(Tree* node);
void VarList(Tree* node);
void StructSpecifier(Tree* node);
void DefList(Tree* node);
void Def(Tree* node);
void Compst(Tree* node);
void Dec(Tree* node);
void DecList(Tree* node);
void ExtDecList(Tree *node);
void FunDec(Tree* node);

Type set_kind(Type type, enum basic_type basic)
{
	type->Kind = BASIC;
	type->Basic = basic;
	return type;
}
Type Specifier(Tree* node)
{
	Type type = (Type)malloc(sizeof(struct TYPE));
	if (strcmp(node->child->name, "TYPE") == 0)
	{
		if (strcmp(node->child->value, "int") == 0)
		{
			type = set_kind(type, INT_);
			return type;
		}
		else
		if (strcmp(node->child->value, "float") == 0)
		{
			type = set_kind(type, FLOAT_);
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
symbol_table set_st_STRUCTURE(symbol_table syt, Tree* node)
{
	strcpy(syt->name, node->child->name);
	if (node->type->Kind == STRUCTURE)
		strcpy(syt->struct_name, node->struct_name);
	syt->line = node->child->size;
	syt->type = node->type;
	return syt;
}
void VarDec_st_Children(Tree* node, symbol_table st, enum var_kind kind)
{
	node->first_verdec = 0;
	node->kind = kind;
	node->symt = st;
	VarDec(node);
}
void VarDec(Tree *node)
{
	if (node->Para->type == NULL)
		node->Para->type = node->type;
	if (node->num == 1)
	{
		if (node->kind == global_var || node->kind == func_body)
		{
			//node->symt->next = NULL;
			if (node->first_verdec)
			{
				//Type type = (Type)malloc(sizeof(struct TYPE));
				//type->Kind = node->type->Kind;
				//node->symt->type = type;
				//trcpy(node->symt->name, node->child->value);
				//node->symt->line = node->child->size;
				//unsigned value = insert_symbol_table(node->symt);
				//printf("%d\n", value);
				symbol_table syt = (symbol_table)malloc(sizeof(struct SymbolTableNode));
				
				syt = set_st_STRUCTURE(syt, node);
				unsigned value = insert_symbol_table(syt);
				printf("%d\n", value);
			}
			else
			{
				//strcpy(node->symt->struct_name, node->struct_name);
				//Type type = (Type)malloc(sizeof(struct TYPE));
				//type->Kind = node->type->kind;
				//node->symt->type = type;
				strcpy(node->symt->name, node->child->value);
				node->symt->line = node->child->size;
				unsigned value = insert_symbol_table(node->symt);
				printf("%d\n", value);
			}
		}
		else
		if (node->kind == func_dec)
		{
			if (node->first_verdec)
			{
				struct para* Para = (struct para*)malloc(sizeof(struct para));
				strcpy(Para->name, node->child->value);
				if (node->type->Kind == STRUCTURE)
					strcpy(Para->struct_name, node->struct_name);
				Para->type = node->type;
				node->Para = Para;
			}
			else
			{
				strcpy(node->Para->name, node->child->name);
				return;
			}
		}
		else
		
		if (node->kind == str_def)
		{}
		//else
		//{
		//	printf("VarDec node type error!\n");
		//}
	}
	else
	{
		if (node->kind == global_var || node->kind == func_body)
		{
			if (node->first_verdec)
			{
				symbol_table syt = (symbol_table)malloc(sizeof(struct SymbolTableNode));
				syt->type = node->type;
				if (node->type->Kind == STRUCTURE)
					strcpy(syt->struct_name, node->struct_name);
				Type type = (Type)malloc(sizeof(struct TYPE));
				type->Kind = ARRAY;
				type->Array.size = atoi(node->child->brother->brother->value);
				type->Array.element = syt->type;
				syt->type = type;
				Tree* children = node->child;
				VarDec_st_Children(children, syt, node->kind);
			}
			else
			{
				Type type = (Type)malloc(sizeof(struct TYPE));
				type->Kind = ARRAY;
				type->Array.size = atoi(node->child->brother->brother->value);
				type->Array.element = node->symt->type;
				node->symt->type = type;
				Tree* children = node->child;
				
				VarDec_st_Children(children, node->symt, node->kind);

			}
		}
		else
		if (node->kind == func_dec)
		{
			if (node->first_verdec)
			{
				struct para* Para = (struct para*)malloc(sizeof(struct para));
				Para->type = node->type;
				if (node->type->Kind == STRUCTURE)
					strcpy(Para->struct_name, node->struct_name);
				Type type = (Type)malloc(sizeof(struct TYPE));
				type->Kind = ARRAY;
				type->Array.size = atoi(node->child->brother->brother->value);
				type->Array.element = Para->type;
				Para->type = type;
				Tree* children = node->child;
				children->first_verdec = 0;
				children->kind = node->kind;
				children->Para = Para;
				
				VarDec(children);
				node->Para = children->Para;
			}
			else
			{
				Type type = (Type)malloc(sizeof(struct TYPE));
				type->Kind = ARRAY;
				type->Array.size = atoi(node->child->brother->brother->value);
				type->Array.element = node->Para->type;
				node->Para->type = type;
				Tree* children = node->child;
				children->first_verdec = 0;
				children->kind = node->kind;
				children->Para = node->Para;
				
				VarDec(children);
				node->Para = children->Para;
			}
		}
		else
		if (node->kind == str_def)
		{}
	
	/*
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
	*/
	}
}
void ParamDec(Tree* node)
{
	assert(strcmp(node->name, "ParamDec") == 0);
	/*
	struct para* Para = (struct para*)malloc(sizeof(struct para));
	Para->type = NULL;
	*/
	Type type = Specifier(node->child);
	/*
	node->child->brother->type = type;
	node->child->brother->Para = Para;
	VarDec(node->child->brother);
	
	struct para* p = node->child->brother->Para;
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
	*/
	Tree* children = node->child;
	if (type->Kind == STRUCTURE)
	{
		Tree* grandchildren = children->child;
		grandchildren->scope = node->scope;
		grandchildren->kind = node->kind;
		StructSpecifier(grandchildren);
		strcpy(node->struct_name, grandchildren->struct_name);
	}
	children = children->brother;
	children->kind = node->kind;
	children->first_verdec = 1;
	children->type = type;
	
	strcpy(children->struct_name, node->struct_name);
	VarDec(children);
	struct para* p = node->child->brother->Para;
	node->func->num_para++;
	if (node->func->list_para == NULL)
	{
		node->func->list_para = p;
		p->next_para = NULL;
	}
	else
	{
		p->next_para = node->func->list_para;
		node->func->list_para = p;
	}
}

void VarList(Tree* node)
{
	assert(strcmp(node->name, "VarList") == 0);
	Tree* children = node->child;
	children->kind = node->kind;
	children->func = node->func;
	ParamDec(children);
	node->func = children->func;
	switch (node->num)
	{
		case 1:
			return;
			break;
		case 3:
			children = children->brother->brother;
			children->kind = node->kind;
			children->func = node->func;
			VarList(children);
			return;
			break;
		default:
			break;
	}
}

void FunDec(Tree* node)
{
	func_def_table func = (func_def_table)malloc(sizeof(struct FunctionDefTableNode));
	func->return_type = node->type->Basic;
	func->num_para = 0;
	
	strcpy(func->name, node->child->value);
	
	if (strcmp(node->child->brother->brother->name, "RP") == 0)
	{
		func->num_para = 0;
		func->list_para = NULL;
		func->line = node->child->brother->brother->size;
		unsigned value = insert_function_def_table(func);
		printf("%d\n", value);
	}
	else
	if (strcmp(node->child->brother->brother->name, "VarList") == 0)
	{
		VarList(node->child->brother->brother);
		func->num_para = 0;
		func->list_para = NULL;
		Tree* children = node->child->brother->brother;
		func->line = children->size;
		children->func = func;
		children->kind = node->kind;
		VarList(children);
		unsigned value = insert_function_def_table(func);
		printf("%d\n", value);
	}
	else
	{
		printf("FunDec does not have other.\n");
		return;
	}
}
Tree *set_Dec_children(Tree* children, Tree* node)
{
	children->type = node->type;
	children->kind = node->kind;
	children->strt = node->strt;
	strcpy(children->struct_name, node->struct_name);
	return children;
}
void Dec(Tree* node)
{
	//if (node->num == 1)
	Tree* children;
	switch (node->kind)
	{
		case str_def:
			//node->child->type = type;
			//VarDec(node->child);
			children = node->child;
			children = set_Dec_children(children, node);
		
			VarDec(children);
		
			node->strt = children->strt;
			if (node->num == 1) return;
			else
			{
				children = children->brother->brother;
				return;
			}
			break;
		case func_body:
			break;
		default:
			break;
	}
	//else
	//if (node->kind == func_body)
	//{}
}

void DecList(Tree* node)
{
	Tree* children = node->child;
	children = set_Dec_children(children, node);
	children->scope = node->scope;
	
	
	if (node->num == 1)
	{
		// if (node->struct_def) node->child->struct_def = 1;
		//Dec(type, node->child);
		switch (node->kind)
		{
			case str_def:
				node->strt = children->strt;
				return;
				break;
			case func_body:
				break;
			default:
				break;
		}
			
		
	}
	else
	{
		//Dec(type, node->child);
		//DecList(type, node->child->brother->brother);
		children = children->brother->brother;
		children = set_Dec_children(children, node);
		children->scope = node->scope;
		switch (node->kind)
		{
			case str_def:
				node->strt = children->strt;
				return;
				break;
			case func_body:
				break;
			default:
				break;
		}
	}
}
void Def(Tree* node)
{
	//if (node->struct_def)
	Type type = (Type)malloc(sizeof(struct TYPE));
	Tree* children = node->child;
	Tree* grandchildren = children->child;
	switch (node->kind)
	{
		case str_def:
			//node->child->struct_def = node->struct_def;
			//node->child->strt = node->strt;
			//children = node->child;
			type = Specifier(node->child);
			//DecList(type, node->child);
			if (type->Kind == STRUCTURE)
			{
				//grandchildren = children->child;
				grandchildren->kind = node->kind;
				grandchildren->scope = node->scope;
				StructSpecifier(grandchildren);
				strcpy(node->struct_name, grandchildren->struct_name);
				
			}
			node->type = type;
			children = children->brother;
			children = set_Dec_children(children, node);
			DecList(children);
			node->strt = children->strt;
			break;
		case func_body:
			children = node->child;
			Type type = (Type)malloc(sizeof(struct TYPE));
			type = Specifier(children);
			DecList(children->brother);
			break;
		default:
			break;
	}
	//else
	//{
		//Type type = (Type)malloc(sizeof(struct TYPE));
		//type = Specifier(node->child);
		//DecList(type, node->child->brother);
	//}
}
void DefList(Tree* node)
{
	if (node->num == 0) return;
	//assert(node->num == 2);
	//if (node->struct_def)
	Tree* children = node->child;
	switch (node->kind)
	{
		case str_def:
			//Def(node);
			//children = node->child;
			children->kind = node->kind;
			children->strt = node->strt;
			children->scope = node->scope;
			Def(children);
			node->strt = children->strt;
			children = children->brother;
			//node->child->brother->struct_def = 1;
			children->kind = node->kind;
			children->scope = node->scope;
			children->strt = node->strt;
			DefList(children);
			node->strt = children->strt;
			break;
		case func_body:
			//children = node->child;
			Def(children);
			children = children->brother;
			DefList(children);
			break;
		default:
			break;
	}
	//else
	//{
	//	Def(node->child);
	//	DefList(node->child->brother);
	//}
}

void StructSpecifier(Tree* node)
{
	assert(strcmp(node->name, "StructSpecifier") == 0);
	//struct_table st = (struct_table)malloc(sizeof(struct StructTableNode));
	//st->type = type;
	if (node->num != 2)
	{
	
		assert(node->num == 5);
		//st->Kind = Definition;
		node->strt->Kind = Definition;
		Tree* children = node->child->brother;
		if (children->num != 0)
		{
			assert(children->num == 1);
			strcpy(node->strt->name, children->child->value);
		}
		children = children->brother->brother;
		//children->strt = st;
		//children->struct_def = 1;
		children->strt = node->strt;
		children->scope = node->scope;
		children->kind = node->kind;
		DefList(children);
		node->strt = children->strt;
	
	}
	else
	{
		printf("The struct name is %s\n", node->child->brother->child->value);
		strcpy(node->struct_name, node->child->brother->child->value);
		return; 
	}
	
}
void Compst(Tree* node)
{
	//DefList(node->child->brother);
}
void ExtDecList(Tree* node)
{
	Tree* children = node->child;
	children->kind = node->kind;
	children->scope = node->scope;
	children->type = node->type;
	strcpy(children->struct_name, node->struct_name);
	children->first_verdec = 1;
	VarDec(children);
	if (node->num == 1) return;
	else
	{
		children = children->brother->brother;
		children->kind = node->kind;
		children->scope = node->scope;
		children->type = node->type;
		strcpy(children->struct_name, node->struct_name);
		ExtDecList(children);
		return;
	}
}
int scope = 0;
Tree* set_children(Tree* children, Tree* node)
{
	children = node->child;
	children->kind = node->kind;
	children->scope = node->scope;
	return children;
}
void search(Tree* node, int blank)
{
	if (strcmp(node->name, "ExtDef") == 0)
	{
		if (strcmp(node->child->brother->name, "FunDec") == 0)
		{
			node->kind = func_dec;
			node->scope = scope;
			scope++;
			Tree* children;
			
			children = set_children(children, node);
			Type type = Specifier(children);
			//node->type = type;
			children = children->brother;
			//children->kind = node->kind;
			children->type = type;
			children->kind = func_dec;
			children->scope = node->scope;
			//children->type = node->type;
			FunDec(children);
			children = children->brother;
			children->kind = func_body;
			children->scope = node->scope;
			
		}
		else
		if (strcmp(node->child->brother->name, "SEMI") == 0)
		{
			node->kind = str_def;
			node->scope = scope;
			scope++;
			Tree* children;
			children = node->child->child;
			children->kind = node->kind;
			children->scope = node->scope;
			struct_table st = (struct_table)malloc(sizeof(struct StructTableNode));
			children->strt = st;
			StructSpecifier(children);
		}
		else
		if (strcmp(node->child->brother->name, "ExtDecList") == 0)
		{
			node->kind = global_var;
			node->scope = scope;
			scope++;
			Tree* children;
			children = set_children(children, node);
			Type type = Specifier(children);
			if (type->Kind == STRUCTURE)
			{
				Tree* grandchildren = children->child;
				grandchildren->kind = node->kind;
				grandchildren->scope = node->scope;
				StructSpecifier(grandchildren);
				strcpy(node->struct_name, grandchildren->struct_name);
			}
			node->type = type;
			children = children->brother;
			children->kind = node->kind;
			children->scope = node->scope;
			strcpy(children->struct_name, node->struct_name);
			children->type = node->type;
			ExtDecList(children);
		}
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
	check_symbol_table();
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
	func_def_table fdt = FunctionDefHash[index];
	while (fdt != NULL)
	{
		if (strcmp(fdt->name, node->name) == 0)
		{
			printf("Error type 4 at Line %d: Redefined function \"%s\"\n", node->line, node->name);
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
			printf("Error type 3 at Line %d: Redefined variable \"%s\"\n", node->line, node->name);
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
