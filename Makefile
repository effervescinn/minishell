all:		
			gcc $$LDFLAGS $$CPPFLAGS -L./libft -lft -ltermcap -lreadline commands.c main.c bins.c tokens.c


fclean:	
			rm -rf *.o
			rm -rf a.out

re:			fclean all


.PHONY:		all clean fclean re