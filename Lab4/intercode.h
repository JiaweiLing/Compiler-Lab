#ifndef INTERCODE_H_
#define INTERCODE_H_
#include "semantic.h"
#include "tree.h"
typedef struct Operand_* Operand;
typedef struct InterCodes_* InterCodes;
typedef struct Varcode_* Varcode;

enum Operand_kind {_VARIABLE = 1, _CONSTANT = 2, _ADDRESS = 3, _FUNCTIONNAME = 4, _TEMP = 5, _LABEL = 6, _RELOP = 7};

enum InterCodes_kind {ASSIGN_ = 1, ADD_ = 2, SUB_ = 3, MUL_ = 4, RETURN_ = 5, IF_ = 6, GOTO_ = 7, LABEL_ = 8, READ_ = 9, WRITE_ = 10, FUNCTION_ = 11, ARG_ = 12, PARAM_ = 13, DEC_ = 14, CALL_ = 15, ASSIGNOP_ = 16, DIV_ = 17};

struct Operand_ 
{
	enum Operand_kind kind;
	union
	{
		int var_number, temp_number, label_number;
		int value;
		char name[100];
		char relop[100];
	} u;
	Operand next;
};
struct InterCode
{
	enum InterCodes_kind kind;
	union
	{
		struct { Operand right, left; } assign;
		struct { Operand result, op1, op2; } binop;
		struct { Operand ret_value; } ret_code;
		struct { Operand op; } function_dec;
		struct { Operand ret, func; } function_call;
		struct { Operand op; } label;
		struct { Operand op, temp1, temp2, label; } If;
		struct { Operand label; } Goto;
		struct { Operand op; } read;
		struct { Operand op; } write;
		struct { Operand op; } arg;
		struct { Operand op; } param;
	} u;
};
struct InterCodes_
{
	struct InterCode code;
	struct InterCodes_ *prev, *next;
};
struct Varcode_
{
	char var_name[100];
	int var_num;
	Varcode next;
};
Varcode var_code;
struct InterCodes_ *Icodes;
void translate(Tree *node, FILE *file);
#endif
