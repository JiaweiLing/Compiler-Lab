%{
	#include"syntax.tab.h"
	int yycolumn = 1;
	int error_state = 0;
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
Float ({Number}+\.{Number}+)|({Number}*\.{Number}*[Ee][+-]?{Number}+)
Comment [/][*][^*]*[*]+([^*/][^*]*[*]+)*[/] 

%%
{Space} {}
\n {yycolumn = 1;}
struct  {
		yylval.node = NewNode("STRUCT", "none", yylineno);
		return STRUCT;
	}
return 	{
		yylval.node = NewNode("RETURN", "none", yylineno);
		return RETURN;
	}
if 	{
		yylval.node = NewNode("IF", "none", yylineno);
		return IF;
	}
else 	{
		yylval.node = NewNode("ELSE", "none", yylineno);
		return ELSE;
	}
while 	{
		yylval.node = NewNode("WHILE", "none", yylineno);
		return WHILE;
	}
{Type} 	{
		yylval.node = NewNode("TYPE", yytext, yylineno);
		return TYPE;
	}
{Int} 	{
		yylval.node = NewNode("INT", yytext, yylineno);
		return INT;
	}
{Float} {
		yylval.node = NewNode("FLOAT", yytext, yylineno);
		return FLOAT;
	}
{Id} 	{
		yylval.node = NewNode("ID", yytext, yylineno);
		return ID;
	}
";" 	{
		yylval.node = NewNode("SEMI", "none", yylineno);
		return SEMI;
	}
"," 	{
		yylval.node = NewNode("COMMA", "none", yylineno);
		return COMMA;
	}
"=" 	{
		yylval.node = NewNode("ASSIGNOP", "none", yylineno);
		return ASSIGNOP;
	}
">"|"<"|">="|"<="|"=="|"!=" 	{
		yylval.node = NewNode("RELOP", "none", yylineno);
		return RELOP;
	}
"+"	{
		yylval.node = NewNode("PLUS", "none", yylineno);
		return PLUS;
	}
"-"	{
		yylval.node = NewNode("MINUS", "none", yylineno);
		return MINUS;
	}
"*" 	{
		yylval.node = NewNode("STAR", "none", yylineno);
		return STAR;
	}
"/" 	{
		yylval.node = NewNode("DIV", "none", yylineno);
		return DIV;
	}
"&&" 	{
		yylval.node = NewNode("AND", "none", yylineno);
		return AND;
	}
"||" 	{
		yylval.node = NewNode("OR", "none", yylineno);
		return OR;
	}
"." 	{
		yylval.node = NewNode("DOT", "none", yylineno);
		return DOT;
	}
"!" 	{
		yylval.node = NewNode("NOT", "none", yylineno);
		return NOT;
	}

"(" 	{
		yylval.node = NewNode("LP", "none", yylineno);
		return LP;
	}
")" 	{
		yylval.node = NewNode("RP", "none", yylineno);
		return RP;
	}
"[" 	{
		yylval.node = NewNode("LB", "none", yylineno);
		return LB;
	}
"]" 	{
		yylval.node = NewNode("RB", "none", yylineno);
		return RB;
	}
"{" 	{
		yylval.node = NewNode("LC", "none", yylineno);
		return LC;
	}
"}" 	{
		yylval.node = NewNode("RC", "none", yylineno);
		return RC;
	}
{Comment} {;}
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
		error_state = 1;
		printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
	}
%%

