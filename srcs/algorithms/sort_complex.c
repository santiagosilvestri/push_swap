/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_complex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 16:19:14 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/10 12:16:32 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	get_max_bits(t_stack *stack)
{
	int	max_index;
	int	bits;

	if (!stack || stack->size <= 1)
		return (0);
	max_index = stack->size - 1;
	bits = 0;
	while (max_index > 0)
	{
		max_index = max_index >> 1;
		bits++;
	}
	return (bits);
}

static int	radix_bit_pass(t_stack *a, t_stack *b, int bit,
		t_ops_count *ops)
{
	int	count;
	int	size;
	int	i;

	count = 0;
	size = a->size;
	i = 0;
	while (i < size)
	{
		if ((a->head->index >> bit) & 1)
			count += ra(a, ops);
		else
			count += pb(a, b, ops);
		i++;
	}
	while (b->size > 0)
		count += pa(a, b, ops);
	return (count);
}

int	sort_complex(t_stack *a, t_stack *b, t_ops_count *ops)
{
	int	count;
	int	bits;
	int	bit;

	if (!a || !b || is_sorted(a))
		return (0);
	if (a->size <= 5)
		return (sort_simple(a, b, ops));
	count = 0;
	bits = get_max_bits(a);
	bit = 0;
	while (bit < bits)
	{
		count += radix_bit_pass(a, b, bit, ops);
		bit++;
	}
	return (count);
}
