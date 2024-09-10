INC_DIR = inc
INCLUDES = $(wildcard $(INC_DIR)/*.inc)

astreus: astreus.o
	ld -o astreus astreus.o

astreus.o: $(INCLUDES) astreus.s
	nasm -felf64 -o astreus.o astreus.s -g -w+all -I$(INC_DIR)/

run:
	nasm -felf64 -o astreus.o astreus.s -g -w+all -I$(INC_DIR)/
	ld -o astreus astreus.o
	./astreus

clean:
	rm -f astreus astreus.o

test: test.c
	gcc -o test test.c
	./test

