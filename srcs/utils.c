#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

char	*clear_quot(char *str)
{
	int		i;
	int		count;
	char	*out;

	i = -1;
	count = 0;
	while (str[++i])
		if (str[i] != 34)
			++count;
	if (!(out = (char*)malloc(sizeof(char) * (count + 1))))
		exit(EXIT_FAILURE);
	i = -1;
	count = -1;
	while (str[++i])
		if (str[i] != 34)
			out[++count] = str[i];
	out[++count] = '\0';
	return (out);
}

char	cd_path_validity(char *path)
{
	DIR		*dir;

	if (!(dir = opendir(path)))
		return (0);
	closedir(dir);
	return (1);
}

char	is_binary(char *path)
{
	int		fd;
	char	buff[300];
	char	type;
	int		bytes;

	type = 0;
	fd = open(path, O_RDONLY);
	if ((bytes = read(fd, buff, 299)) > 0)
	{
		while (--bytes)
			if (buff[bytes] == '\0')
				type = 1;
	}
	close(fd);
	return (type);
}
