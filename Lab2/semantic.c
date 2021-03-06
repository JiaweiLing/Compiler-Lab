#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include"semantic.h"
#include"symbol_table.h"
#include"tree.h"
void errorprint(int errorcode, int line, char* name)
{
	switch (errorcode)
	{
		case 1:
			printf("Error type 1 at Line %d: Undefined variable \"%s\".\n", line, name);
			break;
		case 2:
			printf("Error type 2 at Line %d: Undefined function \"%s\".\n", line, name);
			break;
		case 3:
			printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", line, name);
			break;
		case 4:
			printf("Error type 4 at Line %d: Redefined function \"%s\".\n", line, name);
			break;
		case 5:
			printf("Error type 5 at Line %d: Type mismatched for assignment.\n", line);
			break;
		case 6:
			printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", line);
			break;
		case 7:
			printf("Error type 7 at Line %d: Type mismatched for operands.\n", line);
			break;
		case 8:
			printf("Error type 8 at Line %d: Type mismatched for return.\n", line);
			break;
		case 9:
			printf("Error type 9 at Line %d: Function \"%s\" is not applicable for arguments.\n", line, name);
			break;
		case 10:
			printf("Error type 10 at Line %d: This variable is not an array.\n", line);
			break;
		case 11:
			printf("Error type 11 at Line %d: \"%s\" is not a function.\n", line, name);
			break;
		case 12:
			printf("Error type 12 at Line %d: This number is not an integer.\n", line);
			break;
		case 13:
			printf("Error type 13 at Line %d: Illegal use of \".\"\n", line);
			break;
		case 14:
			printf("Error type 14 at Line %d: Non-existent field \"%s\".\n", line, name);
			break;
		case 15:
			printf("Error type 15 at Line %d: Redefined field \"%s\".\n", line, name);
			break;
		case 16:
			printf("Error type 16 at Line %d: Duplicated name \"%s\".\n", line, name);
			break;
		case 17:
			printf("Error type 17 at Line %d: Undefined structure \"%s\".\n", line, name);
			break;
	}
}

void search(Tree* node, int blank);

Type Specifier(Tree* node);
void VarDec(Tree *node);
void ParamDec(Tree* node);
void VarList(Tree* node);
void StructSpecifier(Tree* node);
void DefList(Tree* node);
void Def(Tree* node);
void ExtDef(Tree* node);
void ExtDefList(Tree* node);
void Compst(Tree* node);
void Dec(Tree* node);
void DecList(Tree* node);
void ExtDecList(Tree *node);
void FunDec(Tree* node);
void Exp(Tree* node);
void Stmt(Tree* node);
void StmtList(Tree* node);
int Args(Tree* node, int num);

void deal_with_value(unsigned value, func_def_table func)
{
	if (value != -1)
	{
		var_table var_t = (var_table)malloc(sizeof(struct VarTableNode));
		strcpy(var_t->name, func->name);
		Type type = (Type)malloc(sizeof(struct TYPE));
		type->Kind = 4;
		var_t->type = type;
		insert_var_table(var_t);
	}
}
Tree *deal_with_children(Tree* children, Tree* node)
{
	children->type = node->type;
	children->kind = node->kind;
	strcpy(children->struct_name, node->struct_name);
	return children;
}
Tree* deal_with_grandchildren(Tree* grandchildren, Tree* node)
{
	grandchildren->scope = node->scope;
	grandchildren->kind = node->kind;
	return grandchildren;
}


Type set_kind(Type type, enum basic_type basic)
{
	type->Kind = 1;
	type->Basic = basic;
	return type;
}
Type Specifier(Tree* node)
{
	if (node == NULL) return NULL;
	Type type = (Type)malloc(sizeof(struct TYPE));
	if (strcmp(node->child->name, "TYPE") == 0)
	{
		if (strcmp(node->child->value, "int") == 0)
		{
			type = set_kind(type, 1);
			return type;
		}
		else
		if (strcmp(node->child->value, "float") == 0)
		{
			type = set_kind(type, 2);
			return type;
		}
		else
		{
			return NULL;
		}
	}
	else
	if (strcmp(node->child->name, "StructSpecifier") == 0)
	{
		type->Kind = 3;
		return type;
	}
	else
	{
		return NULL;
	}
}


void VarDec(Tree *node)
{
	if (node == NULL) return;
	if (node->num == 1)
	{
		if (node->kind == 1 || node->kind == 3)
		{
			//node->vt->next = NULL;
			if (node->first_verdec)
			{
				//Type type = (Type)malloc(sizeof(struct TYPE));
				//type->Kind = node->type->Kind;
				//node->vt->type = type;
				//trcpy(node->vt->name, node->child->value);
				//node->vt->line = node->child->size;
				//unsigned value = insert_var_table(node->vt);
				//printf("%d\n", value);
				var_table var_t = (var_table)malloc(sizeof(struct VarTableNode));
				
				strcpy(var_t->name, node->child->value);
				if (node->type->Kind == 3)
					strcpy(var_t->struct_name, node->struct_name);
				var_t->line = node->child->size;
				var_t->type = node->type;
				unsigned value = insert_var_table(var_t);
				
			}
			else
			{
				//strcpy(node->vt->struct_name, node->struct_name);
				//Type type = (Type)malloc(sizeof(struct TYPE));
				//type->Kind = node->type->kind;
				//node->vt->type = type;
				strcpy(node->vt->name, node->child->value);
				node->vt->line = node->child->size;
				unsigned value = insert_var_table(node->vt);
				
			}
		}
		else
		if (node->kind == 2)
		{
			if (node->first_verdec)
			{
				struct para* Para = (struct para*)malloc(sizeof(struct para));
				var_table var_t = (var_table)malloc(sizeof(struct VarTableNode));
				strcpy(Para->name, node->child->value);
				strcpy(var_t->name, node->child->value);
				var_t->type = node->type;
				insert_var_table(var_t);
				if (node->type->Kind == 3)
					strcpy(Para->struct_name, node->struct_name);
				Para->type = node->type;
				node->Para = Para;
				return;
			}
			else
			{
				strcpy(node->Para->name, node->child->value);
				return;
			}
		}
		else
		
		if (node->kind == 4)
		{
			if (node->first_verdec)
			{
				FieldList fieldlist = (FieldList)malloc(sizeof(struct FIELDLIST));
				strcpy(fieldlist->name, node->child->value);
				if (node->type->Kind == 3)
					strcpy(fieldlist->struct_name, node->struct_name);
				fieldlist->type = node->type;
				node->fieldlist = fieldlist;
			}
			else
			{
				strcpy(node->fieldlist->name, node->child->value);
			}
		}

	}
	else
	{
		if (node->kind == 1 || node->kind == 3)
		{
			if (node->first_verdec)
			{
				var_table var_t = (var_table)malloc(sizeof(struct VarTableNode));
				var_t->type = node->type;
				if (node->type->Kind == 3)
					strcpy(var_t->struct_name, node->struct_name);
				Type type = (Type)malloc(sizeof(struct TYPE));
				
				type->Kind = 2;
				type->Array.size = atoi(node->child->brother->brother->value);
				type->Array.element = var_t->type;
				var_t->type = type;
				Tree* children = node->child;
				children->vt = var_t;
				children->first_verdec = 0;
				children->kind = node->kind;
				VarDec(children);
			}
			else
			{
				Type type = (Type)malloc(sizeof(struct TYPE));
				type->Kind = 2;
				type->Array.size = atoi(node->child->brother->brother->value);
				type->Array.element = node->vt->type;
				node->vt->type = type;
				Tree* children = node->child;
				//VarDec_st_Children(children, node->vt, node->kind);

				children->vt = node->vt;
				children->first_verdec = 0;
				children->kind = node->kind;
				VarDec(children);
			}
		}
		else
		if (node->kind == 2)
		{
			if (node->first_verdec)
			{
				struct para* Para = (struct para*)malloc(sizeof(struct para));
				Para->type = node->type;
				if (node->type->Kind == 3)
					strcpy(Para->struct_name, node->struct_name);
				Type type = (Type)malloc(sizeof(struct TYPE));
				type->Kind = 2;
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
				type->Kind = 2;
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
		if (node->kind == 4)
		{
			if (node->first_verdec)
			{
				FieldList fieldlist = (FieldList)malloc(sizeof(struct FIELDLIST));
				fieldlist->type = node->type;
				if (node->type->Kind == 3)
					strcpy(fieldlist->struct_name, node->struct_name);
				Type type = (Type)malloc(sizeof(struct TYPE));
				type->Kind = 2;
				type->Array.element = fieldlist->type;
				type->Array.size = atoi(node->child->brother->brother->value);
				fieldlist->type = type;
				Tree* children = node->child;
				children->fieldlist = fieldlist;
				children->first_verdec = 0;
				children->kind = node->kind;
				VarDec(children);
				node->fieldlist = children->fieldlist;
			}
			else
			{
				Type type = (Type)malloc(sizeof(struct TYPE));
				type->Kind = 2;
				type->Array.element = node->fieldlist->type;
				type->Array.size = atoi(node->child->brother->brother->value);
				node->fieldlist->type = type;
				Tree* children = node->child;
				children->fieldlist = node->fieldlist;
				children->first_verdec = 0;
				children->kind = node->kind;
				VarDec(children);
				node->fieldlist = children->fieldlist;
			}
		}
	}
}

void ParamDec(Tree* node)
{
	if (node == NULL) return;
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
	if (type->Kind == 3)
	{
		Tree* grandchildren = children->child;
		grandchildren = deal_with_grandchildren(grandchildren, node);

		StructSpecifier(grandchildren);
		strcpy(node->struct_name, grandchildren->struct_name);
	}
	children = children->brother;
	children->kind = node->kind;
	children->first_verdec = 1;
	children->type = type;
	
	strcpy(children->struct_name, node->struct_name);
	VarDec(children);
	struct para* p = children->Para;
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
	if (node == NULL) return;
	Tree* children = node->child;
	children->kind = node->kind;
	children->func = node->func;
	ParamDec(children);
	node->func = children->func;
	if (node->num == 1) return;
	else
	{
		children = children->brother->brother;
		children->kind = node->kind;
		children->func = node->func;
		VarList(children);
		return;
	}
}

void FunDec(Tree* node)
{
	if (node == NULL) return;
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
		deal_with_value(value, func);
	}
	else
	if (strcmp(node->child->brother->brother->name, "VarList") == 0)
	{
		//VarList(node->child->brother->brother);
		func->num_para = 0;
		func->list_para = NULL;
		Tree* children = node->child->brother->brother;
		func->line = children->size;
		children->func = func;
		children->kind = node->kind;
		VarList(children);
		unsigned value = insert_function_def_table(children->func);
		deal_with_value(value, func);
	}
	else
		return;
}

void Dec(Tree* node)
{
	//if (node->num == 1)
	if (node == NULL) return;
	Tree* children = node->child;
	switch (node->kind)
	{
		case 4:
			//node->child->type = type;
			//VarDec(node->child);
			//children = node->child;
			children = deal_with_children(children, node);
			children->strt = node->strt;
			children->first_verdec = 1;
			VarDec(children);
		
			//node->strt = children->strt;
			//if (node->num == 1) return;
			//else
			//{
				//children = children->brother->brother;
				//return;
			//}
			/*
			
			*/
			FieldList fieldlist = children->strt->fieldlist;
			int re_def = 0;
			while (fieldlist != NULL)
			{
				if (strcmp(children->fieldlist->name, fieldlist->name) == 0)
				{
					errorprint(15, children->size, children->fieldlist->name);
					re_def = 1;
					break;
				}
				fieldlist = fieldlist->next;
			}
			if (!re_def)
			{
				if (children->strt->fieldlist == NULL)
				{
					children->strt->fieldlist = children->fieldlist;
					children->fieldlist->next = NULL;
				}
			
				else
				{
					children->fieldlist->next = children->strt->fieldlist;
					children->strt->fieldlist = children->fieldlist;
				}
			}
			node->strt = children->strt;
			return;
			break;
		case 3:
			//return;
			children = deal_with_children(children, node);
			children->first_verdec = 1;
			VarDec(children);
			if (node->num == 1) return;
			else
			{
				//children = children->brother->brother;
				//children->kind = node->kind;
				var_table var_t = search_variable(node->child->child);
				children = node->child->brother->brother;
				children->kind = node->kind;
				Exp(children);
				if (children->exp == 2)
					if (var_t->type->Kind != 1 ||
					(var_t->type->Kind == 1 && var_t->type->Basic != 1))
						errorprint(5, children->size, "");
				else
				if (children->exp == 3)
					if (var_t->type->Kind != 1 ||
					(var_t->type->Kind == 1 && var_t->type->Basic != 2))
						errorprint(5, children->size, "");
			}
			break;
		default:
			break;
	}
	//else
	//if (node->kind == 3)
	//{}
}

void DecList(Tree* node)
{
	if (node == NULL) return;
	Tree* children = node->child;
	children = deal_with_children(children, node);
	children->strt = node->strt;
	children->scope = node->scope;
	Dec(children);
	
	if (node->num == 1)
	{
		// if (node->struct_def) node->child->struct_def = 1;
		//Dec(type, node->child);
		switch (node->kind)
		{
			case 4:
				node->strt = children->strt;
				return;
				break;
			case 3:
				return;
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
		children = deal_with_children(children, node);
		children->strt = node->strt;
		children->scope = node->scope;
		DecList(children);
		switch (node->kind)
		{
			case 4:
				node->strt = children->strt;
				return;
				break;
			case 3:
				return;
				break;
			default:
				break;
		}
	}
}
void ExtDefList(Tree* node)
{
	if (node == NULL) return;
	if (node->child == NULL) return;
	ExtDef(node->child);
	ExtDefList(node->child->brother);
}
void ExtDef(Tree* node)
{
	if (node == NULL) return;
	Type type = (Type)malloc(sizeof(struct TYPE));
	type = Specifier(node->child);
	if (strcmp(node->child->brother->name, "SEMI") == 0) return;
	else
	if (strcmp(node->child->brother->name, "ExtDecList") == 0)
		ExtDecList(node->child->brother);
	else
	if (strcmp(node->child->brother->name, "FunDec") == 0)
	{
		FunDec(node->child->brother);
		func_def_table func = search_func(node->child);
		if (strcmp(node->child->name, "Compst") == 0)
		{
			if (func == NULL) return;
			func->judge_func = 1;
			Compst(node->child->brother);
		}
		else
		if (strcmp(node->child->brother->name, "SEMI") == 0)
		{
			if (func == NULL) return;
			func->judge_func = 0;
		}
	}
	else return;
}
void Def(Tree* node)
{
	//if (node->struct_def)
	if (node == NULL) return;
	Type type = (Type)malloc(sizeof(struct TYPE));
	Tree* children = node->child;
	Tree* grandchildren = children->child;
	switch (node->kind)
	{
		case 4:
			//node->child->struct_def = node->struct_def;
			//node->child->strt = node->strt;
			//children = node->child;
			type = Specifier(children);
			//DecList(type, node->child);
			if (type->Kind == 3)
			{
				//grandchildren = children->child;
				grandchildren = deal_with_grandchildren(grandchildren, node);
				
				StructSpecifier(grandchildren);
				strcpy(node->struct_name, grandchildren->struct_name);
				
			}
			node->type = type;
			children = children->brother;
			children = deal_with_children(children, node);
			children->strt = node->strt;

			DecList(children);
			node->strt = children->strt;
			break;
		case 3:
			type = Specifier(children);
			if (type->Kind == 3)
			{
				//grandchildren = children->child;
				grandchildren = deal_with_grandchildren(grandchildren, node);
				StructSpecifier(grandchildren);
				strcpy(node->struct_name, grandchildren->struct_name);
				
			}
			node->type = type;
			children = children->brother;
			children = deal_with_children(children, node);
			DecList(children);
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
	if (node == NULL) return;
	if (node->num == 0) return;
	
	//if (node->struct_def)
	Tree* children = node->child;
	switch (node->kind)
	{
		case 4:
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
		case 3:
			//children = node->child;
			children->kind = node->kind;
			children->return_type = node->return_type;
			Def(children);
			children = children->brother;
			children->kind = node->kind;
			children->return_type = node->return_type;
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
	if (node == NULL) return;
	
	//struct_table st = (struct_table)malloc(sizeof(struct StructTableNode));
	//st->type = type;
	if (node->num != 2)
	{
	
		//st->Kind = 1;
		//node->strt->Kind = 1;
		Tree* children = node->child->brother;
		if (children->num != 0)
			strcpy(node->strt->name, children->child->value);
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
		strcpy(node->struct_name, node->child->brother->child->value);
		struct_table st = search_struct(node);
		if (st == NULL)
			errorprint(17, node->child->brother->child->size, node->child->brother->child->value);
		return; 
	}
	
}
void Compst(Tree* node)
{
	//DefList(node->child->brother);
	if (node == NULL) return;
	Tree* children = node->child->brother;
	children->kind = node->kind;
	children->return_type = node->return_type;
	DefList(children);
	children = children->brother;
	children->kind = node->kind;
	children->return_type = node->return_type;
	StmtList(children);
}
void Exp(Tree *node)
{
	if (node == NULL) return;
	Tree* children = node->child;
	//printf("num = %d\n", node->num);
	switch (node->num)
	{
		case 1:
			children = node->child;
			if (strcmp(children->name, "ID") == 0)
			{
				//if (search_variable(children) == -1)
					//errorprint(1, children->size, children->value);
				//node->type = children->type;
				//strcpy(node->struct_name, children->struct_name);
				//node->exp = 1;
				var_table var_t = search_variable(children);
				if (var_t != NULL)
				{
					if (var_t->type->Kind == 1)
					{
						if (var_t->type->Basic == 1)
							node->exp = 2;
						else
						if (var_t->type->Basic == 2)
							node->exp = 3;
					}
					else
					if (var_t->type->Kind == 2)
					{
						node->exp = 4;
						int size = 0;
						Type type = var_t->type;
						while (type->Kind != 1 && type->Kind != 3)
						{
							size++;
							type = type->Array.element;
						}
						if (type->Kind == 1)
						{
							if (type->Basic == 1)
								node->arrayType = 2;
							else
							if (type->Basic == 2)
								node->arrayType = 3;
						}
						else
						if (type->Kind == 3)
							node->arrayType = 5;
						node->arraySize = size;
					}
					else
					if (var_t->type->Kind == 3)
					{
						node->exp = 5;
						strcpy(node->struct_name, var_t->struct_name);
					}
					else
					if (var_t->type->Kind == 4)
						node->exp = 6;
					else
						node->exp = 0;
				}
				else
				{
					errorprint(1, children->size, children->value);
					node->exp = 0;
				}
			}
			else
			if (strcmp(children->name, "INT") == 0)
			{
				node->exp = 2;
				node->judge_num = 1;
			}
			else
			if (strcmp(children->name, "FLOAT") == 0)
			{
				node->exp = 3;
				node->judge_num = 1;
			}
			
			break;
		case 2:
			break;
		case 3:
			children = node->child->brother;
			if (strcmp(children->name, "ASSIGNOP") == 0)
			{
				
				Exp(node->child);
				if (node->child->judge_num) 
					errorprint(6, node->child->size, "");
				
				Exp(node->child->brother->brother);
				
				if (node->child->exp == 4 && node->child->num != 1)
				{
					if (node->child->arrayType != node->child->brother->brother->exp)
						errorprint(5, node->child->size, "");
				}
				else
				if (node->child->exp != 0 && node->child->exp != node->child->brother->brother->exp)
				{
					errorprint(5, node->child->size, "");
				}
				
				node->exp = node->child->exp;
			}
			else
			if (strcmp(children->name, "STAR") == 0 ||
			    strcmp(children->name, "DIV") == 0 ||
			    strcmp(children->name, "PLUS") == 0 ||
			    strcmp(children->name, "MINUS") == 0)
			{
				Exp(node->child);
				Exp(node->child->brother->brother);
				if (node->child->exp != node->child->brother->brother->exp ||
				    (node->child->exp != 2 && node->child->exp != 3) ||
				    (node->child->brother->brother->exp != 2 && node->child->brother->brother->exp != 3))
					errorprint(7, node->child->size, "");
				else
					if (node->child->exp == 2 || node->child->exp == 3)
						node->exp = node->child->exp;
			}
			else
			if (strcmp(children->name, "RELOP") == 0)
			{
				Exp(node->child);
				Exp(node->child->brother->brother);
				if (node->child->exp == node->child->brother->brother->exp)
					node->exp = node->child->exp;
			}
			else
			if (strcmp(children->name, "DOT") == 0)
			{
				Exp(node->child);
				if (node->child->exp != 5)
					errorprint(13, node->child->size, "");
			
				else
				{
					struct_table strt = search_struct(node->child);
					FieldList fieldlist = strt->fieldlist;
					while (fieldlist != NULL)
					{
						if (strcmp(fieldlist->name, node->child->brother->brother->value) == 0)
						{
							if (fieldlist->type->Kind == 1)
							{
								if (fieldlist->type->Basic == 1)
									node->exp = 2;
								else
								if (fieldlist->type->Basic == 2)
									node->exp = 3;
							}
							else
							if (fieldlist->type->Kind == 2)
								node->exp = 4;
							else
							if (fieldlist->type->Kind == 3)
								node->exp = 5;
							break;
						}
						fieldlist = fieldlist->next;
					}
					if (fieldlist == NULL)
						errorprint(14, node->child->brother->brother->size, node->child->brother->brother->value);
				}
			}
			else
			if (strcmp(children->name, "LP") == 0)
			{
				Exp(node->child);
				node->exp = node->child->exp;
				
				var_table var_t = search_variable(node->child);
				func_def_table func = search_func(node->child);
				if (var_t == NULL) 
					errorprint(2, node->child->size, node->child->value);
				else
				if (var_t->type->Kind != 4)
					errorprint(11, node->child->size, node->child->value);
				else
				{
					if (func->return_type == 1)
						node->exp = 2;
					else
					if (func->return_type == 2)
						node->exp = 3;
				}
			}
			break;
		case 4:
			if (strcmp(node->child->name, "ID") == 0)
			{
				var_table var_t = search_variable(node->child);
				func_def_table func = search_func(node->child);
				if (var_t == NULL)
					errorprint(2, node->child->size, node->child->value);
				else
				if (var_t->type->Kind != 4)
					errorprint(11, node->child->size, node->child->value);
				else
				{
					children = node->child->brother->brother;
					children->args = NULL;
					int numArgs = Args(children, 0);
					if (numArgs != func->num_para)
						errorprint(9, children->size, func->name);
					else
					{
						struct TypeArgs* TA = children->args;
						struct para* Para = func->list_para;
						while (Para != NULL)
						{
							if (TA->exp == 2)
							{
								if (Para->type->Kind != 1 ||
								(Para->type->Kind == 1 && Para->type->Basic != 1))
									errorprint(9, children->size, func->name);
							}
							else
							if (TA->exp == 3)
							{
								if (Para->type->Kind != 1 ||
								(Para->type->Kind == 1 && Para->type->Basic != 2))
									errorprint(9, children->size, func->name);
							}
							else
							if (TA->exp == 4)
							{
								Type type = Para->type;
								int size = 0;
								while (type->Kind != 1 && type->Kind != 3)
								{
									size++;
									type = type->Array.element;
								}
								if (Para->type->Kind != 2)
									errorprint(9, children->size, func->name);
								else
								if (size != TA->arraySize)
									errorprint(9, children->size, func->name);
							}
							else
							if (TA->exp == 5)
								if (Para->type->Kind != 3)
									errorprint(9, children->size, func->name);
							else
							
							if (TA->exp == 0)
								errorprint(9, children->size, func->name);
							TA = TA->next;
							Para = Para->next_para;
						} 
					}
					if (func->return_type == 1)
						node->exp = 2;
					else
					if (func->return_type == 2)
						node->exp = 3;
				}
			}
			else
			{
				Tree* children = node->child;
				Exp(children);
				node->exp = children->exp;
				node->arrayType = children->arrayType;
				node->arraySize = children->arraySize;
				if (strcmp(node->child->child->name, "ID") == 0)
				{
					if (children->exp != 4)
						errorprint(10, node->child->size, "");
					else
					{
						children = children->brother->brother;
						Exp(children);
						if (children->exp != 2)
							errorprint(12, node->child->brother->brother->size, "");
					}
				}
			}
			break;
		default:
			break;
	}
}
int Args(Tree* node, int num)
{
	Exp(node->child);
	num++;
	struct TypeArgs* TA = (struct TypeArgs*)malloc(sizeof(struct TypeArgs));
	TA->exp = node->child->exp;
	
	if (TA->exp == 4) TA->arraySize = node->child->arraySize;
	if(node->args == NULL)
	{
		node->args = TA;
		TA->next = NULL;
	}
	else
	{
		TA->next = node->args;
		node->args = TA;
	}

	if(node->num == 1) return num;
	else
	{
		Tree* children = node->child->brother->brother;
		children->args = node->args;
		num = Args(children, num);
		node->args = children->args;
		return num;
	}
}
void Stmt(Tree* node)
{
	if (node == NULL) return;
	Tree* children = node->child;
	switch (node->num)
	{
		case 1:
			break;
		case 2:
			children->kind = node->kind;
			Exp(children);
			break;
		case 3:
			children = children->brother;
			children->kind = node->kind;
			Exp(children);
			if (children->exp == 2)
			{
				if (node->return_type->Kind != 1 || (node->return_type->Kind == 1 && node->return_type->Basic != 1))
					errorprint(8, children->size, "");
			}
			else
			if (children->exp == 3)
			{
				if (node->return_type->Kind != 1 || (node->return_type->Kind == 1 && node->return_type->Basic != 2))
					errorprint(8, children->size, "");
			}
			else
				errorprint(8, children->size, "");
			break;
		case 5:
			if (strcmp(children->name, "IF") == 0) Exp(node->child->brother->brother);
			else {}
			break;
		case 7:
			
			break;
	}
}
void StmtList(Tree* node)
{
	if (node == NULL || node->num == 0) return;
	Tree* children = node->child;
	children->kind = node->kind;
	children->return_type = node->return_type;
	Stmt(children);
	children = children->brother;
	children->kind = node->kind;
	children->return_type = node->return_type;
	StmtList(children);
}
void ExtDecList(Tree* node)
{
	if (node == NULL) return;
	Tree* children = node->child;
	children->scope = node->scope;
	children = deal_with_children(children, node);
	children->first_verdec = 1;
	VarDec(children);
	if (node->num == 1) return;
	else
	{
		children = children->brother->brother;
		children->scope = node->scope;
		children = deal_with_children(children, node);
		ExtDecList(children);
		return;
	}
}
int scope = 0;
void search(Tree* node, int blank)
{
	if (strcmp(node->name, "ExtDef") == 0)
	{
		if (strcmp(node->child->brother->name, "FunDec") == 0)
		{
			node->kind = 2;
			node->scope = scope;
			scope++;
			
			
			Tree* children = node->child;
			children->kind = 2;
			children->scope = node->scope;
			Type type = Specifier(children);
			//node->type = type;
			children = children->brother;
			//children->kind = node->kind;
			children->type = type;
			children->kind = 2;
			children->scope = node->scope;
			//children->type = node->type;
			FunDec(children);
			children = children->brother;
			children->kind = 3;
			children->scope = node->scope;
			children->return_type = type;
			Compst(children);
		}
		else
		if (strcmp(node->child->brother->name, "SEMI") == 0)
		{
			node->kind = 4;
			node->scope = scope;
			scope++;
			
			Tree* children = node->child->child;
			children->kind = node->kind;
			children->scope = node->scope;
			struct_table strt = (struct_table)malloc(sizeof(struct StructTableNode));
			children->strt = strt;
			StructSpecifier(children);
			children->strt->line = children->size;
			unsigned value = insert_struct_table(children->strt);
		}
		else
		if (strcmp(node->child->brother->name, "ExtDecList") == 0)
		{
			node->kind = 1;
			node->scope = scope;
			scope++;
			Tree* children = node->child;
			children->kind = node->kind;
			children->scope = node->scope;
			Type type = Specifier(children);
			if (type->Kind == 3)
			{
				Tree* grandchildren = children->child;
				grandchildren = deal_with_grandchildren(grandchildren, node);
				StructSpecifier(grandchildren);
				strcpy(node->struct_name, grandchildren->struct_name);
			}
			node->type = type;
			children = children->brother;
			children->scope = node->scope;
			children = deal_with_children(children, node);
			ExtDecList(children);
		}
	}
	
	if (node->child != NULL) search(node->child, blank + 2);
	if (node->brother != NULL) search(node->brother, blank);
}
void check_semantic(Tree *root)
{
	init_hash();
	search(root, 0);

}


