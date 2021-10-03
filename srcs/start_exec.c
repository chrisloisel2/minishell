/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchristo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 17:06:37 by lchristo          #+#    #+#             */
/*   Updated: 2021/09/30 17:06:39 by lchristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     path(t_shell *shell)
{
    int i;
    char *str;

    str = ft_strjoin("/", shell->cmd->cmds[0]);
    i = 0;
    while (shell->path[i] != NULL)
    {
        execve(ft_strjoin(shell->path[i], str), shell->cmd->cmds, shell->env);
        i++;
    }
    return (1);
}

void    starting_execution(t_shell *shell)
{
    int i;

    i = fork();
    if (i == 0)
        path(shell);
    else
        waitpid(i, NULL, 0);
    display_struct(shell);
}