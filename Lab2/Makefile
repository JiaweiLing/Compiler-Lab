GCC = gcc
FLEX = flex
BISON = bison
CFLAGS = -std=c99

parser: syntax.tab.c tree.c
	$(GCC) tree.c semantic.c hash_table.c syntax.tab.c -lfl -ly -o parser

syntax.tab.c: syntax.y lex.yy.c
	$(BISON) -d syntax.y

lex.yy.c: lexical.l
	$(FLEX) lexical.l

.PHONY: clean test
test:
	./parser test/test1.c
	./parser test/test2.c
	./parser test/test3.c
	./parser test/test4.c
	./parser test/test5.c
	./parser test/test6.c
	./parser test/test7.c
	./parser test/test8.c
	./parser test/test9.c
	./parser test/test10.c
	./parser test/test11.c
	./parser test/test12.c
	./parser test/test13.c
	./parser test/test14.c
	./parser test/test15.c
	./parser test/test16.c
	./parser test/test17.c
	
clean:
	rm -f parser lex.yy.c syntax.tab.c syntax.tab.h syntax.output
