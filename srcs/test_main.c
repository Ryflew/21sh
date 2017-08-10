#include "tosh.h"

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
		rec(node->right);
	}
	if (node->token)
	{
		ft_putstr("token = ");
		ft_putstr(node->token->value);
		if (node->from_fd)
		{
			ft_putstr(" from fd = ");
			ft_putnbr(node->from_fd);
		}
		if (node->to_fd)
		{
			ft_putstr(" to fd = ");
			ft_putnbr(node->to_fd);
		}
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
		init(&sh);
		rec(cmds_line(&sh));
	}
	return (0);
}
