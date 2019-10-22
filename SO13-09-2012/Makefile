all: main

WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

main: Makefile main.c
	$(CC) -o main $(WARNINGS) $(DEBUG) $(OPTIMIZE) main.c common.h priorityBuffer/priorityBuffer.h priorityBuffer/priorityBuffer.c monitor/monitor.h monitor/monitor.c

clean:
	rm -f main

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run:
	./main

