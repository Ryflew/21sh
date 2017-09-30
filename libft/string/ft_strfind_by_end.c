#include "string.h"

int		ft_strfind_by_end(const char *s, int c)
{
    int i;

    i = ft_strlen(s) - 1;
	while (i > 0 && s[i] != c)
		--i;
	return (i);
}