all: so04-03-2011

WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

so04-03-2011: Makefile so04-03-2011.c
	$(CC) -o $@ -lpthread $(WARNINGS) $(DEBUG) $(OPTIMIZE) so04-03-2011.c procedure.h procedure.c

clean:
	rm -f so04-03-2011

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run:
	./so04-03-2011

