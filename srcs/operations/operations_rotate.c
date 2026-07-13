/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations_rotate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/04 16:27:52 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/04 16:42:16 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

static int	rotate(t_stack *stack)
{
	t_node	*node;

	if (!stack || stack->size < 2)
		return (0);
	node = stack->head;
	stack->head = stack->head->next;
	stack->head->prev = NULL;
	node->next = NULL;
	node->prev = stack->tail;
	stack->tail->next = node;
	stack->tail = node;
	return (1);
}

int	ra(t_stack *a, t_ops_count *ops)
{
	if (rotate(a))
	{
		write(1, "ra\n", 3);
		if (ops)
			ops->ra++;
		return (1);
	}
	return (0);
}

int	rb(t_stack *b, t_ops_count *ops)
{
	if (rotate(b))
	{
		write(1, "rb\n", 3);
		if (ops)
			ops->rb++;
		return (1);
	}
	return (0);
}

int	rr(t_stack *a, t_stack *b, t_ops_count *ops)
{
	int	did;

	did = 0;
	if (a && a->size >= 2)
		did += rotate(a);
	if (b && b->size >= 2)
		did += rotate(b);
	if (did)
	{
		write(1, "rr\n", 3);
		if (ops)
			ops->rr++;
		return (1);
	}
	return (0);
}
