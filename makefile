CC	= gcc
YACC= yacc
LEX	= lex

comp:	y.tab.c lex.yy.c calc.c containers.c cfg2.c ssa.c bblock.c symbol.c optim.c assembly.c comp.c
	$(CC) lex.yy.c y.tab.c calc.c containers.c cfg2.c ssa.c bblock.c symbol.c optim.c assembly.c comp.c -o comp

y.tab.c: lab_yacc.y
	$(YACC) -d lab_yacc.y

lex.yy.c: lab_lex.l y.tab.h
	$(LEX) lab_lex.l

clean: 
	rm comp lex.yy.c y.tab.c y.tab.h
