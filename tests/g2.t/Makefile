##
## EPITECH PROJECT, 2023
## coconut test
## File description:
## test
##

SRC	=	separation.c not_error.c

OBJ =	$(SRC:.c=.o)

NAME	= separation

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ)

clean:
	$(RM) *.o

fclean: clean
	$(RM) $(NAME)

re: fclean all