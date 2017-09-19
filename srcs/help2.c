#include "tosh.h"

void	help_cd(void)
{
	first_step("cd");
	ft_putendl("\t- cd [-L | -P] [directory]\n");
	ft_putendl("\t-L : Handle the operand dot-dot logically.");
	ft_putendl("\t-P : Handle the operand dot-dot physically.");
	ft_putendl("\tdirectory : Directory absolute or relative path.\n");
	ft_putstr(C_RESET);
}

void	help_echo(void)
{
	first_step("echo");
	ft_putendl("\t- echo [string ...]\n");
	do_termcap("us");
	ft_putendl("\tThe following operands are supported:\n");
	ft_putstr(C_RESET);
	ft_putstr(C_ITA);
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

void	help_env(void)
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

void	help_hash(void)
{
	first_step("hash");
	ft_putendl("\t- hash [-r | -p name path | -d name]\n");
	ft_putendl("\t-r : Clean the entire hashtab.");
	ft_putendl("\t-p : Add [name path] to the hashtab.");
	ft_putendl("\t-d : Delete [name] from the hashtab.\n");
	ft_putstr(C_RESET);
}

void	help_setenv(void)
{
	first_step("setenv");
	ft_putendl("\t- setenv name value\n");
	ft_putendl("\tname : Environment variable name.");
	ft_putendl("\tvalue : Environment variable value.\n");
	ft_putstr(C_RESET);
}
