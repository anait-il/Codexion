#include "codexion.h"

int	main(int ac, char *av[])
{
	int	state;

	state = parsing(ac, av);
	if (state == 1)
		return (1);
}
