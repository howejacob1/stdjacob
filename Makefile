JOBS ?= $(shell nproc --all 2>/dev/null || nproc)
MAKEFLAGS += -j$(JOBS)

CC ?= gcc
OPT ?= -O2
CFLAGS ?= $(OPT) -Wall -Wextra -Werror -Wpedantic -Wshadow -Wdouble-promotion -Wformat=2 -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition -Wnull-dereference -Wwrite-strings -Wno-unused-result -std=c11 -ffunction-sections -fdata-sections -fPIC -MMD -MP

DEPFLAGS = -MMD -MP

LIB = libstdjacob.a
SRCS = $(filter-out test.c, $(wildcard *.c))
OBJS = $(SRCS:.c=.o)
TEST_ELF = test

all: $(LIB) $(TEST_ELF)

$(LIB): $(OBJS)
	ar rcs $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

$(TEST_ELF): test.c $(LIB)
	$(CC) $(CFLAGS) $< -L. -lstdjacob -o $@ -lm -lcrypto

clean:
	rm -f $(OBJS) $(LIB) $(TEST_ELF) *.d

-include $(OBJS:.o=.d)

.PHONY: all clean
