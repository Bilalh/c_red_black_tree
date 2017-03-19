CC     = clang
# CFLAGS = -g -Wall -O2 -fno-omit-frame-pointer -fsanitize=address
CFLAGS = -g -Wall -O2 -fno-omit-frame-pointer
LIBS   = -lm
objs   = rb_usage rb_array
run    = rb_usage

build: ${objs}

run: build
	./bin/${run}

rb_array: rb_array.o rb_node.o rb_print.o
rb_usage: rb_usage.o rb_node.o rb_print.o

${objs}:
	${CC} ${CFLAGS} $(addprefix bin/,$^)  -o bin/$@ ${LIBS}

%.o:bin %.c
	${CC} ${CFLAGS} -c -o bin/$@ $(filter-out bin,$^)


.PHONY: clean
clean:
	rm -f bin/*.o bin/${objs}

valgrind: build
	valgrind  ./bin/${run} 2>&1 | tee logfile.log

valgrind_full: build
	valgrind --leak-check=full  ./bin/${run}  2>&1 | tee logfile.log

bin:
	mkdir -p bin