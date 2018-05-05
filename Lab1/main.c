#include<stdio.h>
#include<stdlib.h>
#include"lex.yy.c"
#include"syntax.tab.h"
#include"tree.h"

extern void yyrestart(FILE *);
extern int yyparse(void);
Tree *root = NULL;


