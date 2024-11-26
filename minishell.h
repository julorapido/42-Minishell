/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <jsaintho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:23:33 by jsaintho          #+#    #+#             */
/*   Updated: 2024/11/25 17:51:33 by jsaintho         ###   ########.fr       */
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
# define MAX_EXPANDS 1024

# define TOKEN_STOCK_SIZE 128


# define M_TKEN(x) (x == ';') ? SEPARATOR : PIPE 
# define M_CMD(x) (x) ? "STD_IN" : "pipe";


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
	bool	quote_ignoreSpace_aft;
	bool	quote_ignoreSpace_befr;
	struct s_token	*prev;
	struct s_token	*next;
}	token;

typedef struct s_cmd_parsing
{
	int		in;

	int		ou;
	int		append;
	bool	s;
}	parse_cmd;

typedef struct s_expand
{
	char	*key;
	char	*value;
}	t_expand;

typedef struct s_cmd
{
	char	*command;	
	char	*input;
	char	*output;
	int		n_redirections;	
	int		is_append;
	bool	is_piped_out;
	bool	is_piped_in;
	bool	is_stdin;
	bool	is_stdout;
	bool	is_heredoc;
	bool	continue_;
}	t_cmd;

typedef struct	s_env
{
	char			*value;
	struct s_env	*next;
}				t_env;

typedef struct s_minishell
{
	t_env				*env;		// saved environment
	t_cmd				*commands;	// commands list
	size_t				cmd_count;	// command count
	char				**c_args;	// splited args (for built-ins)
	bool				parse_error;	// parse error in current command
	char				e_v[2];			// parse error info (zsh: parse error near `<.//>')
	pid_t				*pid;			// array of all pids, one per command
	int					(*pipes_fd)[2];	// array of pipes
	int					*heredocs;		// array of heredocs
	char 				*splt_cat;		// parseError splitcat
	token				**cmd_tokens;	// arr of tokens
	t_expand			expands[MAX_EXPANDS];
	struct s_cmd_parsing	p_cmd;
	token				*stocked_token;
	char				*stocked_scmdsi;
	int					exstat;
	int					is_expand;
	int					n_expand;
	int					exp_starter;
}	t_minishell;

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


// [PARSING] UTILITESreturn ;
void	fn_revstr(char *up_s);
char	*cmd_remove_lstspace(char *s);
void	print_commands(t_minishell *t_m);
void	apply_space_removal(t_minishell *t_m);
void	apply_is_piped_out(t_minishell *t_m);
void	apply_appends_reverse(t_minishell *t_m);
void	apply_commands_reverse(t_minishell	*t_m);
void	apply_is_stds(t_minishell *t_m);
bool	is_parse_error(char *s);
enum TOKEN_TYPE	switcher(t_minishell *t_m, char *tken, token **t_l);
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
void	parse_quote(t_minishell *t_m, token **cmd_tokens, char **s_cmds, int *i);
void	apply_expands(t_minishell *t);
void 	apply_ignoredSpace_beforeQuote(t_minishell *t_m);
char	**ft_split_quotes(char *s, char c, int remove_quote);


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
int	child_molestor(t_minishell *t_m, t_cmd *c, size_t i, int c_int,
char	**outlist, char **nenv);
int	dupclose(int fd2, int fd1);
int	ft_waiter(t_minishell *t_m);
void	ft_exec2(char *cmd, char **env, t_minishell *tm);
int	nullcommand(t_minishell *t_m, size_t i);
int		open_file(char *file, int n, int append);
char	*my_getenv(char *name, char **env);
char	*bget_path2(char *cmd, char **env);
void	ft_free_tab(char **tab);

// heredoc
int		heredoc(char *eof, t_cmd *cmd);
int		heredocalloc(t_minishell *t_m);
int		delete_heredocs(t_minishell *t_m);
int		childhead_handler(t_minishell *t_m, size_t i, t_cmd *c);
int		childbutt_handler(t_minishell *t_m, size_t i, t_cmd *c, char **outlist);

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
