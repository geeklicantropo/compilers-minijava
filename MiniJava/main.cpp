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
	ofstream assemout( "assem1.txt" );
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

		std::map<const Temp::CTemp*, bool> onStack;


		CInterferenceGraph* inteferenceGraph = new CInterferenceGraph( afg.GetNodes(), &afg, cf->GetFrame(), onStack );
		inteferenceGraph->WriteGraph("interference" + to_string( i )  + ".txt", false, 0);
		int colorNum = 4;
		int step = 0;
		
		inteferenceGraph->SetColors(colorNum);
		inteferenceGraph->WriteGraph("interferenceColored" + to_string( i ) + "step" + to_string( step ) + ".txt", true, colorNum);

		while( !inteferenceGraph->IsColored(colorNum) ) {
			
			step++;
			instrList = inteferenceGraph->UpdateInstructionList( instrList, 4, cf->GetFrame(), afg );
			onStack = inteferenceGraph->GetOnStack();
			/*
			CodeGeneration::IInstructionList* tmp = instrList;
			while( tmp != NULL ) {
				assemout << tmp->GetInstr()->Format();
				tmp  = tmp -> GetNext();
			}

			assemout << "----" << endl;
			*/
			afg = AssemFlowGraph( instrList );
			inteferenceGraph = new CInterferenceGraph( afg.GetNodes(), &afg, cf->GetFrame(), onStack );
			inteferenceGraph->SetColors(colorNum);
			inteferenceGraph->WriteGraph("interferenceColored" + to_string( i ) + "step" + to_string( step ) + ".txt", true, colorNum);
		}
	
		//inteferenceGraph->WriteGraph("interferenceColored" + to_string( i )  + ".txt", true, colorNum);

		CodeGeneration::IInstructionList* instListhead = instrList;
		CodeGeneration::IInstructionList* prev = instrList;
		
		// ������ MOVE a <- a
		for( CodeGeneration::IInstructionList* curr = instListhead; curr != 0; curr = curr->GetNext() ) {
			const CodeGeneration::CMove* instr = dynamic_cast<const CodeGeneration::CMove*>( curr->GetInstr() );
			if( ( instr != 0 && inteferenceGraph->GetColorMap()[instr->GetDst()] == inteferenceGraph->GetColorMap()[instr->GetSrc()] ) ) {
				prev->SetNext( curr->GetNext() );
			} else {
				prev = curr;
			}
		}


		
		instrList = instListhead;
		while( instrList != NULL ) {
			assemout << instrList->GetInstr()->Format( inteferenceGraph->GetColorMap() );
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