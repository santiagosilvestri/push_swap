/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark_progress.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 10:07:59 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/13 10:08:01 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "benchmark.h"
#include <stdio.h>
#include <unistd.h>

#define PROGRESS_WIDTH 32
#define GREEN "\033[32m"
#define RESET "\033[0m"

static void	print_progress_bar(t_progress *progress, int percent)
{
	int	filled;
	int	i;

	filled = percent * PROGRESS_WIDTH / 100;
	printf("\r\033[2KPush Swap Graph Benchmark by sasilves [");
	i = 0;
	while (i < PROGRESS_WIDTH)
	{
		if (i < filled)
			printf("%s█%s", GREEN, RESET);
		else
			printf("░");
		i++;
	}
	printf("] %3d%% (%d/%d)", percent, progress->current,
		progress->total);
	fflush(stdout);
}

static void	progress_update(t_progress *progress)
{
	int	percent;

	if (progress->total <= 0)
		percent = 100;
	else
		percent = progress->current * 100 / progress->total;
	if (percent == progress->last_percent)
		return ;
	progress->last_percent = percent;
	if (progress->interactive)
		print_progress_bar(progress, percent);
}

void	progress_init(t_progress *progress, int total)
{
	progress->current = 0;
	progress->total = total;
	progress->last_percent = -1;
	progress->interactive = isatty(STDOUT_FILENO);
	if (progress->interactive)
		printf("\033[?25l");
	progress_update(progress);
}

void	progress_advance(t_progress *progress)
{
	if (progress->current < progress->total)
		progress->current++;
	progress_update(progress);
}

void	progress_finish(t_progress *progress)
{
	progress->current = progress->total;
	progress_update(progress);
	if (progress->interactive)
		printf("\n\033[?25h");
	else
		printf("Push Swap Graph Benchmark by sasilves: 100%% (%d/%d)\n",
			progress->total, progress->total);
	fflush(stdout);
}
