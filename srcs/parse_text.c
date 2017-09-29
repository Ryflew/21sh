#include "tosh.h"

static char	bred_rules(t_sh *sh, t_fd *fd)
{
	if (sh->current_token->type == FD && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == BRED && \
		sh->lexer->lexems->next->next && \
		((t_token*)sh->lexer->lexems->next->next->data)->type == FD)
	{
		fd->type = BRED;
		fd->to = ft_atoi(sh->current_token->value);
		eat(sh, FD);
		eat(sh, BRED);
		fd->from = ft_atoi(sh->current_token->value);
		eat(sh, FD);
		return (1);
	}
	else if (sh->current_token->type == BRED && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == FD)
	{
		fd->type = BRED;
		fd->to = 0;
		eat(sh, BRED);
		fd->from = ft_atoi(sh->current_token->value);
		eat(sh, FD);
		return (1);
	}
	else if (sh->current_token->type == FD && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == BRED && \
		sh->lexer->lexems->next->next && \
		((t_token*)sh->lexer->lexems->next->next->data)->type == CLOSE_FD)
	{
		fd->type = BRED;
		fd->to = ft_atoi(sh->current_token->value);
		eat(sh, FD);
		eat(sh, BRED);
		fd->from = -2;
		eat(sh, CLOSE_FD);
		return (1);
	}
	return (0);
}

static char fred_rules(t_sh *sh, t_fd *fd)
{
	if (sh->current_token->type == FD && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == FRED && \
		sh->lexer->lexems->next->next && \
		((t_token*)sh->lexer->lexems->next->next->data)->type == FD)
	{
		fd->type = FRED;
		fd->from = ft_atoi(sh->current_token->value);
		eat(sh, FD);
		eat(sh, FRED);
		fd->to = ft_atoi(sh->current_token->value);
		eat(sh, FD);
		return (1);
	}
	else if (sh->current_token->type == FRED && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == FD)
	{
		fd->type = FRED;		
		fd->from = 1;	
		eat(sh, FRED);
		fd->to = ft_atoi(sh->current_token->value);
		eat(sh, FD);
		return (1);
	}
	else if (sh->current_token->type == FD && sh->lexer->lexems->next && \
		((t_token*)sh->lexer->lexems->next->data)->type == FRED && \
		sh->lexer->lexems->next->next && \
		((t_token*)sh->lexer->lexems->next->next->data)->type == CLOSE_FD)
	{
		fd->type = FRED;
		fd->from = ft_atoi(sh->current_token->value);
		eat(sh, FD);
		eat(sh, FRED);
		fd->to = -2;
		eat(sh, CLOSE_FD);
		return (1);
	}
	return (0);	
}

t_fd	*aggregation_rules(t_sh *sh)
{
	t_fd	*fd;

	if (!(fd = (t_fd*)malloc(sizeof(t_fd))))
		ft_exiterror("Malloc failure in aggregation_rules !", 1);
	if (!sh->current_token)
	{
		free(fd);
		return (NULL);
	}
	if (!fred_rules(sh, fd) && !bred_rules(sh, fd))
	{
		free(fd);
		return (NULL);
	}
	return (fd);
}

char			eat(t_sh *sh, e_token token)
{
	if (sh->current_token->type == token)
	{
		sh->lexer->lexems = sh->lexer->lexems->next;
		if (sh->lexer->lexems)
			sh->current_token = sh->lexer->lexems->data;
		else
			sh->current_token = NULL;
	}
	else
		return (-1);
	return (0);
}

void			manage_subshell(t_sh *sh, t_tree *sub_tree)
{
	manage_tree(sh, sub_tree);	
	if (sub_tree)
	{
		if (sh->fds_out)
			ft_clear_list(&sh->fds_out, (&free));
		sh->fds_out = NULL;
		sh->fd_pipe = -1;
		del_command_tree(sub_tree);
	}
}

int		sub_father(/*t_sh *shell, int *fd, */pid_t father)
{
	int ret;

	/*close(fd[1]);
	if (shell->fd_pipe != -1)
	{
		shell->fd_pipe = fd[0];
		if (shell->right_side)
		{
			waitpid(father, &ret, 0);
			shell->fd_pipe = -1;
		}
		else
			waitpid(father, &ret, WNOHANG);
	}
	else*/
	waitpid(father, &ret, 0);
	//shell->return_value = WEXITSTATUS(ret);
	return (ret);
}

#include <errno.h>

char			fork_subshell(t_sh *sh, e_token type)
{
	t_tree	*sub_tree;
	int		ret;
	pid_t	father;
	t_list	*new_lexems;
	char	*line;
	int		i;
	char	**new_tokens;

	if ((sub_tree = commands_line_rules(sh)) == (void*)-1)
		return (0);
	else if (sub_tree)
	{
		if (type == BQT)
		{
			sh->ssbqt = 1;
			sh->pipe_ss[0] = 0;	
			get_fd(sh, sh->pipe_ss);
		}
		if ((father = fork()) == -1)
			ft_exiterror("fork failure !", -1);
		else if (!father)
		{
			ret = EXIT_SUCCESS;
			manage_subshell(sh, sub_tree);
			exit(ret);
		}
		else
			ret = sub_father(father);
	}
	if (type == BQT)
	{	
		sh->ssbqt = 0;	
		close(sh->pipe_ss[1]);
		new_lexems = NULL;
		while ((ret = gnl(sh->pipe_ss[0], &line)) > 0)
		{
			ft_putstr("line => ");
			ft_putendl(line);
			new_tokens = ft_strsplit(line, ' ');
			i = -1;
			while (new_tokens[++i])
				ft_node_push_back(&new_lexems, new_token(NULL, WORD, new_tokens[i]));
			free(line);
			free(new_tokens);
		}
		close(sh->pipe_ss[0]);
		if (ret == -1)
		{
			ft_putendl("gnl failure");
			return (0);
		}
		if (new_lexems)		
			ft_add_list(sh->lexer->lexems, new_lexems);
	}
	return (1);
}

t_token			*text_rules(t_sh *sh)
{
	t_token *token;

	if (!sh->current_token)
		return (NULL);
	token = sh->current_token;
	if (sh->current_token->type == WORD)
		eat(sh, WORD);
	else if (sh->current_token->type == NUM)
		eat(sh, NUM);
	else if (token->type == BQT)
	{
		eat(sh, BQT);
		if (!(fork_subshell(sh, BQT)))
			return ((void*)-1);
		if (eat(sh, EBQT) == -1)
			return ((void*)-1);
		if (!(token = text_rules(sh)))
			return (new_token(NULL, NONE, ""));
	}
	else if (token->type == LPAR)
	{
		eat(sh, LPAR);
		fork_subshell(sh, LPAR);
		if (eat(sh, RPAR) == -1)
			return ((void*)-1);
		if (!(token = text_rules(sh)))
			return (new_token(NULL, NONE, ""));
	}
	else
		return (NULL);
	return (token);
}
