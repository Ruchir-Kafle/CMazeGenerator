CC = gcc
CFLAGS = -g 
RAYLIB_FLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
FILES = main.c linkedList.c

default:
	$(CC) $(CFLAGS) $(FILES) -o main $(RAYLIB_FLAGS)

