typedef struct s_token
{
	char *str;
	char *type; //word, command, pipe, great, less, greatgreat, lessless 
} t_token;

typedef struct	s_info
{
	int d_quote;
	int s_quote;
	char first;
	t_list *head;
	t_token *tokens;
    char *result;
	int i;
	t_list *exp;
}				t_info;

#include "commands.c"
#include "tokens.c"

char *input;
typedef void (*sighandler_t)(int);