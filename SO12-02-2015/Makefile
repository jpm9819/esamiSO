all: so12-02-2015

WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O2

so12-02-2015: Makefile so12-02-2015.c
	$(CC) -o $@ -lpthread $(WARNINGS) $(DEBUG) $(OPTIMIZE) so12-02-2015.c monitor.h monitor.c 

clean:
	rm -f so12-02-2015

# Builder will call this to install the application before running.
install:
	echo "Installing is not supported"

# Builder uses this target to run your application.
run:
	./so12-02-2015

