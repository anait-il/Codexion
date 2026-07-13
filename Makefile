CC = gcc
FILE = main.c helper.c parsing.c clean.c coders.c dongles.c priority_queue.c

run :
	$(CC) $(FILE)

