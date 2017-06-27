#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int ac, char **av)
{
	int fd1 = open(av[1], O_RDWR | O_CREAT, 0777);
	int fd2 = open(av[2], O_RDWR | O_CREAT, 0777);
	char *argv[]={"ls",NULL};
 	char *env[]={"PATH=/usr/local/sbin/:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games",NULL};
	int father;
	int ret;

	if ((father = fork()) == -1)
		exit(-1);
	else if (!father)
	{
		close(1);
		int dup1 = dup(fd1);
		dup2(dup1, fd1);
		close(1);
		int dip = dup(fd2);
		dup2(dip, fd2);
		execve("/bin/ls", argv, env);
		exit(EXIT_SUCCESS);
	}
	else
		waitpid(father, &ret, 0);
	return (0);
}
