CC = gcc
CFLAGS = -g
LDFLAGS = -lm

# Not permanent
export UROOT = $(shell pwd)
export UBUILD = $(UROOT)/U/build
export UBIN = $(UROOT)/U/bin

SOURCES := $(shell find $(UROOT)/U/src -name *.c)
OBJECTS = $(addprefix $(UBUILD)/,$(notdir $(SOURCES:%.c=%.o)))
BINARY = $(UBIN)/U

collect_objects:
	+$(MAKE) -C $(UROOT)/U/src/common
	+$(MAKE) -C $(UROOT)/U/src/lexer
	+$(MAKE) -C $(UROOT)/U/src/parser
	$(MAKE) $(BINARY)

all: collect_objects
	$(info Completed linking of $(shell pwd))

$(BINARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(BINARY)

clean:
	rm -f $(OBJECTS) $(BINARY)