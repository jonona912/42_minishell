/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: opopov <opopov@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 19:50:36 by zkhojazo          #+#    #+#             */
/*   Updated: 2025/05/03 14:51:08 by opopov           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H
# include <string.h>
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

int		ft_free_double_ptr(char **str);
// ft_is functions
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit(int c);
// int		ft_islower(char c);
int		ft_isspace(int c);
int		ft_isprint(int c);
// int		ft_isupper(char c);

// ft_to
char	*ft_itoa(int n);
int		ft_toupper(int c);
int		ft_tolower(int c);

// ft_strings
//		ft_strings - int returns
char	*ft_strchr(const char *s, int c);
// char	*ft_strcpy(char *dest, const char *src);
char	*ft_strdup(char *src);
void	ft_striteri(char *s, void (*f)(unsigned int, char*));
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strjoin_char(const char *s1, char c);
char	*ft_strjoin_three(char const *s1, char const *s2, char const *s3);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *str);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(const char *s1, const char *s2, size_t n);
// char	*ft_strncpy(char *dest, const char *src, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strstr(const char *big, const char *little);
char	*ft_strstr_last(const char *big, const char *little);
char	*ft_strstr_first(const char *big, const char *little);
char	*ft_strrchr(const char *s, int c);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_substr(char const *s, unsigned int start, size_t len);
char	*ft_strcpy(char *dest, const char *src);
// ft_numbers
int		ft_atoi(const char *nptr);

// mem functions
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memcpy(void *dest, const void *src, size_t n);
void	*ft_memmove(void *dest, const void *src, size_t n);
void	*ft_memset(void *s, int c, size_t n);
void	ft_bzero(void *s, size_t n);

// memory allocation
void	*ft_calloc(size_t nmemb, size_t size);
char	**ft_split(char const *s, char c);
// print output
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putstr_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);

// list_functions
void	ft_lstadd_back(t_list **lst, t_list *new);
void	ft_lstadd_front(t_list **lst, t_list *new);
void	ft_lstclear(t_list **lst, void (*del)(void *));
void	ft_lstdelone(t_list *lst, void (*del)(void *));
void	ft_lstiter(t_list *lst, void (*f)(void *));
t_list	*ft_lstlast(t_list *lst);
t_list	*ft_lstnew(void *content);
int		ft_lstsize(t_list *lst);
t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));

#endif
