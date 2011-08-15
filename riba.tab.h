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
     R_NEWLINE = 258,
     R_NUMBER = 259,
     R_BYTES = 260,
     R_QWORD = 261,
     R_COMMA = 262,
     R_SQUOTE = 263,
     R_EQ = 264,
     R_CMD_OPEN = 265,
     R_CMD_CLOSE = 266,
     R_CMD_GET = 267,
     R_CMD_PUT = 268,
     R_CMD_DELETE = 269,
     R_CMD_BATCH = 270,
     R_CMD_COMMIT = 271,
     R_CMD_SNAP = 272,
     R_CMD_UNSNAP = 273,
     R_CMD_PRINT = 274,
     R_CMD_COUNT = 275,
     R_CMD_HELP = 276,
     R_CMD_ABOUT = 277,
     R_CMD_EXIT = 278
   };
#endif
/* Tokens.  */
#define R_NEWLINE 258
#define R_NUMBER 259
#define R_BYTES 260
#define R_QWORD 261
#define R_COMMA 262
#define R_SQUOTE 263
#define R_EQ 264
#define R_CMD_OPEN 265
#define R_CMD_CLOSE 266
#define R_CMD_GET 267
#define R_CMD_PUT 268
#define R_CMD_DELETE 269
#define R_CMD_BATCH 270
#define R_CMD_COMMIT 271
#define R_CMD_SNAP 272
#define R_CMD_UNSNAP 273
#define R_CMD_PRINT 274
#define R_CMD_COUNT 275
#define R_CMD_HELP 276
#define R_CMD_ABOUT 277
#define R_CMD_EXIT 278




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

