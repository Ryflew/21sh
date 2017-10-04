#include "tosh.h"
#include <fcntl.h>

/*void		read_heredoc(char *heredoc_line, int *fd_pipe)
{
	char	buff[501];
	int		ret;
	
	heredoc_line = NULL;
	while ((ret = read(fd, buff, 500)) > 0)
    {
        buff[ret] = 0;
        to_free = heredoc_line;
        heredoc_line = ft_strjoin(heredoc_line, buff);
        if (to_free)
            free(to_free);
    }
    if (ret == -1)
    {
        ft_putendl("read failure");
        return ;
    }
}*/

char		is_bqt_in_heredoc(t_sh *sh)
{
	t_list	*tmp;
	t_token	*token;
	
	tmp = sh->lexer->lexems;
	while (tmp)
	{
		token = (t_token*)tmp->data;
		if (TYPE == BQT)
		{
			if (sh->lexer->lexems)
				sh->current_token = sh->lexer->lexems->data;
			else
				sh->current_token = NULL;
			eat(sh, BQT);
			if (!(subshell(sh, BQT)))
				return (0);
			if (eat(sh, EBQT) == -1)
			{
				ft_putendl("parse error: backquote isn't close");
				return (0);
			}
			while (tmp)
			{
				token = (t_token*)tmp->data;
				if (TYPE == EBQT)
				{
					tmp = tmp->next;
					break;
				}
				tmp = tmp->next;
			}
		}
		tmp = tmp->next;
	}
	return (1);
}

char		**lex_parse_heredoc(t_sh *sh, char *heredoc_line)
{
	t_list	*begin_lexems;
	char	**cmds;

	sh->lexer->line = heredoc_line;
	sh->lexer->her = 1;
	get_lexems(sh);
	begin_lexems = sh->lexer->lexems;
	if (!(is_bqt_in_heredoc(sh)))
	{
		ft_clear_list(&begin_lexems, (void*)&clear_lexems);
		sh->lexer->her = 0;	
		return ((void*)-1);
	}
	cmds = get_cmds(&sh->lexer->lexems, sh);
	ft_clear_list(&begin_lexems, (void*)&clear_lexems);
	sh->lexer->her = 0;
	return (cmds);
}

char		manage_dchevb(t_sh *sh, char *cmd, t_tree *node, int *fd_pipe)
{
	char	*heredoc_line;
	char	*line;
	char	*to_free;
	int		ret;
	char	*prompt;
	char	**cmds;

	line = NULL;
	heredoc_line = NULL;
	cmds = NULL;
	if (!node->left)
		set_old_term(get_shell());
	if (!(prompt = find_env(sh->env, "PS2")))
		prompt = "heredoc> ";
	ft_putstr(prompt);		
	while ((ret = gnl(0, &line) > 0) && ft_strcmp(line, cmd))
	{
		to_free = heredoc_line;
		if (!to_free)
			heredoc_line = ft_strdup(line);
		else
		{
        	heredoc_line = ft_strstrjoin(heredoc_line, "\n", line);
			free(to_free);
		}
		free(line);
		ft_putstr(prompt);
	}
	if (!node->left)
		set_our_term(get_shell());
	if (ret == -1)
	{
		ft_putendl("read failure");
		return (0);
	}
	free(line);
	if (heredoc_line)
	{
		if (heredoc_line && (cmds = (lex_parse_heredoc(sh, heredoc_line))) == (void*)-1)
			return (0);
		//read_heredoc(heredoc_line, fd_pipe);
		int	i;

		i = -1;
		while (cmds && cmds[++i])
			ft_fputendl(cmds[i], fd_pipe[1]);
		ft_strdelpp(&cmds);
	}
	if (node->left)
	{
		close(fd_pipe[1]);
		if (node->to_fd != -1)
			dup2(fd_pipe[0], node->to_fd);
		else
			dup2(fd_pipe[0], 0);
	}
	return (1);
}

void		manage_chevb(t_fd fd)
{
	if (fd.to != -1)
	{
		dup2(fd.to, 0);
		close(fd.to);
	}
	else
		dup2(fd.file, 0);
	close(fd.file);
}

static char	manage_chev(t_tree *node, t_sh *shell)
{
	t_fd	*fd;

	node->right->cmds = get_cmds(&node->right->cmd_tokens, shell);
	if (node->TYPE == DCHEVB && !node->left)
	{
		if (!(manage_dchevb(shell, node->right->cmds[0], node, NULL)))
			return (-1);
	}
	else if (node->TYPE == CHEVF || node->TYPE == DCHEVF
			|| node->TYPE == FRED)
	{
		if (!(fd = (t_fd*)malloc(sizeof(t_fd))))
			exit(-1);
		if ((fd->file = open_file(node)) == -1)
			return (-1);
		if (node->TYPE != FRED)
			fd->from = (node->from_fd == -1) ? 1 : node->from_fd;
		else
			fd->from = node->from_fd;
		if (node->to_fd)
			fd->to = node->to_fd;
		else
			fd->to = -1;
		fd->type = node->TYPE;
		ft_node_push_front(&shell->fds_out, fd);
	}
	return (0);
}

char		operators(t_tree *node, t_sh *shell)
{
	int		ret;

	ret = 0;
	if (node->TYPE == PIPE)
	{
		if (shell->fd_pipe == -1)
			shell->fd_pipe = 0;
	}
	else if (node->TYPE == CHEVB)
	{
		node->right->cmds = get_cmds(&node->right->cmd_tokens, shell);	
		if ((shell->fd_in.file = open_file(node)) == -1)
			return (-1);
		shell->fd_in.to = node->to_fd;
		shell->fd_in.from = -1;
		shell->fd_in.type = node->TYPE;
	}
	else
		return (manage_chev(node, shell));
	return (0);
}
