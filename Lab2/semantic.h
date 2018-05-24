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
	enum {global_var, func_dec, func_body, str_def} kind;
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

#define hash_size 0x3fff
symbol_table SymbolTableHash[hash_size];
func_def_table FunctionDefHash[hash_size];
func_def_table FunctionDecHash[hash_size];
struct_table StructDefHash[hash_size];

void check_semantic(Tree *root);

unsigned hash(char *name);
void init_hash();

int insert_symbol_table(symbol_table node);
void check_symbol_table();

int insert_function_def_table(func_def_table node);
void check_function_table();

void search(Tree* node, int blank);

Type Specifier(Tree* node);
void VarDec(Tree *node);
void ParamDec(Tree* node);
void VarList(Tree* node);
void StructSpecifier(Tree* node);
void DefList(Tree* node);
void Def(Tree* node);
void Compst(Tree* node);
void Dec(Type type, Tree* node);
void DecList(Type type, Tree* node);
void ExtDecList(Tree *node);
void FunDec(Tree* node);

#endif
