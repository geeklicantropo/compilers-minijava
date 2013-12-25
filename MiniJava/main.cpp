#include <iostream>
#include "miniJava.h"
#include "SymbolTable.h"
#include "SymbolTableBuilder.h"
#include "TypeChecker.h"
#include "Translator.h"
#include "IRPrinter.h"
#include "IRTreeGraphVizPrinter.h"

extern int yylex( void );
extern int yyparse( const IProgram*&);

int main()
{
	const IProgram* progr = 0;
	CSymbolTable st;
	yyparse( progr );
	assert( progr != 0 );
	progr->Accept( new CInterpreter() );
	progr->Accept( new CSymbolTableBuilder( &st ) );
	progr->Accept( new CTypeChecker( &st ) );
	const CCodeFragment* cf = 0;
	progr->Accept( new Translator::CTranslator( &st, &cf ) );
	ofstream out("output.txt");
	while( cf != 0 ) {
		out << cf->GetFrame()->GetName()->Name() << endl;
		cout << cf->GetFrame()->GetName()->Name() << endl;
		cf->GetIRTree()->Accept( new IRTreeGraphVizPrinter( out ) );
		cf->GetIRTree()->Accept( new IRTreePrinter() );
		out << endl << endl;
		cout << endl << endl;
		cf = cf->GetNext();
	}
	return 0;
}