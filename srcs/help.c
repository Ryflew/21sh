/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:26:15 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/11/15 15:01:54 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

void		first_step(char *name)
{
	ft_putchar('\n');
	ft_putstr(C_RED);
	do_termcap("us");
	ft_putstr("Using of ");
	ft_putstr(name);
	ft_putstr(" command:");
	ft_putstr(C_RESET);
	ft_putstr("\n\n");
	ft_putstr(C_ITA);
}

static void	help_exit(void)
{
	first_step("exit");
	ft_putendl("\t- exit [value]\n");
	ft_putstr("\tvalue : Return value for parent process. \n\t\tIf omitted, ");
	ft_putstr("the exit status is that of the last command executed.\n\n");
	ft_putstr(C_RESET);
}

static void	help_help(void)
{
	first_step("help");
	ft_putendl("\t- help cd.");
	ft_putendl("\t- help echo.");
	ft_putendl("\t- help env.");
	ft_putendl("\t- help export.");
	ft_putendl("\t- help hash.");
	ft_putendl("\t- help history.");
	ft_putendl("\t- help read.");
	ft_putendl("\t- help setenv.");
	ft_putendl("\t- help shellvar.");
	ft_putendl("\t- help unset.");
	ft_putendl("\t- help unsetenv.");
	ft_putendl("\t- help exit.\n");
	ft_putstr(C_RESET);
}

static char	help_command2(char *cmd)
{
	if (!ft_strcmp(cmd, "history"))
		help_history();
	else if (!ft_strcmp(cmd, "read"))
		help_read();
	else if (!ft_strcmp(cmd, "unset"))
		help_unset();
	else if (!ft_strcmp(cmd, "export"))
		help_export();
	else
		return (0);
	return (1);
}

void		help_command(char **cmd)
{
	if (!*(cmd + 1))
		help_help();
	else if (cmd[1])
	{
		if (!ft_strcmp(cmd[1], "exit"))
			help_exit();
		else if (!ft_strcmp(cmd[1], "cd"))
			help_cd();
		else if (!ft_strcmp(cmd[1], "setenv"))
			help_setenv();
		else if (!ft_strcmp(cmd[1], "unsetenv"))
			help_unsetenv();
		else if (!ft_strcmp(cmd[1], "env"))
			help_env();
		else if (!ft_strcmp(cmd[1], "echo"))
			help_echo();
		else if (!ft_strcmp(cmd[1], "hash"))
			help_hash();
		else if (!ft_strcmp(cmd[1], "shellvar"))
			help_shellvar();
		else if (help_command2(cmd[1]))
			;
		else
			errexit("help", "bad builtin name.");
	}
}
