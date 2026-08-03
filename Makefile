NAME = codexion

CC = cc

FILE = main.c parsing_functions.c parsing.c clean.c coder_setup.c coder_routine.c coder_join.c dongle_utils.c priority_queue.c logs.c time.c monitor.c dongle_logic.c dongles_setup.c heap_utils.c monitor_routine.c

FLAGS = -pthread

OBJ = $(FILE:%.c=%.o)


all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME) && make -s clean
 
%.o: %.c codexion.h
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean all
