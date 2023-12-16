CC = gcc
CFLAGS = -Wall #-Werror
LDFLAGS = -lm

SRCDIR = src
BINDIR = bin

TARGET = $(BINDIR)/main
TEST = $(BINDIR)/test

all: $(TARGET) $(TEST)

test: $(TEST)

$(TARGET): $(filter-out $(SRCDIR)/test.c, $(wildcard $(SRCDIR)/*.c))
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(TEST): $(filter-out $(SRCDIR)/main.c, $(wildcard $(SRCDIR)/*.c))
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -r $(BINDIR)/main $(BINDIR)/test
