CC=gcc
SOURCES=search warmup
FILES := $(subst .c,,$(SOURCES))
FLAGS=-g -Wall -Wvla -Werror -Wno-unused-variable -Wno-unused-but-set-variable

# By default, make runs the first target in the file
all: $(FILES)

% :: %.c shell.c
	$(CC) $(FLAGS) $< -o $@ -lm -lreadline

clean:
	rm -rf $(FILES)

