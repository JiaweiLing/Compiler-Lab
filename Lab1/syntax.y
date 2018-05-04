%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<stdarg.h>
	#include"tree.h"
	#include "lex.yy.c"
	Tree* root_node;
	int yyerror(char *msg);
%}

%union {
	Tree* node;
}

%locations

%token <node> INT
%token <node> FLOAT
%token <node> SEMI COMMA ASSIGNOP RELOP
%token <node> PLUS MINUS STAR DIV
%token <node> AND OR DOT NOT
%token <node> TYPE
%token <node> LP RP LB RB LC RC
%token <node> STRUCT RETURN IF ELSE WHILE ID

%type <node> Program ExtDefList ExtDef ExtDecList
%type <node> Specifier StructSpecifier OptTag Tag
%type <node> VarDec FunDec VarList
%type <node> ParamDec CompSt StmtList Stmt
%type <node> DefList Def DecList Dec Exp Args

%right ASSIGNOP  
%left OR 
%left AND 
%left RELOP
%left PLUS MINUS 
%left STAR DIV
%right NOT
%left DOT LB RB LP RP

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE
%%

Program : ExtDefList {$$ = NewNode("Program");AddChild("Program", $$, 1, $1);root_node = $$;}
	;
ExtDefList : ExtDef ExtDefList {$$ = NewNode("ExtDefList"); AddChild("ExtDefList", $$, 2, $1, $2);}
	| /* Empty */{$$ = NewNode("ExtDefList");}
	;
ExtDef : Specifier ExtDecList SEMI {$$ = NewNode("ExtDef"); AddChild("ExtDef", $$, 3, $1, $2, $3);}
	| Specifier SEMI {$$ = NewNode("ExtDef"); AddChild("ExtDef", $$, 2, $1, $2);}
	| Specifier FunDec CompSt {$$ = NewNode("ExtDef"); AddChild("ExtDef", $$, 3, $1, $2, $3);}
	;
ExtDecList : VarDec {$$ = NewNode("ExtDecList"); AddChild("ExtDecList", $$, 1, $1);}
	| VarDec COMMA ExtDecList {$$ = NewNode("ExtDecList"); AddChild("ExtDecList", $$, 3, $1, $2, $3);}
	;
Specifier : TYPE {$$ = NewNode("Specifier"); AddChild("Specifier", $$, 1, $1);}
	| StructSpecifier {$$ = NewNode("Specifier"); AddChild("Specifier", $$, 1, $1);}
	;
StructSpecifier : STRUCT OptTag LC DefList RC {$$ = NewNode("StructSpecifier"); AddChild("StructSpecifier", $$, 5, $1, $2, $3, $4, $5);}
	| STRUCT Tag {$$ = NewNode("StructSpecifier"); AddChild("StructSpecifier", $$, 2, $1, $2);}
	;
OptTag : ID {$$ = NewNode("OptTag"); AddChild("OptTag", $$, 1, $1);}
	| /* Empty */ {$$ = NewNode("OptTag");}
	;
Tag : ID {$$ = NewNode("Tag"); AddChild("Tag", $$, 1, $1);}
	;
VarDec : ID {$$ = NewNode("VarDec"); AddChild("VarDec", $$, 1, $1);}
	| VarDec LB INT RB {$$ = NewNode("VarDec"); AddChild("VarDec", $$, 4, $1, $2, $3, $4);}
	;
FunDec : ID LP VarList RP {$$ = NewNode("FunDec"); AddChild("FunDec", $$, 4, $1, $2, $3, $4);}
	| ID LP RP {$$ = NewNode("FunDec"); AddChild("FunDec", $$, 3, $1, $2, $3);}
	;
VarList : ParamDec COMMA VarList {$$ = NewNode("VarList"); AddChild("VarList", $$, 3, $1, $2, $3);}
	| ParamDec {$$ = NewNode("VarList"); AddChild("VarList", $$, 1, $1);}
	;
ParamDec : Specifier VarDec {$$ = NewNode("ParamDec"); AddChild("ParamDec", $$, 2, $1, $2);}
	;
CompSt : LC DefList StmtList RC {$$ = NewNode("CompSt"); AddChild("CompSt", $$, 4, $1, $2, $3, $4);}
	;
StmtList : Stmt StmtList {$$ = NewNode("StmtList"); AddChild("StmtList", $$, 2, $1, $2);}
	| /* Empty */ {$$ = NewNode("StmtList");}
	;
Stmt : Exp SEMI {$$ = NewNode("Stmt"); AddChild("Stmt", $$, 2, $1, $2);}
	| CompSt {$$ = NewNode("Stmt"); AddChild("Stmt", $$, 1, $1);}
	| RETURN Exp SEMI {$$ = NewNode("Stmt"); AddChild("Stmt", $$, 3, $1, $2, $3);}
	| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$ = NewNode("Stmt"); AddChild("Stmt", $$, 5, $1, $2, $3, $4, $5);}
	| IF LP Exp RP Stmt ELSE Stmt {$$ = NewNode("Stmt"); AddChild("Stmt", $$, 7, $1, $2, $3, $4, $5, $6, $7);}
	| WHILE LP Exp RP Stmt {$$ = NewNode("Stmt"); AddChild("Stmt", $$, 5, $1, $2, $3, $4, $5);}
	;
DefList : Def DefList {$$ = NewNode("DefList"); AddChild("DefList", $$, 2, $1, $2);}
	| /* Empty */ {$$ = NewNode("DefList");}
	;
Def : Specifier DecList SEMI {$$ = NewNode("Def"); AddChild("Def", $$, 3, $1, $2, $3);}
	;
DecList : Dec {$$ = NewNode("DecList"); AddChild("DecList", $$, 1, $1);}
	| Dec COMMA DecList {$$ = NewNode("DecList"); AddChild("DecList", $$, 3, $1, $2, $3);}
	;
Dec : VarDec {$$ = NewNode("Dec"); AddChild("Dec", $$, 1, $1);}
	| VarDec ASSIGNOP Exp {$$ = NewNode("Dec"); AddChild("Dec", $$, 3, $1, $2, $3);}
	;
Exp : Exp ASSIGNOP Exp {$$ = NewNode("Exp"); AddChild("Exp", $$, 3, $1, $2, $3);}
	| Exp AND Exp {$$ = NewNode("Exp"); AddChild("Exp", $$, 3, $1, $2, $3);}
	| Exp OR Exp {$$ = NewNode("Exp"); AddChild("Exp", $$, 3, $1, $2, $3);}
	| Exp RELOP Exp {$$ = NewNode("Exp"); AddChild("Exp", $$, 3, $1, $2, $3);}
	| Exp PLUS Exp {$$ = NewNode("Exp"); AddChild("Exp", $$, 3, $1, $2, $3);}
	| Exp MINUS Exp {$$ = NewNode("Exp"); AddChild("Exp", $$, 3, $1, $2, $3);}
	| Exp STAR Exp {$$ = NewNode("Exp"); AddChild("Exp", $$, 3, $1, $2, $3);}
	| Exp DIV Exp {$$ = NewNode("Exp"); AddChild("Exp", $$, 3, $1, $2, $3);}
	| LP Exp RP {$$ = NewNode("Exp"); AddChild("Exp", $$, 3, $1, $2, $3);}
	| MINUS Exp {$$ = NewNode("Exp"); AddChild("Exp", $$, 2, $1, $2);}
	| NOT Exp {$$ = NewNode("Exp"); AddChild("Exp", $$, 2, $1, $2);}
	| ID LP Args RP {$$ = NewNode("Exp"); AddChild("Exp", $$, 4, $1, $2, $3, $4);}
	| ID LP RP {$$ = NewNode("Exp"); AddChild("Exp", $$, 3, $1, $2, $3);}
	| Exp LB Exp RB {$$ = NewNode("Exp"); AddChild("Exp", $$, 4, $1, $2, $3, $4);}
	| Exp DOT ID {$$ = NewNode("Exp"); AddChild("Exp", $$, 3, $1, $2, $3);}
	| ID {$$ = NewNode("Exp"); AddChild("Exp", $$, 1, $1);}
	| INT {$$ = NewNode("Exp"); AddChild("Exp", $$, 1, $1);}
	| FLOAT {$$ = NewNode("Exp"); AddChild("Exp", $$, 1, $1);}
	;
Args : Exp COMMA Args {$$ = NewNode("Args"); AddChild("Args", $$, 3, $1, $2, $3);}
	| Exp {$$ = NewNode("Args"); AddChild("Args", $$, 1, $1);}
	;
%%

int yyerror(char * msg)
{
	fprintf(stderr, "error line,column %d, %d: %s\n",yylloc.first_line, yylloc.first_column, msg);
}

