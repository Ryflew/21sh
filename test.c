#include <unistd.h>

struct command
{
	const char **argv;
};

int spawn_proc (int in, int out, struct command *cmd, char **environ)
{
	pid_t pid;

	if ((pid = fork ()) == 0)
	{
		if (in != 0)
		{
			dup2 (in, 0);
			close (in);
		}

		if (out != 1)
		{
			dup2 (out, 1);
			close (out);
		}

		return execve (cmd->argv [0], (char * const *)cmd->argv, environ);
	}

	return pid;
}

int fork_pipes (int n, struct command *cmd, char **environ)
{
	int i;
	pid_t pid;
	int in, fd [2];

	/* The first process should get its input from the original file descriptor 0.  */
	in = 0;

	/* Note the loop bound, we spawn here all, but the last stage of the pipeline.  */
	for (i = 0; i < n - 1; ++i)
	{
		pipe (fd);

		/* f [1] is the write end of the pipe, we carry `in` from the prev iteration.  */
		spawn_proc (in, fd [1], cmd + i, environ);

		/* No need for the write end of the pipe, the child will write here.  */
		close (fd [1]);

		/* Keep the read end of the pipe, the next child will read from there.  */
		in = fd [0];
	}

	/* Last stage of the pipeline - set stdin be the read end of the previous pipe
	 *      and output to the original file descriptor 1. */  
	if (in != 0)
		dup2 (in, 0);

	/* Execute the last stage with the current process. */
	return execve(cmd [i].argv [0], (char * const *)cmd [i].argv, environ);
}

int main (int ac, char **av, char **environ)
{
	//const char *ls[] = { "base64", "/dev/urandom", 0 };
	const char *ls[] = { "/usr/bin/base64", "/dev/urandom", 0 };
	//const char *awk[] = { "head", "-c", "1000", 0 };
	const char *awk[] = { "/usr/bin/head", "-c", "1000", 0 };
	const char *sort[] = { "/usr/bin/grep", "42", 0 };
	const char *uniq[] = { "/usr/bin/wc", "-l", 0 };
	const char *uniq2[] = { "/usr/bin/sed", "-e", "s/1/Yes/g", "-e", "s/0/No/g", 0 };
	struct command cmd [] = { {ls}, {awk}, {sort}, {uniq}, {uniq2} };

	return fork_pipes (5, cmd, environ);
}
