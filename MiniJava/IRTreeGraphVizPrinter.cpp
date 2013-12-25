#include "IRTreeGraphVizPrinter.h"
#include <iostream>

using namespace IRTree;

IRTreeGraphVizPrinter::IRTreeGraphVizPrinter( ofstream& _out ) :
	out( _out )
{
	constNum = nameNum = tempNum = binOpNum = memNum = callNum = eseqNum =
	moveNum = expNum = jumpNum = cjumpNum = seqNum = labelNum = expListNum = stmListNum = 0;
	out << endl;
}

void IRTreeGraphVizPrinter::Visit( const CConst& p )
{
	constNum++;
	out << "CONST" << constNum << "->" << "\"" << p.GetValue() << " " << constNum  << "\"" << endl;
}

void IRTreeGraphVizPrinter::Visit( const CName& p )
{
	nameNum++;
	out << "NAME" << nameNum << "->" << "\"" << p.GetLabel()->Name() << "\"" << endl;
}

void IRTreeGraphVizPrinter::Visit( const CTemp& p )
{
	tempNum++;
	out << "TEMP" << tempNum << "->" << "\"" << p.GetTemp()->Name() << " " << tempNum  << "\"" << endl;
}

void IRTreeGraphVizPrinter::Visit( const CBinOp& p )
{
	binOpNum++;
	st.push( "BINOP" + to_string( binOpNum ) );
	out << "BINOP" << binOpNum << "->";
	out << "\"";
	switch( p.GetBinOp() ) {
	case TBinOp::PLUS:
		out << " + ";
		break;
	case TBinOp::MINUS:
		out << " - ";
		break;
	case TBinOp::MUL:
		out << " * ";
		break;
	case TBinOp::DIV:
		out << " / ";
		break;
	case TBinOp::AND:
		out << " AND ";
		break;
	case TBinOp::OR:
		out << " OR ";
		break;
	case TBinOp::LSHIFT:
		out << " LSHIFT ";
		break;
	case TBinOp::RSHIFT:
		out << " RSHIFT ";
		break;
	case TBinOp::ARSHIFT:
		out << " ARSHIFT ";
		break;
	case TBinOp::XOR:
		out << " XOR ";
		break;
	default:
		break;
	}
	out << binOpNum << "\"";
	out << endl;
	out << st.top() << "->";
	p.GetLeft()->Accept( this );
	out << st.top() << "->";
	p.GetRight()->Accept( this );
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CMem& p )
{
	memNum++;
	st.push( "MEM" + to_string( memNum ) );
	out << st.top() << "->";
	p.GetExp()->Accept( this );
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CCall& p )
{
	callNum++;
	st.push( "CALL" + to_string( callNum ) );
	out << st.top() << "->";
	p.GetExp()->Accept( this );
	out << st.top() << "->";
	p.GetArgs()->Accept( this );
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CEseq& p )
{
	eseqNum++;
	st.push( "ESEQ" + to_string( eseqNum ) );
	out << st.top() << "->";
	p.GetStm()->Accept( this );
	out << st.top() << "->";
	p.GetExp()->Accept( this );
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CMove& p )
{
	moveNum++;
	st.push( "MOVE" + to_string( moveNum ) );
	out << st.top() << "->";
	p.GetDst()->Accept( this );
	out << st.top() << "->";
	p.GetSrc()->Accept( this );
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CExp& p )
{
	expNum++;
	st.push( "EXP" + to_string( expNum ) );
	out << st.top() << "->";
	p.GetExp()->Accept( this );
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CJump& p )
{
	jumpNum++;
	st.push( "JUMP" + to_string( jumpNum ) );
	out << st.top() << "->";
	const Temp::CLabelList* pwh( p.GetTargets() );
	out << pwh->Label()->Name() << endl;
	pwh = pwh->Next();
	while( pwh ) {
		out << st.top() << "->";
		out << pwh->Label()->Name() << endl;
		pwh = pwh->Next();
	}
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CCJump& p )
{
	cjumpNum++;
	st.push( "CJUMP" + to_string( cjumpNum ) );
	out << st.top() << "->";
	out << "\"";
	switch( p.GetRelop() ) {
	case TCJump::EQ:
		out << " == ";
		break;
	case TCJump::NE:
		out << " != ";
		break;
	case TCJump::LT:
		out << " < ";
		break;
	case TCJump::GT:
		out << " > ";
		break;
	case TCJump::LE:
		out << " <= ";
		break;
	case TCJump::GE:
		out << " >= ";
		break;
	case TCJump::ULT:
		out << " ULT ";
		break;
	case TCJump::ULE:
		out << " ULE ";
		break;
	case TCJump::UGT:
		out << " UGT ";
		break;
	case TCJump::UGE:
		out << " UGE ";
		break;
	}
	out << cjumpNum << "\"";
	out << endl;
	out << st.top() << "->";
	p.GetLeft()->Accept( this );
	out << endl;
	out << st.top() << "->";
	p.GetRight()->Accept( this );
	out << endl;
	out << st.top() << "->";
	out << p.GetTrueLabel()->Name() << endl;
	out << st.top() << "->";
	out << p.GetFalseLabel()->Name() << endl;
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CSeq& p )
{
	seqNum++;
	st.push( "SEQ" + to_string( seqNum ) );
	out << st.top() << "->";
	p.GetLeft()->Accept( this );
	out << endl;
	out << st.top() << "->";
	p.GetRight()->Accept( this );
	out << endl;
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CLabel& p )
{
	labelNum++;
	st.push( "LABEL" + to_string( labelNum ) );
	out << st.top() << "->";
	out << p.GetLabel()->Name() << endl;
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CExpList& p )
{
	expListNum++;
	st.push( "EXPLIST" + to_string( expListNum ) );
	out << st.top() << "->";
	p.GetExp()->Accept( this );
	out << endl;
	if( p.GetNext() != 0 ) {
		out << st.top() << "->";
		p.GetNext()->Accept( this );
		out << endl;
	}
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CStmList& p )
{
	stmListNum++;
	st.push( "STMLIST" + to_string( stmListNum ) );
	out << st.top() << "->";
	p.GetStm()->Accept( this );
	out << endl;
	if( p.GetNext() != 0 ) {
		out << st.top() << "->";
		p.GetNext()->Accept( this );
		out << endl;
	}
	st.pop();
}
