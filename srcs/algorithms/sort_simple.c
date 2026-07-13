/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 16:20:46 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/10 15:05:33 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	simple_sort_two(t_stack *a, t_ops_count *ops)
{
	if (a->head->index > a->head->next->index)
		return (sa(a, ops));
	return (0);
}

static int	simple_sort_three(t_stack *a, t_ops_count *ops)
{
	int	count;
	int	first;
	int	second;
	int	third;

	count = 0;
	first = a->head->index;
	second = a->head->next->index;
	third = a->head->next->next->index;
	if (first > second && first > third)
		count += ra(a, ops);
	else if (second > first && second > third)
		count += rra(a, ops);
	if (a->head->index > a->head->next->index)
		count += sa(a, ops);
	return (count);
}

static int	simple_sort_five(t_stack *a, t_stack *b, t_ops_count *ops)
{
	int	count;

	count = 0;
	while (a->size > 3)
	{
		count += move_min_to_top_a(a, ops);
		count += pb(a, b, ops);
	}
	count += simple_sort_three(a, ops);
	while (b->size > 0)
		count += pa(a, b, ops);
	return (count);
}

static int	sort_by_min_selection(t_stack *a, t_stack *b, t_ops_count *ops)
{
	int	count;

	count = 0;
	while (a->size > 1)
	{
		count += move_min_to_top_a(a, ops);
		count += pb(a, b, ops);
	}
	while (b->size > 0)
		count += pa(a, b, ops);
	return (count);
}

int	sort_simple(t_stack *a, t_stack *b, t_ops_count *ops)
{
	if (!a || !b || is_sorted(a))
		return (0);
	if (a->size == 2)
		return (simple_sort_two(a, ops));
	if (a->size == 3)
		return (simple_sort_three(a, ops));
	if (a->size <= 5)
		return (simple_sort_five(a, b, ops));
	return (sort_by_min_selection(a, b, ops));
}
