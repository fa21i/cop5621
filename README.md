# cop5621
PARSER <br/>
  yacc -d new_parser.y <br/>
LEX <br/>
  lex lex.l <br/>
COMPILE <br/>
  gcc lex.yy.c y.tab.c ast.c comp.c -o comp <br/>
  
RUN <br/>
  Reading in files to test <br/>
    ./comp < tests/correct\ programs/sample?(1/2/3).txt <br/>
    change any of the sample files to see possible syntax errors <br/>
  
    
  Drew worked on the lex edits, the grammar setup, and a portion of the rules and testing <br/>
  Faraz worked on more of the rules and implementations <br/>
