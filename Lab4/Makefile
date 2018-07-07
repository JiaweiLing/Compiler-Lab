GCC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99

parser: syntax.tab.c tree.c
	$(GCC) tree.c readwrite.c semantic.c symbol_table_operation.c intercode.c mips32.c syntax.tab.c -lfl -ly -o parser

syntax.tab.c: syntax.y lex.yy.c
	$(BISON) -d syntax.y

lex.yy.c: lexical.l
	$(FLEX) lexical.l

.PHONY: clean test
test:
	./parser test/test1.c testout/out1.s
	./parser test/test2.c testout/out2.s

	
clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
	rm -f testout/*.ir
	rm -f testout/*.s
