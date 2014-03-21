#include "Graph.h"

Node::Node( Graph* g )
{
	currentGraph = g;
	nodeKey = g->nodeCount + 1;
	NodeList* mid = new NodeList( this, 0 );
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

NodeList* Node::cat( NodeList* a, NodeList* b )
{
	if ( a == 0 )
		return b;
	else
		return new NodeList( a->GetNode(), cat( a->GetNext(), b ) );
}

Graph* Node::GetGraph() const 
{
	return currentGraph;
}

NodeList* Node::GetSuccs() const
{
	return succs;
}

NodeList* Node::GetPreds() const
{
	return preds;
}

void Node::SetSuccs( NodeList* _succs )
{
	succs = _succs;
}

void Node::SetPreds( NodeList* _preds )
{
	preds = _preds;
}

NodeList* Node::MergeSuccAndPred()
{
	return cat( GetSuccs(), GetSuccs() );
}

int Node::InDegree()
{
	return preds->Length();
}

int Node::OutDegree()
{
	return succs->Length();
}

int Node::Degree()
{
	return InDegree() + OutDegree();
}

bool Node::GoesTo( const Node* n ) const
{
	return currentGraph->InList( n, succs );
}

bool Node::ComesFrom( const Node* n ) const
{
	return currentGraph->InList( n, preds );
}

bool Node::Achieve( const Node* n )
{
	return GoesTo( n ) || ComesFrom( n );
}

const Node* NodeList::GetNode() const
{
	return node;
}

NodeList* NodeList::GetNext() const
{
	return next;
}

void NodeList::SetNext( NodeList* n )
{
	next = n;
}

int NodeList::Length()
{
	int i = 0;
	for( const NodeList* p = this; p != 0; p = p->GetNext() )
		++i;
	return i;
}

Node* Graph::newNode()
{
	return new Node( this );
}

bool Graph::check( const Node* n )
{
	if ( n->GetGraph() != this )
		return false;
	return true;
}

bool Graph::InList( const Node* n, NodeList* list )
{
	for( NodeList* p = list; p != 0; p = list->GetNext() )
		if ( p->GetNode() == n )
			return true;
	return false;
}

void Graph::AddEdge( Node* from, Node* to )
{
	assert( check( from ) );
	assert( check( to ) );
	if ( from->GoesTo( to ) )
		return;
	to->SetPreds( new NodeList( from, to->GetPreds() ) );
	from->SetSuccs( new NodeList( to, from->GetSuccs() ) );
}

NodeList* Graph::remove( const Node* n, NodeList* list )
{
	assert( list == 0 );
	if ( n == list->GetNode() )
		return list->GetNext();
	else
		return new NodeList( list->GetNode(), remove( n, list->GetNext() ) ); 
}

void Graph::RemoveEdge( Node* from, Node* to )
{
	from->SetSuccs( remove( to, from->GetSuccs() ) );
	to->SetPreds( remove( from, to->GetPreds() ) );
}