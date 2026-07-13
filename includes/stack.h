/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 14:06:56 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/10 14:07:05 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_H
# define STACK_H

typedef struct s_node
{
	int				value;
	int				index;
	struct s_node	*prev;
	struct s_node	*next;
}					t_node;

typedef struct s_stack
{
	t_node			*head;
	t_node			*tail;
	int				size;
}					t_stack;

t_stack				*create_stack(void);
t_node				*create_node(int value);
void				free_stack(t_stack *stack);
int					assign_indexes(t_stack *stack);

#endif
