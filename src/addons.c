/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   addons.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llord <llord@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/21 17:20:57 by llord             #+#    #+#             */
/*   Updated: 2023/01/09 13:56:05 by llord            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../pipex.h"

//closes every fd in the d struct
void	close_fds(t_data *d)
{
	close(d->infile);
	close(d->inpipe);
	close(d->outpipe);
	close(d->outfile);
}

//frees a given array properly
void	free_array(char **array)
{
	int	i;

	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
}

//counts how many sections result from spliting a str around a char
int	count_sections(char *str, char c)
{
	int	i;
	int	n;

	i = -1;
	n = 1;
	while (str[++i])
	{
		if (str[i] == c)
			n++;
	}
	return (n);
}

//finds the lenght to a given char
int	lento(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i] && s[i] != c)
		i++;
	return (i);
}

//properly closes things and kills the child process
void	infanticider(t_data *d)
{
	free_array(d->paths);
	close_fds(d);
	free(d);
	exit(EXIT_FAILURE);
}
