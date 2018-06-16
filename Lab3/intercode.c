#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"intercode.h"
#include"tree.h"

Operand new_element(Tree *node, int identify);
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
static int var_num = 0;

void op_print(Operand op, FILE *file)
{
	if (op->kind == 1)
		fprintf(file, "v%d", op->u.var_number);
	else
	if (op->kind == 2)
		fprintf(file, "#%d", op->u.value);
	else
	if (op->kind == 3)
		{;}
	else
	if (op->kind == 4)
		fprintf(file, "%s", op->u.name);
	else
	if (op->kind == 5)
		fprintf(file, "t%d", op->u.temp_number);
	else
	if (op->kind == 6)
		fprintf(file, "label%d", op->u.label_number);
	else
	if (op->kind == 7)
		fprintf(file, "%s", op->u.relop);
}
Operand new_element(Tree *node, int identify)
{
	Operand op = (Operand)malloc(sizeof(struct Operand_));
	switch (identify)
	{
		case 1:
			op->kind = 1;
			var_num++;
			op->next = NULL;
			op->u.var_num = var_num;
			return op;
			break;
		case 5:
			op->kind = 5;
			temp_num++;
			op->next = NULL;
			op->u.temp_number = temp_num;
			return op;
			break;
		case 6:
			op->kind = 6;
			label_num++;
			op->next = NULL;
			op->u.label_number = label_num;
			return op;
			break;
		case 7:
			op->kind = 7;
			op->next = NULL;
			strcpy(op->u.relop, node->value);
			return op;
			break;
		default:
			break;
	}
}

int search_var(Tree *node)
{
	Varcode vc = var_code;
	for (; vc; vc = vc->next)
		if (strcmp(vc->var_name, node->value) == 0)
		{
			return vc->var_num;
		}
	return -1;
}

void insert_var(Varcode vc)
{
	if (var_code == NULL) var_code = vc;
	else {vc->next = var_code; var_code = vc;}
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
	
	var_code = NULL;
}


void print(FILE *fp)
{
	InterCodes p = Icodes;
	p = p->next;
	while (p != Icodes)
	{
		if (p->code.kind == 1)
		{
			Operand op = p->code.u.assign.left;
			op_print(op, file);
			fprintf(file, " := ");
			op = p->code.u.assign.right;
			op_print(op, file);
			fprintf(file, "\n");
		}
		else
		if (p->code.kind == 2 || p->code.kind == 3 || p->code.kind == 4 || p->code.kind == 17)
		{
			Operand op = p->code.u.binop.result;
			op_print(op, file);
			fprintf(file, " := ");
			op = p->code.u.binop.op1;
			op_print(op, file);
			switch (p->code.kind)
			{
				case 2:
					fprintf(file, " + ");
					break;
				case 3:
					fprintf(file, " - ");
					break;
				case 4:
					fprintf(file, " * ");
					break;
				case 17:
					fprintf(file, " / ");
					break;
				default:
					break;
			}
			op = p->code.u.binop.op2;
			op_print(op, file);
			fprintf(file, "\n");
		}
		else
		if (p->code.kind == 5)
		{
			fprintf(file, "RETURN ");
			Operand op = p->code.u.ret_code.ret_value;
			op_print(op, file);
			fprintf(file, "\n");
		}
		else
		if (p->code.kind == 6)
		{
			fprintf(file, "IF ");
			Operand op = p->code.u.If.temp1;
			op_print(op, file);
			fprintf(file, " ");
			op = p->code.u.If.op;
			op_print(op, file);
			fprintf(file, " ");
			op = p->code.u.If.temp2;
			op_print(op, file);
			fprintf(file, " GOTO ");
			op = p->code.u.If.label;
			op_print(op, file);
			fprintf(file, "\n");
		}
		else
		if (p->code.kind == 7)
		{
			fprintf(file, "GOTO ");
			Operand op = p->code.u.Goto.label;
			op_print(op, file);
			fprintf(file, "\n");
		}
		else
		if (p->code.kind == 8)
		{
			fprintf(file, "LABEL ");
			Operand op = p->code.u.label.op;
			op_print(op, file);
			fprintf(file, " :\n");
		}
		else
		if (p->code.kind == 9)
		{
			fprintf(file, "READ ");
			Operand op = p->code.u.read.op;
			op_print(op, file);
			fprintf(file, "\n");
		}
		else
		if (p->code.kind == 10)
		{
			fprintf(file, "WRITE ");
			Operand op = p->code.u.write.op;
			op_print(op, file);
			fprintf(file, "\n");
		}
		else
		if (p->code.kind == 11)
		{
			fprintf(file, "FUNCTION ");
			Operand op = p->code.u.function_dec.op;
			op_print(op, file);
			fprintf(file, " :\n");
		}
		else
		if (p->code.kind == 12)
		{
			fprintf(file, "ARG ");
			Operand op = p->code.u.arg.op;
			op_print(op, file);
			fprintf(file, "\n");
		}
		else
		if (p->code.kind == 15)
		{
			Operand op = p->code.u.function_call.ret;
			op_print(op, file);
			fprintf(file, " := CALL ");
			op = p->code.u.function_call.func;
			op_print(op, file);
			fprintf(file, "\n");
		}
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
	assert(i_code != NULL);
	InterCodes temp = Icodes;
	temp->prev->next = i_code;
	i_code->prev->next = temp;
	InterCodes p = i_code->prev;
	i_code->prev = temp->prev;
	temp->prev = p;
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
	op->next = NULL;
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
	if (node->num == 0) return NULL;
	InterCodes i_code1 = translate_Stmt(node->child);
	InterCodes i_code2 = translate_StmtList(node->child->brother);
	if (i_code2 == NULL) return i_code1; else return combine(i_code1, i_code2);
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
				Operand l1 = new_element(node->child, 6);
				Operand l2 = new_element(node->child, 6);
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
		default:
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
				constant->next = NULL;
				constant->u.value = atoi(node->child->value);
				InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code->code.kind = 1;
				i_code->code.u.assign.left = p;
				i_code->code.u.assign.right = constant;
				i_code->prev = i_code;
				i_code->next = i_code;
				
				return i_code;
			}
			else
			if (strcmp(node->child->name, "ID") == 0)
			{
				Operand var = new_element(node->child, 1);
				int varnum = search_var(node->child);
				if (varnum == -1)
				{
					Varcode vc = (Varcode)malloc(sizeof(struct Varcode_));
					strcpy(vc->var_name, node->child->value);
					vc->var_num = var->u.var_num;
					vc->next = NULL;
					insert_var(vc);
				}
				else
					var->u.var_num = varnum;
				InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code->code.kind = 1;
				i_code->code.u.assign.left = p;
				i_code->code.u.assign.right = var;
				i_code->prev = i_code;
				i_code->next = i_code;
				
				return i_code;
				
			}
			break;
		case 2:
			if (strcmp(node->child->name, "MINUS")==0)
			{
				Operand temp = new_element(node->child, 5);
				Intercodes i_code1 = translate_Exp(node->child->brother, temp);
				Intercodes i_code2 = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code2->code.kind = 3;
				i_code2->next = i_code2;
				i_code2->prev = i_code2;

				Operand op = (Operand)malloc(sizeof(struct Operand_));
				op->kind = 2;
				op->next = NULL;
				op->u.value = 0;
				i_code2->code.u.binop.op1 = op;
				i_code2->code.u.binop.op2 = temp;
				i_code2->code.u.binop.result = p;
				return combine(i_code1, i_code2);
			}
			else
			{
				Operand l1 = new_element(node->child, 6);
				Operand l2 = new_element(node->child, 6);
				Intercodes i_code = (Intercodes)malloc(sizeof(struct Intercodes_));
				i_code->code.kind = 1;
				i_code->prev = i_code;
				i_code->next = i_code;
				Operand op = (Operand)malloc(sizeof(struct Operand_));
				op->kind = 2;
				op->next = NULL;
				op->u.value = 0;
				i_code->code.u.assign.left = place;
				i_code->code.u.assign.right = op;

				Intercodes i_code2 = translate_Cond(node, l1, l2);

				Intercodes i_code_label1 = (Intercodes)malloc(sizeof(struct Intercodes_));
				i_code_label1->code.kind = 8;
				i_code_label1->code.u.label.op = l1;
				i_code_label1->prev = i_code_label1;
				i_code_label1->next = i_code_label1;

				Intercodes code_inner = (Intercodes)malloc(sizeof(struct Intercodes_));
				code_inner->code.kind = 1;
				code_inner->code.u.assign.left = p;
				code_inner->prev = code_inner;
				code_inner->next = code_inner;
				Operand op_inner = (Operand)malloc(sizeof(struct Operand_));
				op_inner->kind = 2;
				op_inner->next = NULL;
				op_inner->u.value = 1;
				code_inner->code.u.assign.right = op_inner;

				Intercodes i_code_label2 = (Intercodes)malloc(sizeof(struct Intercodes_));
				i_code_label2->code.kind = 8;
				i_code_label2->code.u.label.op = l2;
				i_code_label2->prev = i_code_label2;
				i_code_label2->next = i_code_label2;

				return combine(combine(combine(i_code,i_code2), combine(i_code_label1,code_inner)), i_code_label2);
			}
			
			break;
		case 3:
			if (strcmp(node->child->brother->name, "ASSIGNOP") == 0)
			{
				assert(p != NULL);
				InterCodes i_code = translate_Exp(node->child->brother, p);
				
				Operand temp = new_element(node->child->brother, 5);
				InterCodes i_code1 = translate_Exp(node->child->brother->brother, temp);
				InterCodes i_code2 = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code2->code.kind = 1;
				i_code2->code.u.assign.left = i_code->code.u.assign.right;
				i_code2->code.u.assign.right = temp;
				i_code2->prev = i_code2;
				i_code2->next = i_code2;
				
				return combine(i_code1, combine(i_code, i_code2));
			}
			else
			if (strcmp(node->child->brother->name, "PLUS") == 0 || 
    			    strcmp(node->child->brother->name,"MINUS") == 0 || 
    			    strcmp(node->child->brother->name, "STAR") == 0 || 
    			    strcmp(node->child->brother->name, "DIV") == 0)
			{
				Operand temp1 = new_element(node->child->brother, 5);
				Operand temp2 = new_element(node->child->brother, 5);
			
				Intercodes i_code1 = translate_Exp(node->child, temp1);
				Intercodes i_code2 = translate_Exp(node->child->brother->brother, temp2);
				Intercodes i_code3 = (Intercodes)malloc(sizeof(struct Intercodes_));
			
				i_code3->prev = i_code3;
				i_code3->next = i_code3;
			
				i_code3->code.u.binop.op1 = temp1;
				i_code3->code.u.binop.op2 = temp2;
				i_code3->code.u.binop.result = p;
			
				if (strcmp(node->child->brother->name, "PLUS") == 0)
					i_code3->code.kind = 2;
				else 
				if (strcmp(node->child->brother->name, "MINUS") == 0)
					i_code3->code.kind = 3;
				else 
				if (strcmp(node->child->brother->name, "STAR") == 0)
					i_code3->code.kind = 4;
				else 
				if (strcmp(node->child->brother->name, "DIV") == 0)
					i_code3->code.kind = 17;

				return combine(i_code1,combine(i_code2, i_code3));
			}
			
			else 
			if (strcmp(node->child->brother->name, "AND") == 0 || 
		    	    strcmp(node->child->brother->name, "OR") == 0 || 
		    	    strcmp(node->child->brother->name, "RELOP") == 0)
			{
				Operand l1 = new_element(node->child->brother, 6);
				Operand l2 = new_element(node->child->brother, 6);
			
				Intercodes i_code = (Intercodes)malloc(sizeof(struct Intercodes_));
				i_code->code.kind = 1;
				i_code->prev = i_code;
				i_code->next = i_code;
			
				Operand op = (Operand)malloc(sizeof(struct Operand_));
				op->kind = 2;
				op->next = NULL;
				op->u.value = 0;
			
				i_code->code.u.assign.left = p;
				i_code->code.u.assign.right = op;

				Intercodes i_code1 = translate_Cond(node, l1, l2);

				Intercodes i_code_label1 = (Intercodes)malloc(sizeof(struct Intercodes_));
			
				i_code_label1->code.kind = 8;
				i_code_label1->code.u.label.op = l1;
				i_code_label1->prev = i_code_label1;
				i_code_label1->next = i_code_label1;

				Intercodes code_inner = (Intercodes)malloc(sizeof(struct Intercodes_));
				code_inner->code.kind = 1;
				code_inner->code.u.assign.left = p;
				code_inner->prev = code_inner;
				code_inner->next = code_inner;
				Operand op_inner = (Operand)malloc(sizeof(struct Operand_));
				op_inner->kind = 2;
				op_inner->next = NULL;
				op_inner->u.value = 1;
				code_inner->code.u.assign.right = op_inner;

				Intercodes i_code_label2 = (Intercodes)malloc(sizeof(struct Intercodes_));
				i_code_label2->code.kind = 8;
				i_code_label2->code.u.label.op=l2;
				i_code_label2->prev = i_code_label2;
				i_code_label2->next = i_code_label2;

				return combine(combine(combine(i_code, i_code1), combine(i_code_label1, code_inner)), i_code_label2);
			}
			else 
			if (strcmp(node->child->brother->name, "LP") == 0)
			{
				Intercodes i_code = (Interi_codes)malloc(sizeof(struct Intercodes_));
				i_code->prev = i_code;
				i_code->next = i_code;

				if (strcmp(node->child->value, "read")==0)
				{
					i_code->code.kind = 9;
					i_code->code.u.read.op = p;
				}
				else
				{
					Operand op = (Operand)malloc(sizeof(struct Operand_));
					op->kind = 4;
					op->next = NULL;

					i_code->code.kind = 15;
					strcpy(op->u.name, child->value);
					i_code->code.u.function_call.func = op;
					i_code->code.u.function_call.ret = p;
				}
				return i_code;
			}
			break;
		case 4:
			if (strcmp(node->child->name, "ID") == 0)
			{
				Operand arg = NULL;
			
				Intercodes i_code1 = translate_Args(node->child->brother->brother, arg);

				if (strcmp(node->child->value, "write") == 0)
				{
					Intercodes i_code = (Intercodes)malloc(sizeof(struct Intercodes_));
					i_code->prev = i_code;
					i_code->next = i_code;
					i_code->code.kind = 10;
					i_code->code.u.write.op = arg;

					return combine(i_code1, i_code);
				}
				else
				{
					Operand temp = arg;
					Intercodes i_code2;
					while (temp != NULL)
					{
						Intercodes i_code_arg = (Intercodes)malloc(sizeof(struct Intercodes_));
						i_code_arg->prev = i_code_arg;
						i_code_arg->next = i_code_arg;
						i_code_arg->code.kind = 12;
						i_code_arg->code.u.arg.op = temp;
	
						if (temp == arg) i_code2 = i_code_arg;
						else i_code2 = combine(i_code2, i_code_arg);
					
						temp = temp->next;
					}
					Intercodes i_code_call = (Intercodes)malloc(sizeof(struct Intercodes_));
					i_code_call->prev = i_code_call;
					i_code_call->next = i_code_call;
					i_code_call->code.kind = 15;
					Operand op = (Operand)malloc(sizeof(struct Operand_));
					op->kind = 4;
					op->next = NULL;
					strcpy(op->u.name, node->child->value);
					i_code_call->code.u.function_call.func = op;
					i_code_call->code.u.function_call.ret = place;
					return combine(combine(i_code1, i_code2), i_code_call);
			}	
			break;
		default:
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
			Operand temp1 = new_element(node->child->brother, 5), temp2 = new_element(node->child->brother, 5);
			Operand op = new_element(node->child->brother, 7);
			
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

