GCC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99

parser: main.c syntax.tab.c
	$(GCC) main.c syntax.tab.c -lfl -ly -o parser

syntax.tab.c: syntax.y lex.yy.c
	$(BISON) -d syntax.y

lex.yy.c: lexical.l
	$(FLEX) lexical.l

.PHONY: clean test
test:
	./parser test.cmm
clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
