#include "BasicBlocks.h"

IRTree::CStmList* CStmListList::GetStm() const
{
	return stm;
}

CStmListList* CStmListList::GetNext() const
{
	return next;
}

void CStmListList::SetNext( CStmListList* n )
{
	next = n;
}

void CStmListList::SetStm( IRTree::CStmList* s )
{
	stm = s;
}

BasicBlocks::BasicBlocks( IRTree::CStmList* stms )
{
	done = new Temp::CLabel();
	lastBlock = 0;
	lastStm = 0;
	blocks = 0;
	mkBlocks( stms );	
}

void BasicBlocks::SetBlocks( CStmListList* _blocks )
{
	blocks = _blocks;
}

const CStmListList* BasicBlocks::GetBlocks() const
{
	return blocks;
}

const Temp::CLabel* BasicBlocks::GetDone() const
{
	return done;
}

void BasicBlocks::addStm( const IRTree::IStatement* stm )
{
	IRTree::CStmList* next = new IRTree::CStmList( stm, 0 );
	lastStm->SetNext( next );
	lastStm = next;
}

void BasicBlocks::doStms( IRTree::CStmList* stmList )
{
	if ( stmList == 0 )
		doStms( new IRTree::CStmList( new IRTree::CJump( done ), 0 ) );
	else
	{
		const IRTree::CJump* jump = dynamic_cast<const IRTree::CJump*>( stmList->GetStm() );
		const IRTree::CCJump* cjump = dynamic_cast<const IRTree::CCJump*>( stmList->GetStm() );
		if ( jump != 0 || cjump != 0 )
		{
			addStm( stmList->GetStm() );
			mkBlocks( stmList->GetNext() );
		}
		else
		{
			const IRTree::CLabel* label = dynamic_cast<const IRTree::CLabel*>( stmList->GetStm() );
			if ( label != 0 )
			{
				doStms( new IRTree::CStmList( new IRTree::CJump( ( ( IRTree::CLabel* ) stmList->GetStm() )->GetLabel() ), stmList ) );
			}
			else
			{
				addStm( stmList->GetStm() );
				doStms( stmList->GetNext() );
			}
		}
	}
}

void BasicBlocks::mkBlocks( IRTree::CStmList* stmList )
{
	if( stmList == NULL )
		return;
	else {
		const IRTree::CLabel* label = dynamic_cast<const IRTree::CLabel*>( stmList->GetStm() );
		if( label != 0 ) {
			lastStm = new IRTree::CStmList( stmList->GetStm(), NULL );
			if( lastBlock == NULL ) {
				lastBlock = blocks = new CStmListList( lastStm, NULL );
			} else {
				lastBlock->SetNext( new CStmListList( lastStm, 0 ) );
				lastBlock = lastBlock->GetNext();
			}
			doStms( stmList->GetNext() );
		}
		else
			mkBlocks( new IRTree::CStmList( new IRTree::CLabel( new Temp::CLabel() ), stmList ) );
	}
}