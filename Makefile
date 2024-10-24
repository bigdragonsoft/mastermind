name = mastermind
objs = mastermind.o
objects = $(objs)
opt = -Wall -std=c99 -O2

all: $(name)

$(name): $(objects)
	cc $(opt) -o $(name) $(objects)

mastermind.o: mastermind.c
	cc $(opt) -c mastermind.c -o mastermind.o

clean:
	rm -f $(objects) $(name)

run: $(name)
	./$(name)