CC = gcc
FILE = main.c atoi_function.c parsing.c clean.c coders.c dongles.c priority_queue.c logs.c time.c

run :
	$(CC) $(FILE)
