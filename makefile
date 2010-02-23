PROGRAMS= proj1.out

CC=gcc

CFLAGS=-Wall -O3

RM=rm -f

.PHONY: all
all: $(PROGRAMS:%=%)

.PHONY: assemble
assemble: $(PROGRAMS:%=%.s)

.PHONY: clean
clean:
	-$(RM) $(PROGRAMS:%=%.s) $(PROGRAMS:%=%)

%.s: %.c ; $(CC) $(CFLAGS) -S -o $@ $<

%.out: %.c ; $(CC) $(CFLAGS) -o $@ $<
