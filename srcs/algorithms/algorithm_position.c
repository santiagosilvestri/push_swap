/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   algorithm_position.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 14:36:59 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/10 14:37:01 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	get_min_position(t_stack *stack)
{
	t_node	*current;
	int		min_index;
	int		min_pos;
	int		pos;

	if (!stack || stack->size == 0)
		return (-1);
	current = stack->head;
	min_index = current->index;
	min_pos = 0;
	pos = 0;
	while (current)
	{
		if (current->index < min_index)
		{
			min_index = current->index;
			min_pos = pos;
		}
		current = current->next;
		pos++;
	}
	return (min_pos);
}

static int	get_max_position(t_stack *stack)
{
	t_node	*current;
	int		max_index;
	int		max_pos;
	int		pos;

	if (!stack || stack->size == 0)
		return (-1);
	current = stack->head;
	max_index = current->index;
	max_pos = 0;
	pos = 0;
	while (current)
	{
		if (current->index > max_index)
		{
			max_index = current->index;
			max_pos = pos;
		}
		current = current->next;
		pos++;
	}
	return (max_pos);
}

int	move_min_to_top_a(t_stack *a, t_ops_count *ops)
{
	int	pos;
	int	count;

	pos = get_min_position(a);
	if (pos == -1)
		return (0);
	count = 0;
	if (pos <= a->size / 2)
	{
		while (pos-- > 0)
			count += ra(a, ops);
	}
	else
	{
		while (pos++ < a->size)
			count += rra(a, ops);
	}
	return (count);
}

int	move_max_to_top_b(t_stack *b, t_ops_count *ops)
{
	int	pos;
	int	count;

	pos = get_max_position(b);
	if (pos == -1)
		return (0);
	count = 0;
	if (pos <= b->size / 2)
	{
		while (pos-- > 0)
			count += rb(b, ops);
	}
	else
	{
		while (pos++ < b->size)
			count += rrb(b, ops);
	}
	return (count);
}
