CC     = clang
# CFLAGS = -g -Wall -O2 -fno-omit-frame-pointer -fsanitize=address
CFLAGS = -g -Wall -O2 -fno-omit-frame-pointer
LIBS   = -lm
objs   = rb_example

build: ${objs}

run: build
	./bin/rb_example

rb_example: rb_example.o rb_node.o rb_print.o

${objs}:
	${CC} ${CFLAGS} $(addprefix bin/,$^)  -o bin/$@ ${LIBS}

%.o:%.c
	${CC} ${CFLAGS} -c -o bin/$@ $^


.PHONY: clean
clean:
	rm -f bin/*.o bin/${objs}

valgrind: build
	valgrind --leak-check=full --log-file=logfile.out ./bin/rb_example