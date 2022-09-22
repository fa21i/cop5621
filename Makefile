CC	= gcc
YACC= yacc
LEX	= lex

comp:	y.tab.c lex.yy.c ast.c comp.c
	$(CC) lex.yy.c y.tab.c ast.c comp.c -o comp

y.tab.c: YOUR_yacc.y
	$(YACC) -d YOUR_yacc.y

lex.yy.c: YOUR_lex.l y.tab.h
	$(LEX) YOUR_lex.l

clean: 
	rm comp lex.yy.c y.tab.c y.tab.h
