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
{COMMENT}	;
{WHITESPACE}+	;
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
	(void) fprintf(stderr, "LEXICAL ERROR: Line %d, \"%s\""
		       "\n", line, txt);
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
