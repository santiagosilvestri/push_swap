/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   benchmark_terminal.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/13 10:08:12 by sasilves          #+#    #+#             */
/*   Updated: 2026/07/13 10:08:14 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "benchmark.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#define RESET "\033[0m"
#define BOLD "\033[1m"

static int	terminal_width(void)
{
	struct winsize	window;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &window) == 0
		&& window.ws_col > 40)
		return ((int)window.ws_col);
	return (120);
}

static double	scale_value(t_config *config, double value)
{
	if (config->log_scale)
		return (log10(value + 1.0));
	return (value);
}

static double	unscale_value(t_config *config, double value)
{
	if (config->log_scale)
		return (pow(10.0, value) - 1.0);
	return (value);
}

static double	max_chart_value(t_config *config, t_result *results, int count)
{
	double	maximum;
	int		i;
	int		algorithm;

	maximum = 1.0;
	i = 0;
	while (i < count)
	{
		algorithm = 0;
		while (algorithm < ALG_COUNT)
		{
			if ((config->algorithms & (1U << algorithm))
				&& scale_value(config, results[i].avg[algorithm]) > maximum)
				maximum = scale_value(config, results[i].avg[algorithm]);
			algorithm++;
		}
		i++;
	}
	return (maximum);
}

static double	nice_step(double value)
{
	double	power;
	double	fraction;

	power = 1.0;
	while (value >= 10.0)
	{
		value /= 10.0;
		power *= 10.0;
	}
	while (value > 0.0 && value < 1.0)
	{
		value *= 10.0;
		power /= 10.0;
	}
	fraction = 1.0;
	if (value > 5.0)
		fraction = 10.0;
	else if (value > 2.0)
		fraction = 5.0;
	else if (value > 1.0)
		fraction = 2.0;
	return (fraction * power);
}

static double	chart_maximum(t_config *config, t_result *results, int count)
{
	double	maximum;
	double	step;

	maximum = max_chart_value(config, results, count);
	if (config->log_scale)
		return (ceil(maximum * 5.0) / 5.0);
	step = nice_step(maximum / 5.0);
	return (ceil(maximum / step) * step);
}

static int	visible_groups(t_config *config, int count, int *bar_width,
		int *group_width)
{
	int	available;
	int	selected;
	int	groups;

	available = terminal_width() - 12;
	selected = selected_count(config->algorithms);
	*bar_width = 2;
	*group_width = selected * (*bar_width + 1) + 1;
	if (*group_width * count > available)
	{
		*bar_width = 1;
		*group_width = selected * 2 + 1;
	}
	groups = available / *group_width;
	if (groups < 2)
		groups = 2;
	if (groups > count)
		groups = count;
	return (groups);
}

static int	result_index(int shown_index, int shown_count, int total_count)
{
	if (shown_count <= 1)
		return (total_count - 1);
	return ((shown_index * (total_count - 1)) / (shown_count - 1));
}

static int	grid_tick(int row, int height, int *tick)
{
	int	i;
	int	tick_row;

	i = 0;
	while (i < 5)
	{
		tick_row = (i * height) / 5;
		if (row == tick_row)
		{
			*tick = 5 - i;
			return (1);
		}
		i++;
	}
	return (0);
}

static const char	*bar_cell(double value, double maximum, int height,
		int row)
{
	static const char	*blocks[9] = {
		" ", "▁", "▂", "▃", "▄", "▅", "▆", "▇", "█"
	};
	double				scaled;
	double				bottom;
	int					level;

	scaled = value * height / maximum;
	bottom = height - row - 1;
	if (scaled >= bottom + 1.0)
		return (blocks[8]);
	if (scaled <= bottom)
		return (blocks[0]);
	level = (int)ceil((scaled - bottom) * 8.0);
	if (level < 1)
		level = 1;
	if (level > 8)
		level = 8;
	return (blocks[level]);
}

static void	print_bar_cell(const char *cell, int algorithm, int width,
		int color, int grid)
{
	int	i;

	i = 0;
	while (i < width)
	{
		if (strcmp(cell, " ") != 0)
		{
			if (color)
				printf("%s", algorithm_terminal_color(algorithm));
			printf("%s", cell);
			if (color)
				printf("%s", RESET);
		}
		else if (grid)
			printf("─");
		else
			printf(" ");
		i++;
	}
}

static void	print_chart_row(t_config *config, t_result *results, int count,
		int shown, int bar_width, double maximum, int row)
{
	int	grid;
	int	tick;
	int	i;
	int	algorithm;
	int	index;
	int	color;

	grid = grid_tick(row, config->chart_height, &tick);
	if (grid)
		printf("%8.0f ┤", unscale_value(config, maximum * tick / 5.0));
	else
		printf("         │");
	color = isatty(STDOUT_FILENO);
	i = 0;
	while (i < shown)
	{
		index = result_index(i, shown, count);
		algorithm = 0;
		while (algorithm < ALG_COUNT)
		{
			if (config->algorithms & (1U << algorithm))
			{
				print_bar_cell(bar_cell(scale_value(config,
						results[index].avg[algorithm]), maximum,
						config->chart_height, row), algorithm, bar_width,
					color, grid);
				printf(grid ? "─" : " ");
			}
			algorithm++;
		}
		printf(grid ? "─" : " ");
		i++;
	}
	printf("\n");
}

static void	print_centered(const char *text, int width)
{
	int	length;
	int	left;
	int	right;

	length = (int)strlen(text);
	if (length > width)
		length = width;
	left = (width - length) / 2;
	right = width - length - left;
	printf("%*s%.*s%*s", left, "", length, text, right, "");
}

static void	print_x_axis(t_result *results, int count, int shown,
		int group_width)
{
	char	label[32];
	int		i;
	int		index;

	printf("       0 ┼");
	i = 0;
	while (i < shown * group_width)
	{
		printf("─");
		i++;
	}
	printf("\n          ");
	i = 0;
	while (i < shown)
	{
		index = result_index(i, shown, count);
		snprintf(label, sizeof(label), "%d", results[index].n);
		print_centered(label, group_width);
		i++;
	}
	printf("\n%*sNumber of elements (n)\n", 10, "");
}

static void	print_legend(t_config *config)
{
	int	algorithm;
	int	color;

	color = isatty(STDOUT_FILENO);
	printf("Legend: ");
	algorithm = 0;
	while (algorithm < ALG_COUNT)
	{
		if (config->algorithms & (1U << algorithm))
		{
			if (color)
				printf("%s", algorithm_terminal_color(algorithm));
			printf("██");
			if (color)
				printf("%s", RESET);
			printf(" %s  ", algorithm_name(algorithm));
		}
		algorithm++;
	}
	printf("\n");
}

static const char	*performance_rating(double operations, int size)
{
	if (size == 100)
	{
		if (operations < 700.0)
			return ("Excellent");
		if (operations < 1500.0)
			return ("Good");
		if (operations < 2000.0)
			return ("Minimum pass");
	}
	else
	{
		if (operations < 5500.0)
			return ("Excellent");
		if (operations < 8000.0)
			return ("Good");
		if (operations < 12000.0)
			return ("Minimum pass");
	}
	return ("Below minimum");
}

static void	print_maximum_summary(t_config *config, t_result *result)
{
	int	algorithm;

	printf("\nStatistics for %d elements\n\n", result->n);
	printf("%-12s %11s %11s %11s %11s\n", "Algorithm", "Average",
		"Best", "Worst", "Std. dev.");
	algorithm = 0;
	while (algorithm < ALG_COUNT)
	{
		if (config->algorithms & (1U << algorithm))
			printf("%-12s %11.1f %11.0f %11.0f %11.1f\n",
				algorithm_name(algorithm), result->avg[algorithm],
				result->min[algorithm], result->max[algorithm],
				result->stddev[algorithm]);
		algorithm++;
	}
	printf("\nMeasured disorder: average %.2f%%, minimum %.2f%%, "
		"maximum %.2f%%\n", result->disorder_avg, result->disorder_min,
		result->disorder_max);
}

static void	print_adaptive_routes(t_config *config, t_result *result)
{
	if (!(config->algorithms & (1U << ALG_ADAPTIVE)))
		return ;
	printf("Adaptive routing at %d elements: simple %d, medium %d, "
		"complex %d\n", result->n,
		result->adaptive_routes[ROUTE_SIMPLE],
		result->adaptive_routes[ROUTE_MEDIUM],
		result->adaptive_routes[ROUTE_COMPLEX]);
}

static void	print_performance_rating(t_config *config, t_result *result)
{
	int	algorithm;

	if (config->max_n != 100 && config->max_n != 500)
		return ;
	printf("\nPerformance rating for %d elements\n\n", config->max_n);
	printf("%-12s %18s  %s\n", "Algorithm", "Average operations", "Rating");
	algorithm = 0;
	while (algorithm < ALG_COUNT)
	{
		if (config->algorithms & (1U << algorithm))
			printf("%-12s %18.1f  %s\n", algorithm_name(algorithm),
				result->avg[algorithm], performance_rating(
					result->avg[algorithm], config->max_n));
		algorithm++;
	}
	printf("\nRating based on average operations measured by this benchmark.\n");
	if (config->runs < 10)
		printf("Statistical note: use --runs 10 or more for a more stable "
			"rating.\n");
	if (config->fixed_disorder)
		printf("Note: official thresholds are intended for random inputs; "
			"this run used a %d%% disorder target.\n", config->disorder);
}

void	write_terminal_chart(t_config *config, t_result *results, int count)
{
	double	maximum;
	int		bar_width;
	int		group_width;
	int		shown;
	int		row;
	int		color;

	maximum = chart_maximum(config, results, count);
	shown = visible_groups(config, count, &bar_width, &group_width);
	color = isatty(STDOUT_FILENO);
	printf("\n");
	if (color)
		printf("%s", BOLD);
	printf("Algorithm comparison");
	if (color)
		printf("%s", RESET);
	printf("\n%s average operations — %d runs per size — seed %u\n\n",
		config->log_scale ? "Logarithmic" : "Linear", config->runs,
		config->seed);
	row = 0;
	while (row < config->chart_height)
	{
		print_chart_row(config, results, count, shown, bar_width, maximum, row);
		row++;
	}
	print_x_axis(results, count, shown, group_width);
	print_legend(config);
	if (shown < count)
		printf("Note: terminal width shows %d of %d measured sizes.\n",
			shown, count);
	print_maximum_summary(config, &results[count - 1]);
	print_adaptive_routes(config, &results[count - 1]);
	print_performance_rating(config, &results[count - 1]);
}
