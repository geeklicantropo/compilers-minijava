#pragma once
#include "IRTranslationTree.h"
#include "BasicBlocks.h"
#include <map>
#include <iostream>

class TraceSchedule {
public:
	TraceSchedule( const BasicBlocks* b );
	const IRTree::CStmList* stms;
	const BasicBlocks* theBlocks;
	std::map< const Temp::CLabel*, const CStmListList* > table;

private:
	const IRTree::CStmList* getLast( const IRTree::CStmList* block );
	void trace( const IRTree::CStmList* list );
	const IRTree::CStmList* getNext();
};