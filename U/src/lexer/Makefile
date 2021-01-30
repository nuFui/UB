CC = gcc
CFLAGS = -g

# Using exported: UBUILD

SOURCES = $(wildcard *.c)
OBJECTS = $(addprefix $(UBUILD)/,$(notdir $(SOURCES:%.c=%.o)))

all: $(OBJECTS)
	$(info Completed compiling objects for $(shell pwd))

$(UBUILD)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@