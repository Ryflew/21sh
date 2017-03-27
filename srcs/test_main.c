#include "21sh.h"

int main(int argc, char **argv)
{
	t_sh		sh;
	t_lexer	lexer;

	if (argc == 2)
	{
		sh.lexer = &lexer;
		sh.lexer->line = argv[1];
		cmds_line(&sh);
	}
	return (0);
}
