/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jzelada- <jzelada-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/11 13:54:00 by jzelada-          #+#    #+#             */
/*   Updated: 2026/07/11 13:54:00 by jzelada-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	set_strategy(char *arg, int *strategy, int *strategy_set)
{
	if (ft_strncmp(arg, "--simple", 9) == 0)
		*strategy = FLAG_SIMPLE;
	else if (ft_strncmp(arg, "--medium", 9) == 0)
		*strategy = FLAG_MEDIUM;
	else if (ft_strncmp(arg, "--complex", 10) == 0)
		*strategy = FLAG_COMPLEX;
	else if (ft_strncmp(arg, "--adaptive", 11) == 0)
		*strategy = FLAG_ADAPTIVE;
	else
		return (0);
	if (*strategy_set)
		return (-1);
	*strategy_set = 1;
	return (1);
}

static int	set_flag(char *arg, int *strategy, int *bench, int *strategy_set)
{
	int	ret;

	if (ft_strncmp(arg, "--bench", 8) == 0)
	{
		*bench = 1;
		return (1);
	}
	ret = set_strategy(arg, strategy, strategy_set);
	if (ret == -1)
		return (0);
	return (ret);
}

int	parse_flags(int argc, char **argv, int *strategy, int *bench)
{
	int	i;
	int	strategy_set;

	*strategy = FLAG_ADAPTIVE;
	*bench = 0;
	strategy_set = 0;
	i = 1;
	while (i < argc)
	{
		if (argv[i][0] != '-' || argv[i][1] != '-')
			break ;
		if (!set_flag(argv[i], strategy, bench, &strategy_set))
			return (-1);
		i++;
	}
	return (i);
}
