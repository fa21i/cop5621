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
     VARIABLE = 258,
     CALL = 259,
     INT_FUNCTION = 260,
     BOOL_FUNCTION = 261,
     INT_PARAMETER = 262,
     BOOL_PARAMETER = 263,
     FUNID = 264,
     NUMBER = 265,
     NEGATIVE = 266,
     OPEN_BRACKET = 267,
     CLOSE_BRACKET = 268,
     FUNCTION_DEF = 269,
     GET_INT = 270,
     GET_BOOL = 271,
     EVAL = 272,
     PRINT = 273,
     LET = 274,
     IF = 275,
     NOT = 276,
     INT = 277,
     BOOL = 278,
     TRUE = 279,
     FALSE = 280,
     PLUS = 281,
     MINUS = 282,
     DIV = 283,
     MOD = 284,
     MULT = 285,
     IDENTIFIER = 286,
     NOT_DEFINED = 287,
     EQUAL = 288,
     LESS_THAN = 289,
     GREATER_THAN = 290,
     GREATER_EQUAL = 291,
     LESS_EQUAL = 292,
     AND = 293,
     OR = 294
   };
#endif
/* Tokens.  */
#define VARIABLE 258
#define CALL 259
#define INT_FUNCTION 260
#define BOOL_FUNCTION 261
#define INT_PARAMETER 262
#define BOOL_PARAMETER 263
#define FUNID 264
#define NUMBER 265
#define NEGATIVE 266
#define OPEN_BRACKET 267
#define CLOSE_BRACKET 268
#define FUNCTION_DEF 269
#define GET_INT 270
#define GET_BOOL 271
#define EVAL 272
#define PRINT 273
#define LET 274
#define IF 275
#define NOT 276
#define INT 277
#define BOOL 278
#define TRUE 279
#define FALSE 280
#define PLUS 281
#define MINUS 282
#define DIV 283
#define MOD 284
#define MULT 285
#define IDENTIFIER 286
#define NOT_DEFINED 287
#define EQUAL 288
#define LESS_THAN 289
#define GREATER_THAN 290
#define GREATER_EQUAL 291
#define LESS_EQUAL 292
#define AND 293
#define OR 294




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 19 "lab_yacc.y"
{int num; char* str; struct list* arg;}
/* Line 1529 of yacc.c.  */
#line 129 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

