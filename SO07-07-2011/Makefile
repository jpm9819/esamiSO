all: so07-07-2011

WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

so07-07-2011: Makefile so07-07-2011.c
	$(CC) -o $@ -lpthread $(WARNINGS) $(DEBUG) $(OPTIMIZE) so07-07-2011.c stack.h stack.c

clean:
	rm -f so07-07-2011

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run:
	./so07-07-2011

