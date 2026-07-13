*This project was created as part of the 42 curriculum by `sasilves` and `jzelada-`.*

# Push Swap

`push_swap` sorts a sequence of unique integers using two stacks and a restricted instruction set. Instead of printing the sorted values, it prints the operations required to leave stack `a` sorted in ascending order and stack `b` empty.

This repository contains four selectable sorting strategies and an independent terminal benchmark for comparing their operation counts.

> **Authorship:** the sorting algorithms, adaptive strategy and graphical benchmark described below were designed and implemented by `sasilves`.

## Contents

- [Overview](#overview)
- [Build and usage](#build-and-usage)
- [Program architecture](#program-architecture)
- [Available operations](#available-operations)
- [Sorting algorithms](#sorting-algorithms)
- [Project structure](#project-structure)
- [Internal bench mode](#internal-bench-mode)
- [Graphical benchmark](#graphical-benchmark)
- [Complexity](#complexity)
- [Contributions](#contributions)
- [Resources](#resources)
- [AI usage](#ai-usage)

## Overview

The program receives all values in stack `a`; stack `b` starts empty. A valid execution must:

- use only the eleven allowed operations;
- finish with `a` sorted;
- finish with `b` empty;
- print one operation per line.

| Strategy | Main idea | Best use |
|---|---|---|
| `simple` | Repeated minimum extraction | Small or nearly ordered inputs |
| `medium` | Dynamic index window | Practical performance around 100–500 elements |
| `complex` | Binary LSD radix sort | Predictable scaling on large inputs |
| `adaptive` | Select from inversion percentage | Automatic input-sensitive behavior |

The default strategy is `adaptive`.

## Build and usage

### Compile

```bash
make
```

Build the program and the graphical benchmark:

```bash
make benchmark
```

Other rules:

```bash
make clean
make fclean
make re
make rebenchmark
```

### Run `push_swap`

```bash
./push_swap [--simple | --medium | --complex | --adaptive] [--bench] <integers>
```

Examples:

```bash
./push_swap 4 2 7 1 3
./push_swap --simple 3 2 1
./push_swap --medium "8 2 7 4 1 6 3 5"
./push_swap --complex 9 5 2 8 1 6
./push_swap --adaptive --bench 12 -4 7 0 99 3
```

The parser accepts separate arguments, quoted strings and combinations of both:

```bash
./push_swap 4 "2 7" 1 3
```

Count operations from the shell:

```bash
ARG="4 2 7 1 3"
./push_swap $ARG | wc -l
```

### Run the benchmark

```bash
./ps_benchmark
```

Recommended official-size comparison:

```bash
./ps_benchmark -m 500 -r 10 -a all --log -s 42
```

## Program architecture

```text
Arguments
   │
   ▼
Parse flags and validate integers
   │
   ▼
Build stack a as a doubly linked list
   │
   ▼
Normalize values into indexes 0 ... n - 1
   │
   ▼
Create empty stack b
   │
   ▼
Measure disorder when required
   │
   ▼
Run the selected algorithm
   │
   ▼
Print operations and free memory
```

### Input validation

The program rejects:

- non-numeric tokens;
- isolated `+` or `-` signs;
- values outside the signed 32-bit range;
- duplicate integers;
- invalid or repeated strategy flags;
- allocation failures.

Invalid input prints `Error` to standard error. An empty invocation exits without output.

### Stack representation

The main program uses doubly linked lists:

```c
typedef struct s_node
{
    int             value;
    int             index;
    struct s_node   *prev;
    struct s_node   *next;
}   t_node;

typedef struct s_stack
{
    t_node  *head;
    t_node  *tail;
    int     size;
}   t_stack;
```

Each node stores the original integer and its normalized rank. Keeping both `head` and `tail` simplifies forward and reverse rotations.

### Index normalization

Values are converted into ranks before sorting:

```text
Values:   42  -5  18  90
Indexes:   2   0   1   3
```

The relative order is preserved, but every algorithm can work with the compact range `0 ... n - 1`. This is especially important for radix sort because it avoids sign and magnitude issues.

## Available operations

| Operation | Effect |
|---|---|
| `sa` | Swap the first two elements of `a` |
| `sb` | Swap the first two elements of `b` |
| `ss` | Execute `sa` and `sb` as one operation |
| `pa` | Push the first element of `b` onto `a` |
| `pb` | Push the first element of `a` onto `b` |
| `ra` | Move the first element of `a` to the bottom |
| `rb` | Move the first element of `b` to the bottom |
| `rr` | Execute `ra` and `rb` as one operation |
| `rra` | Move the last element of `a` to the top |
| `rrb` | Move the last element of `b` to the top |
| `rrr` | Execute `rra` and `rrb` as one operation |

An instruction is printed only when it can modify the relevant stack. Internal bench mode updates the corresponding counter through the same operation call.

## Sorting algorithms

> **All four strategies were implemented by `sasilves`.**

### Simple

```bash
./push_swap --simple <integers>
```

The simple strategy combines optimized cases for up to five elements with repeated minimum extraction.

For larger inputs:

```text
while more than one element remains in a:
    find the minimum index
    rotate it to the top through the shortest direction
    pb

while b is not empty:
    pa
```

The positioning helper compares the minimum's distance from the top and bottom before choosing `ra` or `rra`.

**Strengths:** easy to verify, very effective for small inputs.  
**Limitation:** repeated searches and rotations grow quadratically.

```text
Expected complexity: O(n²)
```

### Medium

```bash
./push_swap --medium <integers>
```

The medium strategy uses a dynamic sliding window over normalized indexes. Its initial range is approximately:

```text
range = ceil(sqrt(n)) × 1.4
```

The variable `pushed` tracks how many values have already moved to `b`. Because the accepted upper bound is `pushed + range`, the window advances automatically.

```text
if index <= pushed:
    pb
    rb
    pushed++
else if index <= pushed + range:
    pb
    pushed++
else:
    ra
```

Low indexes are moved deeper into `b`, while values from the active window stay closer to its top. Once `a` is empty, the algorithm repeatedly moves the maximum in `b` to the top and restores it with `pa`.

**Strengths:** good practical operation counts around the official sizes; reacts to input order.  
**Limitation:** its performance depends on the empirical range factor and permutation structure.

```text
Approximate practical behavior: O(n√n)
```

This describes the intended strategy profile, not a formal bound for every permutation.

### Complex

```bash
./push_swap --complex <integers>
```

The complex strategy implements binary Least Significant Digit radix sort over normalized indexes.

For each bit, from least significant to most significant:

```text
if the current bit is 1:
    ra
else:
    pb
```

After the current pass, every value in `b` returns to `a` with `pa`. The bit test is:

```c
(index >> bit) & 1
```

The number of passes is the number of bits required to represent `n - 1`.

**Strengths:** predictable operation pattern and reliable scaling.  
**Limitation:** it can print more operations than medium around some project sizes.

```text
Expected complexity: O(n log n)
```

### Adaptive

```bash
./push_swap --adaptive <integers>
```

Adaptive is the default strategy. It measures the percentage of inverted pairs, where `i < j` is an inversion when `value[i] > value[j]`.

```text
disorder = inversions / (n × (n - 1) / 2) × 100
```

Inversions are counted with a merge-sort-based method in `O(n log n)` time.

```text
disorder < 20%  → simple
disorder < 50%  → medium
disorder >= 50% → complex
```

When `--bench` is active, the selected route is reported as `adaptive-simple`, `adaptive-medium` or `adaptive-complex`.

Adaptive mode is useful for experimenting with strategy selection, but inversion percentage cannot describe every structural property of a permutation. Its quality therefore depends on the chosen thresholds.

### Comparison

| Strategy | Input sensitivity | Main advantage | Main trade-off |
|---|---|---|---|
| Simple | High | Excellent small-input behavior | Quadratic growth |
| Medium | High | Low counts around 100–500 | Heuristic range factor |
| Complex | Low | Stable large-input scaling | Higher counts at some medium sizes |
| Adaptive | High | Automatic selection | Threshold-dependent |

## Project structure

```text
push_swap/
├── Makefile
├── README.md
├── includes/
│   ├── algorithms.h
│   ├── bench.h
│   ├── operations.h
│   ├── push_swap.h
│   └── stack.h
├── libft/
├── srcs/
│   ├── main/
│   ├── parsing/
│   ├── operations/
│   ├── algorithms/
│   └── utils/
│       ├── bench/
│       ├── disorder/
│       └── stack/
└── tools/
    └── benchmark/
```

### Main modules

| Path | Responsibility |
|---|---|
| `srcs/main/` | Program initialization, flag selection and cleanup |
| `srcs/parsing/` | Argument splitting, integer validation and duplicate detection |
| `srcs/operations/` | All push, swap, rotate and reverse-rotate operations |
| `srcs/algorithms/` | Positioning helpers and the four sorting strategies |
| `srcs/utils/stack/` | Stack creation, indexing and memory release |
| `srcs/utils/disorder/` | Merge-sort inversion counting |
| `srcs/utils/bench/` | Metrics for one execution |
| `libft/` | Supporting C utility functions |

### Key algorithm files

| File | Purpose |
|---|---|
| `algorithm_position.c` | Moves minima or maxima through the shortest rotation |
| `algorithm_utils.c` | Shared sorted-state helpers |
| `sort_simple.c` | Small cases and selection-based sorting |
| `sort_medium.c` | Dynamic-window sorting |
| `sort_complex.c` | Binary radix sorting |
| `sort_adaptive.c` | Disorder-based strategy selection |

### Benchmark files

> **The benchmark architecture and implementation were developed by `sasilves`.**

| File | Purpose |
|---|---|
| `benchmark.c` | Coordinates sizes, runs, algorithms and statistics |
| `benchmark_args.c` | Parses options and rejects unsafe workloads |
| `benchmark_exec.c` | Launches `./push_swap` and captures its output |
| `benchmark_verify.c` | Validates and applies every printed instruction |
| `benchmark_utils.c` | Input generation, RNG and disorder utilities |
| `benchmark_progress.c` | Progress display |
| `benchmark_terminal.c` | Chart, summaries, routing report and rating |
| `benchmark.h` | Shared structures, limits and declarations |
| `benchmark_verify.h` | Verification interface |

## Internal bench mode

`--bench` analyzes one specific `push_swap` execution:

```bash
./push_swap --adaptive --bench 4 2 7 1 3
```

Operations remain on standard output; metrics are written to standard error:

```text
[bench] disorder: 50.00%
[bench] strategy: adaptive-complex (O(n*log(n)))
[bench] total ops: ...
[bench] sa: ...
[bench] pb: ...
...
```

It reports disorder, selected strategy, complexity label, total operations and counts for all eleven instructions. Use it for a single input; use `ps_benchmark` for repeated comparisons.

## Graphical benchmark

> **Designed and implemented by `sasilves`.**

`ps_benchmark` launches the real `./push_swap` executable. It does not link directly to the sorting functions.

```text
Generate input
   │
   ├── run selected algorithms with the same values
   ├── validate every instruction and final stack state
   ├── collect operation counts
   ├── calculate statistics
   └── draw the terminal graph
```

The tool is identified as:

```text
sasilves | Push Swap Graph Benchmark
```

### Fair comparison

For every size and run, one input is generated and reused unchanged for every selected algorithm. This avoids comparing one strategy on an easier permutation than another.

Only outputs that pass the internal verifier are included in the results.

### Input modes

#### Random permutations

By default, the benchmark applies Fisher-Yates shuffling to `0 ... n - 1`. A local `xorshift32` generator makes the seed reproducible across supported platforms.

```bash
./ps_benchmark -m 500 -r 10 -s 42
```

#### Controlled disorder

```bash
-d P
--disorder P
```

`P` ranges from `0` to `100` and represents inversion percentage.

```text
0%   → ordered
100% → reverse ordered
50%  → approximately half of all pairs inverted
```

The generator creates a randomized inversion sequence with the requested representable inversion count and decodes it through a Fenwick tree.

```bash
./ps_benchmark -m 500 -r 10 -d 30 -s 42
```

Two permutations with the same inversion percentage may still have different local patterns and rotation costs.

### Correctness verification

The benchmark has its own private verifier and does not require a separate `checker` executable.

For every child process it:

1. reads standard output through a pipe;
2. accepts only valid operation names;
3. applies them to simulated stacks;
4. counts one operation per valid line;
5. checks that `a` is sorted and `b` is empty;
6. validates the child exit status;
7. enforces timeout and output limits.

It rejects malformed instructions, incorrect final states, abnormal termination, excessive output and timeouts. This prevents an incorrect implementation from receiving an artificially low operation count.

### Statistics and graph

For each strategy and size, the benchmark reports:

| Metric | Meaning |
|---|---|
| Average | Mean operation count |
| Best | Lowest observed count |
| Worst | Highest observed count |
| Standard deviation | Variation between runs |

It also stores average, minimum and maximum measured disorder.

The terminal graph plots:

- X-axis: number of elements;
- Y-axis: average operation count.

Linear scale is the default. Logarithmic scale uses `log10(operations + 1)`:

```bash
./ps_benchmark --log
```

Log scale is especially useful when `simple` is included because its larger values can compress the other bars on a linear graph.

### Adaptive routing

When adaptive is selected, the maximum-size summary reports how many runs selected each internal route:

```text
Adaptive routing at 500 elements: simple 0, medium 4, complex 6
```

### Performance rating

Ratings appear only when `--max` is exactly `100` or `500`.

| Size | Excellent | Good | Minimum pass | Below minimum |
|---:|---:|---:|---:|---:|
| 100 | `< 700` | `< 1500` | `< 2000` | `>= 2000` |
| 500 | `< 5500` | `< 8000` | `< 12000` | `>= 12000` |

The rating uses the average at the maximum size. Best, worst and deviation remain visible. At least ten runs are recommended:

```bash
./ps_benchmark -m 500 -r 10 -s 42
```

A warning is shown when official thresholds are applied to controlled-disorder inputs.

### Options

```text
-m, --max N          Maximum input size, 2-50000
-p, --samples N      Number of measured sizes, 1-100
-r, --runs N         Runs averaged per size, 1-100
-a, --algorithms L   all or comma-separated algorithm names
-d, --disorder P     Target inversion percentage, 0-100
-l, --log            Use logarithmic Y-axis scale
-H, --height N       Chart height, 8-40
-t, --timeout N      Seconds allowed per execution, 1-600
-s, --seed N         Reproducible unsigned seed
-h, --help           Display help
```

Defaults:

```text
max=500, samples=10, runs=3, algorithms=all,
random input, linear scale, height=18, timeout=60 seconds
```

Algorithm lists are comma-separated without spaces:

```bash
-a simple,medium
-a medium,complex,adaptive
-a all
```

### Examples

```bash
# Default comparison
./ps_benchmark

# Stable official-size comparison
./ps_benchmark -m 500 -p 10 -r 10 -a all --log -s 42

# Medium versus radix
./ps_benchmark -m 500 -p 15 -r 20 -a medium,complex -s 42

# Adaptive behavior at 10% disorder
./ps_benchmark -m 500 -r 10 -a adaptive -d 10 -s 42

# Compare medium with adaptive at 30% disorder
./ps_benchmark -m 500 -r 10 -a medium,adaptive -d 30 -s 42

# Single-size experiment
./ps_benchmark -m 100 -p 1 -r 20 -a all --log -s 42
```

### Safety limits

| Limit | Value |
|---|---:|
| Maximum input size | `50000` |
| Maximum samples | `100` |
| Maximum runs | `100` |
| Maximum child executions | `5000` |
| Maximum combined workload | `10000000` |
| Maximum explicit simple size | `1000` |
| Maximum operations per child | `20000000` |
| Default / maximum timeout | `60 / 600` seconds |

Additional restrictions apply to large workloads. The 50,000-element maximum is a safety cap, not a guarantee that every machine or strategy will complete that size efficiently. Very large inputs are also subject to the operating system's argument-size limit.

### Interpreting the results

The benchmark measures printed operations, not CPU time. It is useful for empirical comparisons, but a terminal graph does not prove theoretical complexity.

Keep in mind:

- all algorithms receive the same input within a run;
- different runs normally receive different permutations;
- radix counts are comparatively stable for a fixed size;
- simple and medium are more input-sensitive;
- inversion percentage does not fully describe permutation structure;
- averages should be read together with best, worst and deviation;
- ratings are most meaningful with random inputs and enough runs.

For reproducible results, record the complete command and seed.

## Complexity

| Component | Expected complexity | Notes |
|---|---:|---|
| Duplicate detection | `O(n²)` | New values are compared with existing nodes |
| Index assignment | `O(n²)` | Counts smaller values for each node |
| Disorder measurement | `O(n log n)` | Merge-sort inversion counting |
| Simple | `O(n²)` | Repeated minimum search and rotations |
| Medium | Approximately `O(n√n)` | Dynamic-window behavior |
| Complex | `O(n log n)` | One complete scan per binary bit |
| Controlled generation | `O(n log n)` | Inversion sequence and Fenwick decoding |
| Output verification | `O(n + operations)` | Initializes stacks and applies instructions |

Actual operation counts also depend on rotations, input structure and implementation constants.

## Technical decisions

- Doubly linked lists keep direct access to both stack ends.
- Values are normalized before sorting.
- All strategies share the same operation layer and counters.
- Small cases are delegated to simple sorting.
- Positioning helpers choose the shortest rotation direction.
- Adaptive mode uses merge-sort inversion counting.
- Explicit strategies skip disorder measurement unless `--bench` requires it.
- The benchmark executes the real binary and validates its output before counting it.
- Controlled inputs use randomized inversion sequences instead of repeated swaps.
- Linear and logarithmic charts keep `simple` in the same comparison.
- Ratings are intentionally restricted to maximum sizes 100 and 500.

## Contributions

| Login | Main contributions |
|---|---|
| `sasilves` | Stack operations and positioning utilities; simple, medium, complex and adaptive algorithms; optimized disorder measurement; complete graphical benchmark; internal verifier; controlled input generation; terminal graph; benchmark protections; testing and documentation |
| `jzelada-` | Parsing support; flag modularization; internal bench metrics; header organization; libft integration; Makefile support and testing |

### Work by `sasilves` highlighted here

- all four sorting strategies;
- adaptive thresholds and routing;
- minimum/maximum positioning logic;
- the complete `ps_benchmark` tool;
- same-input comparison methodology;
- deterministic and controlled input generation;
- internal output verification;
- statistics, graph rendering and logarithmic scale;
- performance ratings and safety limits.

## Resources

- Donald E. Knuth, *The Art of Computer Programming, Volume 3: Sorting and Searching*.
- [Radix sort](https://en.wikipedia.org/wiki/Radix_sort)
- [Fenwick tree](https://cp-algorithms.com/data_structures/fenwick.html)
- [Counting inversions with merge sort](https://www.geeksforgeeks.org/dsa/inversion-count-in-array-using-merge-sort/)
- [Big-O Cheat Sheet](https://www.bigocheatsheet.com/)
- [push_swap visualizer](https://github.com/o-reo/push_swap_visualizer)

## AI usage

AI was used as a development assistant for code review, benchmark refactoring, test planning and documentation editing. Every suggestion and generated change was reviewed, understood and tested before integration.
