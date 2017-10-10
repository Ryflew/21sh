#include "tosh.h"

static char manage_here_doc_bqt(t_sh *sh, t_token *token, t_list **tmp, t_list **begin_lexems)
{
    if (eat(sh, BQT) == -1)
        eat(sh, BQT_C);
    if (!(subshell(sh, BQT)))
        return (0);
    if (eat(sh, EBQT) == -1)
    {
        ft_putendl("parse error: backquote isn't close");
        return (0);
    }
    while (*tmp)
    {
        token = (t_token*)(*tmp)->data;
        if (*begin_lexems == *tmp)
            *begin_lexems = (*tmp)->next;
        if (TYPE == EBQT)
        {
        	ft_pop_node(tmp, (void*)&clear_lexems);
            break;
        }
        ft_pop_node(tmp, (void*)&clear_lexems);
    }
    return (1);
}

static char	is_bqt_in_heredoc(t_sh *sh, t_list **begin_lexems)
{
	t_list	*tmp;
	t_token	*token;
	
	tmp = *begin_lexems;
	while (tmp)
	{
		token = (t_token*)tmp->data;
        if (TYPE == BQT || TYPE == BQT_C)
        {
            sh->current_token = token;
            sh->lexer->lexems = tmp;
            if (!(manage_here_doc_bqt(sh, token, &tmp, begin_lexems)))
                return (0);
        }
        if (tmp)
            NEXT(tmp);
	}
	return (1);
}

static char **lex_parse_heredoc(t_sh *sh, char *heredoc_line)
{
	t_list	*begin_lexems;
	char	**cmds;

	sh->lexer->line = heredoc_line;
	sh->lexer->her = 1;
	get_lexems(sh);
	begin_lexems = sh->lexer->lexems;
	if (!(is_bqt_in_heredoc(sh, &begin_lexems)))
	{
		ft_clear_list(&begin_lexems, (void*)&clear_lexems);
		sh->lexer->her = 0;	
		return ((void*)-1);
	}
	cmds = get_cmds(&begin_lexems, sh);
	ft_clear_list(&begin_lexems, (void*)&clear_lexems);
	sh->lexer->her = 0;
	return (cmds);
}

char        *read_here_doc(char *cmd, char *prompt)
{
	char	*line;
    char	*to_free;
	int		ret;    
	char	*heredoc_line;
    
    line = NULL;
	heredoc_line = NULL;
    ft_putstr(prompt);		
    while ((ret = gnl(0, &line) > 0) && ft_strcmp(line, cmd))
    {
        to_free = heredoc_line;
        heredoc_line = (!to_free) ? ft_strdup(line) : \
        ft_strstrjoin(to_free, "\n", line);
        if (to_free)
            free(to_free);
        free(line);
        ft_putstr(prompt);
    }
    free(line);
    if (ret == -1)
		ft_exiterror("read failure", -1);
    return (heredoc_line);
}

char        manage_here_doc(t_sh *sh, char *heredoc_line, t_tree *node,
                            int *fd_pipe)
{
    char	**cmds;
    int     i;

    if ((cmds = (lex_parse_heredoc(sh, heredoc_line))) == (void*)-1)
      return (0);
    i = -1;
    while (cmds && cmds[++i] && node->left)
        ft_fputendl(cmds[i], fd_pipe[1]);
    ft_strdelpp(&cmds);
    free(heredoc_line);
    return (1);
}
