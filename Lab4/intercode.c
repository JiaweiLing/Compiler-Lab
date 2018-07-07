#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdarg.h>
#include<assert.h>
#include<stdarg.h>
#include"intercode.h"
#include"tree.h"

void op_print(Operand op, FILE *file);

Operand new_element(Tree *node, int identify);
int search_element(Operand op, int type);
Var set_var_array(Var v, int i, Operand op, int type);

InterCodes combine(InterCodes i_code1, InterCodes i_code2);

int search_var(Tree *node);
void insert_var(Varcode vc);

void init();
void print(FILE *file);
void insert();


void translate_ExtDef(Tree *node);
void translate_ExtDefList(Tree *node);
void translate_ExtDecList(Tree *node);
void translate_FunDec(Tree *node);
InterCodes translate_VarList(Tree *node);
InterCodes translate_ParamDec(Tree *node);
InterCodes translate_CompSt(Tree *node);
InterCodes translate_DefList(Tree *node);
InterCodes translate_Def(Tree *node);
InterCodes translate_DecList(Tree *node);
InterCodes translate_Dec(Tree *node);
Operand translate_VarDec(Tree *node);
InterCodes translate_StmtList(Tree *node);
InterCodes translate_Stmt(Tree *node);
InterCodes translate_Exp(Tree *node, Operand p);
InterCodes translate_Args(Tree *node, Operand *arg);

InterCodes translate_Cond(Tree *node, Operand True, Operand False);

static int label_num = 0;
static int temp_num = 0;
static int var_num = 0;
static int count_var = 0;

static int ebp = 0;
static Operand func_op = NULL;

int search_element(Operand op, int type)
{
	int i;
	for (i = 0; i < count_var; i++)
	{
		Var p = var_array[i];
		switch (type)
		{
			case 1:
				if (p.var && p.num == op->u.var_number) return i;
				break;
			case 2:
				if (p.temp && p.num == op->u.temp_number) return i;
				break;
		}
	}
	return -1;
}

Var set_var_array(Var v, int i, Operand op, int type)
{
	switch (type)
	{
		case 1:
			v.var = 1;
			v.num = op->u.var_number;
			v.offset = ebp;
			break;
		case 2:
			v.temp = 1;
			v.num = op->u.var_number;
			v.offset = ebp;
			break;
	}
	ebp  = ebp - 4;
	return v;
}

void op_print(Operand op, FILE *file)
{
	if (op->kind == 1 || op->kind == 3)
	{
		int index = search_element(op, 1);
		if (index == -1)
		{
			var_array[count_var] = set_var_array(var_array[count_var], count_var, op, 1);
			op->offset = var_array[count_var].offset;
			count_var++;
		}
		else
			op->offset = var_array[index].offset;
	}
	else
	if (op->kind == 5)
	{
		int index = search_element(op, 2);
		if (index == -1)
		{
			var_array[count_var] = set_var_array(var_array[count_var], count_var, op, 2);
			op->offset = var_array[count_var].offset;
			count_var++;
		}
		else
			op->offset = var_array[index].offset;
	}
	
	if (op->kind == 1)
		fprintf(file, "v%d", op->u.var_number);
	else
	if (op->kind == 2)
		fprintf(file, "#%d", op->u.value);
	else
	if (op->kind == 3)
		fprintf(file, "&v%d", op->u.var_number);
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
			op->u.var_number = var_num;
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
	assert(i_code1 != NULL && i_code2 != NULL);

	i_code1->prev->next = i_code2;
	i_code2->prev->next = i_code1;
	
	InterCodes p = i_code2->prev;
	i_code2->prev = i_code1->prev;
	i_code1->prev = p;
	return i_code1;
}

void init()
{
	Icodes = (InterCodes)malloc(sizeof(struct InterCodes_));
	Icodes->next = Icodes;
	Icodes->prev = Icodes;
	
	var_code = NULL;
}


void print(FILE *file)
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
			if (func_op == NULL) func_op = op;
			else {func_op->size = ebp; func_op = op;}
			op_print(op, file);
			ebp = 0;
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
		if (p->code.kind == 13)
		{
			fprintf(file, "PARAM ");
			Operand op = p->code.u.param.op;
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
	func_op->size = ebp;
}

void translate(Tree *node, FILE* fp, FILE *file)
{
	init();
	//printf("0\n");
	translate_ExtDef(node);
	//printf("1\n");
	print(fp);
	mips_code(file);
	fclose(fp);
	fclose(file);
	//printf("2\n");
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

InterCodes set_code(Operand p1, Operand p2, enum InterCodes_kind identify)
{
	InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
	switch (identify)
	{
		case 1:
			i_code->code.kind = 1;
			i_code->code.u.assign.left = p1;
			i_code->code.u.assign.right = p2;
			i_code->prev = i_code;
			i_code->next = i_code;
			return i_code;
			break;
		case 3:
			i_code->code.kind = 3;
			i_code->next = i_code;
			i_code->prev = i_code;
			return i_code;
			break;
		case 5:
			i_code->code.kind = 5;
			i_code->code.u.ret_code.ret_value = p1;
			i_code->next = i_code;
			i_code->prev = i_code;
			return i_code;
			break;
		case 7:
			i_code->code.kind = 7;
			i_code->code.u.Goto.label = p1;
			i_code->prev = i_code;
			i_code->next = i_code;
			return i_code;
			break;
		case 8:
			i_code->code.kind = 8;
			i_code->code.u.label.op = p1;
			i_code->prev = i_code;
			i_code->next = i_code;
			return i_code;
			break;
		case 10:
			i_code->prev = i_code;
			i_code->next = i_code;
			i_code->code.kind = identify;
			return i_code;
			break;
		case 12:
			i_code->prev = i_code;
			i_code->next = i_code;
			i_code->code.kind = 12;
			return i_code;
			break;
		case 13:
			i_code->code.kind = 13;
			i_code->next = i_code;
			i_code->prev = i_code;
			return i_code;
			break;
		case 15:
			i_code->prev = i_code;
			i_code->next = i_code;
			i_code->code.kind = 15;
			return i_code;
			break;
	}
	
}

void translate_ExtDef(Tree *node)
{
	if (strcmp(node->name, "ExtDef") == 0)
	{
		if (strcmp(node->child->brother->name, "FunDec") == 0)
		{
			translate_FunDec(node->child->brother);
			insert(translate_CompSt(node->child->brother->brother));
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
	
	if (strcmp(node->child->brother->brother->name, "VarList") == 0)
		insert(combine(i_code, translate_VarList(node->child->brother->brother)));
	else
		insert(i_code);
}

InterCodes translate_VarList(Tree *node)
{
	if (node->num == 3)
		return combine(translate_ParamDec(node->child), translate_VarList(node->child->brother->brother));
	else
		return translate_ParamDec(node->child);
}

InterCodes translate_ParamDec(Tree *node)
{
	InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
	i_code = set_code(NULL, NULL, 13);
	i_code->code.u.param.op = translate_VarDec(node->child->brother);
	
	return i_code;
}

InterCodes translate_CompSt(Tree *node)
{
	InterCodes i_code1 = translate_DefList(node->child->brother);
	InterCodes i_code2 = translate_StmtList(node->child->brother->brother);
	assert(i_code2 != NULL);
	if (i_code1 == NULL) return i_code2; else return combine(i_code1, i_code2);
}

InterCodes translate_DefList(Tree *node)
{
	if (!node->num) return NULL;
	else
	{
		InterCodes i_code1 = translate_Def(node->child);
		InterCodes i_code2 = translate_DefList(node->child->brother);
		if (i_code1 != NULL && i_code2 != NULL)
			return combine(i_code1, i_code2);
		else
		if (i_code1 != NULL) return i_code1;
		else
		if (i_code2 != NULL) return i_code2;
		else return NULL;
	}
}

InterCodes translate_Def(Tree *node)
{
	return translate_DecList(node->child->brother);
}

InterCodes translate_DecList(Tree *node)
{
	if (node->num == 1)
		return translate_Dec(node->child);
	else
	{
		InterCodes i_code1 = translate_Dec(node->child);
		InterCodes i_code2 = translate_DecList(node->child->brother->brother);
		if (i_code1 != NULL && i_code2 != NULL)
			return combine(i_code1, i_code2);
		else
		if (i_code1 != NULL) return i_code1;
		else
		if (i_code2 != NULL) return i_code2;
		else return NULL;
	}
}

InterCodes translate_Dec(Tree *node)
{
	if (node->num == 1) return NULL;
	else
	{
		Operand op1 = translate_VarDec(node->child);
		InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
		i_code = set_code(op1, NULL, 1);
		
		Operand op2 = new_element(node, 5);
		i_code->code.u.assign.right = op2;
		
		return combine(translate_Exp(node->child->brother->brother, op2), i_code);
	}
}

Operand translate_VarDec(Tree *node)
{
	if (node->num == 1)
	{
		Operand var = new_element(node, 1);
		int varnum = search_var(node->child);
		if (varnum == -1)
		{
			Varcode vc = (Varcode)malloc(sizeof(struct Varcode_));
			strcpy(vc->var_name, node->child->value);
			vc->var_num = var->u.var_number;
			vc->next = NULL;
			insert_var(vc);
		}
		else
			var->u.var_number = varnum;
		return var;
	}
}

InterCodes translate_StmtList(Tree *node)
{
	if (node->num == 0) return NULL;
	assert(strcmp(node->child->name, "Stmt") == 0);
	InterCodes i_code1 = translate_Stmt(node->child);
	assert(strcmp(node->child->brother->name, "StmtList") == 0);
	InterCodes i_code2 = translate_StmtList(node->child->brother);
	if (i_code2 == NULL) return i_code1; else return combine(i_code1, i_code2);
}

InterCodes translate_Stmt(Tree *node)
{
	Operand temp;
	Operand l1, l2, l3;
	if (node->num == 1)
		translate_CompSt(node->child);
	else
	if (node->num == 2)
		return translate_Exp(node->child, new_element(node->child, 5));
	else
	if (node->num == 3)
	{
		Operand temp = new_element(node->child, 5);
		InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
		i_code = set_code(temp, NULL, 5);
		
		return combine(translate_Exp(node->child->brother, temp), i_code);
	}
	else
	if (node->num == 5)
	{
		if (strcmp(node->child->name, "IF") == 0)
		{
			Operand l1 = new_element(node->child, 6);
			Operand l2 = new_element(node->child, 6);
			assert(strcmp(node->child->brother->brother->name, "Exp") == 0);
			InterCodes i_code1 = translate_Cond(node->child->brother->brother, l1, l2);
			assert(strcmp(node->child->brother->brother->brother->brother->name, "Stmt") == 0);
			InterCodes i_code2 = translate_Stmt(node->child->brother->brother->brother->brother);
			
			InterCodes i_code_label1 = (InterCodes)malloc(sizeof(struct InterCodes_));
			
			i_code_label1 = set_code(l1, NULL, 8);
			
			InterCodes i_code_label2 = (InterCodes)malloc(sizeof(struct InterCodes_));
			i_code_label2 = set_code(l2, NULL, 8);
			
			return combine(combine(i_code1, i_code_label1), combine(i_code2, i_code_label2));
		}
		else
		{
			Operand l1 = new_element(node->child, 6);
			Operand l2 = new_element(node->child, 6);
			Operand l3 = new_element(node->child, 6);
				
			InterCodes i_code_label1 = (InterCodes)malloc(sizeof(struct InterCodes_));
			i_code_label1 = set_code(l1, NULL, 8);
				
			InterCodes i_code_label2 = (InterCodes)malloc(sizeof(struct InterCodes_));
			i_code_label2 = set_code(l2, NULL, 8);
				
			InterCodes goto_code = (InterCodes)malloc(sizeof(struct InterCodes_));
			goto_code = set_code(l1, NULL, 7);
				
			InterCodes i_code_label3 = (InterCodes)malloc(sizeof(struct InterCodes_));
			i_code_label3 = set_code(l3, NULL, 8);
			
			return combine(combine(combine(i_code_label1, translate_Cond(node->child->brother->brother, l2, l3)), 
				combine(i_code_label2, translate_Stmt(node->child->brother->brother->brother->brother))), 
				combine(goto_code, i_code_label3));
		}
	}
	else
	if (node->num == 7)
	{
		Operand l1 = new_element(node->child, 6);
		Operand l2 = new_element(node->child, 6);
		Operand l3 = new_element(node->child, 6);

		InterCodes i_code_label1 = (InterCodes)malloc(sizeof(struct InterCodes_));
		i_code_label1 = set_code(l1, NULL, 8);
		
		InterCodes goto_code = (InterCodes)malloc(sizeof(struct InterCodes_));
		goto_code = set_code(l3, NULL, 7);
		
		InterCodes i_code_label2 = (InterCodes)malloc(sizeof(struct InterCodes_));
		i_code_label2 = set_code(l2, NULL, 8);
		
		InterCodes i_code_label3 = (InterCodes)malloc(sizeof(struct InterCodes_));
		i_code_label3 = set_code(l3, NULL, 8);
		
		return combine(combine(combine(combine(translate_Cond(node->child->brother->brother, l1, l2), i_code_label1), 
			combine(translate_Stmt(node->child->brother->brother->brother->brother), goto_code)), 
			combine(i_code_label2, translate_Stmt(node->child->brother->brother->brother->brother->brother->brother))), 
			i_code_label3);
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
				i_code = set_code(p, constant, 1);
				
				
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
					vc->var_num = var->u.var_number;
					vc->next = NULL;
					insert_var(vc);
				}
				else
					var->u.var_number = varnum;
				InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code = set_code(p, var, 1);
				
				return i_code;
				
			}
			break;
		case 2:
			if (strcmp(node->child->name, "MINUS")==0)
			{
				Operand temp = new_element(node->child, 5);
				InterCodes i_code1 = translate_Exp(node->child->brother, temp);
				InterCodes i_code2 = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code2 = set_code(NULL, NULL, 3);
				

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
				InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code->code.kind = 1;
				i_code->prev = i_code;
				i_code->next = i_code;
				Operand op = (Operand)malloc(sizeof(struct Operand_));
				op->kind = 2;
				op->next = NULL;
				op->u.value = 0;
				i_code->code.u.assign.left = p;
				i_code->code.u.assign.right = op;


				InterCodes i_code_label1 = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code_label1 = set_code(l1, NULL, 8);
				InterCodes code_inner = (InterCodes)malloc(sizeof(struct InterCodes_));
				code_inner->code.kind = 1;
				code_inner->code.u.assign.left = p;
				code_inner->prev = code_inner;
				code_inner->next = code_inner;
				Operand op_inner = (Operand)malloc(sizeof(struct Operand_));
				op_inner->kind = 2;
				op_inner->next = NULL;
				op_inner->u.value = 1;
				code_inner->code.u.assign.right = op_inner;

				InterCodes i_code_label2 = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code_label2 = set_code(l2, NULL, 8);

				return combine(combine(combine(i_code, translate_Cond(node, l1, l2)), 
					combine(i_code_label1, code_inner)), 
					i_code_label2);
			}
			
			break;
		case 3:
			if (strcmp(node->child->brother->name, "ASSIGNOP") == 0)
			{
				assert(p != NULL);
				InterCodes i_code = translate_Exp(node->child, p);
				
				Operand temp = new_element(node->child, 5);
				InterCodes i_code2 = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code2 = set_code(i_code->code.u.assign.right, temp, 1);
				
				return combine(translate_Exp(node->child->brother->brother, temp), combine(i_code, i_code2));
			}
			else
			if (strcmp(node->child->brother->name, "PLUS") == 0 || 
    			    strcmp(node->child->brother->name,"MINUS") == 0 || 
    			    strcmp(node->child->brother->name, "STAR") == 0 || 
    			    strcmp(node->child->brother->name, "DIV") == 0)
			{
				Operand temp1 = new_element(node->child->brother, 5);
				Operand temp2 = new_element(node->child->brother, 5);

				InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
			
				i_code->prev = i_code;
				i_code->next = i_code;
			
				i_code->code.u.binop.op1 = temp1;
				i_code->code.u.binop.op2 = temp2;
				i_code->code.u.binop.result = p;
			
				if (strcmp(node->child->brother->name, "PLUS") == 0)
					i_code->code.kind = 2;
				else 
				if (strcmp(node->child->brother->name, "MINUS") == 0)
					i_code->code.kind = 3;
				else 
				if (strcmp(node->child->brother->name, "STAR") == 0)
					i_code->code.kind = 4;
				else 
				if (strcmp(node->child->brother->name, "DIV") == 0)
					i_code->code.kind = 17;

				return combine(translate_Exp(node->child, temp1), 
					combine(translate_Exp(node->child->brother->brother, temp2), i_code));
			}
			
			else 
			if (strcmp(node->child->brother->name, "AND") == 0 || 
		    	    strcmp(node->child->brother->name, "OR") == 0 || 
		    	    strcmp(node->child->brother->name, "RELOP") == 0)
			{
				Operand l1 = new_element(node->child->brother, 6);
				Operand l2 = new_element(node->child->brother, 6);
			
				InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code->code.kind = 1;
				i_code->prev = i_code;
				i_code->next = i_code;
			
				Operand op = (Operand)malloc(sizeof(struct Operand_));
				op->kind = 2;
				op->next = NULL;
				op->u.value = 0;
			
				i_code->code.u.assign.left = p;
				i_code->code.u.assign.right = op;

				InterCodes i_code_label1 = (InterCodes)malloc(sizeof(struct InterCodes_));
			
				i_code_label1 = set_code(l1, NULL, 8);
				InterCodes code_inner = (InterCodes)malloc(sizeof(struct InterCodes_));
				code_inner->code.kind = 1;
				code_inner->code.u.assign.left = p;
				code_inner->prev = code_inner;
				code_inner->next = code_inner;
				
				Operand op_inner = (Operand)malloc(sizeof(struct Operand_));
				op_inner->kind = 2;
				op_inner->next = NULL;
				op_inner->u.value = 1;
				code_inner->code.u.assign.right = op_inner;

				InterCodes i_code_label2 = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code_label2 = set_code(l2, NULL, 8);
				return combine(combine(combine(i_code, translate_Cond(node, l1, l2)), combine(i_code_label1, code_inner)), i_code_label2);
			}
			else 
			if (strcmp(node->child->brother->name, "LP") == 0)
			{
				InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
				i_code->prev = i_code;
				i_code->next = i_code;

				if (strcmp(node->child->value, "read") == 0)
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
					strcpy(op->u.name, node->child->value);
					i_code->code.u.function_call.func = op;
					i_code->code.u.function_call.ret = p;
				}
				return i_code;
			}
			else
			if (strcmp(node->child->brother->name, "Exp") == 0)
				return translate_Exp(node->child->brother, p);
			break;
		case 4:
			if (strcmp(node->child->name, "ID") == 0)
			{
				Operand arg = NULL;
			
				InterCodes i_code1 = translate_Args(node->child->brother->brother, &arg);
				assert(arg != NULL);
				
				if (strcmp(node->child->value, "write") == 0)
				{
					InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
					
					i_code = set_code(NULL, NULL, 10);
					assert(arg != NULL);
					i_code->code.u.write.op = arg;

					return combine(i_code1, i_code);
				}
				else
				{
					Operand temp = arg;
					InterCodes i_code2;
					while (temp != NULL)
					{
						InterCodes i_code_arg = (InterCodes)malloc(sizeof(struct InterCodes_));
						
						i_code_arg = set_code(NULL, NULL, 12);
						i_code_arg->code.u.arg.op = temp;
	
						if (temp == arg) i_code2 = i_code_arg;
						else i_code2 = combine(i_code2, i_code_arg);
					
						temp = temp->next;
					}
					InterCodes i_code_call = (InterCodes)malloc(sizeof(struct InterCodes_));
					i_code_call = set_code(NULL, NULL, 15);
					Operand op = (Operand)malloc(sizeof(struct Operand_));
					op->kind = 4;
					op->next = NULL;
					strcpy(op->u.name, node->child->value);
					i_code_call->code.u.function_call.func = op;
					i_code_call->code.u.function_call.ret = p;
					return combine(combine(i_code1, i_code2), i_code_call);
				}
			}	
			break;
		default:
			break;
	}
}

InterCodes translate_Args(Tree *node, Operand *arg)
{
	Operand temp = new_element(node->child, 5);
	InterCodes i_code = translate_Exp(node->child, temp);
	if (*arg == NULL) *arg = temp;
	else {temp->next = *arg; *arg = temp;}
	if (node->num == 1) return i_code;
	else 
		return combine(i_code, translate_Args(node->child->brother->brother, arg));
}

InterCodes translate_Cond(Tree *node, Operand True, Operand False)
{
	if (strcmp(node->child->name, "NOT") != 0)
	{
		if (strcmp(node->child->brother->name, "RELOP") == 0)
		{
			Operand temp1 = new_element(node->child->brother, 5), temp2 = new_element(node->child->brother, 5);
			Operand op = new_element(node->child->brother, 7);

			
			InterCodes i_code1 = (InterCodes)malloc(sizeof(struct InterCodes_));
			i_code1->code.kind = 6;
			i_code1->code.u.If.op = op;
			i_code1->code.u.If.temp1 = temp1;
			i_code1->code.u.If.temp2 = temp2;
			i_code1->code.u.If.label = True;
			i_code1->prev = i_code1;
			i_code1->next = i_code1;
			
			InterCodes i_code2 = (InterCodes)malloc(sizeof(struct InterCodes_));
			i_code2 = set_code(False, NULL, 7);
			
			return combine(combine(translate_Exp(node->child, temp1), translate_Exp(node->child->brother->brother, temp2)), 
				combine(i_code1, i_code2));
		}
		else
		if (strcmp(node->child->brother->name, "AND") == 0)
		{
			Operand label = new_element(node->child->brother, 6);
			InterCodes i_code_label = (InterCodes)malloc(sizeof(struct InterCodes_));
			i_code_label = set_code(label, NULL, 8);
			
			return combine(combine(translate_Cond(node->child, label, False), i_code_label), 
				translate_Cond(node->child->brother->brother, True, False));
		}
		else
		if (strcmp(node->child->brother->name, "OR") == 0)
		{
			Operand label = new_element(node->child->brother, 6);

			InterCodes i_code_label = (InterCodes)malloc(sizeof(struct InterCodes_));
			i_code_label = set_code(label, NULL, 8);
			
			return combine(combine(translate_Cond(node->child, True, label), i_code_label), 
				translate_Cond(node->child->brother->brother, True, False));
		}
		else
		{
			Operand temp = new_element(node->child->brother, 5);
			InterCodes i_code = (InterCodes)malloc(sizeof(struct InterCodes_));
			i_code->prev = i_code;
			i_code->next = i_code;
			i_code->code.kind = 6;
			i_code->code.u.If.temp1 = temp;
			i_code->code.u.If.label = True;
			
			Operand relop = (Operand)malloc(sizeof(struct Operand_));
			relop->kind = 7;
			relop->next = NULL;
			strcpy(relop->u.relop, "!=");
			i_code->code.u.If.op = relop;
			
			Operand op = (Operand)malloc(sizeof(struct Operand_));
			op->kind = 2;
			op->next = NULL;
			op->u.value = 0;
			i_code->code.u.If.temp2 = op;
			
			InterCodes i_code_label = (InterCodes)malloc(sizeof(struct InterCodes_));
			i_code_label->prev = i_code_label;
			i_code_label->next = i_code_label;
			i_code_label->code.kind = 7;
			i_code_label->code.u.Goto.label = True;
			
			return combine(combine(translate_Exp(node, temp), i_code), i_code_label);
		}
	}
	else
	{
		return translate_Cond(node->child->brother, False, True);
	}
}

