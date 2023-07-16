##
## EPITECH PROJECT, 2023
## coconut test
## File description:
## test
##

SRC	=	ternaries.c	\
		aTerreZbi.c	\

OBJ	=	$(SRC:.c=.o)

NAME	=	a.out

CFLAGS	=	-Wall -Wextra

all:	$(NAME)

$(NAME):	$(OBJ)
	gcc -o $(NAME) $(OBJ) $(CFLAGS)

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)

re: fclean all
