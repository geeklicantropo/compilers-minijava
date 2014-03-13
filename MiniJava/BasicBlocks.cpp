#include "BasicBlocks.h"

const IRTree::CStmList* CStmListList::GetStm() const
{
	return stm;
}

const CStmListList* CStmListList::GetNext() const
{
	return next;
}

BasicBlocks::BasicBlocks( const IRTree::CStmList* stms )
{
	done = new Temp::CLabel();
	lastBlock = 0;
	lastStm = 0;
	blocks = 0;
	mkBlocks( stms );	
}

void BasicBlocks::SetBlocks( const CStmListList* _blocks )
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
	const IRTree::CStmList* next = new IRTree::CStmList( stm, NULL );
	lastStm = new IRTree::CStmList( lastStm->GetStm(), next );
	lastStm = next;
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

void BasicBlocks::mkBlocks( const IRTree::CStmList* stmList )
{
	if ( stmList == NULL )
		return;
	else
	{
		const IRTree::CLabel* label = dynamic_cast<const IRTree::CLabel*>( stmList->GetStm() );
		if ( label != 0 )
		{
			lastStm = new IRTree::CStmList( stmList->GetStm(), NULL );
			if ( lastBlock == NULL )
			{
				lastBlock = blocks = new CStmListList( lastStm, NULL );
			}
			else
			{
				lastBlock = new CStmListList(lastBlock->GetStm(), new CStmListList( stmList, 0 ) );
			}
			doStms( stmList->GetNext() );
		}
		else
			mkBlocks( new IRTree::CStmList( new IRTree::CLabel( new Temp::CLabel() ), stmList ) );
	}
}