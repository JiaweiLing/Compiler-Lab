%{
	#include<stdio.h>
	#include<stdlib.h>
	#include"tree.h"
	#include"intercode.h"
	Tree *root_node;
	int yylex();
	int yyerror(char *msg);
	extern int error_state;
%}
%locations
%union {
	Tree *node;
}


%token <node> INT FLOAT
%token <node> ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV AND
%token <node> OR DOT NOT TYPE LP RP LB RB LC RC
%token <node> STRUCT RETURN IF ELSE WHILE

%type <node> Program ExtDefList ExtDef ExtDecList Specifier 
%type <node> StructSpecifier OptTag Tag VarDec FunDec VarList 
%type <node> ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%right ASSIGNOP  
%left OR 
%left AND 
%left RELOP
%left PLUS MINUS 
%left STAR DIV
%right NOT
%left DOT LB RB LP RP
%%

Program : ExtDefList {root_node = $$ = AddChild("Program", -1, 1, $1);}
	;
ExtDefList : ExtDef ExtDefList {$$ = AddChild("ExtDefList", -1, 2, $1, $2);}
	| /* Empty */{$$ = NewNode("ExtDefList", "", -1);}
	;
ExtDef : Specifier ExtDecList SEMI {$$ = AddChild("ExtDef", -1, 3, $1, $2, $3);}
	| Specifier SEMI {$$ = AddChild("ExtDef", -1, 2, $1, $2);}
	| Specifier FunDec CompSt {$$ = AddChild("ExtDef", -1, 3, $1, $2, $3);}
	| error SEMI {error_state = 1;}
	;
ExtDecList : VarDec {$$ = AddChild("ExtDecList", -1, 1, $1);}
	| VarDec COMMA ExtDecList {$$ = AddChild("ExtDecList", -1, 3, $1, $2, $3);}
	;
Specifier : TYPE {$$ = AddChild("Specifier", -1, 1, $1);}
	| StructSpecifier {$$ = AddChild("Specifier", -1, 1, $1);}
	;
StructSpecifier : STRUCT OptTag LC DefList RC {$$ = AddChild("StructSpecifier", -1, 5, $1, $2, $3, $4, $5);}
	| STRUCT Tag {$$ = AddChild("StructSpecifier", -1, 2, $1, $2);}
	;
OptTag : ID {$$ = AddChild("OptTag", -1, 1, $1);}
	| /* Empty */ {$$ = NewNode("OptTag", "", -1);}
	;
Tag : ID {$$ = AddChild("Tag", -1, 1, $1);}
	;
VarDec : ID {$$ = AddChild("VarDec", -1, 1, $1);}
	| VarDec LB INT RB {$$ = AddChild("VarDec", -1, 4, $1, $2, $3, $4);}
	| VarDec LB error SEMI {error_state = 1;}
	;
FunDec : ID LP VarList RP {$$ = AddChild("FunDec", -1, 4, $1, $2, $3, $4);}
	| ID LP RP {$$ = AddChild("FunDec", -1, 3, $1, $2, $3);}
	| error RP {error_state = 1;}
	;
VarList : ParamDec COMMA VarList {$$ = AddChild("VarList", -1, 3, $1, $2, $3);}
	| ParamDec {$$ = AddChild("VarList", -1, 1, $1);}
	;
ParamDec : Specifier VarDec {$$ = AddChild("ParamDec", -1, 2, $1, $2);}
	;
CompSt : LC DefList StmtList RC {$$ = AddChild("CompSt", -1, 4, $1, $2, $3, $4);}
	| LC DefList error RC {error_state = 1;}
	;
StmtList : Stmt StmtList {$$ = AddChild("StmtList", -1, 2, $1, $2);}
	| /* Empty */ {$$ = NewNode("StmtList", "", -1);}
	;
Stmt : Exp SEMI {$$ = AddChild("Stmt", -1, 2, $1, $2);}
	| CompSt {$$ = AddChild("Stmt", -1, 1, $1);}
	| RETURN Exp SEMI {$$ = AddChild("Stmt", -1, 3, $1, $2, $3);}
	| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$ = AddChild("Stmt", -1, 5, $1, $2, $3, $4, $5);}
	| IF LP Exp RP Stmt ELSE Stmt {$$ = AddChild("Stmt", -1, 7, $1, $2, $3, $4, $5, $6, $7);}
	| IF LP Exp RP error ELSE Stmt {error_state = 1;}
	| WHILE LP Exp RP Stmt {$$ = AddChild("Stmt", -1, 5, $1, $2, $3, $4, $5);}
	| Exp LB error SEMI {error_state = 1;}
	| IF LP error SEMI {error_state = 1;}
	| LP Exp error SEMI {error_state = 1;}
	| ID LP error SEMI {error_state = 1;}
	| error SEMI {error_state = 1;}
	;
DefList : Def DefList {$$ = AddChild("DefList", -1, 2, $1, $2);}
	| /* Empty */ {$$ = NewNode("DefList", "", -1);}
	;
Def : Specifier DecList SEMI {$$ = AddChild("Def", -1, 3, $1, $2, $3);}
	| Specifier error SEMI {error_state = 1;}
	;
DecList : Dec {$$ = AddChild("DecList", -1, 1, $1);}
	| Dec COMMA DecList {$$ = AddChild("DecList", -1, 3, $1, $2, $3);}
	;
Dec : VarDec {$$ = AddChild("Dec", -1, 1, $1);}
	| VarDec ASSIGNOP Exp {$$ = AddChild("Dec", -1, 3, $1, $2, $3);}
	| VarDec ASSIGNOP error {error_state = 1;}
	;
Exp : Exp ASSIGNOP Exp {$$ = AddChild("Exp", -1, 3, $1, $2, $3);}
	| Exp AND Exp {$$ = AddChild("Exp", -1, 3, $1, $2, $3);}
	| Exp OR Exp {$$ = AddChild("Exp", -1, 3, $1, $2, $3);}
	| Exp RELOP Exp {$$ = AddChild("Exp", -1, 3, $1, $2, $3);}
	| Exp PLUS Exp {$$ = AddChild("Exp", -1, 3, $1, $2, $3);}
	| Exp MINUS Exp {$$ = AddChild("Exp", -1, 3, $1, $2, $3);}
	| Exp STAR Exp {$$ = AddChild("Exp", -1, 3, $1, $2, $3);}
	| Exp DIV Exp {$$ = AddChild("Exp", -1, 3, $1, $2, $3);}
	| LP Exp RP {$$ = AddChild("Exp", -1, 3, $1, $2, $3);}
	| MINUS Exp {$$ = AddChild("Exp", -1, 2, $1, $2);}
	| NOT Exp {$$ = AddChild("Exp", -1, 2, $1, $2);}
	| ID LP Args RP {$$ = AddChild("Exp", -1, 4, $1, $2, $3, $4);}
	| ID LP RP {$$ = AddChild("Exp", -1, 3, $1, $2, $3);}
	| Exp LB Exp RB {$$ = AddChild("Exp", -1, 4, $1, $2, $3, $4);}
	| Exp LB error RB {error_state = 1;}
	| Exp DOT ID {$$ = AddChild("Exp", -1, 3, $1, $2, $3);}
	| ID {$$ = AddChild("Exp", -1, 1, $1);}
	| INT {$$ = AddChild("Exp", -1, 1, $1);}
	| FLOAT {$$ = AddChild("Exp", -1, 1, $1);}
	;
Args : Exp COMMA Args {$$ = AddChild("Args", -1, 3, $1, $2, $3);}
	| Exp {$$ = AddChild("Args", -1, 1, $1);}
	;
%%
#include"lex.yy.c"
extern FILE* yyin;
int main(int argc, char** argv) 
{
	if (argc <= 1) return 1;
	FILE* f = fopen(argv[1], "r");
	if (!f) 
	{
		perror(argv[1]);
		return 1;
	}
	
	yyrestart(f);
	yyin = f;
	
	FILE* file = fopen(argv[2], "w");
	if (!file)
	{
		perror(argv[2]);
		return 1;
	}
	while (!feof(yyin)) yyparse();
	
	if (!error_state) 
	{
		//PrintTree(root_node, 0);
		check_semantic(root_node);
		//printf("1\n");
		translate(root_node, file);
	}

}
int yyerror(char * msg)
{
	fprintf(stderr, "Error type B at Line %d: %s\n", yylloc.first_line, msg);
}

