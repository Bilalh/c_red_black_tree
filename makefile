CC     = clang
# CFLAGS = -g -Wall -O2 -fno-omit-frame-pointer -fsanitize=address
CFLAGS = -g -Wall -O2 -fno-omit-frame-pointer
LIBS   = -lm
objs   = rb_example rb_array
run    = rb_array

build: ${objs}

run: build
	./bin/${rb_example}

rb_array: rb_array.o rb_node.o rb_print.o
rb_example: rb_example.o rb_node.o rb_print.o

${objs}:
	${CC} ${CFLAGS} $(addprefix bin/,$^)  -o bin/$@ ${LIBS}

%.o:%.c
	${CC} ${CFLAGS} -c -o bin/$@ $^


.PHONY: clean
clean:
	rm -f bin/*.o bin/${objs}

valgrind: build
	valgrind  ./bin/${run} 2>&1 | tee logfile.log

valgrind_full: build
	valgrind --leak-check=full  ./bin/${run}  2>&1 | tee logfile.log