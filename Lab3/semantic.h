#ifndef SEMANTIC_H_
#define SEMANTIC_H_

typedef struct TYPE* Type;
typedef struct FIELDLIST* FieldList;
typedef struct FUNPARALIST* FunParaList;

typedef struct VarTableNode* var_table;
typedef struct FunctionDefTableNode* func_def_table;
typedef struct StructTableNode* struct_table;
enum basic_type {INT_ = 1, FLOAT_ = 2};
enum var_kind {global_var = 1, func_dec = 2, func_body = 3, str_def = 4};
enum exp_type {NONE__ = 0, VAR__ = 1, INT__ = 2, FLOAT__ = 3, ARRAY__ = 4, STRUCTURE__ = 5, FUNC__ = 6};

struct TypeArgs
{
	enum exp_type exp;
	struct TypeArgs* next;
	int arraySize;
};

struct para
{
	char name[100];
	Type type;
	char struct_name[100];
	struct para* next_para;
};

struct StructTableNode
{
	enum {Definition = 1, Declaration = 2} Kind;
	char name[100];
	int line;
	FieldList fieldlist;
	struct_table next;
};

struct FunctionDefTableNode
{
	char name[100];
	enum basic_type return_type;
	int num_para;
	int judge_func;
	int line;
	struct para* list_para;
	func_def_table next;
};

struct VarTableNode
{
	char name[100];
	int scope;
	int line;
	Type type;
	char struct_name[100];
	var_table next;
};

struct TYPE
{
	enum {NONE = 0, BASIC = 1, ARRAY = 2, STRUCTURE = 3, FUNCTION = 4} Kind;
	union
	{
		enum basic_type Basic;
		struct
		{
			Type element;
			int size;
		} Array;
		FieldList fieldlist;
	};
};

struct FUNPARALIST
{
	char name[100];
	char struct_name[100];
	Type type;
	FunParaList next;
};

struct FIELDLIST
{
	char name[100];
	char struct_name[100];
	Type type;
	FieldList next;
};

void errorprint(int errorcode, int line, char* name);
void readwrite();
#endif
