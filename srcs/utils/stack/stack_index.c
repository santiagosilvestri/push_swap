/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack_index.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 16:30:02 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/04 16:30:04 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	count_smaller_values(t_stack *stack, t_node *target)
{
	t_node	*current;
	int		count;

	count = 0;
	current = stack->head;
	while (current)
	{
		if (current->value < target->value)
			count++;
		current = current->next;
	}
	return (count);
}

int	assign_indexes(t_stack *stack)
{
	t_node	*current;

	if (!stack)
		return (0);
	current = stack->head;
	while (current)
	{
		current->index = count_smaller_values(stack, current);
		current = current->next;
	}
	return (1);
}
