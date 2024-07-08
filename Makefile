CC := gcc
CFLAGS := #-Wextra -Wall
INCLUDE := include
SRCS := src/main.c src/parser.c src/handler.c src/server.c src/utils.c
EXE := bin/live


compile:
	$(CC) -o $(EXE) $(SRCS) -I $(INCLUDE) $(CFLAGS)

run:
	@./$(EXE)
