/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     NAME = 258,
     CONST = 259,
     BOOLCONST = 260,
     MULTOP = 261,
     COMPARATOR = 262,
     ADDOP = 263,
     MINOP = 264,
     DEFINE = 265,
     FUNCTION = 266,
     BOOLOP = 267,
     IF = 268,
     LET = 269,
     TYPE = 270,
     PRINT = 271,
     RPAREN = 272,
     LPAREN = 273,
     NOT = 274
   };
#endif
/* Tokens.  */
#define NAME 258
#define CONST 259
#define BOOLCONST 260
#define MULTOP 261
#define COMPARATOR 262
#define ADDOP 263
#define MINOP 264
#define DEFINE 265
#define FUNCTION 266
#define BOOLOP 267
#define IF 268
#define LET 269
#define TYPE 270
#define PRINT 271
#define RPAREN 272
#define LPAREN 273
#define NOT 274




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 12 "new_parser.y"
{int val; char* str;}
/* Line 1529 of yacc.c.  */
#line 89 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

