CC ?= cc
CFLAGS ?= -std=c2x -Og -g \
          -Wall -Wextra -Werror -Wpedantic \
          -Wconversion -Wno-sign-conversion \
          -Wshadow -Wdouble-promotion \
          -Wformat=2 -Wformat-overflow=2 -Wformat-truncation=2 -Wundef \
          -Wstrict-prototypes -Wmissing-prototypes -Wold-style-definition \
          -Wnull-dereference -Wwrite-strings -Wcast-qual -Wswitch-enum \
          -Wduplicated-cond -Wduplicated-branches -Wlogical-op \
          -Wrestrict -Wnested-externs -Wjump-misses-init \
          -Wbad-function-cast -Wcast-align -Wfloat-equal \
          -Wvla -Walloca \
          -Wredundant-decls -Wstrict-overflow=5 -Wpointer-arith \
          -Wstringop-overflow -Wstringop-truncation -Winit-self \
          -Wunsuffixed-float-constants \
          -Warray-bounds=2 -Wstrict-aliasing=3 \
          -fstack-protector-strong -fstack-clash-protection \
          -fanalyzer \
          -fsanitize=address,undefined,leak -fno-omit-frame-pointer \
          -D_FORTIFY_SOURCE=2

LDFLAGS ?= -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now \
           -fsanitize=address,undefined,leak

TEST_BIN := test_stdjacob
TEST_SRC := test.c stdjacob.c base64.c

.PHONY: all test clean

all: test

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(TEST_SRC) stdjacob.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(TEST_SRC)

clean:
	rm -f $(TEST_BIN)
