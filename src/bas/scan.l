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
  #include <stdint.h>
	
  /* Function prototypes */
  void yyerror(const char *txt);
  void lex_number(const char *txt);

  /* Global variables */
  int line = 1;  
%}

COMMENT		#.*
LINEFEED	\n
WHITESPACE	[\t ]|{LINEFEED}
NUMBER		(0x[0-9a-fA-F]+)|[0-9]+

%%
{LINEFEED}	line++;
{COMMENT}	{ }
{WHITESPACE}+	{ }
{NUMBER}	lex_number(yytext);
nop		(void) printf("%c%c%c", 0, 0, 0);
halt		(void) printf("%c%c%c", 1, 0, 0);
add		(void) printf("%c", 2);
sub		(void) printf("%c", 3);
mul		(void) printf("%c", 4);
load		(void) printf("%c", 5);
loadv		(void) printf("%c", 6);
store		(void) printf("%c", 7);
r0		(void) printf("%c", 0);
r1		(void) printf("%c", 1);
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
 * @brief Convert number to decimal without leading zeros
 * @param txt Pointer to matched string
 */
void lex_number(const char *txt)
{
  char *end = NULL;
  errno = 0;
  uint64_t num = strtoll(txt, &end, 0);
  if (errno != 0 || end == NULL || end == txt)
    yyerror("Number-Error");
  (void) fprintf(stderr, "num: %llu\n", num);
  (void) fwrite(&num, sizeof(uint64_t), 1, stdout);
  (void) printf("%c%c", 0, 0);
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
