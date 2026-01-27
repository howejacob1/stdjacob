CC ?= cc
CFLAGS ?= -std=c11 -O2 -g -Wall -Wextra -Werror -Wpedantic \
          -Wshadow -Wdouble-promotion \
          -Wformat=2 \
          -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition \
          -Wnull-dereference -Wwrite-strings

TEST_BIN := test_stdjacob
TEST_SRC := test.c stdjacob.c

.PHONY: all test clean

all: test

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) stdjacob.h
	$(CC) $(CFLAGS) -o $@ $(TEST_SRC)

clean:
	rm -f $(TEST_BIN)
