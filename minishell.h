#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <signal.h>
# include <term.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>
# include "libft/libft.h"
# include <sys/stat.h>
# include <sys/types.h>
# include <fcntl.h>

typedef struct s_token
{
	char	*str;
	char	type;
	char	**args;
	int		pipe;
	int		print;
}				t_token;

typedef struct s_info
{
	int		d_quote;
	int		s_quote;
	t_list	*head;
	t_token	*tokens;
	char	*result;
	int		i;
	int		i2;
	t_list	*exp;
	t_list	*extra_exp;
	t_list	*pths;
	t_list	*pwd;
	t_list	*oldpwd;
	int		oldpwd_flag;
	t_list	*heredoc;
	char	**pths_array;
	char	*str_pwd;
	char	*str_oldpwd;
	int		pipes_num;
	int		*pids;
}				t_info;

typedef struct s_global
{
	char	*input;
	int		f;
	int		ex_status;
	char	*prompt;
	int		hd_ex;
}				t_global;

t_global	g_global;

typedef void	(*t_sighandler_t)(int);

int		ft_putchar(int c);
void	sig_int(int d);
void	history(t_info *info);
void	free_tokens(t_info *info);
void	free_args(t_info *info);
void	printf_tokens_err(int error);
int		unexpected_tokens(t_token *tokens);
void	make_paths(t_info *info);
void	copy_pwds(t_info *info);
void	free_paths_array(t_info *info);
void	define_fd_out(t_info *info);
int		define_fd_in(t_info *info);
int		define_fd_built_in(t_info *info);
int		count_files(t_info *info, int q);
int		count_redir(t_info *info);
void	opening_error(char *filename);
void	search_heredoc(t_info *info);
char	*heredoc_str(char *stop, char **buf, int *len, t_info *info);
void	redirects_solo(t_info *info);
void	unlink_files(t_info *info);
void	opening_error_scnd(char *filename);
void	files_to_unlink(t_info *info, char *filename);
void	extra_export(t_info *info, int a);
void	swap_content(t_list *list1, t_list *list2);
void	export_order(t_info *info, int i);
void	make_exp(t_info *info);
char	*no_leaks_join(char *str1, char *str2);
void	echo(t_info *info);
void	pwd(t_info *info);
void	env(t_info *info);
char	*add_quotes(char *str);
void	print_exp_vars(t_info *info);
void	no_quotes(char *str);
void	find_existing_var(char *var_name, t_info *info, int a);
char	*var_name_in_str(char *str);
void	export(t_info *info);
void	program_define(t_info *info);
void	set_pointers(t_info *info);
void	new_pwd(t_info *info);
void	exec_command(t_info *info, int pid);
void	free_list(t_list **list);
int		set_start(t_info *info);
char	*change_shlvl(char *str);
void	exit_ctrl_d(t_info *info);
void	exit_minishell(t_info *info);
void	unset(t_info *info);
void	cd(t_info *info);
void	remove_from_extra_exp(t_list **list, char *var);
void	compare_vars(t_list *tmp, t_list **list, t_list **prev);
int		check_unset_var(t_info *info, int a);
void	echo_print(t_info *info, int a, int n);
void	join_env_and_extra(t_info *info, t_list **exp_tmp, int *i);
void	remove_extra(t_list **tmp, t_list **list, t_list **prev);
void	one_equality(char *str, char **new, int i);
int		check_var_name(char *var_name);
char	*remove_eqs(char *str, char *ptr_to_eq);
void	find_and_join(char *ptr_to_eq, t_info *info, char *var, int a);
void	var_name_error(int *a, char **var_name, t_info *info);
int		plus_before_eq(char **ptr_to_eq, t_info *info, char **var_name, int a);
int		export_end(char **ptr_to_eq, t_info *info, char **var_name, int *a);
int		export_if_pwd(t_info *info, int *a, char **ptr_to_eq);
void	search_heredoc_end(t_info *info, char **filename, int fd, int i);
void	g_redirects_solo(t_info *info, int i);
void	found_g_redirect(int *fd, t_info *info, int i, int *flag);
char	**make_envp_arr(t_info *info);
void	start_of_line(t_info *info);
int		check_pipe(void);
char	*make_no_enters_string(char **closed_str, char **line);
int		unexpected_return(t_token *tokens, int i);
char	*close_pipe(char *line);
int		check_pipe_scnd(void);
void	make_env(char **envp, t_list **head);
int		count_quotes(char *str);
char	*vars(char **str, t_list *head);
void	dollar(char **str, char **newstr, char **start, t_info *info);
char	*replace_vars(char *str, t_info *info);
char	**make_tokens(char *str);
void	handle_token(char *tmp_token, char **str);
t_token	*delete_quotes(char **tmp_arr);
void	less_args(t_token *tokens, int i);
void	define_types(t_info *info);
void	command_args(t_token *tokens, int i);
void	set_args(t_info *info);
void	set_pipes(t_info *info);
void	command_types(t_info *info);
char	*find_bin(t_info *info);
void	check_syms(char **str, t_info *info, char **newstr, char **start);
void	s_quote(char **str, char **newstr, char **start);
void	question(char **str, char **newstr, char **start);
void	define_types(t_info *info);
void	define_command(t_info *info, int *i);
void	pipe_redir(char **str, char ***arr, char **start, int *i);
void	meet_quotes(char **str);
void	quotes_after(char **str);
void	finish_tokens(char **str, char ***arr, char **start, int *i);
void	define_greatless(t_info *info, int *i);
void	exec_printable(t_info *info, char *cmd);
void	exec_builtin(t_info *info);
int		no_print(t_info **info, int ***fd, int *k);
void	exec_fork(t_info **info, int ***fd, int **pids, int *k);
void	first_fork(t_info **info, char *cmd, int ***fd, int *k);
void	between_fork(t_info **info, char *cmd, int ***fd, int *k);
void	last_fork(t_info **info, char *cmd, int ***fd, int *k);
int		exec_printable_buildin_scnd(t_info *info);
int		exec_printable_buildin_frst(t_info *info);
char	**create_new_exec_array(t_info *info, int files, int smb);
void	add_from_the_first(t_info *info, int q, char **array, int *k);
int		check_flags(char **tmp_token, int *i, int *flag);
void	s_flag(char **tmp_token, char **str, int *i, int *j);
void	d_flag(char **tmp_token, char **str, int *i, int *j);
void	no_flags(char **tmp_token, char **str, int *i, int *j);

#endif