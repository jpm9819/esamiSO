all: main server client

WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

main: Makefile main.c
	$(CC) -o main $(WARNINGS) $(DEBUG) $(OPTIMIZE) main.c msg.h

server: Makefile server.c
	$(CC) -lpthread -o server $(WARNINGS) $(DEBUG) $(OPTIMIZE) server.c msg.h shared_buffer.h shared_buffer.c

client: Makefile client.c
	$(CC) -o client $(WARNINGS) $(DEBUG) $(OPTIMIZE) client.c msg.h

clean:
	rm -f main server client

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run:
	./main

