all:		
			gcc $$LDFLAGS $$CPPFLAGS -L./libft -lft -ltermcap -lreadline export.c heredoc.c commands.c main.c tokens.c bins.c -g -fsanitize=address


fclean:	
			rm -rf *.o
			rm -rf a.out

re:			fclean all


.PHONY:		all clean fclean re
