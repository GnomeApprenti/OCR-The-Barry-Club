# Makefile

CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -O3 `pkg-config --cflags sdl2 SDL2_image`
LDFLAGS =
LDLIBS = `pkg-config --libs sdl2 SDL2_image` -lm


SRC = line_detection.c
OBJ = ${SRC:.c=.o}
EXE = ${SRC:.c=.}

all: line_detection

line_detection = {OBJ}


.PHONY: clean


clean:
	${RM} ${OBJ}
	${RM} ${EXE}

# END