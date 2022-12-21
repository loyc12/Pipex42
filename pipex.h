/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llord <llord@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:05:28 by llord             #+#    #+#             */
/*   Updated: 2022/12/21 14:17:17 by llord            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <time.h>
# include <string.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>

typedef enum e_state
{
	STATE_ERR_INPUT = -1,
	STATE_DEFAULT = 0,
}			t_state;

typedef struct s_data
{
	int		infile;
	char	*cmd1;
	int		inpipe;

	int		outpipe;
	char	*cmd2;
	int		outfile;

	char	**paths;
	char	**envp;
}			t_data;

//from libft_imports
void	*ft_calloc(int count, int size);

//from pathers
char	**ft_split(char *str, char c);
char	*add_to_path(char *path, char *s);
void	get_paths(t_data *d);

//from forkers
void	first_fork(t_data *d, pid_t *child);
void	second_fork(t_data *d, pid_t *child);

#endif
