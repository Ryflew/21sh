#include "tosh.h"

void		first_step(char *name)
{
	ft_putchar('\n');
	ft_putstr(C_RED);
	do_termcap("us");
	ft_putstr("Using of ");
	ft_putstr(name);
	ft_putendl(" command:");
	ft_putstr(C_RESET);
	ft_putstr(C_ITA);
	ft_putchar('\n');
}

static void	help_unsetenv(void)
{
	first_step("unsetenv");
	ft_putendl("\t- setenv name ...\n");
	ft_putendl("\tname : Environment variable name.\n");
	ft_putstr(C_RESET);
}

static void	help_exit(void)
{
	first_step("exit");
	ft_putendl("\t- exit [value]\n");
	ft_putendl("\tvalue : Return value for parent process.\n");
	ft_putstr(C_RESET);
}

static void	help_help(void)
{
	first_step("help");
	ft_putendl("\t- help cd.");
	ft_putendl("\t- help echo.");
	ft_putendl("\t- help env.");
	ft_putendl("\t- help hash.");
	ft_putendl("\t- help history.");
	ft_putendl("\t- help read.");
	ft_putendl("\t- help setenv.");
	ft_putendl("\t- help unsetenv.");
	ft_putendl("\t- help exit.\n");
	ft_putstr(C_RESET);
}

void		help_command(char **cmd) // trop de ligne
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
		else if (!ft_strcmp(cmd[1], "history"))
			help_history();
		else if (!ft_strcmp(cmd[1], "read"))
			help_read();
		else
			errexit("help", "bad builtin name.");
	}
}
