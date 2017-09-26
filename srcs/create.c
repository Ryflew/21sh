#include "tosh.h"

t_tree	*create_node(t_tree *left, t_token *token, t_list *tokens, t_tree *rig)
{
	t_tree *new_node;

	if (!(new_node = (t_tree *)malloc(sizeof(t_tree))))
		ft_exiterror("Malloc failure", 1);
	new_node->left = left;
	new_node->right = rig;
	new_node->token = token;
	new_node->cmds = list_to_tabstr(tokens);
	new_node->to_fd = -1;
	new_node->from_fd = -1;
	new_node->tmp_env = NULL;
	new_node->aggregations = NULL;
	ft_clear_list(&tokens, &free);
	return (new_node);
}

t_token	*new_token(t_lexer *lexer, e_token token_type, char *value)
{
	t_token *token;

	if (!(token = (t_token *)malloc(sizeof(t_token))))
		exit(-1);
	if (lexer)
		lexer->line += ft_strlen(value);
	token->type = token_type;
	if (token_type != WORD && token_type != TILD && token_type != NUM && \
		token_type != FD)
		value = ft_strdup(value);
	token->value = value;
	return (token);
}
