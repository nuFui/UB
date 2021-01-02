CC = gcc
CFLAGS = -g

SRC_COMMON = $(wildcard U/src/common/*.c)
SRC_LEXER = $(wildcard U/src/lexer/*.c)
SRC_PARSER = $(wildcard U/src/parser/*.c)
SRC_MUNIT = U/src/test/U/test/munit/munit.c
SRC = U/src/test/parser_node_test.c

SRCS = $(SRC_COMMON) $(SRC_LEXER) $(SRC_PARSER) $(SRC_MUNIT) $(SRC)

BIN = U/src/test/bin/

all: parser_node_test

parser_node_test: %: %.o


%: %.o
	$(CC) $^ U/src/test/$@
	

%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $(BIN)$@


clean:
	rm -f $(BIN)*.o