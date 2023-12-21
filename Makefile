all: tp.o arvore.o
	@gcc tp.o arvore.o -o exe
	@rm -r tp.o arvore.o
tp.o: tp.c
	@gcc tp.c -c -Wall
arvore.o: arvore.c
	@gcc arvore.c -c -Wall
run:
	@./exe