#include "tosh.h"

static void	manage_subshell(t_sh *sh, t_tree *sub_tree)
{
	sh->fd_pipe = -1;
	sh->fds_out = NULL;
	sh->lexer->her = 0;
	sh->return_value = 0;
	browse_tree(sub_tree, sh, NULL, 1);
	if (sub_tree)
	{
		if (sh->fds_out)
			ft_clear_list(&sh->fds_out, (&free));
		sh->fds_out = NULL;
		sh->fd_pipe = -1;
		del_command_tree(sub_tree);
	}
}

static char	*get_new_tokens(t_sh *sh)
{
	char	*tokens_line;
	char	*to_free;
	int		ret;
	char	buff[501];

	tokens_line = NULL;
	while ((ret = read(sh->pipe_ss[0], buff, 500)) > 0)
	{
		buff[ret] = 0;
		to_free = tokens_line;
		tokens_line = ft_strjoin(tokens_line, buff);
		if (to_free)
			free(to_free);
	}
	if (ret == -1)
	{
		ft_putendl("read failure");
		return ((void*)-1);
	}
	return (tokens_line);
}

static char	add_new_tokens(t_sh *sh)
{
	int		i;
	t_list	*new_lexems;
	char	*tokens_line;
	char	**new_tokens;

	sh->ssbqt = 0;
	close(sh->pipe_ss[1]);
	if ((tokens_line = get_new_tokens(sh)) == (void*)-1)
		return (0);
	if (!(new_tokens = ft_strsplit(tokens_line, '\n')))
		return (1);
	i = -1;
	new_lexems = NULL;
	while (new_tokens[++i])
	{
		ft_node_push_back(&new_lexems, new_token(NULL, WORD, new_tokens[i]));
		free(new_tokens[i]);
	}
	free(tokens_line);
	free(new_tokens);
	close(sh->pipe_ss[0]);
	if (new_lexems)
		ft_add_list(sh->lexer->lexems, new_lexems);
	return (1);
}

static void	fork_subshell(t_sh *sh, t_tree *sub_tree)
{
	int		ret;
	pid_t	father;

	if ((father = fork()) == -1)
		ft_exiterror("fork failure !", -1);
	else if (!father)
	{
		ret = EXIT_SUCCESS;
		manage_subshell(sh, sub_tree);
		exit(ret);
	}
	else
		waitpid(father, &ret, 0);
}

char		subshell(t_sh *sh, e_token type)
{
	t_tree	*sub_tree;

	if ((sub_tree = commands_line_rules(sh)) == (void*)-1 || !sub_tree)
		return (0);
	if (type == BQT)
	{
		sh->ssbqt = 1;
		sh->pipe_ss[0] = 0;
		get_fd(sh, sh->pipe_ss);
	}
	fork_subshell(sh, sub_tree);
	if (sh->fds_out)
		ft_clear_list(&sh->fds_out, (&free));
	del_command_tree(sub_tree);
	if (type == BQT)
		return (add_new_tokens(sh));
	return (1);
}
