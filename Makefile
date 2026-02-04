CC ?= tcc
CFLAGS ?= -O2 -Wall -std=c11

OBJS = stdjacob.o utils.o wer.o base64.o

all: $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

.PHONY: all clean
