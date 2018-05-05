GCC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99

parser: syntax.tab.c tree.c
	$(GCC) tree.c syntax.tab.c -lfl -ly -o parser

syntax.tab.c: syntax.y lex.yy.c
	$(BISON) -d syntax.y

lex.yy.c: lexical.l
	$(FLEX) lexical.l

.PHONY: clean test
test:
	./parser test/test9.c
clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
