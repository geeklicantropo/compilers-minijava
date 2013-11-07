#include <iostream>
#include "miniJava.h"

extern int yylex( void );
extern int yyparse( const IProgram*& );

int main()
{
	const IProgram* progr = 0;
	yyparse( progr );
	progr->Accept( new CInterpreter() );
	return 0;
}