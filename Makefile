SRCS_DIR = srcs/
FILES = main2.c preparsing.c utils.c free.c errors.c redirects1.c redirects.c export5.c export4.c export3.c export2.c export1.c exit.c unset.c cd.c env_pwd.c echo.c export.c heredoc.c commands.c commands1.c main.c parser.c bins.c replace_vars.c make_tokens.c make_tokens_utils.c delete_quotes.c delete_quotes2.c unexpected_tokens.c define_types.c program_define.c exec_fork.c no_print.c dollar.c count_quotes.c less_command_args.c
CFILES = $(addprefix ${SRCS_DIR}, ${FILES})
OFILES = $(patsubst %.c,%.o,$(CFILES))
DEPS = $(patsubst %.o,%.d,$(OFILES))
DEPFLAGS = -MMD -MF $(@:.o=.d)
CC = gcc
NAME = minishell
FLAGS = -g -Wall -Wextra -Werror
READLINE = -lreadline 
FT_LIB = $(LIBFT_DIR)/libft.a
LIBFT_DIR = ./libft



all:		$(NAME)

$(NAME):	$(OFILES)
			make -C $(LIBFT_DIR)
			$(CC) $(FLAGS) $(OFILES)  $(READLINE) -L/Users/${USER}/.brew/opt/readline/lib -I/Users/${USER}/.brew/opt/readline/include $(FT_LIB) -o $(NAME)
-include $(DEPS)

%.o: 		%.c
			$(CC) $(FLAGS) -I/Users/${USER}/.brew/opt/readline/include -c $< $(DEPFLAGS) -o $(<:.c=.o)

clean:
			make clean -C $(LIBFT_DIR)
			rm -rf $(OFILES)
			rm -rf $(DEPS)

fclean:		clean
			make fclean -C $(LIBFT_DIR)
			rm -rf $(NAME) 

re:			fclean all


.PHONY:		all clean fclean re
