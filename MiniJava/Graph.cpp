#include "Graph.h"

CNode::CNode( CGraph* g )
{
	currentGraph = g;
	nodeKey = g->nodeCount + 1;
	CNodeList* mid = new CNodeList( this, 0 );
	if ( g->last == 0 )
	{
		g->last = mid;
		g->nodes = g->last;
	}
	else
	{
		g->last->SetNext( mid );
		g->last = g->last->GetNext();
	}
}

CNodeList* CNode::cat( CNodeList* a, CNodeList* b )
{
	if ( a == 0 )
		return b;
	else
		return new CNodeList( a->GetNode(), cat( a->GetNext(), b ) );
}

CGraph* CNode::GetGraph() const 
{
	return currentGraph;
}

CNodeList* CNode::GetSuccs() const
{
	return succs;
}

CNodeList* CNode::GetPreds() const
{
	return preds;
}

void CNode::SetSuccs( CNodeList* _succs )
{
	succs = _succs;
}

void CNode::SetPreds( CNodeList* _preds )
{
	preds = _preds;
}

CNodeList* CNode::MergeSuccAndPred()
{
	return cat( GetSuccs(), GetSuccs() );
}

int CNode::InDegree()
{
	return preds->Length();
}

int CNode::OutDegree()
{
	return succs->Length();
}

int CNode::Degree()
{
	return InDegree() + OutDegree();
}

bool CNode::GoesTo( const CNode* n ) const
{
	return currentGraph->InList( n, succs );
}

bool CNode::ComesFrom( const CNode* n ) const
{
	return currentGraph->InList( n, preds );
}

bool CNode::Adjacent( const CNode* n )
{
	return GoesTo( n ) || ComesFrom( n );
}

const CNode* CNodeList::GetNode() const
{
	return node;
}

CNodeList* CNodeList::GetNext() const
{
	return next;
}

void CNodeList::SetNext( CNodeList* n )
{
	next = n;
}

int CNodeList::Length()
{
	int i = 0;
	for( const CNodeList* p = this; p != 0; p = p->GetNext() )
		++i;
	return i;
}

CNode* CGraph::newNode()
{
	return new CNode( this );
}

bool CGraph::check( const CNode* n )
{
	if ( n->GetGraph() != this )
		return false;
	return true;
}

bool CGraph::InList( const CNode* n, CNodeList* list )
{
	for( CNodeList* p = list; p != 0; p = list->GetNext() )
		if ( p->GetNode() == n )
			return true;
	return false;
}

void CGraph::AddEdge( CNode* from, CNode* to )
{
	assert( check( from ) );
	assert( check( to ) );
	if ( from->GoesTo( to ) )
		return;
	to->SetPreds( new CNodeList( from, to->GetPreds() ) );
	from->SetSuccs( new CNodeList( to, from->GetSuccs() ) );
}

CNodeList* CGraph::remove( const CNode* n, CNodeList* list )
{
	assert( list == 0 );
	if ( n == list->GetNode() )
		return list->GetNext();
	else
		return new CNodeList( list->GetNode(), remove( n, list->GetNext() ) ); 
}

void CGraph::RemoveEdge( CNode* from, CNode* to )
{
	from->SetSuccs( remove( to, from->GetSuccs() ) );
	to->SetPreds( remove( from, to->GetPreds() ) );
}