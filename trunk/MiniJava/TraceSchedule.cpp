#include "TraceSchedule.h"
#include "assert.h"

TraceSchedule::TraceSchedule( BasicBlocks* b )
{
	theBlocks = b;
	for( const CStmListList* list = b->GetBlocks(); list != 0; list = list->GetNext() ) {
		table[((IRTree::CLabel*)list->GetStm()->GetStm())->GetLabel()] = list->GetStm();
	}
	stms = getNext();
	table.clear();
}  

IRTree::CStmList* TraceSchedule::getLast( IRTree::CStmList* block )
{
	IRTree::CStmList* list = block;
	while( list->GetNext()->GetNext() != 0 ) 
		list = list->GetNext();
	return list;
}

void TraceSchedule::trace( IRTree::CStmList* list )
{
	while( true ) {
		const IRTree::CLabel* label = dynamic_cast<const IRTree::CLabel*>( list->GetStm() );
		assert( label != 0 );
		table.erase( label->GetLabel() );

		IRTree::CStmList* last = getLast( list );
		const IRTree::IStatement* s = last->GetNext()->GetStm();
		const IRTree::CJump* jump = dynamic_cast< const IRTree::CJump* >( s );
		const IRTree::CCJump* cjump = dynamic_cast< const IRTree::CCJump* >( s );

		if( jump != 0 ) {
			IRTree::CStmList* target = table[jump->GetTargets()->Label()];
			if( jump->GetTargets()->Next() == 0 && target != 0 ) {
				last->SetNext( target );
				list = target;
			} else {
				last->GetNext()->SetNext( getNext() );
				break;
			}
		} else if( cjump != 0 ) {
			IRTree::CStmList* t = table[cjump->GetTrueLabel()];
			IRTree::CStmList* f = table[cjump->GetFalseLabel()];
			if( f != 0 ) {
				last->GetNext()->SetNext( f );
				list = f;
			} else if( t != 0 ) {
				const IRTree::CCJump* newcjump = new IRTree::CCJump( IRTree::CCJump::NotRelop( cjump->GetRelop() ), cjump->GetLeft(),
					cjump->GetRight(), cjump->GetFalseLabel(), cjump->GetTrueLabel() );
				last->GetNext()->SetStm( newcjump );
				last->GetNext()->SetNext( t );
				list = t;
			} else {
				const Temp::CLabel* newLabel = new Temp::CLabel;
				const IRTree::CCJump* newcjump = new IRTree::CCJump( cjump->GetRelop(), cjump->GetLeft(),
					cjump->GetRight(), cjump->GetTrueLabel(), newLabel );
				IRTree::CStmList* tmp = new IRTree::CStmList( new IRTree::CLabel( newLabel ), new IRTree::CStmList( new IRTree::CJump( cjump->GetFalseLabel() ), getNext() ) );
				last->GetNext()->SetStm( newcjump );
				last->GetNext()->SetNext( tmp );
				break;
			}
		} else {
			assert( false );
			cerr << "TraceShedule Error!" << endl;
			break;
		}
	}
}
	
IRTree::CStmList* TraceSchedule::getNext()
{
	if( theBlocks->GetBlocks() == 0 ) { 
		return new IRTree::CStmList( new IRTree::CLabel( theBlocks->GetDone() ) , 0);
	} else {
		IRTree::CStmList* stmList = theBlocks->GetBlocks()->GetStm();
		const IRTree::CLabel* label = dynamic_cast<const IRTree::CLabel*>( stmList->GetStm() );
		assert( label != 0 );
		if( table.find(label->GetLabel()) != table.end() ) {
			trace( stmList );
			return stmList;
		} else {
			theBlocks->SetBlocks( theBlocks->GetBlocks()->GetNext() );
			return getNext();
		}
	}
}