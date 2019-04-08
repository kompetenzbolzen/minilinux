CC = gcc
CFLAGS =
LDFLAGS = --static
OUTPUT = init

SRC = init.c

build:
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(OUTPUT)

.PHONY: copy clean

clean:
	rm -df $(OUTPUT)

copy:
	cp $(OUTPUT) ../root/init
