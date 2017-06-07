#include "21sh.h"

t_tree *create_node(t_tree *left, t_token *token, t_list *tokens, t_tree *right)
{
	t_tree *new_node;

	if (!(new_node = (t_tree *)malloc(sizeof(t_tree))))
		ft_exiterror("Malloc failure", 1);
	new_node->left = left;
	new_node->right = right;
	new_node->token = token;
	new_node->cmds = list_to_tabstr(tokens);
	new_node->to_fd = -1;
	new_node->from_fd = -1;
	ft_clear_list(&tokens, free);
	return (new_node);
}

t_token *new_token(t_lexer *lexer, e_token token_type, char *value)
{
	t_token *token;

	if (!(token = (t_token *)malloc(sizeof(t_token))))
		exit(-1);
	if (is_string_op(*value))
	{
		if (!lexer->string_operator)
			lexer->string_operator = *value;
		else
			lexer->string_operator = 0;
	}
	if (token_type == FRED) //|| token_type == BRED)
		lexer->red = 1;
	else
		lexer->red = 0;
	lexer->line += ft_strlen(value);
	token->type = token_type;
	//token->value = clear_quot(value);
	if (token_type != WORD && token_type != NUM && token_type != TEXT && token_type != FD)
		value = ft_strdup(value);
	token->value = value;
	return (token);
}
