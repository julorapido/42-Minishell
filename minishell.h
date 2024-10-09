/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsaintho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 15:23:33 by jsaintho          #+#    #+#             */
/*   Updated: 2024/10/09 16:20:08 by jsaintho         ###   ########.fr       */
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


# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define ERROR 1
# define SUCCESS 0

# define MAX_CMDS 512

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
	ARGUMENT
};

//	TOKEN
typedef struct s_token
{
	char 			*cmd;
	enum TOKEN_TYPE	t;
	struct s_token	*prev;
	struct s_token	*next;
}	token;

//	COMMAND
typedef struct s_cmd
{
	char	*command;	
	char	*input;
	char	*output;
}	t_cmd;

// ENV (as linked-list)
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
}	t_minishell;


// TOKEN UTILITES
token			*token_new(char *s, enum TOKEN_TYPE ty);
void			token_push(token **token_lst, token *new_t);
void			show_tokens(token **cmd_tokens);
token			*token_last(token	*head);
char			*token_type_to_str(enum TOKEN_TYPE t);
enum TOKEN_TYPE	char_to_token(char c);


// CMD UTILITES
void	fn_revstr(char *up_s);
char	*cmd_remove_lstspace(char *s);
void	rev_tm_commands(t_minishell	*t_m);
void	print_commands(t_minishell *t_m);
void	appyl_space_removal(t_minishell *t_m);

// ENV
char	*get_env(char **env);
char	*get_path(char *cmd, char **env);
int		env_init(t_minishell *t_m, char **argv);

// PARSING
int		parse_commands(t_minishell *t_m, token **cmd_tokens);
int		parse_tokens(char *cmd, token **cmd_tokens);

// EXECUTOR
void	exec_cmds(t_minishell *t_m);



// BUILT-IN
int		f__pwd(void);
int		f__env(t_env *env);
int		f__echo(char **args);
int		f__unset(t_minishell *t_m);
int		is_builtin(char *c);
void	run_builtin(t_minishell *t_m, int n_builtin);



#endif
