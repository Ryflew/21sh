#include "tosh.h"

static void go_hist_opt(t_sh *shell, char **av, char *opt)
{
	if (opt[0]) // -c
	{
		ft_strdelpp(&(shell->history));
		shell->hist_first = 0;
	}
	else if (opt[1] && *av) // -d
		hist_d(shell, ft_atoi(*av)); // supprime de l'historique a l'offset specifie
	else if (opt[2]) // -a
		hist_a(shell, *av); // Ajoute au fichier specifier toutes les lignes taper depuis le lancement du shell
	else if (opt[3]) // -r
		hist_r(shell, *av); // Ajoute le fichier specifier en param a la suite de l'historique
	else if (opt[4]) // -w
		hist_w(shell, *av); // Ajoute dans le fichier specifier l'historique courant (a la place)
	else if (opt[5]) // -s
		add_to_history(shell, *av); // Ajoute l'arg a la fin de l'historique
}

static void history_get_opt(char **av)
{
	char	opt[6];
	int		i;

	ft_bzero(opt, 6);
	++av;
	while (*av && **av == '-')
	{
		i = 1;
		while (**av)
		{
			if (*(*av + i) == 'c')
				opt[0] = 1;
			else if (*(*av + i) == 'd')
				opt[1] = 1;
			else if (*(*av + i) == 'a')
				opt[2] = 1;
			else if (*(*av + i) == 'r')
				opt[3] = 1;
			else if (*(*av + i) == 'w')
				opt[4] = 1;
			else if (*(*av + i) == 's')
				opt[5] = 1;
			else
				break ;
			++i;
		}
		++av;
	}
	go_hist_opt(get_shell(), av, opt);
}

void	treat_history_cmd(t_tree *tree)
{
	if (tree->left)
		treat_history_cmd(tree->left);
	if (tree->right)
		treat_history_cmd(tree->right);
	if (tree->cmds && !ft_strcmp(tree->cmds[0], "history"))
		history_get_opt(tree->cmds);
}