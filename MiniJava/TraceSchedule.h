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
	std::map<IRTree::CLabel*, CStmListList*> table;

private:
	const IRTree::CStmList* GetLast( const IRTree::CStmList* block );
	void trace( const IRTree::CStmList* l );
	const IRTree::CStmList* getNext();
};