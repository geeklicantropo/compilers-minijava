#pragma once
#include "IRTranslationTree.h"
#include "Temp.h"

class CStmListList {
public:
	CStmListList( IRTree::CStmList* _stm, CStmListList* _next ) : stm( _stm ), next( _next ) {}
	IRTree::CStmList* GetStm() const;
	CStmListList* GetNext() const;
	void SetStm( IRTree::CStmList* s );
	void SetNext( CStmListList* n );
private:
	IRTree::CStmList* stm;
	CStmListList* next;
};

class BasicBlocks {
public:
	BasicBlocks( IRTree::CStmList* stms );
	const CStmListList* GetBlocks() const;
	const Temp::CLabel* GetDone() const;
	void SetBlocks( CStmListList* _blocks );
private:
	CStmListList* blocks;
	const Temp::CLabel* done;

	CStmListList* lastBlock;
	IRTree::CStmList* lastStm;

	void addStm( const IRTree::IStatement* stm );
	void doStms( IRTree::CStmList* stmList);
	void mkBlocks( IRTree::CStmList* stmList );
};