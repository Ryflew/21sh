#include "ascii.h"

int	ft_isblank(int c)
{
	if ((c > 8 && c < 14) || c == 32)
		return (1);
	return (0);
}
