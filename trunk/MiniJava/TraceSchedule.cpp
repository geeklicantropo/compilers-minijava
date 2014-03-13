#include "TraceSchedule.h"

TraceSchedule::TraceSchedule( const BasicBlocks* b )
{
	
}

const IRTree::CStmList* TraceSchedule::getLast( const IRTree::CStmList* block )
{
	const IRTree::CStmList* list = block;
	while( list->GetNext()->GetNext()!=0 ) 
		list = list->GetNext();
	return list;
}

void TraceSchedule::trace( const IRTree::CStmList* list )
{
	while( true ) {
		const IRTree::CLabel* label = (const IRTree::CLabel*) list->GetStm();
		table.erase( label->GetLabel() );


	}
}
	
const IRTree::CStmList* TraceSchedule::getNext()
{
	return 0;
}