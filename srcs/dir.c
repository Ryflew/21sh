#include "tosh.h"
#include <sys/stat.h>

DIR		*open_dir(char *dir_name, t_token *token)
{
    DIR				*dir;
    char            *dir_path;

    if (TYPE != EXPR || VAL[0] != '/')
        dir_path = ft_strjoin("./", dir_name);
    else
        dir_path = dir_name;
	if (!(dir = opendir(dir_path)))
	{
        if (dir_path[0] == '.')
            free(dir_path);
        ft_putstr("ERROR: opendir failure -> ");
        ft_putendl(dir_name);
		return (NULL);
    }
    if (dir_path[0] == '.')
        free(dir_path);
	return (dir);
}

int		is_dir(char *dir_name, t_token *token)
{
	struct stat statbuf;
    char        *dir_path;
    
    if (TYPE != EXPR || VAL[0] != '/')
        dir_path = ft_strjoin("./", dir_name);
    else
        dir_path = dir_name;
    if (stat(dir_path, &statbuf) != 0)
    {
        if (dir_path[0] == '.')
            free(dir_path);
        return (0);
    }
    if (dir_path[0] == '.')
        free(dir_path);
	return (S_ISDIR(statbuf.st_mode));
}

void    clear_old_expr(t_sh *sh, t_list **lexems, char match)
{
    t_token *token;
    
    while (*lexems)
    {
        token = (t_token*)(*lexems)->data;
        if (TYPE == NONE)
        {
            if (match)
                ft_pop_node(lexems, (void*)&clear_lexems);
            else
                *lexems = (*lexems)->next;
            break;
        }
        if (match)
            ft_pop_node(lexems, (void*)&clear_lexems);
        else
            *lexems = (*lexems)->next;
    }
    if (match && (!*lexems || !(*lexems)->prev))
        sh->lexer->lexems = *lexems;
}