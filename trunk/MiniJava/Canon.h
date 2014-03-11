#pragma once
#include "IRTranslationTree.h"
#include "Temp.h"

class CStmExpList {
public:
	CStmExpList( const IRTree::IStatement* _stm, const IRTree::CExpList* _exps ) : stm( _stm ), exps( _exps ) {}
	const IRTree::CExpList* GetExps() const;
	const IRTree::IStatement* GetStm() const;
private:
	const IRTree::CExpList* exps;
	const IRTree::IStatement* stm;
};



bool IsNop( const IRTree::IStatement* stm );
const IRTree::IStatement* Seq( const IRTree::IStatement* a, const IRTree::IStatement* b );
bool Commute( const IRTree::IStatement* a, const IRTree::IExpression* b );
const IRTree::CStmList* Linear( const IRTree::CSeq* s, const IRTree::CStmList* l );
const IRTree::CStmList* Linear( const IRTree::IStatement* s, const IRTree::CStmList* l);
const IRTree::CStmList* Linearize( const IRTree::IStatement* s );

const IRTree::CEseq* DoExp( const IRTree::CEseq* e );
const IRTree::CEseq* DoExp( const IRTree::IExpression* e );

const IRTree::IStatement* DoStm( const IRTree::CSeq* s );
const IRTree::IStatement* DoStm( const IRTree::CMove* s );
const IRTree::IStatement* DoStm( const IRTree::CExp* s );
const IRTree::IStatement* DoStm( const IRTree::IStatement* s );

const IRTree::IStatement* ReorderStm( const IRTree::IStatement* s );
const IRTree::CEseq* ReorderExp( const IRTree::IExpression* e );

const CStmExpList* Reorder( const IRTree::CExpList*	exps );

static const CStmExpList* nopNull = new CStmExpList( new IRTree::CExp( new IRTree::CConst( 0 ) ), NULL );
