/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lchristo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 17:06:37 by lchristo          #+#    #+#             */
/*   Updated: 2021/10/05 14:26:12 by lchristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     check_builtin(t_shell *shell, char *str)
{
    int i;

    i = 1;
    // if (shell->cmd->msg_error != NULL)
    // {
    //    ft_putstr_fd(shell->cmd->msg_error, 2);
    //    free(shell->cmd->msg_error);
    //     return (0);
    // }
    if (compare(str, "echo"))
        i = builtin_echo(shell->cmd->cmds);
    if (compare(str, "env"))
        i = builtin_env(shell);
    if (compare(str, "export"))
        i = builtin_export(shell, shell->cmd->cmds);
    if (compare(str, "unset"))
        i = builtin_unset(shell, shell->cmd->cmds);
    if (i < 0)
        printf("problem\n");
    return (i);
}

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

    display_struct(shell);
    if (shell->cmd->fd_out != -1)
        dup2(shell->cmd->fd_out, 1);
    if (check_builtin(shell, shell->cmd->cmds[0]) == 0)
        return ;
    i = fork();
    if (i == 0)
        path(shell);
    else
        waitpid(i, NULL, 0);
    // display_struct(shell);
}
