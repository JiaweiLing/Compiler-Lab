#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include"mips32.h"

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
			
			int reg1 = getreg(op1);
			int reg2 = getreg(op2);
			
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
			
			int reg1 = getreg(op1);
			int reg2 = getreg(op2);
			int reg3 = getreg(op3);
			
			fprintf(file, "  add $t%d, $t%d, $t%d\n", reg1, reg2, reg3);
		}
		else
		if (p->code.kind == 3)
		{
			Operand op1 = p->code.u.binop.result;
			Operand op2 = p->code.u.binop.op1;
			Operand op3 = p->code.u.binop.op2;
			
			int reg1 = getreg(op1);
			int reg2 = getreg(op2);
			int reg3 = getreg(op3);
			
			fprintf(file, "  sub $t%d, $t%d, $t%d\n", reg1, reg2, reg3);
		}
		else
		if (p->code.kind == 4)
		{
			Operand op1 = p->code.u.binop.result;
			Operand op2 = p->code.u.binop.op1;
			Operand op3 = p->code.u.binop.op2;
			
			int reg1 = getreg(op1);
			int reg2 = getreg(op2);
			int reg3 = getreg(op3);
			
			fprintf(file, "  mul $t%d, $t%d, $t%d\n", reg1, reg2, reg3);
		}
		else
		if (p->code.kind == 17)
		{
			Operand op1 = p->code.u.binop.result;
			Operand op2 = p->code.u.binop.op1;
			Operand op3 = p->code.u.binop.op2;
			
			int reg1 = getreg(op1);
			int reg2 = getreg(op2);
			int reg3 = getreg(op3);
			
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
