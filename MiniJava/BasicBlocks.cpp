#include "BasicBlocks.h"

const IRTree::CStmList* CStmListList::getStm() const
{
	return stm;
}

const CStmListList* CStmListList::getNext() const
{
	return next;
}

BasicBlocks::BasicBlocks( const IRTree::CStmList* stms )
{
	done = new Temp::CLabel();
	mkBlocks( stms );
}

const CStmListList* BasicBlocks::getBlocks() const
{
	return blocks;
}

const Temp::CLabel* BasicBlocks::getDone() const
{
	return done;
}

void BasicBlocks::addStm( const IRTree::IStatement* stm )
{
	const IRTree::CStmList* next = new IRTree::CStmList( stm, NULL );
	lastStm->SetNext( next );
	lastStm = lastStm->GetNext();
}

void BasicBlocks::doStms( const IRTree::CStmList* stmList )
{
	if ( stmList == NULL )
		doStms( new IRTree::CStmList( new IRTree::CJump( done ), NULL ) );
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