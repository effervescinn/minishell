#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <term.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>
#include "libft/libft.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

typedef struct	s_token
{
	char *str;
	char type; //word, command, pipe, great, less, GREAT, LESS 
	char **args;
	int pipe;
	int print;
}				t_token;

typedef struct	s_info
{
	int d_quote;
	int s_quote;
	t_list *head;
	t_token *tokens;
    char *result;
	int i;
	int i2;
	t_list *exp;
	t_list *extra_exp;
	t_list *pths;
	t_list *pwd;
	t_list *oldpwd;
	int oldpwd_flag;
	t_list *heredoc;
	char **pths_array;
	char *str_pwd;
	char *str_oldpwd;
	int pipes_num;
}				t_info;

typedef struct s_global
{
	char *input;
	int f;
	int ex_status;
	char *prompt;
}				t_global;



extern int file;
extern t_global g_global;

typedef void (*sighandler_t)(int);


//main.c
int ft_putchar(int c);
void sig_int(int d);
void history(t_info *info);
void free_tokens(t_info *info);
void free_args(t_info *info);
void printf_tokens_err(int error);
int unexpected_tokens(t_token *tokens);


void make_paths(t_info *info);
void copy_pwds(t_info *info);
void free_paths_array(t_info *info);
void define_fd_out(t_info *info);
int define_fd_in(t_info *info);
int define_fd_built_in(t_info *info);
int count_files(t_info *info, int q);
int count_redir(t_info *info);
void opening_error(char *filename);
void search_heredoc(t_info *info);
char *heredoc_str(char *stop, char **buf, int *len, t_info *info);
void redirects_solo(t_info *info);
void unlink_files(t_info *info);
void opening_error_scnd(char *filename);


//commands.c
void extra_export(t_info *info, int a);
void swap_content(t_list *list1, t_list *list2);
void export_order(t_info *info, int i);
void make_exp(t_info *info);
char *no_leaks_join(char *str1, char *str2);
void echo(t_info *info);
void pwd(t_info *info);
void env(t_info *info);
char *add_quotes(char *str);
void print_exp_vars(t_info *info);
void no_quotes(char *str);
void find_existing_var(char *var_name, t_info *info, int a);
char *var_name_in_str(char *str, char *ptr_to_eq);
void export(t_info *info);
void program_define(t_info *info);
void set_pointers(t_info *info);
void new_pwd(t_info *info);
void exec_command(t_info *info, int pid);
void free_list(t_list **list);
int set_start(t_info *info);



//tokens.c
void make_env(char **envp, t_list **head);
int count_quotes(char *str);
char *vars(char **str, t_list *head);
void dollar(char **str, char **newstr, char **start, t_info *info);
char *replace_vars(char *str, t_info *info);
char **make_tokens(char *str);
void handle_token(char *tmp_token, t_token *token);
t_token *delete_quotes(char **tmp_arr);
void less_args(t_token *tokens, int i);
void define_types(t_info *info);
void command_args(t_token *tokens, int i);
void set_args(t_info *info);
void set_pipes(t_info *info);
void command_types(t_info *info);

//bins.c
char *find_bin(t_info *info);
