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
    // if (shell->cmd->fd_out != -1)
        // dup2(shell->cmd->fd_out, shell->stdout);
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

t_cmd   *first_cmd(t_shell *shell)
{
    t_cmd *next;

    next = shell->cmd;
    if (shell->cmd->prev == NULL)
        return (shell->cmd);
    while (next->prev != NULL)
        next = next->prev;
    return (next);
}

int     nb_pipes(t_shell *shell)
{
    t_cmd   *next;
    int     i;

    i = 0;
    next = shell->cmd;
    while (next->next != NULL)
    {
        i++;
        next = next->next;
    }
    return (i);
}

void    build_pipes(t_shell *shell, int i)
{
    i--;
    while(i >= 0)
    {
        shell->tpipe[i] = malloc(sizeof(int)* 2);
        pipe(shell->tpipe[i]);
        i--;
    }
}

void    redirection_pipe(t_shell *shell)
{
    int in;

    printf("%d\n", shell->cmd->num);
    in = (shell->cmd->num - 1);
    dup2(shell->tpipe[in][1], shell->cmd->fd_out);
    dup2(shell->tpipe[in][0], shell->cmd->fd_in);
}

void    build_tabs(t_shell *shell)
{
    int nb_pipe;

    nb_pipe = nb_pipes(shell) + 1;
    shell->tpipe = malloc(sizeof(int) * nb_pipe);
    build_pipes(shell, nb_pipe);
    shell->tpid = malloc(sizeof(pid_t) * nb_pipe + 1);
}

void    wait_pid(t_shell *shell)
{
    int i;

    i = 0;
    while (i <= shell->cmd->num)
    {
        waitpid(shell->tpid[i], NULL, 0);
        i++;
    }
}

void    execution(t_shell *shell)
{
    shell->tpid[shell->cmd->num] = (int)fork();
    if (shell->tpid[shell->cmd->num] == 0)
    {
        redirection_pipe(shell);
        if (shell->cmd->msg_error != NULL)
        {
            print_error(shell);
            return ;
        }
        if (compare(shell->cmd->cmds[0], "cd"))
        {
            builtin_cd(shell, shell->cmd->cmds);
            return ;
        }
        if (compare(shell->cmd->cmds[0], "exit"))
            exit (0);
        if (shell->cmd->cmds)
            path(shell);
    }
    if (shell->cmd->next != NULL)
    {
        shell->cmd = shell->cmd->next;
        execution(shell);
    }
    wait_pid(shell);
}

void    starting_execution(t_shell *shell)
{
    build_tabs(shell);
    execution(shell);
}
