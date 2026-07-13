/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_swap.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/10 14:07:15 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/10 14:07:17 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PUSH_SWAP_H
# define PUSH_SWAP_H

# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include "../libft/libft.h"

# include "stack.h"
# include "bench.h"
# include "operations.h"
# include "algorithms.h"

# define FLAG_SIMPLE	1
# define FLAG_MEDIUM	2
# define FLAG_COMPLEX	3
# define FLAG_ADAPTIVE	4

t_stack		*parse_args(int argc, char **argv, int *arg_start);
int			parse_single_arg(t_stack *stack, char *arg);
int			parse_string_arg(t_stack *stack, char *arg);
double		compute_disorder(t_stack *a);
void		print_bench(t_bench *bench);
int			parse_flags(int argc, char **argv, int *strategy, int *bench);

#endif
