CC ?= tcc
CFLAGS ?= -O2 -Wall -std=c11

OBJS = stdjacob.o utils.o wer.o base64.o
TEST_ELF = test

all: $(OBJS) $(TEST_ELF)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_ELF): test.c $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

clean:
	rm -f $(OBJS) $(TEST_ELF)

.PHONY: all clean
