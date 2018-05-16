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

Tree *NewNode(char *name, char *value, int line)
{
	Tree *p = (Tree *)malloc(sizeof(Tree));
	p->name = name;
	strcpy(p->value, value);
	if (line != -1)
	{
		p->size = line;
		p->temp = 1;
		p->empty = 0;
	}
	else
	if (line == -1)
	{
		p->temp = 0;
		p->empty = 1;
	}
	p->child = NULL;
	p->brother = NULL;
	return p;
}
Tree *AddChild(char *name, int line, int num, ...)
{
	va_list arg_tree;
	Tree *p;
	Tree *p_brother;
	int i;
	
	Tree *parent = (Tree *)malloc(sizeof(Tree));
	parent->name = name;

	va_start(arg_tree, line);
	
	if (line != -1)
	{
		parent->size = line;
		parent->empty = 0;
	}
	if (line == -1) parent->temp = 0;
	
	p = va_arg(arg_tree, Tree*);
	
	parent->child = p;
	if (line == -1) parent->size = p->size;
	p_brother = p;
	
	for (i = 1; i < num; i++)
	{
		p = va_arg(arg_tree, Tree*);
		p_brother->brother = p;
		p_brother = p;
	}
	return parent;
}

void PrintTree(Tree *parent, int blank)
{
	int i;
	if (parent->empty == 0 && parent->temp == 0)
	{
		for (i = 0; i < blank; i++) printf(" ");
		printf("%s (%d)\n", parent->name, parent->size);
	}
	else
	if (parent->empty == 0 && parent->temp == 1)
	{
		for (i = 0; i < blank; i++) printf(" ");
		if (strcmp(parent->name, "ID") == 0 || strcmp(parent->name, "TYPE") == 0)
			printf("%s: %s\n",parent->name, parent->value);
		else
		if (strcmp(parent->name, "INT") == 0)
			printf("%s: %d\n", parent->name, strtol(parent->value, NULL, 0));
		else
		if (strcmp(parent->name, "FLOAT") == 0)
			printf("%s: %f\n", parent->name, atof(parent->value));
		else
			printf("%s\n", parent->name);
		
	}
	
	if (parent->child != NULL) PrintTree(parent->child, blank + 2);
	if (parent->brother != NULL) PrintTree(parent->brother, blank);
}
