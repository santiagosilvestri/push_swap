*This project was created as part of the 42 curriculum by sasilves.*

# Libft

## Description

**Libft** is a custom C library developed as part of the 42 curriculum.
The goal of this project is to recreate a set of standard C library functions, implement additional utility functions, and build a small linked list API.

By coding these functions from scratch, this project helps develop a deeper understanding of memory management, string manipulation, character handling, pointers, dynamic allocation, and basic data structures in C.

The final result is a static library named:

```bash
libft.a
```

This library can be reused in future C projects throughout the 42 cursus.

---

## Project Structure

All source files are located at the root of the repository.

Expected files include:

```bash
Makefile
libft.h
ft_*.c
```

The `libft.h` header contains all function prototypes and the linked list structure definition.

The project is compiled into the static library `libft.a`.

---

## Instructions

To compile the library, run:

```bash
make
```

This will generate:

```bash
libft.a
```

The project is compiled using:

```bash
cc -Wall -Wextra -Werror
```

The Makefile includes the following mandatory rules:

```bash
make        # Builds libft.a
make all    # Builds libft.a
make clean  # Removes object files
make fclean # Removes object files and libft.a
make re     # Rebuilds the library from scratch
```

If bonus functions are included, they can be compiled with:

```bash
make bonus
```

---

## Library Overview

The library is divided into three main parts:

1. Libc functions
2. Additional utility functions
3. Linked list functions

---

## Part 1 - Libc Functions

This section contains reimplementations of standard C library functions.
Each function keeps the same behavior as the original one, but uses the `ft_` prefix.

### Character Checking Functions

```c
int ft_isalpha(int c);
int ft_isdigit(int c);
int ft_isalnum(int c);
int ft_isascii(int c);
int ft_isprint(int c);
```

These functions check whether a character belongs to a specific category, such as alphabetic, numeric, printable, or ASCII.

### String Functions

```c
size_t ft_strlen(const char *s);
size_t ft_strlcpy(char *dst, const char *src, size_t size);
size_t ft_strlcat(char *dst, const char *src, size_t size);
char *ft_strchr(const char *s, int c);
char *ft_strrchr(const char *s, int c);
int ft_strncmp(const char *s1, const char *s2, size_t n);
char *ft_strnstr(const char *big, const char *little, size_t len);
char *ft_strdup(const char *s);
```

These functions handle common string operations such as calculating length, copying, concatenating, searching, comparing, and duplicating strings.

### Memory Functions

```c
void *ft_memset(void *b, int c, size_t len);
void ft_bzero(void *s, size_t n);
void *ft_memcpy(void *dst, const void *src, size_t n);
void *ft_memmove(void *dst, const void *src, size_t len);
void *ft_memchr(const void *s, int c, size_t n);
int ft_memcmp(const void *s1, const void *s2, size_t n);
void *ft_calloc(size_t count, size_t size);
```

These functions manipulate raw memory blocks, compare memory regions, search inside memory, and allocate initialized memory.

### Conversion Functions

```c
int ft_atoi(const char *str);
int ft_toupper(int c);
int ft_tolower(int c);
```

These functions convert strings to integers and handle uppercase/lowercase character conversion.

---

## Part 2 - Additional Functions

This section contains useful functions that are not part of the standard libc, or that behave differently from standard functions.

```c
char *ft_substr(char const *s, unsigned int start, size_t len);
char *ft_strjoin(char const *s1, char const *s2);
char *ft_strtrim(char const *s1, char const *set);
char **ft_split(char const *s, char c);
char *ft_itoa(int n);
char *ft_strmapi(char const *s, char (*f)(unsigned int, char));
void ft_striteri(char *s, void (*f)(unsigned int, char *));
void ft_putchar_fd(char c, int fd);
void ft_putstr_fd(char *s, int fd);
void ft_putendl_fd(char *s, int fd);
void ft_putnbr_fd(int n, int fd);
```

### Purpose of These Functions

- `ft_substr` creates a substring from a given string.
- `ft_strjoin` joins two strings into a new allocated string.
- `ft_strtrim` removes selected characters from the beginning and end of a string.
- `ft_split` splits a string into an array of strings using a delimiter.
- `ft_itoa` converts an integer into a string.
- `ft_strmapi` creates a new string by applying a function to each character.
- `ft_striteri` applies a function to each character of a string in place.
- `ft_putchar_fd` writes a character to a file descriptor.
- `ft_putstr_fd` writes a string to a file descriptor.
- `ft_putendl_fd` writes a string followed by a newline to a file descriptor.
- `ft_putnbr_fd` writes an integer to a file descriptor.

---

## Part 3 - Linked List Functions

The project also includes a basic linked list API based on the following structure:

```c
typedef struct s_list
{
	void			*content;
	struct s_list	*next;
} t_list;
```

The `content` pointer allows each node to store any type of data, while `next` points to the next node in the list.

### List Functions

```c
t_list *ft_lstnew(void *content);
void ft_lstadd_front(t_list **lst, t_list *new);
int ft_lstsize(t_list *lst);
t_list *ft_lstlast(t_list *lst);
void ft_lstadd_back(t_list **lst, t_list *new);
void ft_lstdelone(t_list *lst, void (*del)(void *));
void ft_lstclear(t_list **lst, void (*del)(void *));
void ft_lstiter(t_list *lst, void (*f)(void *));
t_list *ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
```

### Purpose of These Functions

- `ft_lstnew` creates a new list node.
- `ft_lstadd_front` adds a node at the beginning of a list.
- `ft_lstsize` counts the number of nodes of a list.
- `ft_lstlast` returns the last node of a list.
- `ft_lstadd_back` adds a node at the end of a list.
- `ft_lstdelone` deletes one node using a given delete function.
- `ft_lstclear` deletes and frees an entire list.
- `ft_lstiter` applies a function to the content of each node.
- `ft_lstmap` creates a new list by applying a function to each node content.

---

## Usage Example

After compiling the library, it can be used in another C file by including the header:

```c
#include "libft.h"
```

Example:

```c
#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *str;

	str = ft_strdup("Hello, Libft!");
	if (!str)
		return (1);
	printf("%s\n", str);
	free(str);
	return (0);
}
```

Compile it with:

```bash
cc -Wall -Wextra -Werror main.c libft.a -o test
```

Then run:

```bash
./test
```

---

## Technical Requirements

This project follows the 42 technical requirements:

- The project is written in C.
- The code follows the 42 Norm.
- No global variables are used.
- Helper functions are declared as `static` when needed.
- Memory allocated on the heap is properly freed when necessary.
- The library is created using `ar`.
- `libtool` is not used.
- The static library `libft.a` is created at the root of the repository.
- The Makefile does not relink unnecessarily.

---

## Testing

Test files are not part of the final submission, but they can be useful during development.

Possible testing strategies include:

- Comparing `ft_` functions with their original libc equivalents.
- Testing edge cases such as empty strings, `NULL` pointers where applicable, zero sizes, and negative numbers.
- Checking memory leaks with tools such as:

```bash
valgrind
```

Example:

```bash
valgrind --leak-check=full ./test
```

---

## Resources

The following resources were useful for understanding the concepts behind this project:

- The C Programming Language, Brian W. Kernighan and Dennis M. Ritchie.
- Linux manual pages:

```bash
man strlen
man memset
man memcpy
man memmove
man calloc
man strdup
```

- 42 School documentation and project subject.
- Peer discussions and code reviews during the learning process.
- Standard C library documentation.
- GNU C Library documentation.
- BSD string function documentation for functions such as `strlcpy` and `strlcat`.

---

## AI Usage

Artificial intelligence tools were used only as a support resource for documentation and learning purposes.

AI was used to help:

- Structure this README file.
- Improve English wording and clarity.
- Summarize the purpose of the library.
- Organize the list of implemented functions.
- Explain the role of each part of the project.

AI was not used as a replacement for understanding or implementing the project logic.
All source code must be written, reviewed, tested, and understood by the student.

---

## Author

Created by:

```bash
sasilves
```

42 Madrid.
