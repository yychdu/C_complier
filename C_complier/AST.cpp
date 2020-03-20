#include "AST.h"
#include "source.h"
stack* ops, * ids;
void ast_stack_init()
{
	ops = stack_init(null_arg);
	ids = stack_init(null_arg);
}
void ast_stack_destroy()
{
	stack_destroy(ops);
	stack_destroy(ids);
}
tree_node* build_decl(vector* _vec, size_t* _pos)//非标识符退出
{
	if (!_vec)
		catch_exce(6);
	tree_node* decl_node = set_ast_node(decl_stmt, NULL, null_arg);
	tree_node** ptr = &(decl_node->ch[0]);
	token* tk;
	while (*_pos < _vec->size 
		&& (tk = vec_get_data(_vec, *_pos, token))->id < 32)
	{
		*ptr = set_ast_node_by_token(tk);
		ptr = &((*ptr)->ch[1]);
		++(*_pos);
	}
	return decl_node;
}
tree_node* build_func_decl(vector* _vec, size_t* _pos)//右括号退出
{
	if (!_vec)
		catch_exce(6);
	token* tk = vec_get_data(_vec, *_pos, token);
	tree_node* func_node = set_ast_node(arg_list, NULL, null_arg), * tmp = NULL;
	func_node->ch[0] = set_ast_node_by_token(tk);
	tree_node** ptr = &(func_node->ch[0]->ch[1]);
	*_pos += 2;//去除函数名和左括号
	while (*_pos < _vec->size)
	{
		tk = vec_get_data(_vec, *_pos, token);
		if (tk->id == 72 || tk->id == 64)//逗号
		{
			*ptr = tmp, ptr = &((*ptr)->ch[1]);
			tmp = NULL;
			if (tk->id == 64)
				break;
			++(*_pos);
		}
		else if (tk->id < 32)
			tmp = build_decl(_vec, _pos);
		else if (tk->id == 101)
		{
			if(!tmp)
				catch_exce(1); 
			if (*_pos + 1 >= _vec->size)
				catch_exce(0);
			tmp->ch[0]->ch[1] = set_ast_node_by_token(tk);
			token* nxt = vec_get_data(_vec, *_pos + 1, token);
			if (nxt->id != 72 && nxt->id != 64)
				catch_exce(4);
			++(*_pos);
		}
		else
			catch_exce(11);
	}
	return func_node;
}
tree_node* build_func_def(vector* _vec, size_t* _pos)//右括号退出
{
	if (!_vec)
		catch_exce(6);
	token* tk = vec_get_data(_vec, *_pos, token);
	tree_node* func_node = set_ast_node(func_stmt, NULL, null_arg), * tmp = NULL;
	func_node->ch[0] = set_ast_node_by_token(tk);
	tree_node** ptr = &(func_node->ch[0]->ch[1]);
	(*_pos) += 2;
	while (*_pos < _vec->size)
	{
		tk = vec_get_data(_vec, *_pos, token);
		if (tk->id == 72 || tk->id == 64)
		{
			*ptr = tmp, ptr = &((*ptr)->ch[1]);
			tmp = NULL;
			if (tk->id == 64)
				break;
			++(*_pos);
		}
		else if (tk->id != 64)
			tmp = build_expr(_vec, _pos);
	}
	return func_node;
}
tree_node* build_ptr_decl(vector* _vec, size_t* _pos)//右方括号退出
{
	if (!_vec)
		catch_exce(6);
	token* tk = vec_get_data(_vec, *_pos, token);
	tree_node* ptr_node = set_ast_node(ptr_stmt, NULL, null_arg), * tmp = NULL;
	ptr_node->ch[0] = set_ast_node_by_token(tk);
	tree_node** ptr = &(ptr_node->ch[0]->ch[1]);
	*_pos += 2;
	while (*_pos < _vec->size)
	{
		if (tk->id == 72 || tk->id == 66)
		{
			if (!tmp)
				*ptr = set_ast_node(-1, NULL, null_arg);
			else
				*ptr = tmp;
			tmp = NULL;
			ptr = &((*ptr)->ch[1]);
			if (tk->id == 66 && *_pos + 1 < _vec->size
				&& vec_get_data(_vec, *_pos + 1, token)->id != 65)
				break;
		}
		else
			tmp = build_expr(_vec,  _pos);
		++(*_pos);
	}
	return ptr_node;
}
tree_node* build_ptr_def(vector* _vec, size_t* _pos)//右方括号退出
{
	if (!_vec)
		catch_exce(6);
	token* tk = vec_get_data(_vec, *_pos, token);
	tree_node* ptr_node = set_ast_node(ptr_stmt, NULL, null_arg), * tmp = NULL;
	ptr_node->ch[0] = set_ast_node_by_token(tk);
	tree_node** ptr = &(ptr_node->ch[0]->ch[1]);
	*_pos += 2;
	while (*_pos < _vec->size)
	{
		if (tk->id == 72 || tk->id == 66)
		{
			if (!tmp)
				catch_exce(1);
			*ptr = tmp;
			ptr = &((*ptr)->ch[1]);
			tmp = NULL;
			if (tk->id == 66 && *_pos + 1 < _vec->size
				&& vec_get_data(_vec, *_pos + 1, token)->id != 65)
				break;
		}
		else
			tmp = build_expr(_vec, _pos);
		++(*_pos);
	}
	return ptr_node;
}
tree_node* build_if(vector* _vec,  size_t* _pos)//右括号退出
{
	if (!_vec)
		catch_exce(6);
	token* tk = vec_get_data(_vec, *_pos, token);
	tree_node* if_node = set_ast_node(if_stmt, NULL, null_arg), * tmp = NULL;
	if_node->ch[0] = set_ast_node_by_token(tk);
	tree_node** ptr = &(if_node->ch[0]->ch[1]);
	*_pos += 2;
	while (*_pos < _vec->size)
	{
		tk = vec_get_data(_vec, *_pos, token);
		if (tk->id == 72 || tk->id == 64)
		{
			if (!tmp)
				catch_exce(1);
			*ptr = tmp;
			ptr = &((*ptr)->ch[1]);
			tmp = NULL;
			if (tk->id == 64)
				break;
		}
		else
			tmp = build_expr(_vec, _pos);
	}
	return if_node;
}
tree_node* build_while(vector* _vec, size_t* _pos)//右括号退出
{
	if (!_vec)
		catch_exce(6);
	token* tk = vec_get_data(_vec, *_pos, token);
	tree_node* while_node = set_ast_node(while_stmt, NULL, null_arg), * tmp = NULL;
	while_node->ch[0] = set_ast_node_by_token(tk);
	tree_node** ptr = &(while_node->ch[0]->ch[1]);
	*_pos += 2;
	while (*_pos < _vec->size && (tk = vec_get_data(_vec, *_pos, token))->id != 64)
	{
		if (tk->id == 72 || tk->id == 64)
		{
			if (!tmp)
				catch_exce(1);
			*ptr = tmp;
			ptr = &((*ptr)->ch[1]);
			tmp = NULL;
			if (tk->id == 64)
				break;
		}
		else
			tmp = build_expr(_vec, _pos);
		++(*_pos);
	}
	return while_node;
}
tree_node* build_for(vector* _vec, size_t* _pos)//右括号退出
{
	if (!_vec)
		catch_exce(6);
	token* tk = vec_get_data(_vec, *_pos, token);
	tree_node* for_node = set_ast_node(for_stmt, NULL, null_arg), * tmp = NULL;
	for_node->ch[0] = set_ast_node_by_token(tk);
	tree_node** ptr = &(for_node->ch[0]->ch[1]);
	*_pos += 2;
	int expr_t = 0;
	while (*_pos < _vec->size)
	{
		tk = vec_get_data(_vec, *_pos, token);
		if (tk->id == 71 || tk->id == 64)
		{
			if (!tmp)
				catch_exce(1);
			*ptr = tmp;
			ptr = &((*ptr)->ch[1]);
			tmp = NULL;
			if (tk->id == 64 && expr_t == 2)
				break;
			else if (tk->id == 71 && expr_t < 2)
				++expr_t;
			else
				catch_exce(2);
			++(*_pos);
		}
		else
			tmp = build_expr(_vec, _pos);
	}
	return for_node;
}
tree_node* build_expr(vector* _vec, size_t* _pos)//分号和右括号退出
{
	if (!_vec)
		catch_exce(6);
	tree_node* expr_node = set_ast_node(expr_stmt, NULL, null_arg);//表达式子树树根结点
	token* tk;
	while (*_pos < _vec->size)
	{
		tk = vec_get_data(_vec, *_pos, token);
		if (tk->id >= 32 && tk->id < 63 || tk->id == 72)//运算符
		{
			if (!ops->size)
				stack_push_no_copy(ops, set_ast_node_by_token(tk));
			else
			{
				token* top = (token*)((tree_node*)stack_top(ops))->dat;
				while (ops->size && get_prev(tk->id) > get_prev(top->id))
				{
					if (ids->size <= 1)
						catch_exce(5);
					tree_node* top_n = (tree_node*)stack_top_and_pop_no_delete(ops);
					top_n->ch[0] = (tree_node*)stack_top_and_pop_no_delete(ids);
					top_n->ch[0]->ch[1] = (tree_node*)stack_top_and_pop_no_delete(ids);
					stack_push_no_copy(ids, top_n);
				}
				stack_push_no_copy(ops, set_ast_node_by_token(tk));
			}
		}
		else if (tk->id >= 100 && tk->id != 101)//数字常量和字面量
			stack_push_no_copy(ids, set_ast_node_by_token(tk));
		else if (tk->id == 101)//变量
		{
			token* nxt = vec_get_data(_vec, *_pos + 1, token);
			if (nxt->id == 63)//左圆括号
				stack_push_no_copy(ids, build_func_def(_vec, _pos));
			else if (nxt->id == 65)//左方括号
				stack_push_no_copy(ids, build_ptr_def(_vec, _pos));
			else
				stack_push_no_copy(ids, set_ast_node_by_token(tk));
		}
		else if (tk->id == 71 || tk->id == 64)//分号
		{
			while (ops->size)
			{
				if (ids->size <= 1)
					catch_exce(5);
				tree_node* top = (tree_node*)stack_top_and_pop_no_delete(ops);
				top->ch[0] = (tree_node*)stack_top_and_pop_no_delete(ids);
				top->ch[0]->ch[1] = (tree_node*)stack_top_and_pop_no_delete(ids);
				stack_push_no_copy(ids, top);
			}
			break;
		}
		else if (tk->id == 63)
		{
			++(*_pos);
			stack_push_no_copy(ids, build_expr(_vec, _pos));
		}
		++(*_pos);
	}
	expr_node->ch[0] = (tree_node*)stack_top_and_pop_no_delete(ids);
	if (ops->size || ids->size)
		catch_exce(5);
	return expr_node;
}
bitree* token_to_tree(vector* _vec)
{
	if (!_vec)
		catch_exce(6);
	bitree* btr = bitree_init(token_arg);
	btr->rt->ch[0] = set_ast_node(sen_beg, NULL, null_arg);
	tree_node** ptr = &(btr->rt->ch[0]->ch[1]), ** cur_s = &(btr->rt->ch[1]);
	for (size_t i = 0; i < _vec->size;)
	{
		token* tk = vec_get_data(_vec, i, token);
		if (tk->id < 32)//标识符
		{
			if (tk->id == 15)//if
				*ptr = build_if(_vec, &i);
			else if (tk->id == 31)//while
				*ptr = build_while(_vec, &i);
			else if (tk->id == 13)//for
				*ptr = build_for(_vec, &i);
			else if (tk->id == 19 || tk->id == 9)//return
			{
				*ptr = set_ast_node_by_token(tk);
				++i;
			}
			//break和continue
			else
			{
				*ptr = build_decl(_vec, &i);
				ptr = &((*ptr)->ch[1]);
				tk = vec_get_data(_vec, i, token);
				if (tk->id >= 100)
				{
					if (i + 1 >= _vec->size)
						catch_exce(0);
					if (vec_get_data(_vec, i + 1, token)->id == 63)
						*ptr = build_func_decl(_vec, &i);
					else if (vec_get_data(_vec, i + 1, token)->id == 65)
						*ptr = build_ptr_decl(_vec, &i);
					else if (vec_get_data(_vec, i + 1, token)->id == 42)
						*ptr = build_expr(_vec, &i);
				}
			}
			ptr = &((*ptr)->ch[1]);
		}
		else if (tk->id >= 100)
		{
			*ptr = build_expr(_vec, &i);
			ptr = &((*ptr)->ch[1]);
		}
		else if (tk->id == 67 || tk->id == 71)
		{
			if (tk->id == 67)//左花括号
				*cur_s = set_ast_node(bloc_stmt, NULL, null_arg);
			else if (tk->id == 71)//分号
				*cur_s = set_ast_node(sen_beg, NULL, null_arg);
			ptr = &((*cur_s)->ch[0]);
			cur_s = &((*cur_s)->ch[1]);
			++i;
		}
		else
			++i;
	}
	return btr;
}