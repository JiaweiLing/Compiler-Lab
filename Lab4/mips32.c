#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include<assert.h>
#include"mips32.h"

static int Count = 0;
static int front_arg = 1;
static int count_arg = 0;
Operand func_op = NULL;

static int front_para = 1;
static int count_para = 0;
static int index_para = 0;

void init_reg();
int get_reg();
void print_reg(FILE *file, int i);
void deal(FILE *file, int i, Operand op);
void mips_code(FILE *file);
void transform(FILE *file);

void init_reg()
{
	for (int i = 0; i < 32; i++)
		Reg[i] = 0;
}

int get_reg()
{
	int i;
	for (i = 8; i <= 25; i++)
		if (!Reg[i]) {Reg[i] = 1; return i;}
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
	}
}

void print_relop(FILE* file, int reg1, int reg2, Operand label, int type)
{
	fprintf(file, "  ");
	switch (type)
	{
		case 1:
			fprintf(file, "beq");
			break;
		case 2:
			fprintf(file, "bne");
			break;
		case 3:
			fprintf(file, "blt");
			break;
		case 4:
			fprintf(file, "ble");
			break;
		case 5:
			fprintf(file, "bgt");
			break;
		case 6:
			fprintf(file, "bge");
			break;
	}
	fprintf(file, " $");
	print_reg(file, reg1);
	fprintf(file, ", $");
	print_reg(file, reg2);
	fprintf(file, ", label%d\n", label->u.label_number);
}

void deal(FILE *file, int i, Operand op)
{
	fprintf(file, "  sw $");
	print_reg(file, i);
	fprintf(file, ", %d($fp)\n", op->offset);
	Reg[i] = 0;
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
			fprintf(file, "label%d:\n", op->u.label_number);
		}
		else
		if (p->code.kind == 1)
		{
			Operand op1 = p->code.u.assign.left;
			Operand op2 = p->code.u.assign.right;
			front_arg = 1;
			int reg1, reg2;
			
			if (op2->kind == 2)
			{
				reg1 = get_reg();
				fprintf(file, "  li $");
				print_reg(file, reg1);
				fprintf(file, ", %d\n", op2->u.value);
				deal(file, reg1, op1);
			}
			else
			{
				int reg1 = get_reg(), reg2 = get_reg();
				//printf("%d\n", op2->offset);
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
			int reg2;
			int reg3 = get_reg();
			
			if (op2->kind == 3)
			{
				reg2 = get_reg();
				fprintf(file, "  la $");
				print_reg(file, reg2);
				fprintf(file, ", %d($fp)\n", op2->offset);
				print_lw(file, reg3, op3->offset);
			}
			else
			{
				reg2 = get_reg();
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
			Reg[reg2] = 0;
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
			int reg3;
			print_lw(file, reg2, op2->offset);
			
			if (op3->kind == 2)
			{
				reg3 = get_reg();
				fprintf(file, "  li $");
				print_reg(file, reg3);
				fprintf(file, ", %d\n", op3->u.value);
				
			}
			else
			{
				reg3 = get_reg();
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
		if (p->code.kind == 5)
		{
			Operand op = p->code.u.ret_code.ret_value;
			int reg = get_reg();
			front_arg = 1;
			
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
		{
			Operand temp1 = p->code.u.If.temp1;
			Operand temp2 = p->code.u.If.temp2;
			
			Operand op = p->code.u.If.op;
			
			Operand label = p->code.u.If.label;
			
			front_arg = 1;
			
			int reg1 = get_reg();
			int reg2 = get_reg();
			
			print_lw(file, reg1, temp1->offset);
			print_lw(file, reg2, temp2->offset);
			
			assert(op->kind == 7);
			
			if (strcmp(op->u.relop, "==") == 0)
				print_relop(file, reg1, reg2, label, 1);
			else
			if (strcmp(op->u.relop, "!=") == 0)
				print_relop(file, reg1, reg2, label, 2);
			else
			if (strcmp(op->u.relop, "<") == 0)
				print_relop(file, reg1, reg2, label, 3);
			else
			if (strcmp(op->u.relop, "<=") == 0)
				print_relop(file, reg1, reg2, label, 4);
			else
			if (strcmp(op->u.relop, ">") == 0)
				print_relop(file, reg1, reg2, label, 5);
			else
			if (strcmp(op->u.relop, ">=") == 0)
				print_relop(file, reg1, reg2, label, 6);
			
			deal(file, reg1, temp1);
			deal(file, reg2, temp2);
		}
		else
		if (p->code.kind == 7)
		{
			front_arg = 1;
			Operand op = p->code.u.Goto.label;
			fprintf(file, "  j label%d\n", op->u.label_number);
		}
		else
		if (p->code.kind == 9)
		{
			fprintf(file, "  jal read\n");
			
			Operand op = p->code.u.read.op;
			front_arg = 1;
			
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
			front_arg = 1;
			
			int reg = get_reg();
			print_lw(file, reg, op->offset);
			
			Reg[reg] = 0;
			
			fprintf(file, "  move $a0, $");
			print_reg(file, reg);
			fprintf(file, "\n");
			fprintf(file, "  jal write\n");
			
		}
		else
		if (p->code.kind == 12)
		{
			if (front_arg)
			{
				front_arg = 0;
				count_arg = 0;
				count_arg++;
				
				InterCodes i_code = p->next;
				
				while (i_code->code.kind == 12)
				{
					count_arg++;
					i_code = i_code->next;
				}
				
				if (count_arg - 4 > 0) Count = count_arg - 4;
				else Count = 0;
				
				fprintf(file, "  addi $sp, $sp, %d\n", -Count * 4);
				
			}
			count_arg--;
			if (count_arg >= 4)
			{
				Operand op = p->code.u.arg.op;
				int reg = get_reg();
				print_lw(file, reg, op->offset);
				
				fprintf(file, "  sw $");
				print_reg(file, reg);
				fprintf(file, ", %d($sp)\n", (count_arg - 3) * 4);
				
				Reg[reg] = 0;
			}
			else
			{
				Operand op = p->code.u.arg.op;
				int reg = get_reg();
				print_lw(file, reg, op->offset);
				
				fprintf(file, "  move $a%d, $", count_arg);
				print_reg(file, reg);
				fprintf(file, "\n");
				Reg[reg] = 0;
			}
		}
		else
		if (p->code.kind == 13)
		{
			front_arg = 1;
			
			if (front_para)
			{
				front_para = 0;
				
				count_para = 0;
				count_para++;
				index_para = 0;
				
				InterCodes i_code = p->next;
				
				while (i_code->code.kind == 13)
				{
					count_para++;
					i_code = i_code->next;
				}
			}
			
			if (index_para >= 4)
			{
				Operand op = p->code.u.param.op;
				int reg = get_reg();
				
				fprintf(file, "  lw $");
				print_reg(file, reg);
				fprintf(file, ", %d($fp)\n", (index_para - 3) * 4 + 8);
				
				deal(file, reg, op);
				index_para++;
			}
			else
			{
				Operand op = p->code.u.param.op;
				
				int reg = get_reg();
				
				fprintf(file, "  move $");
				print_reg(file, reg);
				fprintf(file, ", $a%d\n", index_para);
				
				deal(file, reg, op);
				index_para++;
			}
		}
		else
		if (p->code.kind == 14)
		{}
		else
		if (p->code.kind == 15)
		{
			Operand op = p->code.u.function_call.ret;
			Operand func = p->code.u.function_call.func;
			
			front_arg = 1;
			
			int reg = get_reg();
			
			fprintf(file, "  jal %s\n", func->u.name);
			fprintf(file, "  move $");
			print_reg(file, reg);
			fprintf(file, ", $v0\n");
		}
		p = p->next;
	}
}
