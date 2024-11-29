/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:23:33 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/29 16:58:11 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <limits.h>
#include <libft.h>
#include <signal.h>
#include <dirent.h> 
#include <sys/types.h>
#include <sys/wait.h>



# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define ERROR 1
# define SUCCESS 0

# define MAX_EXPANDS 1024

# define MACRO_ZERO(x) ( (x) > 0 ? (1) : (0))
/*
{
	PIPE = |
	OPERATOR = [>, <, >>]
	COMMAND = [bin, ls, cat -e, pwd]
	VARIABLE = "<: Hello World!2"
}
*/

typedef struct s_file
{
    char    *f_name;
	bool	expand;
	bool	append;
	bool	_out;
	bool	heredoc;
}               t_file;

typedef struct s_command
{
    char        *command;
    t_file      files[300];
    int         f_i;
	int			n_out;
	int			n_in;
	int			is_piped_in;
	int			is_piped_out;
	int			is_heredoc;
	char		*output;
	char		*input;
}       t_cmd;

enum TOKEN_TYPE
{
	COMMAND,
	COMMAND_FLAG,
	PIPE,
	LESS,
	LESS_LESS,
	GREAT,
	GREAT_GREAT,
	QUOTE,
	SEPARATOR,
	PARSE_ERROR,
	ARGUMENT
};

typedef struct s_expand
{
	char	*key;
	char	*value;
}	t_expand;


typedef struct	s_env
{
	char			*value;
	struct s_env	*next;
}				t_env;

typedef struct s_minishell
{
	t_env				*env;		// saved environment
	char				**c_args;	// splited args (for built-ins)
	pid_t				*pid;			// array of all pids, one per command
	int					(*pipes_fd)[2];	// array of pipes
	int					*heredocs;		// array of heredocs
	t_expand			expands[MAX_EXPANDS];
	int					exstat;
	int					exp_starter;
	t_cmd				*cmds;	// commands list
	size_t				cmd_count;
	char				*set;
}	t_minishell;

// gautoier
char *rm_quotes(char *s);
char	**ft_split_quotes(char *s, char c, int remove_quote);
void	apply_quote_removal(char **t);


// [PARSING] 
void	fdp_parsing(char *cmd, t_minishell *t);
char	parse_errors(t_mltsplit *s);
int		quote_errors(char	*s_cmds);

// ENV
char	*get_env(char **env);
char	*get_env_path(t_minishell *t_m);
int		env_init(t_minishell *t_m, char **argv);

// BUILT-IN
int		f__cd(char **args, t_minishell *t_m);
int		f__pwd(int fd_out);
int		f__env(t_env *env, int fdout);
int		f__echo(char **args, int fdout);
int		f__unset(t_minishell *t_m);
void	f__export(t_minishell *t, int fdout);
int		is_builtin(char *c);
void	run_builtin(t_minishell *t_m, int n_builtin, int fdout, t_cmd *cmd);


/* GAUTIER LE G.O.A.T DU MULTITHREADING */

//expands
char *delchar(char *src, char rmv);

// EXECUTOR
extern int	sigcatch;
void free_c_args(t_minishell	*t_m);
void	exec_cmds(t_minishell *t_m);
int	child_molestor(t_minishell *t_m, t_cmd *c, size_t i, int c_int, char **nenv);
int	dupclose(int fd2, int fd1);
int	ft_waiter(t_minishell *t_m);
void	ft_exec2(char *cmd, char **env);
int	nullcommand(t_minishell *t_m, size_t i);
int		open_file(char *file, int n, int append);
char	*my_getenv(char *name, char **env);
char	*bget_path2(char *cmd, char **env);
void	ft_free_tab(char **tab);

// heredoc
int	heredoc(t_file *f);
int		heredocalloc(t_minishell *t_m);
int		delete_heredocs(t_minishell *t_m);
int		childhead_handler(t_minishell *t_m, size_t i, t_cmd *c);
int	childbutt_handler(t_minishell *t_m, size_t i, t_cmd *c);

// UTILS
char	**pipe_env(t_minishell *t_m);
void	shlvladd(char *env, char **env_l);
void	f__exit(t_minishell *t_m, t_cmd *c);

// SIGNALS
// void handle_sigint(int sig);
char	*tmpnamer(void);
void 	handler(int signum);
void	int_heredoc(int signum);
void	signalsetter(int signum, void (*handler)(int));
void	shlvlhandler(char **env);
void	shlvldetector(t_env *env);
void	signalignore(int signum);

//builtins
int	restorefds(t_cmd *c, int fd_stds[2]);
int	openfds(t_cmd *c, int fd_stds[2], int fd_duped[2]);
int	builtindirector(t_minishell *t_m, t_cmd *c, int n_builtin);
t_env	*findenv(char *name, t_env *env);
void	trim_tab(char **t, t_minishell *tm);

#endif
