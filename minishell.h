/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:23:33 by jsaintho          #+#    #+#             */
/*   Updated: 2024/12/06 15:48:12 by jsaintho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H

# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include <limits.h>
# include <libft.h>
# include <signal.h>
# include <dirent.h> 
# include <sys/types.h>
# include <sys/wait.h>

# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define ERROR 1
# define SUCCESS 0

# define MAX_EXPANDS 1024

# define M_1(a, b) ((a) ? (" ") : (b))

# define FT ft_strlen
# define HS handle_spaces
# define FSF ft_strjoin_free
# define FSR ft_strdup
# define TO t->cmds
# define IR_ insert_replace
# define FC_ ft_strchr
# define FE_ findenv
# define LF_ last_file
# define FM ft_m_strchr_i
# define F_R ft_str_remvchr

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
    char	*f_name;
	bool	expand;
	bool	append;
	bool	_out;
	bool	heredoc;
}			t_file;

typedef struct s_command
{
	char	*command;
	t_file	files[300];
	int		f_i;
	int		n_out;
	int		n_in;
}			t_cmd;

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
	int					exstat;
	int					exp_starter;
	t_cmd				*cmds;	// commands list
	size_t				cmd_count;
	char				*set;
	t_mltsplit			*sq;
	int					T;
}	t_minishell;

// gautoier
char	**ft_split_quotes(char *s, char c, int remove_quote);

// [PARSING] 
void	fdp_parsing(char *cmd, t_minishell *t);
char	parse_errors(t_mltsplit *s, char *line);
void	free_multisplit(t_mltsplit *s);
void	apply_expands(t_minishell *t);

// ENV
char	*get_env_path(t_minishell *t_m);
int		env_init(t_minishell *t_m, char **argv);

// BUILT-IN
int		f__cd(char **args, t_minishell *t_m);
int		f__pwd(int fd_out);
int		f__env(t_env *env, int fdout);
int		f__echo(char **args, int fdout);
int		f__unset(t_minishell *t_m);
int		f__export(t_minishell *t);
int		is_builtin(char *c);
//void	run_builtin(t_minishell *t_m, int n_builtin, int fdout, t_cmd *cmd);
int		run_builtin(t_minishell *t_m, int n_builtin, int fdout, t_cmd *cmd);
int		f__exit(t_minishell *t_m, t_cmd *c);

/* GAUTIER LE G.O.A.T DU MULTITHREADING */
//expands

// EXECUTOR
extern int	sigcatch;
void	exec_cmds(t_minishell *t_m);
int		child_molestor(t_minishell *t_m, t_cmd *c, size_t i, int c_int, char **nenv);
int		open_file(char *file, int n, int append);
char	*bget_path2(char *cmd, char **env);
void	ft_free_tab(char **tab);

// heredoc
int		heredocalloc(t_minishell *t_m);
int		delete_heredocs(t_minishell *t_m);

// UTILS
char	**pipe_env(t_minishell *t_m);
void	shlvladd(char *env, char **env_l);

// SIGNALS
void	handler(int signum);
void	int_heredoc(int signum);
void	signalsetter(int signum, void (*handler)(int));
void	signalignore(int signum);

//builtins
int	builtindirector(t_minishell *t_m, t_cmd *c, int n_builtin);
t_env	*findenv(char *name, t_env *env);

#endif
