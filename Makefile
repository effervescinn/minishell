all:		

			gcc -g $$LDFLAGS $$CPPFLAGS -L./libft -lft -ltermcap -lreadline heredoc.c commands.c main.c tokens.c bins.c -fsanitize=address

fclean:	
			rm -rf *.o
			rm -rf a.out

re:			fclean all


.PHONY:		all clean fclean re
