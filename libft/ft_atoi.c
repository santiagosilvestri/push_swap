/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/01 10:32:58 by sasilves          #+#    #+#             */
/*   Updated: 2026/06/10 12:18:39 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sig;
	int	num;

	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == ' ')
		i++;
	sig = 1;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sig = -sig;
		i++;
	}
	num = 0;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		num = (num * 10) + (nptr[i] - '0');
		i++;
	}
	return (num * sig);
}
