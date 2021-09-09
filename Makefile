all:		

			gcc -g $$LDFLAGS $$CPPFLAGS -L./libft -lft -ltermcap -lreadline heredoc.c commands.c main.c tokens.c bins.c replace_vars.c make_tokens.c delete_quotes.c unexpected_tokens.c define_types.c -fsanitize=address

fclean:	
			rm -rf *.o
			rm -rf a.out

re:			fclean all


.PHONY:		all clean fclean re
