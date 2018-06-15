typedef struct Operand_* Operand;
typedef struct InterCodes_* InterCodes;

struct Operand_ 
{
	enum {VARIABLE = 1, CONSTANT = 2, ADDRESS = 3, FUNCTIONNAME = 4, TEMP = 5, LABEL = 6} kind;
	union
	{
		int var_number, temp_number, label_number;
		int value;
		char name[100];
	} u;
};
struct InterCode
{
	enum {ASSIGN = 1, ADD = 2, SUB = 3, MUL = 4, RETURN = 5, IF = 6, GOTO = 7, LABEL = 8, READ = 9, WRITE = 10, FUNCTION = 11, ARG = 12, PARAM = 13, DEC = 14, CALL = 15} kind;
	union
	{
		struct { Operand right, left; } assign;
		struct { Operand result, op1, op2; } binop;
		struct { Operand op; } function_dec;
		struct { Operand op; } function_all;
		struct { Operand op; } label;
	} u;
}
struct InterCodes_
{
	InterCode code;
	struct InterCodes_ *prev, *next;
};
