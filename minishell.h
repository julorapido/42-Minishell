/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:23:33 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/24 13:21:16 by jsaintho         ###   ########.fr       */
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

# define MAX_CMDS 512
# define MAX_OUTFILES 64

/*
{
	PIPE = |
	OPERATOR = [>, <, >>]
	COMMAND = [bin, ls, cat -e, pwd]
	VARIABLE = "<: Hello World!2"
}
*/

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

typedef struct s_token
{
	char 			*cmd;
	enum TOKEN_TYPE	t;
	struct s_token	*prev;
	struct s_token	*next;
}	token;

typedef struct s_cmd
{
	char	*command;	
	char	*input;
	char	*output;
	int		n_redirections;
	int		appends[MAX_OUTFILES];
	bool	is_piped_out;
	bool	is_stdin;
	bool	is_heredoc;
}	t_cmd;

typedef struct	s_env
{
	char			*value;
	struct s_env	*next;
}				t_env;

typedef struct s_minishell
{
	t_env	*env;		// saved environment
	t_cmd	*commands;	// commands list
	size_t	cmd_count;	// command count
	char	**c_args;	// splited args (for built-ins)
	bool	parse_error;	// parse error in current command
	char	e_v[2];			// parse error info (zsh: parse error near `<.//>')
	pid_t	*pid;			// array of all pids, one per command
	int		(*pipes_fd)[2];	// array of pipes
	int		*heredocs;		// array of heredocs
	char 	*splt_cat;		// parseError splitcat
	token	**cmd_tokens;	// arr of tokens
}	t_minishell;


typedef struct s_pipe
{
	int		pfd[2];
	t_cmd	*commands;
	int		argc;
	char	**argv;
	char	**envp;
	t_minishell	*t_m;
}	t_pipe;

typedef struct s_pipes {
	char	**argv;
	char	**env;
	int		argc;
	int		pfd[2];
	int		save;
	int		xcmd;
	pid_t	*pid;
	t_minishell	*t_m;
}				t_pipes;




// TOKEN UTILITES
token			*token_new(char *s, enum TOKEN_TYPE ty);
void			token_push(token **token_lst, token *new_t);
token			*token_last(token	*head);
char			*token_type_to_str(enum TOKEN_TYPE t);
enum TOKEN_TYPE	char_to_token(char c);
bool			str_is_onlysep(char *s);
bool			is_char_operator(char c);
void			show_tokens(token **cmd_tokens);
void			token_dbl_push(token **token_lst);
void			free_tokens(token **t_arr);


// [PARSING] UTILITES
void	fn_revstr(char *up_s);
char	*cmd_remove_lstspace(char *s);
void	print_commands(t_minishell *t_m);
void	apply_space_removal(t_minishell *t_m);
void	apply_is_piped_out(t_minishell *t_m);
void	apply_appends_reverse(t_minishell *t_m);
void	apply_commands_reverse(t_minishell	*t_m);
bool	is_parse_error(char *s);
enum TOKEN_TYPE	switcher(char *tken, token **t_l);
char	*fn_realloc_strcat(char *filled_str, char *cncat_str, int space_it);
int		switcher_i(char *tken);


// ENV
char	*get_env(char **env);
char	*get_env_path(t_minishell *t_m);
int		env_init(t_minishell *t_m, char **argv);


// PARSING
int		parse_tokens(char *cmd, token **cmd_tokens, t_minishell *t_m);
int		parse_commands(t_minishell *t_m, token **cmd_tokens);
int		parse_errors(char *cmd, t_minishell *t_m);
void	parse_free(t_minishell *t_m);




// BUILT-IN
int		f__cd(char **args, t_minishell *t_m);
int		f__pwd(int fd_out);
int		f__env(t_env *env, int fdout);
int		f__echo(char **args, int fdout);
int		f__unset(t_minishell *t_m);
void	f__export(t_minishell *t);
int		is_builtin(char *c);
void	run_builtin(t_minishell *t_m, int n_builtin, int fdout);


/* GAUTIER LE G.O.A.T DU MULTITHREADING */
// EXECUTOR
void free_c_args(t_minishell	*t_m);
void	exec_cmds(t_minishell *t_m);
int	child_molestor(t_minishell *t_m, t_cmd *c, size_t i, int c_int,
	char **outlist, char **nenv);
int	dupclose(int fd2, int fd1);
int	ft_waiter(t_minishell *t_m);
void	ft_exec2(char *cmd, char **env);
int	nullcommand(t_minishell *t_m, size_t i);

// heredoc
int		heredoc(char *eof, t_cmd *cmd, t_minishell *t_m);
int		heredocalloc(t_minishell *t_m);
int		delete_heredocs(t_minishell *t_m);
int		childhead_handler(t_minishell *t_m, size_t i, t_cmd *c);
int		childbutt_handler(t_minishell *t_m, size_t i, t_cmd *c, char **outlist);

// UTILS
char	**pipe_env(t_minishell *t_m);
void shlvladd(char *env);

// SIGNALS
// void handle_sigint(int sig);
char	*tmpnamer(void);
void 	handler(int signum);
void	int_heredoc(int signum);
void	signalsetter(int signum, void (*handler)(int));
void	shlvlhandler(char **env);
void	shlvldetector(t_env *env);
void	signalignore(int signum);
// PIPEX
void	exit_handler(int n_exit);
int		open_file(char *file, int n, int append);
char	*my_getenv(char *name, char **env);
char	*bget_path2(char *cmd, char **env);
void	ft_free_tab(char **tab);

// // PIPEX
// void	exit_handler(int n_exit);
// int		open_file(char *file, int n);
// char	*my_getenv(char *name, char **env);
// char	*bget_path2(char *cmd, char **env);
// void	ft_free_tab(char **tab);
// int		heredoc(char *eof, t_cmd *cmd, int i);
// int		pipex(int argc, char **argv, char **env, t_minishell *t_m);
// int		hereorfile(t_pipes pipex);
// // EXECUTOR
// void	exec_cmds(t_minishell *t_m);

#endif
