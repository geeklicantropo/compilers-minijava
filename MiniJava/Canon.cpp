#include "Canon.h"

const IRTree::CExpList* CStmExpList::getExps() const
{
	return exps;
}

const IRTree::IStatement* CStmExpList::getStm() const
{
	return stm;
}

const IRTree::CEseq* DoExp( const IRTree::CEseq* e )
{
	const IRTree::IStatement* stms = DoStm( e->GetStm() );
	const IRTree::CEseq* expr = DoExp( e->GetExp() );
	return new IRTree::CEseq( new IRTree::CSeq( stms, expr->GetStm() ), expr->GetExp() );
}

const IRTree::CEseq* DoExp( const IRTree::IExpression* e )
{
	const IRTree::CEseq* eseq = dynamic_cast<const IRTree::CEseq*>( e );
	if ( eseq != 0 )
		return DoExp( (IRTree::CEseq*) e );
	return ReorderExp( e );
}

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
	const CStmExpList* list = Reorder( s->GetChild() );
	return new IRTree::CSeq( list->getStm(), s->build( list->getExps() ) ); //need BUILD
}

const IRTree::CEseq* ReorderExp( const IRTree::IExpression* e )
{
	const CStmExpList* list = Reorder( e->GetChild() );
	return new IRTree::CEseq( list->getStm(), e->build( list->getExps() ) );
}

const CStmExpList* Reorder( const IRTree::CExpList*	exps )
{
	if ( exps == NULL )
		return nopNull;
	const IRTree::IExpression* exp = exps->GetExp();
	const IRTree::CCall* call = dynamic_cast<const IRTree::CCall*>( exp );
	if ( exp == NULL )
	{
		Temp::CTemp* temp = new Temp::CTemp();
		IRTree::IExpression* e = new IRTree::CEseq( new IRTree::CMove( new IRTree::CTemp( temp ), exp ),
			new IRTree::CTemp( temp ) );
		return Reorder( new IRTree::CExpList( e, exps->GetNext() ) );
	}
	else
	{
		const IRTree::CEseq* eseq = DoExp( exp );
		const CStmExpList* list = Reorder( exps->GetNext() );
		if ( commute( list->getStm(), eseq->GetExp() ) )
		{
			return new CStmExpList( new IRTree::CSeq( eseq->GetStm(), list->getStm() ),
				new IRTree::CExpList( eseq->GetExp(), list->getExps() ) );
		}
		else
		{
			Temp::CTemp* t = new Temp::CTemp();
			return new CStmExpList( new IRTree::CSeq( eseq->GetStm(),
				new IRTree::CSeq( new IRTree::CMove( new IRTree::CTemp( t ), eseq->GetExp() ),
				list->getStm() ) ), new IRTree::CExpList( new IRTree::CTemp( t ), list->getExps() ) );
		}
	}
	return 0;
}