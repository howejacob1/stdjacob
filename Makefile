CC ?= cc
CFLAGS ?= -std=c2x -O2 -g \
          -Wall -Wextra -Werror -Wpedantic \
          -Wconversion -Wsign-conversion -Warith-conversion \
          -Wshadow -Wdouble-promotion \
          -Wformat=2 -Wundef \
          -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition \
          -Wnull-dereference -Wwrite-strings -Wcast-qual -Wswitch-enum \
          -Wduplicated-cond -Wduplicated-branches -Wlogical-op \
          -Wrestrict -Wnested-externs -Wjump-misses-init \
          -Wbad-function-cast -Wcast-align -Wfloat-equal \
          -Wvla -Walloca \
          -Wredundant-decls -Wstrict-overflow=5 -Wpointer-arith \
          -Wstringop-overflow -Wstringop-truncation -Winit-self \
          -fstack-protector-strong \
          -D_FORTIFY_SOURCE=2

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
