#ifndef TREE_H_
#define TREE_H_
#include "semantic.h"
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
	int arraySize;
	
	enum exp_type arrayType;
	Type type, return_type;
	struct para* Para;
	struct TypeArgs* args;
	struct_table strt;
	var_table vt;
	func_def_table func;
	FieldList fieldlist;
}Tree;
Tree *CreateTree(char *name);
Tree *NewNode(char *name, char *value, int line);
Tree *AddChild(char *name, int line, int num, ...);
void PrintTree(Tree *parent, int blank);
void check_semantic(Tree *root);

#endif
