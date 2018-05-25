#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

#include"semantic.h"

#define hash_size 0x3fff
symbol_table SymbolTableHash[hash_size];
func_def_table FunctionDefHash[hash_size];
func_def_table FunctionDecHash[hash_size];
struct_table StructDefHash[hash_size];

unsigned hash(char *name);
void init_hash();

int insert_symbol_table(symbol_table node);
int insert_function_def_table(func_def_table node);
int insert_struct_table(struct_table st);

symbol_table search_symbol(Tree* node);
func_def_table search_func(Tree* node);
struct_table search_struct(Tree* node);
#endif
