#include "tree.h"

typedef struct TYPE* Type;
typedef struct FIELDLIST* FieldList;
typedef struct FUNPARALIST* FunParaList;

typedef struct SymbolTableNode* symbol_table;
typedef struct FunctionDefTableNode* func_def_table;
typedef struct FunctionDecTableNode* func_dec_table;

struct para
{
	char name[40];
	Type type;
	struct para* next_para;
};

struct StructTableNode
{
	char *name;
	Type type;
	struct StructTableNode *next;
};

struct FunctionDecTableNode
{
	char name[100];
	Type type;
	func_dec_table *next;
}

struct FunctionDefTableNode
{
	char name[100];
	enum {INT_, FLOAT_} return_type;
	int num_para;
	struct para* list_para;
	func_def_table *next;
}

struct SymbolTableNode
{
	char *name;
	Type type;
	int scope;
	symbol_table next;
	symbol_table scope_next;
};

struct TYPE
{
	enum {BASIC, ARRAY, STRUCTURE} Kind;
	union
	{
		enum {INT_, FLOAT_} Basic;
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
symbol_table FunctionDefHash[hash_size];
symbol_table FunctionDecHash[hash_size];
symbol_table StructDefHash[hash_size];

void check_semantic(tree *root);
unsigned hash(char *name);
void init_hash();
void insert_symbol_table(symbol_table node);
#endif


