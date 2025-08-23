#
# UFRGS - Compiladores B - Marcelo Johann - 2009/2 - Etapa 1
#
# Makefile for three compiler calls
# Read the comments on Makefile2. All of them apply here too.
# But now the hash table is compiled in a separate gcc call
# Therefore, there must be a header of it to be included in scanner.l
#

lexical_scanner: lex.yy.o main.o hash.o
	gcc -o lexical_scanner lex.yy.o main.o hash.o

main.o: main.c
	gcc -c main.c

hash.o: hash.c
	gcc -c hash.c

lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

lex.yy.c: scanner.l
	flex --header-file=lex.yy.h scanner.l 
clean:
	rm *.o lex.yy.c lexical_scanner

remake: clean lexical_scanner

build: clean
		tar cvzf lexical_scanner.tgz .