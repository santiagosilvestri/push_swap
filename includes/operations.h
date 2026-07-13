/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operations.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 14:07:29 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/10 14:07:32 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef OPERATIONS_H
# define OPERATIONS_H

# include "bench.h"
# include "stack.h"

int	sa(t_stack *a, t_ops_count *ops);
int	sb(t_stack *b, t_ops_count *ops);
int	ss(t_stack *a, t_stack *b, t_ops_count *ops);

int	pa(t_stack *a, t_stack *b, t_ops_count *ops);
int	pb(t_stack *a, t_stack *b, t_ops_count *ops);

int	ra(t_stack *a, t_ops_count *ops);
int	rb(t_stack *b, t_ops_count *ops);
int	rr(t_stack *a, t_stack *b, t_ops_count *ops);

int	rra(t_stack *a, t_ops_count *ops);
int	rrb(t_stack *b, t_ops_count *ops);
int	rrr(t_stack *a, t_stack *b, t_ops_count *ops);

#endif
