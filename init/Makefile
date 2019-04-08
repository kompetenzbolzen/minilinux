CC = gcc
CFLAGS =
LDFLAGS = --static
OUTPUT = init.elf

SRC = init.c

build: init.o
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(OUTPUT)

.PHONY: copy clean

clean:
	rm -df $(OUTPUT)

copy:
	objcopy -O binary $(OUTPUT) ../root/init
