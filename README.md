# cop5621
PARSER
  yacc -d new_parser.y
  lex lex.l
  gcc y.tab.c y.tab.h -ll
  
  Reading in files!
    ./a.out < tests/correct\ programs/sample?.txt
    ./a.out < tests/lex\ errors/sample?.txt
    
  Drew worked on the lex edits, the grammar setup, and a portion of the rules and testing
  Faraz worked on more of the rules and implementations
