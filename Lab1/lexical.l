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
{Int} {printf("INT: %s\n", yytext); return INT;}
{Float} {printf("FLOAT: %s\n", yytext); return FLOAT;}
";" {printf("SEMI\n"); return SEMI;}
"," {printf("COMMA\n"); return COMMA;}
"=" {printf("ASSIGNOP\n"); return ASSIGNOP;}
">"|"<"|">="|"<="|"=="|"!=" {printf("RELOP\n"); return RELOP;}
"+" {printf("PLUS\n"); return PLUS;}
"-" {printf("SUB\n"); return MINUS;}
"*" {printf("MUL\n"); return STAR;}
"/" {printf("DIV\n"); return DIV;}
"&&" {printf("AND\n"); return AND;}
"||" {printf("OR\n"); return OR;}
"." {printf("DOT\n"); return DOT;}
"!" {printf("NOT\n"); return NOT;}
{Type} {printf("TYPE: %s\n", yytext); return TYPE;}
"(" {printf("LP\n"); return LP;}
")" {printf("RP\n"); return RP;}
"[" {printf("LB\n"); return LB;}
"]" {printf("RB\n"); return RB;}
"{" {printf("LC\n"); return LC;}
"}" {printf("RC\n"); return RC;}
struct {printf("STRUCT\n"); return STRUCT;}
return {printf("RETURN\n"); return RETURN;}
if {printf("IF\n"); return IF;}
else {printf("ELSE\n"); return ELSE;}
while {printf("WHILE\n"); return WHILE;}
{Id} {printf("ID: %s\n", yytext); return ID;}
"//" {char c = input(); while (c != '\n') c = input();}
\"[^\"]*\" {
	if (yytext[yyleng - 2] == '\\') {
		yyless(yyleng - 1);
		yymore();
	} 
	else {
		/* process the string literal */
		}
}
. {
	printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
}
%%

