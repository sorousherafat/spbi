PREFIX ?= /usr/local
CC ?= gcc
CFLAGS := -Iinclude/ -Wall -Wextra -Werror -g
LEX := flex
YACC := bison -d

BINDIR := $(PREFIX)/bin
SRCDIR := src
INCDIR := include
BUILDDIR := build

SRC := $(filter-out $(SRCDIR)/lexer.c $(SRCDIR)/parser.c, $(wildcard $(SRCDIR)/*.c))
GEN := $(SRCDIR)/lexer.c $(SRCDIR)/parser.c
ALL_SRC := $(SRC) $(GEN)
OBJS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(ALL_SRC))
EXEC := $(BUILDDIR)/spbi

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRCDIR)/parser.c $(SRCDIR)/parser.h: $(SRCDIR)/parser.y
	$(YACC) -o $(SRCDIR)/parser.c $(SRCDIR)/parser.y

$(SRCDIR)/lexer.c: $(SRCDIR)/lexer.l $(SRCDIR)/parser.h
	$(LEX) -o $(SRCDIR)/lexer.c $(SRCDIR)/lexer.l

clean:
	rm -f $(EXEC) $(BUILDDIR)/* $(SRCDIR)/parser.c $(SRCDIR)/parser.h $(SRCDIR)/lexer.c

install: $(EXEC)
	install -d $(BINDIR)
	install -m 0755 $(EXEC) $(BINDIR)

.PHONY: all clean install
