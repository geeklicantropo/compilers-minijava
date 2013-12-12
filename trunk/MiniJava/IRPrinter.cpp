#include "IRPrinter.h"
#include <iostream>

void IRTreePrinter::Visit( const CConst& p )
{
	cout << "CONST(" << p.GetValue() << ")";
}

void IRTreePrinter::Visit( const CName& p )
{
	cout << "NAME(" << p.GetLabel()->Name() << ")";
}

void IRTreePrinter::Visit( const CTemp& p )
{
	cout << "TEMP(" << p.GetTemp()->Name() << ")";
}

void IRTreePrinter::Visit( const CBinOp& p )
{
	cout << "BINOP(";

	switch( p.GetBinOp() ) {
	case TBinOp::PLUS:
		cout << " + ";
		break;
	case TBinOp::MINUS:
		cout << " - ";
		break;
	case TBinOp::MUL:
		cout << " * ";
		break;
	case TBinOp::DIV:
		cout << " / ";
		break;
	case TBinOp::AND:
		cout << " AND ";
		break;
	case TBinOp::OR:
		cout << " OR ";
		break;
	case TBinOp::LSHIFT:
		cout << " LSHIFT ";
		break;
	case TBinOp::RSHIFT:
		cout << " RSHIFT ";
		break;
	case TBinOp::ARSHIFT:
		cout << " ARSHIFT ";
		break;
	case TBinOp::XOR:
		cout << " XOR ";
		break;
	default:
		break;
	}
	
	cout << ", ";
	p.GetLeft()->Accept( this );
	cout << ", ";
	p.GetRight()->Accept( this );
	cout << ")" ;
}

void IRTreePrinter::Visit( const CMem& p )
{
	cout << "MEM(";
	p.GetExp()->Accept( this );
	cout << ")";
}

void IRTreePrinter::Visit( const CCall& p )
{
	cout << "CALL(";
	p.GetExp()->Accept( this );
	cout << ", ";
	p.GetArgs()->Accept( this );
	cout << ")" ;
}

void IRTreePrinter::Visit( const CEseq& p )
{
	cout << "ESEQ(";
	p.GetStm()->Accept( this );
	cout << ", ";
	p.GetExp()->Accept( this );
	cout<< ")";
}

void IRTreePrinter::Visit( const CMove& p )
{
	cout << "MOVE(";
	p.GetDst()->Accept( this );
	cout << ", ";
	p.GetSrc()->Accept( this );
	cout << ")";
}

void IRTreePrinter::Visit( const CExp& p )
{
	cout << "EXP(";
	p.GetExp()->Accept( this );
	cout<< ")";
}

void IRTreePrinter::Visit( const CJump& p )
{
	cout << "JUMP(";
	const Temp::CLabelList* pwh( p.GetTargets() );
	while( pwh ) {
		cout << pwh->Label()->Name() << ", ";
		pwh = pwh->Next();
	}
	cout<< ")";
}

void IRTreePrinter::Visit( const CCJump& p )
{
	cout << "CJUMP(";
	switch( p.GetRelop() ) {
	case TCJump::EQ:
		cout << " == ";
		break;
	case TCJump::NE:
		cout << " != ";
		break;
	case TCJump::LT:
		cout << " < ";
		break;
	case TCJump::GT:
		cout << " > ";
		break;
	case TCJump::LE:
		cout << " <= ";
		break;
	case TCJump::GE:
		cout << " >= ";
		break;
	case TCJump::ULT:
		cout << " ULT ";
		break;
	case TCJump::ULE:
		cout << " ULE ";
		break;
	case TCJump::UGT:
		cout << " UGT ";
		break;
	case TCJump::UGE:
		cout << " UGE ";
		break;
	}
	cout << ", ";
	p.GetLeft()->Accept( this );
	cout << ", ";
	p.GetRight()->Accept( this );
	cout << ", ";
	cout << p.GetTrueLabel()->Name() << ", " << p.GetFalseLabel()->Name() << ")";
}

void IRTreePrinter::Visit( const CSeq& p )
{
	cout << "SEQ(";
	p.GetLeft()->Accept( this );
	cout << ", ";
	p.GetRight()->Accept( this );
	cout << ")";
}

void IRTreePrinter::Visit( const CLabel& p )
{
	cout <<p.GetLabel()->Name();
}

void IRTreePrinter::Visit( const CExpList& p )
{
	p.GetExp()->Accept( this );
	if( p.GetNext() != 0 ) {
		p.GetNext()->Accept( this );
	}
}

void IRTreePrinter::Visit( const CStmList& p )
{
	p.GetStm()->Accept( this );
	if( p.GetNext() != 0 ) {
		p.GetNext()->Accept( this );
	}
}
