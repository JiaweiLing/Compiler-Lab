#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include"mips32.h"


void init_reg()
{
	for (int i = 0; i < 32; i++)
		reg[i] = 0;
}

int get_reg()
{
	int i;
	for (i = 8; i <= 25; i++)
		if (!reg[i]) {reg[i] = 1; return 1;}
	return -1;
}

void print_reg(int i)
{
	switch (i)
	{
		case 0:
			fprintf(fp, "$0");
			break;
		case 1:
			fprintf(fp, "at");
			break;
		case 2:
			fprintf(fp, "v0");
			break;
		case 3:
			fprintf(fp, "v1");
			break;
		case 4:
			fprintf(fp, "a0");
			break;
		case 5:
			fprintf(fp, "a1");
			break;
		case 6:
			fprintf(fp, "a2");
			break;
		case 7:
			fprintf(fp, "a3");
			break;
		
		case 8:
			fprintf(fp, "t0");
			break;
		case 9:
			fprintf(fp, "t1");
			break;
		case 10:
			fprintf(fp, "t2");
			break;
		case 11:
			fprintf(fp, "t3");
			break;
		case 12:
			fprintf(fp, "t4");
			break;
		case 13:
			fprintf(fp, "t5");
			break;
		case 14:
			fprintf(fp, "t6");
			break;
		case 15:
			fprintf(fp, "t7");
			break;
		
		case 16:
			fprintf(fp, "s0");
			break;
		case 17:
			fprintf(fp, "s1");
			break;
		case 18:
			fprintf(fp, "s2");
			break;
		case 19:
			fprintf(fp, "s3");
			break;
		case 20:
			fprintf(fp, "s4");
			break;
		case 21:
			fprintf(fp, "s5");
			break;
		case 22:
			fprintf(fp, "s6");
			break;
		case 23:
			fprintf(fp, "s7");
			break;
		
		case 24:
			fprintf(fp, "t8");
			break;
		case 25:
			fprintf(fp, "t9");
			break;
		
		case 26:
			fprintf(fp, "k0");
			break;
		case 27:
			fprintf(fp, "k1");
			break;
			
		case 28:
			fprintf(fp, "gp");
			break;
		case 29:
			fprintf(fp, "sp");
			break;
		case 30:
			fprintf(fp, "fp");
			break;
		case 31:
			fprintf(fp, "ra");
			break;
		case 0:
			fprintf(fp, "$0");
			break;
	}
}
void deal(FILE *file, int i, Operand op)
{
	fprintf(fp, "  sw $");
	print_reg(i);
	fprintf(fp, ", %d($fp)\n", op->offset);
	reg[i] = 0;
}

void mips_code(FILE *file)
{
	const char *prefix = 	""
				".data\n"
				"_prompt: .asciiz \"Enter an integer:\"\n"
				"_ret: .asciiz \"\\n\"\n"
				".globl main\n"
				".text\n"
				"read:\n"
				"  li $v0, 4\n"
				"  la $a0, _prompt\n"
				"  syscall\n"
				"  li $v0, 5\n"
				"  syscall\n"
				"  jr $ra\n"
				"\n"
				"write:\n"
				"  li $v0, 1\n"
				"  syscall\n"
				"  li $v0, 4\n"
				"  la $a0, _ret\n"
				"  syscall\n"
				"  move $v0, $0\n"
				"  jr $ra\n"
				"\n";
	fprintf(file, "%s", prefix);
	init_reg();
	transform(file);
}

void transform(FILE *file)
{
	InterCodes p = Icodes;
	p = p->next;
	while (p != Icodes)
	{
		if (p->code.kind == 11)
		{
			Operand op = p->code.u.function_dec.op;
			fprintf(file, "%s:\n", op->u.name);
		}
		else
		if (p->code.kind == 8)
		{
			Operand op = p->code.u.label.op;
			fprintf(file, "label%d\n", op->u.label_number);
		}
		else
		if (p->code.kind == 1)
		{
			Operand op1 = p->code.u.assign.left;
			Operand op2 = p->code.u.assign.right;
			
			int reg1 = get_reg(op1);
			int reg2 = get_reg(op2);
			
			if (op2->kind == 2)
				fprintf(file, "  li $t%d, %d\n", reg1, op2->u.value);
			else
				fprintf(file, "  move $t%d, $t%d\n", reg1, reg2);
		}
		else
		if (p->code.kind == 2)
		{
			Operand op1 = p->code.u.binop.result;
			Operand op2 = p->code.u.binop.op1;
			Operand op3 = p->code.u.binop.op2;
			
			int reg1 = get_reg(op1);
			int reg2 = get_reg(op2);
			int reg3 = get_reg(op3);
			
			fprintf(file, "  add $t%d, $t%d, $t%d\n", reg1, reg2, reg3);
		}
		else
		if (p->code.kind == 3)
		{
			Operand op1 = p->code.u.binop.result;
			Operand op2 = p->code.u.binop.op1;
			Operand op3 = p->code.u.binop.op2;
			
			int reg1 = get_reg(op1);
			int reg2 = get_reg(op2);
			int reg3 = get_reg(op3);
			
			fprintf(file, "  sub $t%d, $t%d, $t%d\n", reg1, reg2, reg3);
		}
		else
		if (p->code.kind == 4)
		{
			Operand op1 = p->code.u.binop.result;
			Operand op2 = p->code.u.binop.op1;
			Operand op3 = p->code.u.binop.op2;
			
			int reg1 = get_reg(op1);
			int reg2 = get_reg(op2);
			int reg3 = get_reg(op3);
			
			fprintf(file, "  mul $t%d, $t%d, $t%d\n", reg1, reg2, reg3);
		}
		else
		if (p->code.kind == 17)
		{
			Operand op1 = p->code.u.binop.result;
			Operand op2 = p->code.u.binop.op1;
			Operand op3 = p->code.u.binop.op2;
			
			int reg1 = get_reg(op1);
			int reg2 = get_reg(op2);
			int reg3 = get_reg(op3);
			
			fprintf(file, "  div $t%d, $t%d, $t%d\n", reg1, reg2, reg3);
		}
		else
		if (p->code.kind == 5)
		{}
		else
		if (p->code.kind == 6)
		{}
		else
		if (p->code.kind == 7)
		{}
		else
		if (p->code.kind == 9)
		{}
		else
		if (p->code.kind == 10)
		{}
		else
		if (p->code.kind == 12)
		{}
		else
		if (p->code.kind == 13)
		{}
		else
		if (p->code.kind == 14)
		{}
		else
		if (p->code.kind == 15)
		{}
		p = p->next;
	}
}
