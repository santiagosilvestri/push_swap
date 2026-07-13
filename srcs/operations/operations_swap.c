/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_swap.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 16:28:12 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/04 16:51:51 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	swap(t_stack *stack)
{
	t_node	*first;
	t_node	*second;
	t_node	*third;

	if (!stack || stack->size < 2)
		return (0);
	first = stack->head;
	second = first->next;
	third = second->next;
	second->prev = NULL;
	second->next = first;
	first->prev = second;
	first->next = third;
	if (third)
		third->prev = first;
	else
		stack->tail = first;
	stack->head = second;
	return (1);
}

int	sa(t_stack *a, t_ops_count *ops)
{
	if (swap(a))
	{
		write(1, "sa\n", 3);
		if (ops)
			ops->sa++;
		return (1);
	}
	return (0);
}

int	sb(t_stack *b, t_ops_count *ops)
{
	if (swap(b))
	{
		write(1, "sb\n", 3);
		if (ops)
			ops->sb++;
		return (1);
	}
	return (0);
}

int	ss(t_stack *a, t_stack *b, t_ops_count *ops)
{
	int	did;

	did = 0;
	if (a && a->size >= 2)
		did += swap(a);
	if (b && b->size >= 2)
		did += swap(b);
	if (did)
	{
		write(1, "ss\n", 3);
		if (ops)
			ops->ss++;
		return (1);
	}
	return (0);
}
