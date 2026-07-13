/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_adaptive.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 16:16:42 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/10 12:20:14 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	set_strategy(t_bench *bench, char *name, char *complexity)
{
	bench->strategy_name = name;
	bench->complexity = complexity;
}

int	sort_adaptive(t_stack *a, t_stack *b, t_bench *bench)
{
	if (!a || !b || !bench)
		return (0);
	if (bench->disorder < ADAPTIVE_SIMPLE_LIMIT)
	{
		set_strategy(bench, "adaptive-simple", "O(n^2)");
		return (sort_simple(a, b, &bench->ops));
	}
	if (bench->disorder < ADAPTIVE_MEDIUM_LIMIT)
	{
		set_strategy(bench, "adaptive-medium", "O(n*sqrt(n))");
		return (sort_medium(a, b, &bench->ops));
	}
	set_strategy(bench, "adaptive-complex", "O(n*log(n))");
	return (sort_complex(a, b, &bench->ops));
}
