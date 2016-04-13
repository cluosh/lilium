/*
 * Copyright (C) 2016  Michael Pucher (cluosh)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

/*
 * ------------------------------------------------------------
 * FLEX PART
 * ------------------------------------------------------------
 */

%option	noyywrap
%{  
  #include <stdlib.h>
  #include <stdio.h>
	
  /* Function prototypes */
  void yyerror(const char *txt);

  /* Global variables */
  int line = 1;  
%}

COMMENT		#.*
LINEFEED	\n
WHITESPACE	[\t ]|{LINEFEED}

%%
{LINEFEED}	line++;
{COMMENT}	{ }
{WHITESPACE}+	{ }
add		(void) printf("%c",0);
sub		(void) printf("%c",1);
mul		(void) printf("%c",2);
halt		(void) printf("%c%c%c",3,0,0);
load		(void) printf("%c%c%c",4,0,0);
store		(void) printf("%c%c%c",5,0,0);
r0		(void) printf("%c",0);
r1		(void) printf("%c",1);
.		yyerror(yytext);
%%

/*
 * ------------------------------------------------------------
 * C PART
 * ------------------------------------------------------------
 */

/**
 * @brief Quit on lexical error
 * @details Global variables: line
 * @param txt Matched error
 */
void yyerror(const char *txt)
{
  (void) fprintf(stderr, "LEXICAL ERROR: Line %d, \"%s\"\n", line, txt);
  exit(EXIT_FAILURE);
}

/**
 * @brief Main entry point
 * @return EXIT_SUCCESS, if no errors happened
 */
int main(void)
{
  yylex();
  return EXIT_SUCCESS;
}
