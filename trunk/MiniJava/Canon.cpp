#include "Canon.h"

const IRTree::IStatement* DoStm( const IRTree::CSeq* s )
{
	return new IRTree::CSeq( DoStm( s->GetLeft() ), DoStm( s->GetRight() ) );
}

const IRTree::IStatement* DoStm( const IRTree::CMove* s )
{
	const IRTree::CTemp* tempDst = dynamic_cast<const IRTree::CTemp*>( s->GetDst() );
	const IRTree::CCall* callSrc = dynamic_cast<const IRTree::CCall*>( s->GetSrc() );
	const IRTree::CEseq* eseqDst = dynamic_cast<const IRTree::CEseq*>( s->GetDst() );
	const IRTree::CEseq* eseqSrc = dynamic_cast<const IRTree::CEseq*>( s->GetSrc() );
	
	if( (tempDst != 0) && (callSrc != 0) ) {
		return ReorderStm( new CMoveCall( tempDst, callSrc ) );
	} else if( eseqDst != 0 ) {
		return DoStm( new IRTree::CSeq( eseqDst->GetStm(), new IRTree::CMove( eseqDst->GetExp() , s->GetSrc() ) ) );
	} else {
		return ReorderStm( s );
	}
}

const IRTree::IStatement* DoStm( const IRTree::CExp* s )
{
	const IRTree::CCall* exp = dynamic_cast<const IRTree::CCall*>( s->GetExp() );
	if( exp != 0 ) {
		return ReorderStm( new CExpCall( exp ) );
	} else {
		return ReorderStm( s );
	}
}
const IRTree::IStatement* DoStm( const IRTree::IStatement* s )
{
	const IRTree::CSeq* seq = dynamic_cast<const IRTree::CSeq*>( s );
	const IRTree::CMove* move = dynamic_cast<const IRTree::CMove*>( s );
	const IRTree::CExp* exp = dynamic_cast<const IRTree::CExp*>( s );
	
	if( seq != 0 ) {
		return DoStm( seq );
	} else if ( move != 0 ) {
		return DoStm( move );
	} else if ( exp != 0 ) {
		return DoStm( exp );
	}
	return ReorderStm( s );
}

CMoveCall::CMoveCall( const IRTree::CTemp* d, const IRTree::CCall* s )
{
}

const IRTree::CExpList* CMoveCall::GetChild() const
{
	return 0;
}

void CMoveCall::Accept( IRTree::IRTreeVisitor* v ) const
{
}

const IRTree::CExpList* CExpCall::GetChild() const
{
	return 0;
}

CExpCall::CExpCall( const IRTree::CCall* c )
{
}

void CExpCall::Accept( IRTree::IRTreeVisitor* v ) const
{
}

const IRTree::IStatement* ReorderStm( const IRTree::IStatement* s )
{
	return 0;
}
const IRTree::CEseq* ReorderExp( const IRTree::CExp* e )
{
	return 0;
}

const CStmExpList* Reorder( const IRTree::CExpList*	exps )
{
	return 0;
}