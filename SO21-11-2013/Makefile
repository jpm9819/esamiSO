all: chat

WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

chat: Makefile chat.c
	$(CC) -o chat -lpthread $(WARNINGS) $(DEBUG) $(OPTIMIZE) chat.c

clean:
	rm -f chat

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run:
	./chat

