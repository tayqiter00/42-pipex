NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRC = main.c execution.c utils.c

OBJ =  $(SRC:.c=.o)

LIBFTDIR = libft/
LIBFT.A = $(LIBFTDIR)libft.a

all: $(NAME)

$(NAME): $(OBJ)
		make -C $(LIBFTDIR)
		$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -L$(LIBFTDIR) -lft

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
		rm -f $(OBJ)
		make clean -C $(LIBFTDIR)

fclean: clean
		rm -f $(NAME)
		make fclean -C $(LIBFTDIR)

re: fclean all