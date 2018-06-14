#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"intercode.h"
#include"tree.h"

void init();
void writecode(FILE *file);
void insert();
void translate(Tree *node, FILE *file);
void translate_ExtDef(Tree *node);
void translate_ExtDefList(Tree *node);
void translate_ExtDecList(Tree *node);
void translate_FunDec(Tree *node);
void translate_VarList(Tree *node);
void translate_ParamDec(Tree *node);
void translate_CompSt(Tree *node);
void translate_DefList(Tree *node);
void translate_Def(Tree *node);
void translate_DecList(Tree *node);
void translate_Dec(Tree *node);
void translate_VarDec(Tree *node);
void translate_StmtList(Tree *node);
void translate_Stmt(Tree *node);
void translate_Exp(Tree *node);
void translate_Args(Tree *node);
void translate_Cond(Tree *node);

void init()
{
	Icodes = (InterCodes)malloc(sizeof(struct InterCodes_));
	Icodes->next = Icodes;
	Icodes->prev = Icodes;
}

void writecode(FILE *file)
{
	InterCodes p = Icodes;
	while (p != Icodes)
	{
		p = p->next;
	}
}

void insert()
{

}

void translate(Tree *node, FILE *file)
{
	init_code();
	translate_ExtDef(node);
}

void translate_ExtDef(Tree *node)
{
	if (strcmp(node->name, "ExtDef") == 0)
	{
		if (strcmp(node->child->brother->name == "FunDec") == 0)
		{
			translate_FunDec(node->child->brother);
			translate_CompSt(node->child->brother->brother);
		}
	}
	if (node->child != NULL) translate_ExtDef(node->child);
	if (node->brother != NULL) translate_ExtDef(node->brother);
}

void translate_ExtDefList(Tree *node)
{

}

void translate_ExtDecList(Tree *node)
{

}

void translate_FunDec(Tree *node)
{
	InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
	i_code->code.kind = 11;
	Operand op = (Operand)malloc(sizeof(struct Operand_));
	op->kind = 4;
	strcpy(op->u.name, node->child->value);
	i_code->code.u.function_all.op = op;
	insert(i_code);
}

void translate_VarList(Tree *node)
{

}

void translate_ParamDec(Tree *node)
{

}

void translate_CompSt(Tree *node)
{

}

void translate_DefList(Tree *node)
{

}

void translate_Def(Tree *node)
{

}

void translate_DecList(Tree *node)
{

}

void translate_Dec(Tree *node)
{

}

void translate_VarDec(Tree *node)
{

}

void translate_StmtList(Tree *node)
{

}

void translate_Stmt(Tree *node)
{

}

InterCodes translate_Exp(Tree *node, Operand p)
{
	Tree* children = node->child;
	switch (node->num)
	{
		case 1:
			if (strcmp(node->child->name, "INT") == 0)
			{
				Operand constant = (Operand)malloc(sizeof(struct Operand_));
				constant->kind = 2;
				constant->u.value = atoi(node->child->value);
				InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code->code.kind = 1;
				i_code->code.u.assign.left = p;
				i_code->code.u.assign.right = constant;
				return i_code;
			}
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
	}
}

void translate_Args(Tree *node)
{

}

void translate_Cond(Tree *node)
{

}

