# cop5621
PARSER
  win_bison -d new_parser.y /
  win_flex lex.l /
  gcc lex.yy.c new_parser.tab.c
  
  Reading in files to test
    ./a.out tests/correct\ programs/sample?(1/2/3).txt
    change any of the sample files to see possible syntax errors
  
    
  Drew worked on the lex edits, the grammar setup, and a portion of the rules and testing
  Faraz worked on more of the rules and implementations
