#include "tosh.h"

void	help_history(void)
{
	first_step("history");
	ft_putendl("\t- history");
	ft_putendl("\t- history -c");
	ft_putendl("\t- history -d offset");
	ft_putendl("\t- history [-arw] filename");
	ft_putendl("\t- history -s arg\n");
	ft_putendl("\tWithout options, print the history list with their offset.\n");
	ft_putendl("\t-c : Clear the history list.");
	ft_putendl("\t-d : Delete the history entry at position offset.");
	ft_putendl("\t-a : Append the new history lines to the history file.");
	ft_putendl("\t-r : Read the current history file and append its contents" \
		"to the history list.");
	ft_putendl("\t-w : Write out the current history to the history file.");
	ft_putendl("\t-s : The args are added to the end of the history list as" \
		"a single entry.\n");
	ft_putstr(C_RESET);
}