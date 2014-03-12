#include "TraceSchedule.h"

TraceSchedule::TraceSchedule( const BasicBlocks* b )
{
	
}

const IRTree::CStmList* TraceSchedule::GetLast( const IRTree::CStmList* block )
{
	const IRTree::CStmList* l = block;
	while( l->GetNext()->GetNext()!=0 ) 
		l = l->GetNext();
	return l;
}

void TraceSchedule::trace( const IRTree::CStmList* l )
{
	
}

const IRTree::CStmList* TraceSchedule::getNext()
{
	return 0;
}