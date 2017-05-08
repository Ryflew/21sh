#include <sys/stat.h>
#include <dirent.h>
#include "21sh.h"

static char	*get_start_str(t_sh *shell)
{
	int		end;
	int		i;

	i = shell->j + 1;
	end = i;
	while (--i > -1 && shell->command[i] != ' ')
		;
	return (ft_strsub(shell->command, i + 1, end - (i + 1)));
}

static char	*check_dir_content(char *part, char *path)
{
	DIR			*dir;
	t_dirent	*ent;

	if (!(dir = opendir(path)))
		return (0);
	while ((ent = readdir(dir)))
	{
		if (!ft_strncmp(ent->d_name, part, ft_strlen(part)))
		{
			closedir(dir);
			return (ft_strdup(ent->d_name));
		}
	}
	closedir(dir);
	return (NULL);
}

static char	*find_match_binary(t_sh *shell, char *tosearch)
{
	int		i;
	char	*content;
	char	*out;
	char	**tmp;

	if (!(content = find_env(shell->env, "PATH")))
			return (NULL);
	tmp = ft_strsplit(content, ':');
	content = tosearch;
	i = -1;
	while (tmp[++i])
	{
		if ((out = check_dir_content(content, tmp[i])))
		{
			ft_strdelpp(&tmp);
			return (out);
		}
	}
	ft_strdelpp(&tmp);
	return (NULL);
}

static void	print_missing(t_sh *shell, char *part, char *all)
{
	int	i;

	i = -1;
	while (part[++i] && all[i] && part[i] == all[i])
		;
	while (all[i])
		add_char(shell->command, &(shell->j), shell, all[i++]);
}

void	go_completion(t_sh *shell)
{
	char	*tmp;
	char	*name;

	if (shell->j == -1)
		return ;
	shell->command[shell->j + 1] = '\0';
	if (ft_strchr(shell->command, ' ') || shell->command[0] == '/')
	{
		// print les dirs current
	}
	else
	{
		tmp = get_start_str(shell);
		name = find_match_binary(shell, tmp);
		if (name)
		{
			print_missing(shell, tmp, name);
			free(name);
		}
		free(tmp);
	}
}
