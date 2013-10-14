#include <iostream>

extern int yylex( void );
extern int yyparse( void );

int main()
{
	yyparse();
	system( "pause" );
	return 0;
}