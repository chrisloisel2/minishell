/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljulien <ljulien@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 17:06:37 by lchristo          #+#    #+#             */
/*   Updated: 2021/10/07 21:08:57 by lchristo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int     check_builtin(t_shell *shell, char *str)
{
    int i;

    i = -1;
    if (compare(str, "echo"))
        i = builtin_echo(shell->cmd->cmds);
    if (compare(str, "env"))
        i = builtin_env(shell);
    if (compare(str, "export"))
        i = builtin_export(shell, shell->cmd->cmds);
    if (compare(str, "unset"))
        i = builtin_unset(shell, shell->cmd->cmds);
    if (compare(str, "pwd"))
        i = builtin_pwd(shell);
    if (i > 0)
        printf("problem\n");
    return (i);
}

int     path(t_shell *shell)
{
    int i;
    char *str;

    i = 0;
    str = ft_strjoin("/", shell->cmd->cmds[0]);
    if (shell->cmd->fd_out != -1)
        dup2(shell->cmd->fd_out, shell->stdout);
    if (check_builtin(shell, shell->cmd->cmds[0]) != -1)
        exit (1) ;
    while (shell->path[i] != NULL)
    {
        execve(ft_strjoin(shell->path[i], str), shell->cmd->cmds, shell->env);
        i++;
    }
    execve(ft_strjoin("./", str), shell->cmd->cmds, shell->env);
    printf("minishell: %s: command not found\n", shell->cmd->cmds[0]);
    return (1);
}

void    print_error(t_shell *shell)
{
    ft_putendl_fd(shell->cmd->msg_error, 2);
    free(shell->cmd->msg_error);
}

void    starting_execution(t_shell *shell)
{
    int i;

    if (shell->cmd->msg_error != NULL)
    {
        print_error(shell);
        return ;
    }
    if (shell->cmd->next != NULL)
    {
        shell->cmd = shell->cmd->next;
        starting_execution(shell);
        shell->cmd = shell->cmd->prev;
    }
    if (compare(shell->cmd->cmds[0], "cd"))
    {
        i = builtin_cd(shell, shell->cmd->cmds);
        return ;
    }
    if (compare(shell->cmd->cmds[0], "exit"))
        exit (0);
    i = fork();
    if (i == 0)
    {
        if (shell->cmd->cmds)
            path(shell);
    }
    else
        waitpid(i, NULL, 0);
}
