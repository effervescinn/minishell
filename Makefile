all:		

			gcc -Wall -Wextra -Werror -g $$LDFLAGS $$CPPFLAGS -L./libft -lft -ltermcap -lreadline main2.c preparsing.c utils.c free.c errors.c redirects1.c redirects.c export5.c export4.c export3.c export2.c export1.c exit.c unset.c cd.c env_pwd.c echo.c export.c heredoc.c commands.c main.c parser.c bins.c replace_vars.c make_tokens.c make_tokens_utils.c delete_quotes.c unexpected_tokens.c define_types.c program_define.c exec_fork.c no_print.c dollar.c count_quotes.c less_command_args.c #-fsanitize=address

fclean:	
			rm -rf *.o
			rm -rf a.out

re:			fclean all


.PHONY:		all clean fclean re
