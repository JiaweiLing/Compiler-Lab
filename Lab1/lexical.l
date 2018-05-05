%{
	#include"syntax.tab.h"
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
Type "int"|"float"
Id {Letter}({Letter}|{Number})*
Dec ([1-9][0-9]*)|0
Oct 0[0-7]*
Hex 0[Xx][0-9a-fA-F]*
Int {Dec}|{Oct}|{Hex}
Float ({Number}+\.{Number}+)|({Number}*\.{Number}[Ee][+-]?{Number}+)


%%
{Space} {}
\n {yycolumn = 1;}
struct  {
		yylval.node = NewNode("STRUCT", yylineno);
		return STRUCT;
	}
return 	{
		yylval.node = NewNode("RETURN", yylineno);
		return RETURN;
	}
if 	{
		yylval.node = NewNode("IF", yylineno);
		return IF;
	}
else 	{
		yylval.node = NewNode("ELSE", yylineno);
		return ELSE;
	}
while 	{
		yylval.node = NewNode("WHILE", yylineno);
		return WHILE;
	}
{Type} 	{
		yylval.node = NewNode("TYPE", yylineno);
		return TYPE;
	}
{Int} 	{
		yylval.node = NewNode("INT", yylineno);
		return INT;
	}
{Float} {
		yylval.node = NewNode("FLOAT", yylineno);
		return FLOAT;
	}
{Id} 	{
		yylval.node = NewNode("ID", yylineno);
		return ID;
	}
";" 	{
		yylval.node = NewNode("SEMI", yylineno);
		return SEMI;
	}
"," 	{
		yylval.node = NewNode("COMMA", yylineno);
		return COMMA;
	}
"=" 	{
		yylval.node = NewNode("ASSIGNOP", yylineno);
		return ASSIGNOP;
	}
">"|"<"|">="|"<="|"=="|"!=" 	{
		yylval.node = NewNode("RELOP", yylineno);
		return RELOP;
	}
"+"	{
		yylval.node = NewNode("PLUS", yylineno);
		return PLUS;
	}
"-"	{
		yylval.node = NewNode("MINUS", yylineno);
		return MINUS;
	}
"*" 	{
		yylval.node = NewNode("STAR", yylineno);
		return STAR;
	}
"/" 	{
		yylval.node = NewNode("DIV", yylineno);
		return DIV;
	}
"&&" 	{
		yylval.node = NewNode("AND", yylineno);
		return AND;
	}
"||" 	{
		yylval.node = NewNode("OR", yylineno);
		return OR;
	}
"." 	{
		yylval.node = NewNode("DOT", yylineno);
		return DOT;
	}
"!" 	{
		yylval.node = NewNode("NOT", yylineno);
		return NOT;
	}

"(" 	{
		yylval.node = NewNode("LP", yylineno);
		return LP;
	}
")" 	{
		yylval.node = NewNode("RP", yylineno);
		return RP;
	}
"[" 	{
		yylval.node = NewNode("LB", yylineno);
		return LB;
	}
"]" 	{
		yylval.node = NewNode("RB", yylineno);
		return RB;
	}
"{" 	{
		yylval.node = NewNode("LC", yylineno);
		return LC;
	}
"}" 	{
		yylval.node = NewNode("RC", yylineno);
		return RC;
	}


"//" 	{
		char c = input();
		while (c != '\n') c = input();
	}
\"[^\"]*\" 	{
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
. 	{
		printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
	}
%%

