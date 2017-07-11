#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h> 
#include <unistd.h>
#include "21sh.h"

static void	validity_path(char *cmd, t_sh *shell, int i)
{
	DIR			*dir;
	t_dirent	*ent;
	char		*tmp;
	char		**tab;

	tab = ft_strsplit(find_env(shell->env, "PATH"), ':');
	while (tab[++i])
	{
		if (!(dir = opendir(tab[i])))
			continue ;
		while ((ent = readdir(dir)))
		{
			if (!ft_strcmp(ent->d_name, cmd))
			{
				tmp = ft_strstrjoin(tab[i], "/", ent->d_name);
				add_hash_line(cmd, tmp, get_shell());
				free(tmp);
				ft_strdelpp(&tab);
				closedir(dir);
				return ;
			}
		}
		closedir(dir);
	}
	ft_strdelpp(&tab);
}

void 		try_add_hashtab(t_tree *node, t_sh *shell)
{
	if (node->left)
		try_add_hashtab(node->left, shell);
	if (node->right)
	{
		if (node->token->type != CHEVB && node->token->type != DCHEVB && 
			node->token->type != CHEVF && node->token->type != DCHEVF &&
			node->token->type != FRED)
			try_add_hashtab(node->right, shell);
	}
	if (node->cmds && is_builtins(node->cmds))
		validity_path(node->cmds[0], shell, -1);
}
