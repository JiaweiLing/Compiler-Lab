#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"intercode.h"
#include"tree.h"

Operand new_temp();
Operand new_label();
Operand new_relop(Tree *node);
InterCodes combine(InterCodes i_code1, InterCodes i_code2);
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
InterCodes translate_Stmt(Tree *node);
InterCodes translate_Exp(Tree *node);
void translate_Args(Tree *node);
InterCodes translate_Cond(Tree *node);

static int label_num = 0;
static int temp_num = 0;

Operand new_temp()
{
	Operand newtemp = (Operand)malloc(sizeof(struct Operand_));
	newtemp->kind = 5;
	temp_num++;
	newtemp->u.temp_number = temp_num;
	return newtemp;
}

Operand new_label()
{
	Operand newlabel = (Operand)malloc(sizeof(struct Operand_));
	newlabel->kind = 6;
	label_num++;
	newlabel->u.label_number = label_num;
	return newlabel;
}

Operand new_relop(Tree *node)
{
	Operand op = (Operand)malloc(sizeof(struct Operand_));
	op->kind = 7;
	strcpy(op->relop, node->value);
	return op;
}

InterCodes combine(InterCodes i_code1, InterCodes i_code2)
{
	i_code1->prev = i_code2->next;
	i_code2->prev = i_code1->next;
	i_code1->next->next = i_code2;
	i_code2->next = i_code1;
	return i_code1;
}
void init()
{
	Icodes = (InterCodes)malloc(sizeof(struct InterCodes_));
	Icodes->next = Icodes;
	Icodes->prev = Icodes;
}


void print(FILE *file)
{
	InterCodes p = Icodes;
	p = p->next;
	while (p != Icodes)
	{
		if (p->code.kind = 11)
			fprintf(file, "FUNCTION %s :\n", p->code.u.function_dec.op->u.name);
		p = p->next;
	}
}

void translate(Tree *node, FILE *file)
{
	init();
	translate_ExtDef(node);
	print(file);
}

void insert(InterCodes i_code)
{
	InterCodes p = Icodes;
	p->prev->next = i_code;
	i_code->next = p;
	i_code->prev = p->prev;
	p->prev = i_code;
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
	i_code->code.u.function_dec.op = op;
	i_code->prev = i_code;
	i_code->next = i_code;
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
	translate_DefList(node->child->brother);
	translate_StmtList(node->child->brother->brother);
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
	translate_Stmt(node->child);
	translate_StmtList(node->child->brother);
}

InterCodes translate_Stmt(Tree *node)
{
	switch (node->num)
	{
		case 1:
			break;
		case 2:
			return translate_Exp(node->child, NULL);
			break;
		case 3:
		
			break;
		case 5:
			if (strcmp(node->child->name, "IF") == 0)
			{
				Operand l1 = new_label();
				Operand l2 = new_label();
				InterCodes i_code1 = translate_Cond(node->child->brother->brother, l1, l2);
				InterCodes i_code2 = translate_Cond(node->child->brother->brother->brother->brother);
				
				InterCodes i_code_label1 = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code_label1->code.kind = 8;
				i_code_label1->code.u.label.op = l1;
				i_code_label1->prev = i_code_label1;
				i_code_label1->next = i_code_label1;
				
				InterCodes i_code_label2 = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code_label2->code.kind = 8;
				i_code_label2->code.u.label.op = l2;
				i_code_label2->prev = i_code_label2;
				i_code_label2->next = i_code_label2;
				return combine(combine(i_code1, i_code_label1), combine(i_code2, i_code_label2));
			}
			break;
		case 7:
			break;
	}
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
				i_code->prev = i_code;
				i_code->next = i_code;
				
				return i_code;
			}
			break;
		case 2:
			break;
		case 3:
			if (strcmp(node->child->brother->name, "ASSIGNOP") == 0)
			{
				Operand temp = new_temp();
				InterCodes i_code = translate_Exp(node->child->brother->brother, temp);
			}
			break;
		case 4:
			break;
	}
}

void translate_Args(Tree *node)
{

}

InterCodes translate_Cond(Tree *node, Operand True, Operand False)
{
	if (strcmp(node->child->name, "NOT") != 0)
	{
		if (strcmp(node->child->brother->name, "RELOP") == 0)
		{
			Operand temp1 = new_temp(), temp2 = new_temp();
			Operand op = new_relop(node->child->brother);
			
			InterCodes i_code1 = translate_Exp(node->child, temp1);
			InterCodes i_code2 = translate_Exp(node->child->brother->brother, temp2);
			
			InterCodes i_code3 = (InterCodes)malloc(sizeof(struct InterCodes_));
			i_code3->code.kind = 6;
			i_code3->code.u.If.op = op;
			i_code3->code.u.If.temp1 = temp1;
			i_code3->code.u.If.temp2 = temp2;
			i_code3->code.u.If.label = True;
			i_code3->prev = i_code3;
			i_code3->next = i_code3;
			
			InterCodes i_code4 = (InterCodes)malloc(sizeof(struct InterCodes_));
			i_code4->code.kind = 7;
			i_code4->code.u.Goto.label = False;
			i_code4->prev = i_code4;
			i_code4->next = i_code4;
			
			return combine(combine(i_code1, i_code2), combine(i_code3, i_code4));
		}
		else
		if (strcmp(node->child->brother->name, "AND") == 0)
		{
		
		}
		else
		if (strcmp(node->child->brother->name, "OR") == 0)
		{
		
		}
		else
		{
		
		}
	}
}

