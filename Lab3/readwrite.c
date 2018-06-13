#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"tree.h"
#include"semantic.h"
#include"symbol_table.h"

func_def_table set_type(func_def_table node, int identify)
{
	switch (identify)
	{
		case 1:
			strcpy(node->name, "read");
			node->return_type = 1;
			node->num_para = 0;
			node->list_para = NULL;
			break;
		case 2:
			strcpy(node->name, "write");
			node->return_type = 1;
			node->num_para = 1;
			break;
		default:
			break;
	}
	return node;
}
void readwrite_table(int identify)
{
	var_table var = (var_table)malloc(sizeof(struct VarTableNode));
	Type type = (Type)malloc(sizeof(struct TYPE));
	type->kind = 4;
	var->type = type;
	if (identify == 1) strcpy(var->name, "read");
	else strcpy(var->name, "write");
	insert_var_table(var);
}
void readwrite()
{
	func_def_table read = (func_def_table)malloc(sizeof(struct FunctionDefTableNode));
	func_def_table write = (func_def_table)malloc(sizeof(struct FunctionDefTableNode));
	
	read = set_type(read, 1);
	write = set_type(write, 2);
	
	struct para* Para = (struct para*)malloc(sizeof(struct para));
	Type type = (Type)malloc(sizeof(struct TYPE));
	type->kind = 1;
	type->basic = 1;
	Para->type = type;
	Para->next_para = NULL;
	write->list_para = para;
	
	read->next = NULL;
	write->next = NULL;
	
	insert_function_def_table(read);
	insert_function_def_table(write);
	
	readwrite_table(1);
	readwrite_table(2);
}
