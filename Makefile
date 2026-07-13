NAME		= push_swap
BENCHMARK	= ps_benchmark

CC			= cc
CFLAGS		= -Wall -Wextra -Werror
INCLUDES	= -I includes -I libft

LIBFT_DIR	= libft
LIBFT		= $(LIBFT_DIR)/libft.a

SRCS		= srcs/main/main.c \
			  srcs/main/main_utils.c \
			  srcs/parsing/parsing.c \
			  srcs/parsing/parsing_utils.c \
			  srcs/utils/stack/stack.c \
			  srcs/utils/stack/stack_index.c \
			  srcs/utils/disorder/disorder.c \
			  srcs/utils/bench/bench.c \
			  srcs/operations/operations_push.c \
			  srcs/operations/operations_rotate.c \
			  srcs/operations/operations_reverse_rotate.c \
			  srcs/operations/operations_swap.c \
			  srcs/algorithms/algorithm_utils.c \
			  srcs/algorithms/algorithm_position.c \
			  srcs/algorithms/sort_simple.c \
			  srcs/algorithms/sort_medium.c \
			  srcs/algorithms/sort_complex.c \
			  srcs/algorithms/sort_adaptive.c

BENCH_SRCS	= tools/benchmark/benchmark.c \
			  tools/benchmark/benchmark_args.c \
			  tools/benchmark/benchmark_exec.c \
			  tools/benchmark/benchmark_progress.c \
			  tools/benchmark/benchmark_terminal.c \
			  tools/benchmark/benchmark_utils.c \
			  tools/benchmark/benchmark_verify.c

OBJS		= $(SRCS:.c=.o)
BENCH_OBJS	= $(BENCH_SRCS:.c=.o)

all: $(NAME)

benchmark: $(NAME) $(BENCHMARK)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(BENCHMARK): $(BENCH_OBJS)
	$(CC) $(CFLAGS) $(BENCH_OBJS) -lm -o $(BENCHMARK)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(MAKE) -C $(LIBFT_DIR) clean
	rm -f $(OBJS) $(BENCH_OBJS)

fclean: clean
	$(MAKE) -C $(LIBFT_DIR) fclean
	rm -f $(NAME) $(BENCHMARK)

re: fclean all

rebenchmark: fclean benchmark

.PHONY: all benchmark clean fclean re rebenchmark
