/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ljulien <ljulien@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/02 19:33:30 by ljulien           #+#    #+#             */
/*   Updated: 2021/10/06 17:33:49 by ljulien          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft.h"

enum e_types //enumeration pour les different type de token utile que dans la partie parsing-lexer.
{
	ERROR,
	TEXT,
	TRUNC,
	APPEND,
	HEREDOC,
	INPUT,
	PIPE,
};

typedef	struct	s_token t_token;
typedef	struct	s_cmd t_cmd;

typedef	struct	s_token //struture pour les token n'est utile que pour le parsing.
{
	enum e_types	type;
	char			*line;
	int				fd;
	t_token			*next;
}	t_token;

typedef	struct	s_cmd //struture pour les commande chaque commande succesives est separee par un pipe.
{
	int		num;// num de la commande
	char	**cmds; //cmds[0] est la commandes et le reste jusqu'a cmds[n] == NULL sont des arguments pour la commande.
	int		fd_in; //fd de redirection d'entree.
	int		fd_out;	//fd de redirection de sortie.
	char	*msg_error;//message d'erreur si non nul ne pas executer la commande et passer qu prochain pipe
	t_cmd	*next;
	t_cmd	*prev;
}	t_cmd;	

typedef struct s_shell //struture pour minishell il sert a stocke et passer facilement des donnees en argument.
{
	char	**env;//tableau de string contenant les variables d'environement.
	char	**exp;//tableau contenant les valeur d'export non initialisee.
	char	**path;//tableau de string contenant les chemins de path.
	int		*tpid;//tableau de pid pour waitpid
	int		**tpipe;//tableau de pipe pour dup2
	int		exit_status;// Value to change with the exit status of the executed pipe or command;
	char	*pwd;//string where is store the path to the current directory.
	t_token	*tokens;//utile que dans la partie parsing
	t_cmd	*cmd;//pointeur vers la premiere commande.
}	t_shell;

void		initialization_shell(t_shell *shell, char **ap);
int			check_path(char *path, char *cmd);
void		search_cmd(t_shell *shell, char *cmd);
void		exit_message_error(t_shell *shell, char *msg);
void		ft_tokenclear(t_token **lst);
char		**ft_freetabs(char **t);
char		*search_env(char **env, char *search);
char 		*cur_dir_name(void);
void    	handle_prompt(void);
void    	handle_prompt_heredoc(void);
void		exit_free(t_shell *shell);
int			get_next_line(int fd, char** line);
t_token		*ft_tokennew(enum e_types type, char *line);
void		ft_token_add_back(t_token **atoken, t_token *new);
t_token		*ft_tokenlast(t_token *token);
char		*env_value(char **env, char *search);
void		message_error(char *msg);
int			tokenizer(t_shell *shell, char *line);
void		parsing(t_shell *shell);
int 		check_syntax_error(t_shell *shell, int error);
char		*parsing_tokenizer(t_shell *shell , char *line);
void    	print_export(t_shell *shell);
char		**delete_env(char **ap, char *str);
int			ft_strcmp_sep(char *s1, char *s2, char sep);
void    	starting_execution(t_shell *shell);
void    	display_struct(t_shell *shell);
int     	compare(char *in, char *out);
int     	builtin_echo(char **args);// <- echo
int			builtin_env(t_shell *shell);// <- env
int			builtin_export(t_shell *shell, char **args);// <- export
int			builtin_unset(t_shell *shell, char **args); // <-unset
int			builtin_cd(t_shell *shell, char **arg);// <-cd
int     	builtin_pwd(t_shell *shell);// <-pwd
#endif
