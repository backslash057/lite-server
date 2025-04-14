CC := gcc
CFLAGS := #-Wextra -Wall
INCLUDE := include
SRCS := src/config.c src/main.c src/request.c src/server.c src/utils.c
EXE := bin/lite

compile:
	@$(CC) -o $(EXE) $(SRCS) -I $(INCLUDE) $(CFLAGS)


test:
	@$(CC) -o $(EXE) src/config.c -I $(INCLUDE) $(CFLAGS)
	@./$(EXE)


run: compile
	@./$(EXE)
