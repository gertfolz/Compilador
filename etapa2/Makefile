# Gert Willem Folz - 00288550 
# Trabalho 2 da disciplina de compiladores 

etapa2: y.tab.c lex.yy.c
	gcc -o etapa2 lex.yy.c
y.tab.c: parser.y
	yacc -d parser.y
lex.yy.c: scanner.l
	lex scanner.l

clean: 
	rm lex.yy.c y.tab.c etapa2