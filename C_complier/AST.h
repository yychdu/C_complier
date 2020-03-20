#pragma once
#include "stack.h"
#include "tabledef.h"
#include "ast_node.h"
#define ast_get_token_id(_ptr) ((ast_node*)((tree_node*)_ptr)->dat)->id
enum stmt_list
{
	if_stmt = 125,	/*if”Ôæ‰*/	else_stmt,
	for_stmt,	/*for”Ôæ‰*/		while_stmt,	/*while”Ôæ‰*/	
	ptr_decl,					ptr_stmt,	/*±Ì¥Ô Ω*/		
	func_decl,					func_stmt,					
	sen_beg,					bloc_stmt,					
	expr_stmt,					decl_stmt,					
	arg_list
};
void ast_stack_init();
void ast_stack_destroy();
tree_node* build_decl(vector* _vec, size_t* _pos);
tree_node* build_func_def(vector* _vec, size_t* _pos);
tree_node* build_func_decl(vector* _vec, size_t* _pos);
tree_node* build_ptr_def(vector* _vec, size_t* _pos);
tree_node* build_ptr_decl(vector* _vec, size_t* _pos);
tree_node* build_expr(vector* _vec, size_t* _pos);
tree_node* build_while(vector* _vec, size_t* _pos);
tree_node* build_for(vector* _vec, size_t* _pos);
bitree* token_to_tree(vector* _vec);