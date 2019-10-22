all: main

WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

main: Makefile main.c
	$(CC) -o $@ $(WARNINGS) $(DEBUG) $(OPTIMIZE) main.c shared/* hoare/*

clean:
	rm -f main

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run:
	./main

