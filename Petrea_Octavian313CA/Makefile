CC=gcc
CFLAGS=-Wall -Wextra -std=c99

TARGETS = image_editor

build: $(TARGETS)

image_editor: image_editor.c image_functions.c
	$(CC) $(CFLAGS) image_editor.c image_functions.c -o image_editor -lm

pack:
	zip -FSr 313CA_PetreaOctavian_Proiect.zip README Makefile *.c *.h

clean:
	rm -f *.o $(TARGETS)

valgrind:
	valgrind --leak-check=full ./image_editor

build:
	$(CC) -g image_editor.c image_functions.c -o image_editor -lm

build_with_O3:
	$(CC) -O3 image_editor.c image_functions.c -o image_editor -lm

build_with_O0:
	$(CC) -O0 image_editor.c image_functions.c -o image_editor -lm

gprof_gcc:
	$(CC) -pg image_editor.c image_functions.c -o image_editor -lm

gprof_run:
	./image_editor
	gprof image_editor gmon.out > analysis.txt


.PHONY: pack clean
