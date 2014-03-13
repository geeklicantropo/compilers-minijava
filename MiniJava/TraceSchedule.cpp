#include "TraceSchedule.h"

TraceSchedule::TraceSchedule( BasicBlocks* b )
{
	theBlocks = b;
	for( const CStmListList* list = b->GetBlocks(); list!=0; list=list->GetNext() ) {
		table[((IRTree::CLabel*)list->GetStm()->GetStm())->GetLabel()] = list->GetStm();
	}
	stms = getNext();
	table.clear();
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

		const IRTree::CStmList* last = getLast( list );
		const IRTree::IStatement* s = last->GetNext()->GetStm();
		const IRTree::CJump* jump = dynamic_cast< const IRTree::CJump* >( s );
		const IRTree::CCJump* cjump = dynamic_cast< const IRTree::CCJump* >( s );

		if( jump != 0 ) {
			const IRTree::CStmList* target = table[jump->GetTargets()->Label()];
			if( jump->GetTargets()->Next() == 0 && target != 0 ) {
				last = new IRTree::CStmList( last->GetStm(), target );
				list = target;
			} else {
				last = new IRTree::CStmList( last->GetStm(), new IRTree::CStmList( last->GetNext()->GetStm(), getNext() ) );
				break;
			}
		} else if( cjump != 0 ) {
			const IRTree::CStmList* t = table[cjump->GetTrueLabel()];
			const IRTree::CStmList* f = table[cjump->GetFalseLabel()];
			if( f != 0 ) {
				last = new IRTree::CStmList( last->GetStm(), new IRTree::CStmList( last->GetNext()->GetStm(), f ) );
				list = f;
			} else if( t != 0 ) {
				const IRTree::CCJump* newcjump = new IRTree::CCJump( IRTree::CCJump::NotRelop( cjump->GetRelop() ), cjump->GetLeft(),
					cjump->GetRight(), cjump->GetFalseLabel(), cjump->GetTrueLabel() );
				last = new IRTree::CStmList( last->GetStm(), new IRTree::CStmList( newcjump, t ) );
				list = t;
			} else {
				const Temp::CLabel* newLabel = new Temp::CLabel;
				const IRTree::CCJump* newcjump = new IRTree::CCJump( cjump->GetRelop(), cjump->GetLeft(),
					cjump->GetRight(), cjump->GetTrueLabel(), newLabel );
				const IRTree::CStmList* tmp = new IRTree::CStmList( new IRTree::CLabel( newLabel), new IRTree::CStmList( new IRTree::CJump( cjump->GetFalseLabel() ), getNext() ) );
				last = new IRTree::CStmList( last->GetStm(), new IRTree::CStmList( newcjump, tmp ) );
				
				break;
			}
		} else {
			cerr << "TraceShedule Error!" << endl;
			break;
		}
	}
}
	
const IRTree::CStmList* TraceSchedule::getNext()
{
	if( theBlocks->GetBlocks() == 0 ) { 
		return new IRTree::CStmList( new IRTree::CLabel( theBlocks->GetDone() ) , 0);
	}
	else {
		const IRTree::CStmList* stmList = theBlocks->GetBlocks()->GetStm();
		IRTree::CLabel* label = (IRTree::CLabel*)stmList->GetStm();
		if( table.find(label->GetLabel()) != table.end() ) {
			trace(stmList);
			return stmList;
		}
		else {
			theBlocks->SetBlocks( theBlocks->GetBlocks()->GetNext() );
			return getNext();
		}
	}
}