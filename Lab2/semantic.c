#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include"tree.h"

void check_semantic(tree *root)
{

}

unsigned hash_table(char *name)
{
	unsigned int val = 0, i;
	for (; *name; ++name)
	{
		val = (val << 2) + *name;
		if (i = val & ~0x3fff) val = (val ^ (i >> 12)) & 0x3fff;
	}
	return val;
}

void init_hash()
{
	int i;
	for (i = 0; i < hash_size; i++)
		symbolhash[i] = NULL;
}

void insert_symbol_table(symbolpt node)
{
	unsigned index = hash_table(node->name);
	if (symbolhash[index] == NULL)
	{
		symbolhash[index] = node;
		node->next = NULL;
		return 0;
	}
	else
	{
		node->next = symbolhash[index];
		symbolhash[index] = node;
		return 1;
	}
}
