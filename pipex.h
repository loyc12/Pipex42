/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llord <llord@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 15:05:28 by llord             #+#    #+#             */
/*   Updated: 2022/12/14 14:31:52 by llord            ###   ########.fr       */
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
	STATE_ERR_CMD = -4,		//cmd couldn't run
	STATE_ERR_PID = -3,		//cmd couldn't run
	STATE_ERR_FILE = -2,	//file couldn't open
	STATE_ERR_INPUT = -1,	//bad argv count
	STATE_NULL = 0,
	STATE_SUCCESS = 1,
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

	int		state;

}			t_data;

#endif
