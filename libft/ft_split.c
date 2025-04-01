/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 17:07:15 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/02/16 20:43:08 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <stdlib.h>

static int	ft_substrlen(char const *s, char c)
{
	int	ret;

	ret = 0;
	while (*s)
	{
		s++;
		ret++;
		if (*s == c || (*(s) == '\0'))
			return (ret);
	}
	return (0);
}

static char	**ft_freemem(char **s, size_t n)
{
	while (n)
	{
		free (*(s + n));
		n--;
	}
	free (*(s));
	free (s);
	return (NULL);
}

static int	ft_countwords(char const *s, char c)
{
	int	ret;

	ret = 0;
	while (*s)
	{
		while (*s == c && *s)
		{
			s++;
			if (!*s)
				return (ret);
		}
		while (*s != c && *s)
			s++;
		ret++;
	}
	return (ret);
}

static	size_t	ft_countchar(const char *s, char c)
{
	size_t	ret;

	ret = 0;
	while (*(s + ret) && *(s + ret) == c)
		ret++;
	return (ret);
}

char	**ft_split(char const *s, char c)
{
	size_t		i;
	size_t		temp_size;
	size_t		s_pos;
	char		**ret;
	size_t		word_count;

	i = 0;
	s_pos = 0;
	word_count = ft_countwords(s, c);
	ret = (char **)malloc((word_count + 1) * sizeof(char *));
	if (!ret)
		return (NULL);
	while (i < word_count)
	{
		s_pos += ft_countchar((s + s_pos), c);
		temp_size = ft_substrlen(s + s_pos, c) + 1;
		*(ret + i) = (char *)ft_calloc(temp_size, sizeof(char));
		if (!(*(ret + i)))
			return (ft_freemem(ret, i));
		ft_strlcpy(*(ret + i), (s + s_pos), temp_size);
		s_pos += temp_size;
		i++;
	}
	*(ret + i) = NULL;
	return (ret);
}

// #define RED "\033[0;31m"  // WRONG
// #define GREEN "\033[0;32m" // CORRECT
// #define PURPLE "\033[0;35m" // ORIGINAL FUNCTION
// #define BLUE "\033[0;34m" // OWN FUNCTION
// #define RESET "\033[0m"
// void	ft_print_split_test(char const *s, char c)
// {
// 	printf("wordcount: %i\n", ft_countwords(s, c));
// 	printf("substrlen: %i\n", ft_substrlen(s, c));
// 	printf(BLUE"ft_split"RESET" s: "BLUE"%s"RESET " c: "BLUE"%c\n"RESET, s, c);
// 	char	**d_ptr = ft_split(s, c);
// 	int i = 0;
// 	while (*d_ptr != NULL)
// 	{
// 		printf("*(ret + %i): %s\n", i, *d_ptr);
// 		d_ptr++;
// 		i++;
// 	}
// }
// // cc ft_split.c ft_strlcpy.c ft_calloc.c ft_strlen.c 
// // ft_bzero.c ft_memset.c && ./a.out
// // int main() // compiling -> 
// // {
// // 	// char *s = "abdec deabd |cfghrkdkc-djue /cadfa12~e";
// // 	// char c = 'c';

// // 	// printf("substrlen: %i\n", ft_substrlen(s + 4, c));
// // 	// printf("countchar: %i\n", ft_countchar(s, c));
// // 	// ft_print_split_test(s, c);
// // 	// ft_print_split_test("   lorem   ipsum dolor     sit amet,
// //   // consectetur   adipiscing elit. Sed non risus. Suspendisse   ", ' ');
// // 	// ft_print_split_test("lorem ipsum dolor sit amet,
// //	// consectetur adipiscing elit. Sed non risus.
// //	//  Suspendisse lectus tortor,
// //	// dignissim sit amet, adipiscing nec, ultricies 
// //	// sed, dolor. Cras elementum 
// //	// ultricies diam. Maecenas ligula massa, varius a, 
// //	// semper congue, euismod non, mi.", 'i');
// // 	ft_print_split_test("^^^1^^2a,^^^^3^^^^--h^^^^", '^');
// // 	return (0);
// // }
