/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasilves <sasilves@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/02 17:22:54 by sasilves          #+#    #+#             */
/*   Updated: 2026/06/10 12:05:56 by sasilves         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char sep)
{
	size_t	i;
	size_t	words;

	i = 0;
	words = 0;
	while (s[i])
	{
		while (s[i] && s[i] == sep)
			i++;
		if (s[i])
		{
			words++;
			while (s[i] && s[i] != sep)
				i++;
		}
	}
	return (words);
}

static size_t	word_len(char const *s, char sep)
{
	size_t	i;

	i = 0;
	while (s[i] && s[i] != sep)
		i++;
	return (i);
}

static char	*make_word(char const *s, char sep)
{
	char	*word;
	size_t	len;
	size_t	i;

	len = word_len(s, sep);
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = s[i];
		i++;
	}
	word[i] = '\0';
	return (word);
}

static void	free_mem(char **array, size_t words)
{
	size_t	i;

	i = 0;
	while (i < words)
	{
		free(array[i]);
		i++;
	}
	free(array);
}

char	**ft_split(char const *s, char c)
{
	char	**array;
	size_t	j;

	if (s == NULL)
		return (NULL);
	array = malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (array == NULL)
		return (NULL);
	j = 0;
	while (*s != '\0')
	{
		while (*s != '\0' && *s == c)
			s++;
		if (*s != '\0')
		{
			array[j] = make_word(s, c);
			if (array[j] == NULL)
				return (free_mem(array, j), NULL);
			j++;
		}
		while (*s != '\0' && *s != c)
			s++;
	}
	array[j] = NULL;
	return (array);
}
