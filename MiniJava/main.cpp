#include <iostream>
#include "miniJava.h"

extern int yylex( void );
extern int yyparse( void );

int main()
{
	
	yyparse(  );
	return 0;
}