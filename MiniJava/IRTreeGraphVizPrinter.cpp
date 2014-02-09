#include "IRTreeGraphVizPrinter.h"
#include <iostream>

using namespace IRTree;

IRTreeGraphVizPrinter::IRTreeGraphVizPrinter( ofstream& _out, vector<string>& _labels ) :
	out( _out ), labels( _labels )
{
	id = 0;
	out << endl;
}

void IRTreeGraphVizPrinter::Visit( const CConst& p )
{
	out << id++ << "->";
	labels.push_back( "CONST" );
	out << id++ << endl;
	labels.push_back( to_string(p.GetValue()) );
}

void IRTreeGraphVizPrinter::Visit( const CName& p )
{
	out << id++ << "->";
	labels.push_back( "NAME" );
	out << id++ << endl;
	labels.push_back( p.GetLabel()->Name() );
}

void IRTreeGraphVizPrinter::Visit( const CTemp& p )
{
	out << id++ << "->";
	labels.push_back( "TEMP" );
	out << id++ << endl;
	labels.push_back( p.GetTemp()->Name() );
}

void IRTreeGraphVizPrinter::Visit( const CBinOp& p )
{
	st.push( id++ );
	out << id - 1 << "->";
	labels.push_back( "BINOP" );
	switch( p.GetBinOp() ) {
	case TBinOp::PLUS:
		out << id++;
		labels.push_back( " + " );
		break;
	case TBinOp::MINUS:
		out << id++;
		labels.push_back( " - " );
		break;
	case TBinOp::MUL:
		out << id++;
		labels.push_back( " * " );
		break;
	case TBinOp::DIV:
		out << id++;
		labels.push_back( " / " );
		break;
	case TBinOp::AND:
		out << id++;
		labels.push_back( "AND" );
		break;
	case TBinOp::OR:
		out << id++;
		labels.push_back( "OR" );
		break;
	case TBinOp::LSHIFT:
		out << id++;
		labels.push_back( "LSHIFT" );
		break;
	case TBinOp::RSHIFT:
		out << id++;
		labels.push_back( "RSHIFT" );
		break;
	case TBinOp::ARSHIFT:
		out << id++;
		labels.push_back( " ARSHIFT " );
		break;
	case TBinOp::XOR:
		out << id++;
		labels.push_back( "XOR" );
		break;
	default:
		break;
	}
	out << endl;
	out << st.top() << "->";
	p.GetLeft()->Accept( this );
	out << st.top() << "->";
	p.GetRight()->Accept( this );
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CMem& p )
{
	st.push( id++ );
	labels.push_back( "MEM" );
	out << st.top() << "->";
	p.GetExp()->Accept( this );
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CCall& p )
{
	st.push( id++ );
	labels.push_back( "CALL" );
	out << st.top() << "->";
	p.GetExp()->Accept( this );
	out << st.top() << "->";
	p.GetArgs()->Accept( this );
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CEseq& p )
{
	st.push( id++ );
	labels.push_back( "ESEQ" );
	out << st.top() << "->";
	p.GetStm()->Accept( this );
	out << st.top() << "->";
	p.GetExp()->Accept( this );
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CMove& p )
{
	st.push( id++ );
	labels.push_back( "MOVE" );
	out << st.top() << "->";
	p.GetDst()->Accept( this );
	out << st.top() << "->";
	p.GetSrc()->Accept( this );
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CExp& p )
{
	st.push( id++ );
	labels.push_back( "EXP" );
	out << st.top() << "->";
	p.GetExp()->Accept( this );
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CJump& p )
{
	st.push( id++ );
	labels.push_back( "JUMP" );
	out << st.top() << "->";
	const Temp::CLabelList* pwh( p.GetTargets() );
	out << id++ << endl;
	labels.push_back( pwh->Label()->Name() );
	pwh = pwh->Next();
	while( pwh ) {
		out << st.top() << "->";
		out << id++ << endl;
		labels.push_back( pwh->Label()->Name() );
		pwh = pwh->Next();
	}
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CCJump& p )
{

	st.push( id++ );
	labels.push_back( "CJUMP" );
	out << st.top() << "->";
	switch( p.GetRelop() ) {
	case TCJump::EQ:
		out << id++;
		labels.push_back( " == " );
		break;
	case TCJump::NE:
		out << id++;
		labels.push_back( " != " );
		break;
	case TCJump::LT:
		out << id++;
		labels.push_back( " < " );
		break;
	case TCJump::GT:
		out << id++;
		labels.push_back( " > " );
		break;
	case TCJump::LE:
		out << id++;
		labels.push_back( " <= " );
		break;
	case TCJump::GE:
		out << id++;
		labels.push_back( " >= " );
		break;
	case TCJump::ULT:
		out << id++;
		labels.push_back( " ULT " );
		break;
	case TCJump::ULE:
		out << id++;
		labels.push_back( " ULE " );
		break;
	case TCJump::UGT:
		out << id++;
		labels.push_back( " UGT " );
		break;
	case TCJump::UGE:
		out << id++;
		labels.push_back( " UGE " );
		break;
	}
	out << endl;
	out << st.top() << "->";
	p.GetLeft()->Accept( this );
	out << endl;
	out << st.top() << "->";
	p.GetRight()->Accept( this );
	out << endl;
	out << st.top() << "->";
	out << id++ << endl;
	labels.push_back( p.GetTrueLabel()->Name() );
	out << st.top() << "->";
	out << id++ << endl;
	labels.push_back( p.GetFalseLabel()->Name() );
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CSeq& p )
{
	st.push( id++ );
	labels.push_back( "SEQ" );
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
	st.push( id++ );
	labels.push_back( "LABEL" );
	out << st.top() << "->";
	out << id++ << endl;
	labels.push_back( p.GetLabel()->Name() );
	st.pop();
}

void IRTreeGraphVizPrinter::Visit( const CExpList& p )
{
	st.push( id++ );
	labels.push_back( "EXPLIST" );
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
	st.push( id++ );
	labels.push_back( "STMLIST" );
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