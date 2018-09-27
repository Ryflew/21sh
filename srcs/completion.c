/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   completion.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/18 18:06:09 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/27 16:37:38 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static char	bin_completion_print(t_sh *shell, char *part, char c)
{
	char	*content;
	char	**tmp;
	char	ret;

	if (!(content = find_env(shell->env, "PATH")))
		return (0);
	tmp = ft_strsplit(content, ':');
	if (!(shell->tab_request && c == 'n'))
		ft_putchar('\n');
	ret = print_in_dir(tmp, NULL, part, c);
	ft_strdelpp(&tmp);
	return (ret);
}

static char	dir_completion_print(char *part, char c)
{
	char	*path;
	char	buff[4097];
	char	*slash;

	if (!(get_shell()->tab_request && c == 'n'))
		ft_putchar('\n');
	if (!part && (path = getcwd(buff, 4097)))
		return (print_in_dir(NULL, path, NULL, c));
	else
	{
		if (!ft_strcmp(part, "."))
			return (print_in_dir(NULL, ".", ".", c));
		else
		{
			if (!(slash = ft_strrchr(part, '/')))
				return (print_in_dir(NULL, ".", part, c));
			else
			{
				ft_strncpy(buff, part, slash - part + 1);
				buff[slash - part + 1] = '\0';
				return (print_in_dir(NULL, buff, !*(slash + 1) ? NULL : \
					slash + 1, c));
			}
		}
	}
}

static void	go_completion3(t_sh *shell, char *part, char is_binary, char c)
{
	int		need_print;
	char	save_cmd[MAX_CMD];
	char	to_add[MAX_CMD];

	to_add[0] = '\0';
	convert_tild(shell, part);
	need_print = is_binary ? get_bin_occur(shell, part, to_add, -1) : \
		get_dir_occur(part, to_add, 0);
	if (to_add[0])
		write_to_prompt(shell, to_add, need_print ? NULL : part);
	else if (need_print)
	{
		ft_strcpy(save_cmd, shell->command);
		if (is_binary && bin_completion_print(shell, part, c))
			return ;
		if (!is_binary && dir_completion_print(part, c))
			return ;
		get_cursor(shell);
		sig_hand(0);
		ft_strcpy(save_cmd + ft_strlen(save_cmd), to_add);
		write_to_prompt(shell, save_cmd, NULL);
	}
	else
		slash_if_dir(shell, part);
}

static void	go_completion2(t_sh *shell, char is_binary, char c)
{
	char	save_cmd[MAX_CMD];

	if (is_binary && !find_env(shell->env, "PATH"))
		return ;
	if (is_binary && bin_completion_print(shell, NULL, c))
		return ;
	if (!is_binary && dir_completion_print(NULL, c))
		return ;
	save_cmd[0] = '\0';
	if (shell->j > -1)
		ft_strcpy(save_cmd, shell->command);
	get_cursor(shell);
	sig_hand(0);
	if (save_cmd[0])
		write_to_prompt(shell, save_cmd, NULL);
}

void		go_completion(t_sh *shell, char c)
{
	char	part[MAX_CMD];
	char	is_binary;

	if (shell->pos.cursor.y != shell->pos.last.y || \
		shell->pos.cursor.x != shell->pos.last.x)
		return ;
	shell->command[shell->j + 1] = '\0';
	get_start_str(shell, part);
	is_binary = is_binary_dir(shell);
	if (!part[0])
		go_completion2(shell, is_binary, c);
	else
		go_completion3(shell, part, is_binary, c);
}
