%{
  #include <string>

  #include "cc/parser/scanner.h"

  // yyFlexScanner implementation
  #undef YY_DECL
  #define YY_DECL int Parser::Scanner::yylex(\
    Parser::Parser::semantic_type * const lval, \
    Parser::Parser::location_type *location)

  // Additional parameters
  #define YY_USER_ACTION loc->step(); loc->columns(yyleng);
%}