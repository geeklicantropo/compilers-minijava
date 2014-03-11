#pragma once
#include "IRTranslationTree.h"
#include "Temp.h"

class CStmListList {
public:
	CStmListList( const IRTree::CStmList* _stm, const CStmListList* _next ) : stm( _stm ), next( _next ) {}
	const IRTree::CStmList* getStm();
	const CStmListList* getNext();
private:
	const IRTree::CStmList* stm;
	const CStmListList* next;
};

class BasicBlocks {
public:
	BasicBlocks( const IRTree::CStmList* stms );
	const CStmListList* getBlocks();
	const Temp::CLabel* getDone();
private:
	const CStmListList* blocks;
	const Temp::CLabel* done;

	const CStmListList* lastBlock;
	const IRTree::CStmList* lastStm;

	void addStm( const IRTree::IStatement* stm );
	void doStms( const IRTree::CStmList* stmList);
	void mkBlocks( const IRTree::CStmList* stmList );
};