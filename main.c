#include "codexion.h"

int	main(int ac, char *av[])
{
	int		state;
	t_args	data;
	t_coder	coders[data.number_of_coders];

	state = parsing(ac, av, &data);
	if (state == 1)
		return (1);
	setup_coders(data, &coders);
}
