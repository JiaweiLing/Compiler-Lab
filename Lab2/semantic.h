#ifndef SEMANTIC_H_
#define SEMANTIC_H_

typedef struct TYPE* Type;
typedef struct FIELDLIST* FieldList;
typedef struct FUNPARALIST* FunParaList;

typedef struct SymbolTableNode* symbol_table;
typedef struct FunctionDefTableNode* func_def_table;
typedef struct FunctionDecTableNode* func_dec_table;
typedef struct StructTableNode* struct_table;
enum basic_type {INT_ = 1, FLOAT_ = 2};
enum var_kind {global_var = 1, func_dec = 2, func_body = 3, str_def = 4};
enum exp_type {NONE__ = 0, VAR__ = 1, INT__ = 2, FLOAT__ = 3, ARRAY__ = 4, STRUCTURE__ = 5, FUNC__ = 6};

struct TypeArgs
{
	enum exp_type exp;
	struct TypeArgs* next;
};

struct para
{
	char name[40];
	Type type;
	char struct_name[100];
	struct para* next_para;
};

struct StructTableNode
{
	enum {Definition = 1, Declaration = 2} Kind;
	char *name;
	Type type;
	int line;
	FieldList fieldlist;
	struct_table next;
};

struct FunctionDecTableNode
{
	char name[100];
	Type type;
	func_dec_table next;
};

struct FunctionDefTableNode
{
	char name[100];
	enum basic_type return_type;
	int num_para;
	int line;
	struct para* list_para;
	func_def_table next;
};

struct SymbolTableNode
{
	char name[100];
	int scope;
	int line;
	Type type;
	char struct_name[100];
	symbol_table next;
	symbol_table scope_next;
};

typedef struct tree
{
	char *name;
	char value[1000];
	int temp, empty, size, num;
	struct tree *child, *brother;
	enum var_kind kind;
	enum exp_type exp;
	int first_verdec;
	char struct_name[100];
	int scope;
	int judge_num;
	Type type, return_type;
	struct para* Para;
	struct TypeArgs* args;
	struct_table strt;
	symbol_table symt;
	func_def_table func;
	FieldList fieldlist;
}Tree;
Tree *CreateTree(char *name);
Tree *NewNode(char *name, char *value, int line);
Tree *AddChild(char *name, int line, int num, ...);
void PrintTree(Tree *parent, int blank);

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
	char *name;
	char struct_name[100];
	Type type;
	FunParaList next;
};

struct FIELDLIST
{
	char *name;
	char struct_name[100];
	Type type;
	FieldList next;
};

void check_semantic(Tree *root);
void errorprint(int errorcode, int line, char* name);

#endif
