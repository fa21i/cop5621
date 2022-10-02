# cop5621
PARSER
  yacc -d new_parser.y /
  lex lex.l /
  gcc lex.yy.c y.tab.c ast.c comp.c -o comp
  
  Reading in files to test
    ./comp < tests/correct\ programs/sample?(1/2/3).txt
    change any of the sample files to see possible syntax errors
  
    
  Drew worked on the lex edits, the grammar setup, and a portion of the rules and testing
  Faraz worked on more of the rules and implementations
