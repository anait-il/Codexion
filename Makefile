CC = gcc
FILE = main.c helper.c parsing.c clean.c setup_coders.c

run :
	$(CC) $(FILE)

