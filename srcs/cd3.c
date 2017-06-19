#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "21sh.h"

char	*get_with_tilde(char *path, t_env *env)
{
	char	*home;
	char	*tmp;
	int		i;

	i = -1;
	if (!(home = find_env(env, "HOME")))
		return (ft_strdup(path));
	while (*home && path[++i] && (path[i] == *home))
		++home;
	if (*home)
		return (ft_strdup(path));
	if (!path[i + 1])
		return (ft_strdup("~"));
	home = ft_strsub(path + i + 1, 0, ft_strlen(path) - i - 1);
	tmp = ft_strjoin("~", home);
	free(home);
	return (tmp);
}

void	cd_tilde(char *str, t_env *env, t_sh *shell, t_cd *opt)
{
	char	*tmp;
	char	*tmp2;

	if (!(tmp = find_env(env, "HOME")))
	{
		errexit("cd", "HOME not set.");
		return ;
	}
	if (!str)
		tmp2 = ft_strdup(tmp);
	else
		tmp2 = ft_strjoin(tmp, str + 1);
	cd_current_dir(tmp2, env, shell, opt);
	free(tmp2);
}

void	print_cd_error(char *tmp, char *path)
{
	t_stat	file;

	if (!lstat(tmp, &file) && S_ISREG(file.st_mode))
		errexit(path, "Not a directory.");
	else if (access(tmp, F_OK) == -1)
		errexit("cd", "No such file or directory.");
	else
		errexit(tmp, "Permission denied.");
}

void	init_setenv(char ***av, t_env **env, char *tmp)
{
	if (!(*av = (char**)malloc(sizeof(char*) * 4)))
		exit(EXIT_FAILURE);
	(*av)[0] = ft_strdup("setenv");
	(*av)[1] = ft_strdup("OLDPWD");
	(*av)[2] = ft_strdup(tmp);
	(*av)[3] = NULL;
	set_env(*av, env);
	ft_strdelpp(av);
}
