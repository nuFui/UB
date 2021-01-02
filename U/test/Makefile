CC = gcc
CFLAGS = -g

BIN = UB/U/test/bin/
EXEC = UB/U/test/

SRC_COMMON = $(wildcard UB/U/src/common/*.c)
SRC_LEXER = $(wildcard UB/U/src/lexer/*.c)
SRC_PARSER = $(wildcard UB/U/src/parser/*.c)
SRC_MUNIT = UB/munit/munit.c
SRC = UB/U/test/parser_node_test.c

SRCS = $(SRC_COMMON) $(SRC_LEXER) $(SRC_PARSER) $(SRC_MUNIT) $(SRC)
OBJS = $(patsubst %.c, $(BIN)%.o, $(SRCS))

all: parser_node_test

parser_node_test: %: %.o

%: %.o
	$(CC) $^ -o $(EXEC)$@
	

%.o: SRCS
	$(CC) $(CFLAGS) -c $< -o $(BIN)$@


clean:
	rm -f $(BIN)*.o