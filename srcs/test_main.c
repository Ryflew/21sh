#include "21sh.h"

void rec(t_tree *node)
{
	ft_putstr("(");
	if (node->left)
	{
	ft_putendl("left");
		rec(node->left);
	}
	if (node->right)
	{
	ft_putendl("right");
		rec(node->left);
	}
	if (node->token)
	{
	ft_putendl("token");
		ft_putstr(node->token->value);
	}
	if (node->tokens)
	{
		t_list *tmp = node->tokens;
		t_token *token;
		while (tmp)
		{
			token = tmp->data;
			ft_putstr(token->value);
			if (tmp->next)
				ft_putstr(" ");
			tmp = tmp->next;
		}
	}
	ft_putendl(")");
}

int main(int argc, char **argv)
{
	t_sh		sh;
	t_lexer	lexer;

	if (argc == 2)
	{
		sh.lexer = &lexer;
		sh.lexer->line = argv[1];
		rec(cmds_line(&sh));
	}
	return (0);
}
