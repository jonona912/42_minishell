/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zkhojazo <zkhojazo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 13:46:51 by zkhojazo          #+#    #+#             */
/*   Updated: 2024/11/24 12:57:33 by zkhojazo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	ft_char_exists(char const c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

static const char	*ft_startcmp(char const *s1, char const *set)
{
	int	i;

	i = 0;
	while (ft_char_exists(*(s1 + i), set) && *(set) && *(s1 + i))
		i++;
	return (s1 + i);
}

static const char	*ft_endcmp(char const *s1, char const *set)
{
	size_t	s_l;

	s_l = ft_strlen(s1) - 1;
	while (*(s1 + s_l) && ft_char_exists(*(s1 + s_l), set))
		s_l--;
	return (s1 + s_l);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char		*ret;
	char const	*s1_start;
	char const	*s1_end;
	size_t		space_size;

	if (!s1 || !set)
		return (NULL);
	s1_start = ft_startcmp(s1, set);
	s1_end = ft_endcmp(s1, set);
	if (s1_start > s1_end)
		return (ft_calloc(1, sizeof(char)));
	space_size = (size_t)(s1_end - s1_start + 1);
	ret = (char *)ft_calloc((space_size + 1), sizeof(char));
	if (!ret)
		return (NULL);
	ft_strlcpy(ret, s1_start, space_size + 1);
	return (ret);
}

// #define RED "\033[0;31m"  // WRONG
// #define GREEN "\033[0;32m" // CORRECT
// #define PURPLE "\033[0;35m" // ORIGINAL FUNCTION
// #define BLUE "\033[0;34m" // OWN FUNCTION
// #define RESET "\033[0m"
// void	ft_print_strtrim_test(char const *s1, char const *set, const char *test)
// {
// 	printf(BLUE"\nft_strtrim" RESET " s1: 
//	"PURPLE"%s"RESET " set:"PURPLE"%s\n"RESET, s1, set);
// 	char	*ft_str = ft_strtrim(s1, set);
// 	strcmp(ft_str, test) == 0
// 	? printf(GREEN"Test: %s\nFt  : %s\n"RESET, test, ft_str)
// 	: printf(RED"Test: %s\nFt  : %s\n"RESET, test, ft_str);
// }

// int main()
// {
// 	ft_print_strtrim_test(" lorem ipsum dolor sit amet", "l ",
// 	 "orem ipsum dolor sit amet");
// 	ft_print_strtrim_test("lorem \n ipsum \t dolor \n sit \t amet", " ",
// 	 "lorem \n ipsum 	 dolor \n sit 	 amet");
// 	ft_print_strtrim_test("lorem ipsum dolor sit amet", "te",
// 	 "lorem ipsum dolor sit am");
// 	ft_print_strtrim_test("lorem ipsum dolor sit amet", "tel",
// 	 "orem ipsum dolor sit am");
// }
