#ifndef SEMANTIC_H_
#define SEMANTIC_H_

typedef struct TYPE* Type;
typedef struct FIELDLIST* FieldList;
typedef struct FUNPARALIST* FunParaList;

typedef struct SymbolTableNode* symbol_table;
typedef struct FunctionDefTableNode* func_def_table;
typedef struct FunctionDecTableNode* func_dec_table;
typedef struct StructTableNode* struct_table;
enum basic_type {INT_, FLOAT_};
enum var_kind {global_var, func_dec, func_body, str_def};
struct para
{
	char name[40];
	Type type;
	char struct_name[100];
	struct para* next_para;
};

struct StructTableNode
{
	enum {Definition, Declaration} Kind;
	char *name;
	Type type;
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
	int first_verdec;
	char struct_name[100];
	int scope;
	
	Type type;
	struct para* Para;
	struct_table strt;
	symbol_table symt;
	func_def_table func;
}Tree;
Tree *CreateTree(char *name);
Tree *NewNode(char *name, char *value, int line);
Tree *AddChild(char *name, int line, int num, ...);
void PrintTree(Tree *parent, int blank);

struct TYPE
{
	enum {NONE, BASIC, ARRAY, STRUCTURE} Kind;
	union
	{
		enum basic_type Basic;
		struct
		{
			Type element;
			int size;
		} Array;
		FieldList structure;
	};
};

struct FUNPARALIST
{
	char *name;
	Type type;
	FunParaList next;
};

struct FIELDLIST
{
	char *name;
	Type type;
	FieldList next;
};

void check_semantic(Tree *root);



#endif
