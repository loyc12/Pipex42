/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_imports.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llord <llord@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/20 15:22:49 by llord             #+#    #+#             */
/*   Updated: 2023/01/09 13:56:08 by llord            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

void	*ft_bzero(void *s, int n)
{
	char	*ptr;

	ptr = (char *)s;
	while (n--)
		*ptr++ = 0;
	return (s);
}

void	*ft_calloc(int count, int size)
{
	void	*ptr;
	int		i;

	i = (count * size) + 1;
	if (i <= 0)
		return (NULL);
	ptr = (void *)malloc(i);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, i);
	return (ptr);
}
