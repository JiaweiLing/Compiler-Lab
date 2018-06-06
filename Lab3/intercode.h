typedef struct Operand_* Operand;

struct Operand_ 
{
	enum {VARIABLE = 1, CONSTANT = 2, ADDRESS = 3} kind;
	union
	{
		int var_no;
		int value;
	} u;
};
struct InterCode
{
	enum {ASSIGN = 1, ADD = 2, SUB = 3, MUL = 4} kind;
	union
	{
		struct { Operand right, left; } assign;
		struct { Operand result, op1, op2; } binop;
	} u;
}
struct InterCodes
{
	InterCode code;
	struct InterCodes *prev, *next;
};
