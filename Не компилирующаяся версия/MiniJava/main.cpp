#include <iostream>

extern int yylex( void );
extern int yyparse( void );

int main()
{
	yyparse();
	return 0;
}