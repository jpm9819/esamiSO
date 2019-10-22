all: so-16.10.2014

WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

so-16.10.2014: Makefile so-16.10.2014.c
	$(CC) -lpthread -o $@ $(WARNINGS) $(DEBUG) $(OPTIMIZE) so-16.10.2014.c data_structure.c thread.c data_structure.h

clean:
	rm -f so-16.10.2014

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run:
	./so-16.10.2014

