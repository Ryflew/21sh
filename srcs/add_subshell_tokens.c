#include "tosh.h"

static char	*get_new_tokens(t_sh *sh)
{
	char	*tokens_line;
	char	*to_free;
	int		ret;
	char	buff[501];
	int		*fd;

	fd = (int*)ft_go_end_list(sh->pipe_ss)->data;
	close(fd[1]);
	tokens_line = NULL;
	while ((ret = read(fd[0], buff, 500)) > 0)
	{
		buff[ret] = 0;
		to_free = tokens_line;
		tokens_line = ft_strjoin(tokens_line, buff);
		if (to_free)
			free(to_free);
	}
	if (ret == -1)
	{
		errexit("21sh", "read failure");
		return ((void*)-1);
	}
	return (tokens_line);
}

static void	split_line_to_tokens(char *tokens_line, t_list **new_lexems)
{
	char	**new_tokens;
	int		i;
	
	if (!(new_tokens = ft_strsplit(tokens_line, '\n')))
		return ;
	i = -1;
	while (new_tokens[++i])
	{
		ft_node_push_back(new_lexems, new_token(NULL, WORD, new_tokens[i]));
		free(new_tokens[i]);
	}
	free(new_tokens);
}

void		add_subshell_tokens(t_sh *sh, enum e_token type)
{
	t_list	*new_lexems;
	char	*tokens_line;
	char	*trim_line;

	if ((tokens_line = get_new_tokens(sh)) == (void*)-1)
		return ;
	new_lexems = NULL;
	if (type == BQT)
		split_line_to_tokens(tokens_line, &new_lexems);
	else
	{
		trim_line = ft_strtrim(tokens_line);
		ft_node_push_back(&new_lexems, new_token(NULL, WORD, "echo"));
		ft_node_push_back(&new_lexems, new_token(NULL, WORD, trim_line));
		free(trim_line);
	}
	free(tokens_line);
	if (new_lexems)
		ft_add_list(sh->lexer->lexems, new_lexems);
}
