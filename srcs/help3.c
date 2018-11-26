/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/10/10 20:26:26 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/11/15 15:56:07 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tosh.h"

void	help_history(void)
{
	first_step("history");
	ft_putendl("\t- history");
	ft_putendl("\t- history -c");
	ft_putendl("\t- history -d offset");
	ft_putendl("\t- history [-arw] filename");
	ft_putendl("\t- history -s arg\n");
	ft_putendl("\tWithout options, print the history list with their " \
		"offset.\n");
	ft_putendl("\t-c : Clear the history list.");
	ft_putendl("\t-d : Delete the history entry at position offset.");
	ft_putendl("\t-a : Append the new history lines to the history file.");
	ft_putendl("\t-r : Read the current history file and append its contents" \
		"to the history list.");
	ft_putendl("\t-w : Write out the current history to the history file.");
	ft_putendl("\t-s : The args are added to the end of the history list as" \
		" a single entry.\n");
	ft_putstr(C_RESET);
}

void	help_read(void)
{
	first_step("read");
	ft_putendl("\tread [-ers] [-a array] [-d delim] [-n nchars] [-p prompt]" \
		" [-u fd] [name ...]\n");
	ft_putendl("\t-a array: assign the words read to sequential indices of " \
		"the array.");
	ft_putendl("\t-d delim: continue until the first character of DELIM is " \
		"read, rather than newline.");
	ft_putendl("\t-e: use get_next_line to obtain the line in an " \
		"interactive shell.");
	ft_putendl("\t-n nchars: return after reading NCHARS characters rather " \
		"than waiting for a newline.");
	ft_putendl("\t-p prompt: output the string PROMPT without a trailing " \
		"newline before attempting to read.");
	ft_putendl("\t-r: do not allow backslashes to escape any characters.");
	ft_putendl("\t-s: do not echo input coming from a terminal.");
	ft_putendl("\t-u fd: read from file descriptor FD instead of the standard" \
		" input.\n");
	ft_putstr(C_RESET);
}

void	help_unsetenv(void)
{
	first_step("unsetenv");
	ft_putendl("\t- setenv name ...\n");
	ft_putendl("\tname : Environment variable name.\n");
	ft_putstr(C_RESET);
}

void	help_unset(void)
{
	first_step("unset");
	ft_putendl("\tunset [name ...]\n");
	ft_putstr(C_RESET);
}

void	help_export(void)
{
	first_step("export");
	ft_putendl("\texport [-n] [name[=value] ...] or export -p\n");
	ft_putendl("\t-n: Remove the export property from subsequent NAMEs.");
	ft_putendl("\t-p: Print the export list.\n");
	ft_putstr(C_RESET);
}
