%{
	#include"syntax.tab.h"
	#include"tree.h"
	int yycolumn = 1;
	#define YY_USER_ACTION \
		yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; \
		yylloc.last_column = yycolumn + yyleng - 1; \
		yycolumn += yyleng;
%}
%option yylineno

Delim [ \t]
Space {Delim}+
Letter [_a-zA-Z]
Number [0-9]
Id {Letter}({Letter}|{Number})*
Dec ([1-9][0-9]*)|0
Oct 0[0-7]*
Hex 0[Xx][0-9a-fA-F]*
Int {Dec}|{Oct}|{Hex}
Float ({Number}+\.{Number}+)|({Number}*\.{Number}[Ee][+-]?{Number}+)
Type "int"|"float"

%%
{Space} {}
\n {yycolumn = 1;}
{Int} 
	{
		yylval.node = NewNode("INT");
		return INT;
	}
{Float} 
	{
		yylval.node = NewNode("FLOAT");
		return FLOAT;
	}
";" 
	{
		yylval.node = NewNode("SEMI");
		return SEMI;
	}
"," 
	{
		yylval.node = NewNode("COMMA");
		return COMMA;
	}
"=" 
	{
		yylval.node = NewNode("ASSIGNOP");
		return ASSIGNOP;
	}
">"|"<"|">="|"<="|"=="|"!=" 
	{
		yylval.node = NewNode("RELOP");
		return RELOP;
	}
"+"
	{
		yylval.node = NewNode("PLUS");
		return PLUS;
	}
"-"
	{
		yylval.node = NewNode("MINUS");
		return MINUS;
	}
"*" 
	{
		yylval.node = NewNode("STAR");
		return STAR;
	}
"/" 
	{
		yylval.node = NewNode("DIV");
		return DIV;
	}
"&&" 
	{
		yylval.node = NewNode("AND");
		return AND;
	}
"||" 
	{
		yylval.node = NewNode("OR");
		return OR;
	}
"." 
	{
		yylval.node = NewNode("DOT");
		return DOT;
	}
"!" 
	{
		yylval.node = NewNode("NOT");
		return NOT;
	}
{Type} 
	{
		yylval.node = NewNode("TYPE");
		return TYPE;
	}
"(" 
	{
		yylval.node = NewNode("LP");
		return LP;
	}
")" 
	{
		yylval.node = NewNode("RP");
		return RP;
	}
"[" 
	{
		yylval.node = NewNode("LB");
		return LB;
	}
"]" 
	{
		yylval.node = NewNode("RB");
		return RB;
	}
"{" 
	{
		yylval.node = NewNode("LC");
		return LC;
	}
"}" 
	{
		yylval.node = NewNode("RC");
		return RC;
	}
struct 
	{
		yylval.node = NewNode("STRUCT");
		return STRUCT;
	}
return 
	{
		yylval.node = NewNode("RETURN");
		return RETURN;
	}
if 
	{
		yylval.node = NewNode("IF");
		return IF;
	}
else 
	{
		yylval.node = NewNode("ELSE");
		return ELSE;
	}
while 
	{
		yylval.node = NewNode("WHILE");
		return WHILE;
	}
{Id} 
	{
		yylval.node = NewNode("ID");
		return ID;
	}

"//" 
	{
		char c = input();
		while (c != '\n')
			c = input();
	}
\"[^\"]*\" 
	{
		if (yytext[yyleng - 2] == '\\')
		{
			yyless(yyleng - 1);
			yymore();
		} 
		else
		{
			/* process the string literal */
		}
	}

. 
	{
		printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
	}
%%

