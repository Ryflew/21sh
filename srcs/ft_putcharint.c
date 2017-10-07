#include <unistd.h>

int	ft_putcharint(int c)
{
	return (write(1, &c, 1));
}
