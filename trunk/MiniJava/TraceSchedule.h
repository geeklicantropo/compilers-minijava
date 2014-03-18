#pragma once
#include "IRTranslationTree.h"
#include "BasicBlocks.h"
#include <map>
#include <iostream>

class TraceSchedule {
public:
	TraceSchedule( BasicBlocks* b );
	IRTree::CStmList* stms;
	BasicBlocks* theBlocks;
	std::map< const Temp::CLabel*, IRTree::CStmList* > table;

private:
	IRTree::CStmList* getLast( IRTree::CStmList* block );
	void trace( IRTree::CStmList* list );
	IRTree::CStmList* getNext();
};