/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark_verify.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 10:08:56 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/13 10:08:57 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BENCHMARK_VERIFY_H
# define BENCHMARK_VERIFY_H

# include <sys/types.h>

int	verify_push_swap_output(int fd, pid_t pid, int *values, int size,
		int timeout_seconds);

#endif
