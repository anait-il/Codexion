NAME = codexion

CC = cc

FILE = main.c atoi_function.c parsing.c clean.c coders.c dongles.c priority_queue.c logs.c time.c

FLAGS = 

OBJ = $(FILE:%.c=%.o)


all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) && make clean
 
%.o: %.c codexion.h
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
