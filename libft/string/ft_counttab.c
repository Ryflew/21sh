#include "string.h"

size_t	ft_counttab(char **tabu)
{
	size_t		count;

	count = 0;
	while (tabu[count])
		++count;
	return (count);
}
