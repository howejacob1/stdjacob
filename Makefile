CC ?= gcc
CFLAGS ?= -O2 -Wall -std=c11

DEPFLAGS = -MMD -MP

LIB = libstdjacob.a
OBJS = stdjacob.o utils.o wer.o base64.o
TEST_ELF = test

all: $(LIB) $(TEST_ELF)

$(LIB): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(TEST_ELF): test.c $(LIB)
	$(CC) $(CFLAGS) $< -L. -lstdjacob -o $@ -lm

clean:
	rm -f $(OBJS) $(LIB) $(TEST_ELF) *.d

-include $(OBJS:.o=.d)

.PHONY: all clean
