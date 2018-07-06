#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<assert.h>
#include"mips32.h"

void init_reg();
int get_reg();
void print_reg(FILE *file, int i);
void deal(FILE *file, int i, Operand op);
void mips_code(FILE *file);
void transform(FILE *file);

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

void print_reg(FILE *file, int i)
{
	switch (i)
	{
		case 0:
			fprintf(file, "$0");
			break;
		case 1:
			fprintf(file, "at");
			break;
		case 2:
			fprintf(file, "v0");
			break;
		case 3:
			fprintf(file, "v1");
			break;
		case 4:
			fprintf(file, "a0");
			break;
		case 5:
			fprintf(file, "a1");
			break;
		case 6:
			fprintf(file, "a2");
			break;
		case 7:
			fprintf(file, "a3");
			break;
		
		case 8:
			fprintf(file, "t0");
			break;
		case 9:
			fprintf(file, "t1");
			break;
		case 10:
			fprintf(file, "t2");
			break;
		case 11:
			fprintf(file, "t3");
			break;
		case 12:
			fprintf(file, "t4");
			break;
		case 13:
			fprintf(file, "t5");
			break;
		case 14:
			fprintf(file, "t6");
			break;
		case 15:
			fprintf(file, "t7");
			break;
		
		case 16:
			fprintf(file, "s0");
			break;
		case 17:
			fprintf(file, "s1");
			break;
		case 18:
			fprintf(file, "s2");
			break;
		case 19:
			fprintf(file, "s3");
			break;
		case 20:
			fprintf(file, "s4");
			break;
		case 21:
			fprintf(file, "s5");
			break;
		case 22:
			fprintf(file, "s6");
			break;
		case 23:
			fprintf(file, "s7");
			break;
		
		case 24:
			fprintf(file, "t8");
			break;
		case 25:
			fprintf(file, "t9");
			break;
		
		case 26:
			fprintf(file, "k0");
			break;
		case 27:
			fprintf(file, "k1");
			break;
			
		case 28:
			fprintf(file, "gp");
			break;
		case 29:
			fprintf(file, "sp");
			break;
		case 30:
			fprintf(file, "fp");
			break;
		case 31:
			fprintf(file, "ra");
			break;
		case 0:
			fprintf(file, "$0");
			break;
	}
}
void deal(FILE *file, int i, Operand op)
{
	fprintf(file, "  sw $");
	print_reg(file, i);
	fprintf(file, ", %d($fp)\n", op->offset);
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

void print_lw(FILE *file, int reg, int offset)
{
	fprintf(file, "  lw $");
	print_reg(file, reg);
	fprintf(file, ", %d($fp)\n", offset);
}

static Count = 0;
void transform(FILE *file)
{
	InterCodes p = Icodes;
	p = p->next;
	while (p != Icodes)
	{
		if (p->code.kind == 11)
		{
			front_arg = 1;
			Operand op = p->code.u.function_dec.op;
			func_op = op;
			
			fprintf(file, "\n%s:\n", op->u.name);
			
			fprintf(file, "  addi $sp, $sp, -8\n");
			fprintf(file, "  sw $ra, 8($sp)\n");
			fprintf(file, "  sw $fp, 4($sp)\n");
			fprintf(file, "  move $fp, $sp\n");
			
			fprintf(file, "  addi $sp, $sp, %d\n", op->size);
			front_para = 1;
		}
		else
		if (p->code.kind == 8)
		{
			front_arg = 1;
			Operand op = p->code.u.label.op;
			fprintf(file, "label%d\n", op->u.label_number);
		}
		else
		if (p->code.kind == 1)
		{
			Operand op1 = p->code.u.assign.left;
			Operand op2 = p->code.u.assign.right;
			front_arg = 1;
			int reg1 = get_reg();
			int reg2 = get_reg();
			
			if (op2->kind == 2)
			{
				fprintf(file, "  li $");
				print_reg(file, reg1);
				fprintf(file, ", %d\n", op2->u.value);
				deal(file, reg1, op1);
			}
			else
			{
				
				print_lw(file, reg2, op2->offset);
				
				fprintf(file, "  move $");
				print_reg(file, reg1);
				fprintf(file, ", $");
				print_reg(file, reg2);
				fprintf(file, "\n");
				
				deal(file, reg1, op1);
				deal(file, reg2, op2);
			}
		}
		else
		if (p->code.kind == 2)
		{
			Operand op1 = p->code.u.binop.result;
			Operand op2 = p->code.u.binop.op1;
			Operand op3 = p->code.u.binop.op2;
			
			front_arg = 1;
			
			int reg1 = get_reg();
			int reg2 = get_reg();
			int reg3 = get_reg();
			
			if (op2->kind == 3)
			{
				fprintf(file, "  la $");
				print_reg(file, reg2);
				fprintf(file, ", %d($fp)\n", op2->offset);
				print_lw(file, reg3, op3->offset);
			}
			else
			{
				print_lw(file, reg2, op2->offset);
				print_lw(file, reg3, op3->offset);
			}
			
			fprintf(file, "  add $");
			print_reg(file, reg1);
			fprintf(file, ", $");
			print_reg(file, reg2);
			fprintf(file, ", $");
			print_reg(file, reg3);
			fprintf(file, "\n");
				
			deal(file, reg1, op1);
			reg[reg2] = 0;
			deal(file, reg3, op3);
			
		}
		else
		if (p->code.kind == 3 || p->code.kind == 17)
		{
			Operand op1 = p->code.u.binop.result;
			Operand op2 = p->code.u.binop.op1;
			Operand op3 = p->code.u.binop.op2;
			
			front_arg = 1;
			
			int reg1 = get_reg();
			int reg2 = get_reg();
			int reg3 = get_reg();
			
			print_lw(file, reg2, op2->offset);
			print_lw(file, reg3, op3->offset);
			
			if (p->code.kind == 3)
			{
				fprintf(file, "  sub $");
				print_reg(file, reg1);
				fprintf(file, ", $");
				print_reg(file, reg2);
				fprintf(file, ", $");
				print_reg(file, reg3);
				fprintf(file, "\n");
			}
			else
			{
				fprintf(file, "  div $");
				print_reg(file, reg2);
				fprintf(file, ", $");
				print_reg(file, reg3);
				fprintf(file, "\n");
				
				fprintf(file, "  mflo $");
				print_reg(file, reg1);
				fprintf(file, "\n");
			}
			deal(file, reg1, op1);
			deal(file, reg2, op2);
			deal(file, reg3, op3);
		}
		else
		if (p->code.kind == 4)
		{
			Operand op1 = p->code.u.binop.result;
			Operand op2 = p->code.u.binop.op1;
			Operand op3 = p->code.u.binop.op2;
			
			front_arg = 1;
			
			int reg1 = get_reg();
			int reg2 = get_reg();
			int reg3 = get_reg();
			print_lw(file, reg2, op2->offset);
			
			if (op3->kind == 2)
			{
				fprintf(file, "  li $");
				print_reg(file, reg3);
				fprintf(file, ", %d\n", op3->value);
				
			}
			else
			{
				print_lw(file, reg3, op3->offset);
			}
			
			fprintf(file, "  mul $");
			print_reg(file, reg1);
			fprintf(file, ", $");
			print_reg(file, reg2);
			fprintf(file, ", $");
			print_reg(file, reg3);
			fprintf(file, "\n");
				
			deal(file, reg1, op1);
			deal(file, reg2, op2);
			if (op3->kind != 2) deal(file, reg3, op3);
		}
		else
		if (p->code.kind == 17)
		{
			
		}
		else
		if (p->code.kind == 5)
		{
			Operand op = p->code.u.ret_code.ret_value;
			int reg = get_reg();
			
			print_lw(file, reg, op->offset);
			
			fprintf(file, "  move $v0, $");
			print_reg(file, reg);
			fprintf(file, "\n");
			
			fprintf(file, "  lw $ra, 8($fp)\n");
			fprintf(file, "  lw $fp, 4($fp)\n");
			fprintf(file, "  addi $sp, $sp, %d\n", (4 * Count) + 8 -func_op->size);
			fprintf(file, "  jr $ra\n");
		}
		else
		if (p->code.kind == 6)
		{}
		else
		if (p->code.kind == 7)
		{}
		else
		if (p->code.kind == 9)
		{
			fprintf(file, "  jal read\n");
			Operand op = p->code.u.read.op;
			int reg = get_reg();
			
			fprintf(file, "  move $");
			print_reg(file, reg);
			fprintf(file, ", $v0\n");
			
			deal(file, reg, op);
		}
		else
		if (p->code.kind == 10)
		{
			Operand op = p->code.u.write.op;
			int reg = get_reg();
			
			print_lw(file, reg, op->offset);
			fprintf(file, "  move $a0, $");
			print_reg(file, reg);
			fprintf(file, "\n");
			fprintf(file, "  jal write\n");
		}
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
