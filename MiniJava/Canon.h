#pragma once
#include "IRTranslationTree.h"
#include "Temp.h"

class CStmExpList {
public:
	CStmExpList( const IRTree::IStatement* _stm, const IRTree::CExpList* _exps ) : stm( _stm ), exps( _exps ) {}
	const IRTree::CExpList* getExps() const;
	const IRTree::IStatement* getStm() const;
private:
	const IRTree::CExpList* exps;
	const IRTree::IStatement* stm;
};

class CMoveCall : public IRTree::IStatement {
public:
	CMoveCall( const IRTree::CTemp* d, const IRTree::CCall* s );
	const IRTree::CExpList* GetChild() const;
	void Accept( IRTree::IRTreeVisitor* v ) const;
private:
};

class CExpCall : public IRTree::IStatement {
public:
	CExpCall( const IRTree::CCall* c );
	const IRTree::CExpList* GetChild() const;
	void Accept( IRTree::IRTreeVisitor* v ) const;
private:
};

const IRTree::CEseq* DoExp( const IRTree::CEseq* e );
const IRTree::CEseq* DoExp( const IRTree::IExpression* e );

const IRTree::IStatement* DoStm( const IRTree::CSeq* s );
const IRTree::IStatement* DoStm( const IRTree::CMove* s );
const IRTree::IStatement* DoStm( const IRTree::CExp* s );
const IRTree::IStatement* DoStm( const IRTree::IStatement* s );

const IRTree::IStatement* ReorderStm( const IRTree::IStatement* s );
const IRTree::CEseq* ReorderExp( const IRTree::IExpression* e );

const CStmExpList* Reorder( const IRTree::CExpList*	exps );

const CStmExpList* nopNull = new CStmExpList( new IRTree::CExp( new IRTree::CConst( 0 ) ), NULL );

bool commute( const IRTree::IStatement* a, const IRTree::IExpression* b );