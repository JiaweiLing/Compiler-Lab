#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>
#include"tree.h"

Tree *CreateTree(char *name)
{
	Tree *root_node = (Tree *)malloc(sizeof(Tree));
	root_node->name = name;
	return root_node;
}

Tree *NewNode(char *name)
{
	Tree *p = (Tree *)malloc(sizeof(Tree));
	p->name = name;
	p->child = NULL;
	p->brother = NULL;
	return p;
}
void AddChild(char *name, Tree *parent, int num, ...)
{
	va_list arg_tree;
	Tree *p;
	Tree *p_brother;
	int i;
	
	va_start(arg_tree, num);
	p = va_arg(arg_tree, Tree*);
	parent->child = p;
	p_brother = p;
	
	for (i = 1; i < num; i++)
	{
		p = va_arg(arg_tree, Tree*);
		p_brother->brother = p;
		p_brother = p;
	}
}

void PrintTree(Tree *parent)
{
	printf("%s\n", parent->name);
	if (parent->child != NULL) PrintTree(parent->child);
	
	Tree *brother = parent->brother;
	while (brother != NULL)
	{
		PrintTree(brother);
		brother = brother->brother;
	}
}
