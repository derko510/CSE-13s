EXECBIN  = mathlib-test
CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra
LIBFLAGS = -lm
OBJS = e.o bbp.o euler.o madhava.o newton.o viete.o mathlib-test.o

.PHONY: all clean format

all: $(EXECBIN)

$(EXECBIN): $(OBJS)
	$(CC) -o $@ $^ $(LIBFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXECBIN) $(OBJS)

format:
	clang-format -i -style=file *.[ch]

