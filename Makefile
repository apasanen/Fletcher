
all: fletcher

CFLAGS=-O3 -Wall -pedantic

rand.bin:
	dd if=/dev/urandom bs=128 count=1 of=$@

rand.f16:rand.bin
	./fletcher rand.bin > $@

.PHONY: clean test

test:fletcher rand.bin rand.f16
	./fletcher rand.bin > test.f16
	diff test.f16 rand.f16

clean:
	$(RM) test.f16 fletcher
