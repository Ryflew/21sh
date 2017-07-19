#include "21sh.h"
#include <fcntl.h>

static int	open_chevb(t_tree *node)
{
	int		fd_file;

	if ((fd_file = open(node->right->cmds[0], O_RDONLY)) == -1)
		errexit("No such file or directory", node->right->cmds[0]);
	return (fd_file);
}

static int	open_dchevf(t_tree *node)
{
	int		fd_file;

	if ((fd_file = open(node->right->cmds[0], O_WRONLY | O_APPEND | \
		O_CREAT, 0644)) == -1)
		errexit("No such file or directory", node->right->cmds[0]);
	return (fd_file);
}

static int	open_chevf(t_tree *node)
{
	int		fd_file;

	if ((fd_file = open(node->right->cmds[0], O_WRONLY | O_CREAT | \
		O_TRUNC, 0644)) == -1)
		errexit("No such file or directory", node->right->cmds[0]);
	return (fd_file);
}

int			open_file(t_tree *node)
{
	int	ret;

	ret = 0;
	if (node->token && node->token->type == CHEVB)
		return (open_chevb(node));
	else if (node->token && (node->token->type == CHEVF || \
		node->token->type == FRED))
		return (open_chevf(node));
	else if (node->token && node->token->type == DCHEVF)
		return (open_dchevf(node));
	else
		return (ret);
}
