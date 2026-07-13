/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 14:09:31 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/10 14:09:32 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static void	error_exit(t_stack *a, t_stack *b)
{
	free_stack(a);
	free_stack(b);
	write(2, "Error\n", 6);
	exit(1);
}

static void	run_sort(t_stack *a, t_stack *b, int strategy, t_bench *bench)
{
	if (strategy == FLAG_SIMPLE)
	{
		bench->strategy_name = "simple";
		bench->complexity = "O(n^2)";
		sort_simple(a, b, &bench->ops);
	}
	else if (strategy == FLAG_MEDIUM)
	{
		bench->strategy_name = "medium";
		bench->complexity = "O(n*sqrt(n))";
		sort_medium(a, b, &bench->ops);
	}
	else if (strategy == FLAG_COMPLEX)
	{
		bench->strategy_name = "complex";
		bench->complexity = "O(n*log(n))";
		sort_complex(a, b, &bench->ops);
	}
	else
		sort_adaptive(a, b, bench);
}

static t_stack	*init_stacks(int argc, char **argv, int arg_start, t_stack **b)
{
	t_stack	*a;

	a = parse_args(argc, argv, &arg_start);
	if (!a || a->size == 0)
		error_exit(a, NULL);
	if (!assign_indexes(a))
		error_exit(a, NULL);
	*b = create_stack();
	if (!*b)
		error_exit(a, NULL);
	return (a);
}

int	main(int argc, char **argv)
{
	t_stack	*a;
	t_stack	*b;
	t_bench	bench;
	int		strategy;
	int		arg_start;

	if (argc < 2)
		return (0);
	arg_start = parse_flags(argc, argv, &strategy, &bench.enabled);
	if (arg_start == -1)
		error_exit(NULL, NULL);
	if (arg_start >= argc)
		return (0);
	a = init_stacks(argc, argv, arg_start, &b);
	bench.ops = (t_ops_count){0};
	bench.disorder = 0.0;
	if (strategy == FLAG_ADAPTIVE || bench.enabled)
		bench.disorder = compute_disorder(a);
	run_sort(a, b, strategy, &bench);
	if (bench.enabled)
		print_bench(&bench);
	free_stack(a);
	free_stack(b);
	return (0);
}
