#include <iostream>
#include "miniJava.h"
#include "SymbolTable.h"
#include "SymbolTableBuilder.h"
#include "TypeChecker.h"
#include "Translator.h"
#include "IRPrinter.h"
#include "IRTreeGraphVizPrinter.h"
#include "Canon.h"
#include "TraceSchedule.h"
#include "CodeGeneration.h"
#include "FlowGraph.h"
#include "InterferenceGraph.h"

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
	ofstream out( "output.txt" );
	ofstream assemout( "assem.txt" );
	int i = 0;
	while( cf != 0 ) {
		++i;
		out << cf->GetFrame()->GetName()->Name() << endl;
		cout << cf->GetFrame()->GetName()->Name() << endl;
		
		const IRTree::IStatement* tmp =  DoStm( cf->GetIRTree() );
		//tmp->Accept( new IRTreeGraphVizPrinter( out, labels ) );
		IRTree::CStmList* tmpStm = Linearize( tmp );
		
		BasicBlocks* bb = new BasicBlocks( tmpStm );
		
		const CStmListList* block = bb->GetBlocks();
		out << "digraph G {" << endl;
		vector<string> labels;
		IRTreeGraphVizPrinter* gp = new IRTreeGraphVizPrinter( out, labels );
		while( block != 0 ) {
			out << "subgraph {" << endl;
			block->GetStm()->Accept( gp );
			out << "}";
			out << endl << endl;
			block = block->GetNext();
		}
		for( int i = 0; i < labels.size(); ++i ) {
			out << i << " [label=\"" << labels[i] << "\"]" << endl;
		}
		out << "}" << endl;
		labels.clear();
		TraceSchedule ts = TraceSchedule( bb );
		IRTree::CStmList* list = ts.stms;
		assemout << cf->GetFrame()->GetName()->Name() << endl << endl;
		
		CodeGeneration::CCodeGenerator cg = CodeGeneration::CCodeGenerator( cf->GetFrame(), ts.stms );	
		CodeGeneration::IInstructionList* instrList  = cg.GetHead();

		AssemFlowGraph afg( instrList );

		CInterferenceGraph* inteferenceGraph = new CInterferenceGraph( afg.GetNodes(), &afg );
		inteferenceGraph->WriteGraph("interference" + to_string( i )  + ".txt", false, 0);
		inteferenceGraph->SetColors(7);
		inteferenceGraph->WriteGraph("interferenceColored" + to_string( i )  + ".txt", true, 7);

		while( instrList != NULL ) {
			assemout << instrList->GetInstr()->Format();
			instrList  = instrList -> GetNext();
		}

		assemout << endl;
		
		//tmpStm->Accept( new IRTreeGraphVizPrinter( out, labels ) );
		out << "digraph trace {" << endl;
		ts.stms->Accept( new IRTreeGraphVizPrinter( out, labels ) );
		for( int i = 0; i < labels.size(); ++i ) {
			out << i << " [label=\"" << labels[i] << "\"]" << endl;
		}
		out << "}" << endl;
		
		cout << endl << endl;
		cf = cf->GetNext();
	}


	return 0;
}