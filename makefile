# My makefile

all: CATONUXMV STATE
	gcc source/main.c objects/caToNuXmv.o objects/state.o -o teste
CATONUXMV:
	gcc -c source/headers/caToNuXmv.c -o objects/caToNuXmv.o
STATE: 
	gcc -c source/headers/state.c -o objects/state.o
debug: CATONUXMV STATE
	gcc -g source/main.c objects/caToNuXmv.o objects/state.o -o teste
clean:
	rm -rf objects/*.o
