/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:26:20 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/29 14:42:52 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

static void	help_echo2(void)
{
	ft_putendl("\t\\a : Write an <alert>.");
	ft_putendl("\t\\b : Write a <backspace>.");
	ft_putendl("\t\\c : Suppress the <newline> that otherwise follows the " \
		"final argument in the output.");
	ft_putendl("\t\tAll characters following the \'\\c\' in the arguments " \
		"shall be ignored.");
	ft_putendl("\t\\f : Write a <form-feed>.");
	ft_putendl("\t\\n : Write a <newline>.");
	ft_putendl("\t\\r : Write a <carriage-return>.");
	ft_putendl("\t\\t : Write a <tab>.");
	ft_putendl("\t\\v : Write a <vertical-tab>.");
	ft_putendl("\t\\\\ : Write a backslash character.");
	ft_putendl("\t\\0num : Write an 8-bit value that is the zero, one, two, " \
		"or three-digit octal number num.");
	ft_putendl("\t$? : Write the last process return value.");
	ft_putendl("\t$[env variable name] : Write the selected env variable " \
		"value.\n");
	ft_putstr(C_RESET);
}

void		help_echo(void)
{
	first_step("echo");
	ft_putendl("\t- echo [-neE] [string ...]\n");
	ft_putstr("\t-n : The trailing newline is suppressed.\n");
	ft_putstr("\t-e : Interpretation of the following backslash-escaped");
	ft_putstr(" characters is enabled.\n");
	ft_putstr("\t-E : Disables the interpretation of these escape");
	ft_putstr("character.\n\n");
	do_termcap("us");
	ft_putstr("The following operands are supported:");
	ft_putstr(C_RESET);
	ft_putstr(C_ITA);
	ft_putstr("\n\n");
	help_echo2();
}

void		help_env(void)
{
	first_step("env");
	ft_putendl("\t- env [-i] [-u name] [name=value]... [utility "\
		"[argument...]]\n");
	ft_putendl("\t-i : Clean the entire environment.");
	ft_putendl("\t-u : Delete the selected variable.");
	ft_putendl("\tname : Environment variable name.");
	ft_putendl("\tvalue : Environment variable value.");
	ft_putendl("\tutility : Program name.");
	ft_putendl("\targument : Program parameter.\n");
	ft_putstr(C_RESET);
}

void		help_hash(void)
{
	first_step("hash");
	ft_putendl("\t- hash [-r | -p name path | -d name]\n");
	ft_putendl("\t-r : Clean the entire hashtab.");
	ft_putendl("\t-p : Add [name path] to the hashtab.");
	ft_putendl("\t-d : Delete [name] from the hashtab.\n");
	ft_putstr(C_RESET);
}

void		help_setenv(void)
{
	first_step("setenv");
	ft_putendl("\t- setenv name value\n");
	ft_putendl("\tname : Environment variable name.");
	ft_putendl("\tvalue : Environment variable value.\n");
	ft_putstr(C_RESET);
}
