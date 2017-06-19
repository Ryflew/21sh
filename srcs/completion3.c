#include "21sh.h"

char	*find_match_binary(t_sh *shell, char *tosearch)
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
