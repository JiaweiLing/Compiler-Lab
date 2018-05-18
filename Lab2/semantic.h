#include "tree.h"

typedef struct TYPE* Type;
typedef struct FIELDLIST* FieldList;
typedef struct FUNPARALIST* FunParaList;
typedef struct SymbolTableNode* symbolpt;

struct SymbolTableNode
{
	char *name;
	Type type;
	int scope;
	symbolpt next;
	symbolpt scope_next;
};

struct TYPE
{
	enum {BASIC, ARRAY, STRUCTURE} Kind;
	union
	{
		int Basic;
		struct
		{
			Type elem;
			int size;
		} Array;
		FieldList Structure;
		struct
		{
			int num_para;
			Type return_type;
			FunParaList fpl;
		} func;
	} Union;
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
symbolpt symbolhash[hash_size];
void check_semantic(tree *root);
unsigned hash(char *name);
void init_hash();
void insert_symbol_table(symbolpt node);

