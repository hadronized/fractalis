CC= gcc
CFLAGS= -W -Wall -g
CLIBS= -lSDL -lm
INCLUDE= -I../include
OBJ= main.o log.o mandelbrot.o
LIBNAME= fractalis

all: $(OBJ)
	$(CC) $^ -o $(LIBNAME) $(CFLAGS) $(CLIBS)

%.o: ../src/%.c
	$(CC) -c $< -o $@ $(INCLUDE) $(CFLAGS)

log.o: ../src/meta/log.c
	$(CC) -c $< -o $@ $(INCLUDE) $(CFLAGS)

clean:
	@rm -rf *.o $(LIBNAME)
