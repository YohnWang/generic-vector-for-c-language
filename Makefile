CC=gcc

srcs=$(wildcard *.c)
objs=$(srcs:%.c=%.o)
cflags=-Wall -Wextra -fanalyzer $(default_includes) $(macros) $(optimization)
default_includes=
macros=-DDEBUG 
ldflags=
optimization=-Ofast

main.exe: $(objs)
	$(CC) $^ -o $@ $(ldflags)

%.o:%.c 
	$(CC) -c $(cflags) $< -o $@

sinclude $(srcs:.c=.d)

%.d: %.c
	$(CC) -MM $(cflags) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

.PHONY: clean run

run: main.exe 
	./main.exe

clean:
	find . '(' -name *.o -or -name *.d -or -name *.exe ')' -delete