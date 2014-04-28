#pragma once
#include <assert.h>
#include "InterferenceGraph.h"
#include <fstream>
#include <stack>
#include "Symbols.h"

CInterferenceGraphNode :: CInterferenceGraphNode(const Temp::CTemp *t, int _id)
{
	temp = t;
	color = -1;
	id = _id;
	freezed = false;
}

void CInterferenceGraphNode::AddEdge(CInterferenceGraphNode *n, bool isMove)
{
	if ( edgeMap.find(n) != edgeMap.end() && edgeMap.find(n)->second->IsMove() == isMove ) return;
	else edgeMap.insert ( make_pair(n, new CInterferenceGraphEdge(this, n, isMove) ) );

}

bool CInterferenceGraphNode::ExistEdge(CInterferenceGraphNode *n, bool isMove)
{
	return ( edgeMap.find(n) != edgeMap.end() && edgeMap.find(n)->second->IsMove() == isMove );
}

void CInterferenceGraphNode::DeleteAllEdges()
{
	for ( auto e : edgeMap )
	{
		e.second->getSecond()->DeleteEdge(this, false);
		e.second->getSecond()->DeleteEdge(this, true);
	}
}

void CInterferenceGraphNode::DeleteEdge(CInterferenceGraphNode *n, bool isMove)
{
	if ( edgeMap.find(n) != edgeMap.end() && edgeMap.find(n)->second->IsMove() == isMove )
		edgeMap.erase( edgeMap.find(n) );
}

const Temp::CTemp* CInterferenceGraphNode::GetTemp()
{
	return temp;
}

int CInterferenceGraphNode::GetId()
{
	return id;
}

std::map<CInterferenceGraphNode*, CInterferenceGraphEdge*> CInterferenceGraphNode::GetEdgeMap()
{
	return edgeMap;
}

bool CInterferenceGraphNode::HasMoveEdge()
{
	for (auto e : edgeMap)
		if (e.second->IsMove()) 
			return true;
	return false;
}

bool CInterferenceGraphNode::IsFreezed()
{
	return freezed;
}

void CInterferenceGraphNode::SetFreeze(bool f)
{
	freezed = f;
}

CInterferenceGraphEdge::CInterferenceGraphEdge (CInterferenceGraphNode *f, CInterferenceGraphNode *s, bool _isMove)
{
	isMove = _isMove;
	first = f;
	second = s;
}
	
CInterferenceGraphNode* CInterferenceGraphEdge::getFirst()
{
	return first;
}
	
CInterferenceGraphNode* CInterferenceGraphEdge::getSecond()
{
	return second;
}

bool CInterferenceGraphEdge::IsMove()
{
	return isMove;
}

void CInterferenceGraph::AddNode(const Temp::CTemp *t)
{
	if ( nodeMap.find( t ) == nodeMap.end() ) 
		nodeMap.insert( pair<const Temp::CTemp*, CInterferenceGraphNode*> (t , new CInterferenceGraphNode(t, nextId++ )) );
}

void CInterferenceGraph::AddNode(CInterferenceGraphNode *n)
{
	const Temp::CTemp *t = n -> GetTemp();
	if ( nodeMap.find( t ) == nodeMap.end() ) 
		nodeMap.insert( make_pair (t , n) );
}

void CInterferenceGraph::AddEdge(CInterferenceGraphEdge *e)
{
	e->getFirst() ->AddEdge( e->getSecond(), e->IsMove() );
	e->getSecond()->AddEdge( e->getFirst(),  e->IsMove() );
}

void CInterferenceGraph::DeleteEdge(CInterferenceGraphEdge *e)
{
	e->getFirst()->DeleteEdge(e->getSecond(), e->IsMove() );
	e->getSecond()->DeleteEdge(e->getFirst(), e->IsMove() );
}

void CInterferenceGraph::DeleteNode(CInterferenceGraphNode *n)
{
	n->DeleteAllEdges();

	if ( nodeMap.find( n->GetTemp() ) != nodeMap.end() )
		nodeMap.erase( nodeMap.find( n->GetTemp() ) );

}

bool CInterferenceGraph::existEdge(CInterferenceGraphEdge *e)
{
	return e->getFirst()->ExistEdge(e->getSecond(), e->IsMove() );
}

CInterferenceGraphNode* CInterferenceGraph::getNode(const Temp::CTemp *t)
{
	if ( nodeMap.find( t ) != nodeMap.end() ) 
		return nodeMap[t];
	else
		return NULL;

}

int CInterferenceGraph::nodesCount()
{
	return nodeMap.size();
}

bool CInterferenceGraph::PopEdge(CInterferenceGraphEdge* edge, int id, int k)
{
	CInterferenceGraphNode* fst = edge->getFirst();
	CInterferenceGraphNode* snd = edge->getSecond();

	string name = fst->GetTemp()->getName() + "&" + snd->GetTemp()->getName();
	const Temp::CTemp* temp = new Temp::CTemp( CSymbol::CSymbolGet(name) );
	
	CInterferenceGraphNode* node = new CInterferenceGraphNode( temp, id );

	for ( auto n : nodeMap )
	{
		if (fst != n.second && snd != n.second)
		{
			if ( fst->ExistEdge(n.second, true ) ) AddEdge( new CInterferenceGraphEdge(node, n.second, true) );
			if ( fst->ExistEdge(n.second, false) ) AddEdge( new CInterferenceGraphEdge(node, n.second, false) );
			if ( snd->ExistEdge(n.second, true ) ) AddEdge( new CInterferenceGraphEdge(node, n.second, true) );
			if ( snd->ExistEdge(n.second, false) ) AddEdge( new CInterferenceGraphEdge(node, n.second, false) );
		}
	}

	if (node->GetEdgeMap().size() < k)
	{
		AddNode(node);
		DeleteNode(fst);
		DeleteNode(snd);
		return true;
	}

	return false;
}

CInterferenceGraph::CInterferenceGraph(CNodeList* flowNodes, AssemFlowGraph* flowGraph)
{ 
	nextId = 1;
	nodeMap.clear();

	CNodeList* flowNodes1 = flowNodes;

	while (flowNodes != NULL)
	{
		for ( auto temp : flowGraph->GetUseSet(flowNodes->GetNode()) )
		{
			AddNode(temp);
		}
		flowNodes = flowNodes->GetNext();
	}

	flowNodes = flowNodes1;
	while (flowNodes != NULL)
	{
		for ( auto temp : flowGraph->GetDefSet(flowNodes->GetNode()) )
		{
			AddNode(temp);
		}
		flowNodes = flowNodes->GetNext();
	}

	flowNodes = flowNodes1;
	while (flowNodes != NULL) //добавление всех вершин
	{	
		
		if ( flowGraph->IsMove(flowNodes->GetNode()) )
		{
			const Temp::CTemp* dst = ( dynamic_cast <const CodeGeneration::CMove*> ( flowGraph->GetInstruction( flowNodes->GetNode() ) ) ) -> GetDst();
			const Temp::CTemp* src = ( dynamic_cast <const CodeGeneration::CMove*> ( flowGraph->GetInstruction( flowNodes->GetNode() ) ) ) -> GetSrc();
			
			if (src != dst)
			{
				getNode( dst )->AddEdge( getNode( src ), true );
				getNode( src )->AddEdge( getNode( dst ), true );
			}

			for ( auto to : flowGraph->GetLiveOut( flowNodes->GetNode() ) )
			{
				if (to != src && to != dst)
				{
					getNode( dst )->AddEdge( getNode( to ), false );
					getNode( to )->AddEdge( getNode( dst ), false );
				}
			}

		} else
		{
			for ( auto from : flowGraph->GetDefSet( flowNodes->GetNode() ) )
			{
				for ( auto to : flowGraph->GetLiveOut( flowNodes->GetNode() ) )
				{
					if (to != from)
					{
						getNode( from )->AddEdge( getNode( to ), false );
						getNode( to )->AddEdge( getNode( from ), false );
					}
				}
			}
		}
		flowNodes = flowNodes->GetNext();
	}

} 

void CInterferenceGraph::WriteGraph(string path)
{
	ofstream out(path);
	
	out << "digraph G {" << endl;

	for ( auto n : nodeMap )
	{
		for ( auto e : n.second->GetEdgeMap() )
		{
			out << n.second->GetId() << "->" << e.second->getSecond()->GetId();
			if ( e.second->IsMove() ) 
				out << " [style=dotted]"; 
			out << endl;
		}
	}

	for ( auto n : nodeMap )
	{
		out << n.second->GetId() << "[label=\"" << n.second->GetTemp()->getName() << "\"]" << endl;
	}

	out << "}" ;
}

CInterferenceGraph::CInterferenceGraph(CInterferenceGraph* graph)
{
	//здесь бы конструктор копирования
}

const Temp::CTemp* CInterferenceGraph::simplify(int K)
{
	for (auto n : nodeMap)
	{
		if (n.second->GetEdgeMap().size() < K && !n.second->HasMoveEdge() ||
			n.second->GetEdgeMap().size() < K &&  n.second->HasMoveEdge() && n.second->IsFreezed() )
		{
			DeleteNode(n.second);
			return n.second->GetTemp();
		}

	}
	return NULL;
}

bool CInterferenceGraph::coalesce(int id, int k)
{
	for (auto n : nodeMap)
	{
		for ( auto e : n.second->GetEdgeMap() )
		{
			if ( e.second->IsMove() && !e.second->getFirst()->IsFreezed() && !e.second->getSecond()->IsFreezed() )
				if ( PopEdge ( e.second, id, k ) )
					return true;
		}
	}
	return false;
}

bool CInterferenceGraph::freeze(int k)
{
	for (auto n : nodeMap)
	{
		if ( !n.second->IsFreezed() && n.second->GetEdgeMap().size() < k && n.second->HasMoveEdge() )
		{
			n.second->SetFreeze(true);
			return true;
		}
	}
	return false;
}

const Temp::CTemp* CInterferenceGraph::spill()
{
	int min = 100000;
	CInterferenceGraphNode* node = NULL;

	for (auto n : nodeMap)
	{
		if (n.second->GetEdgeMap().size() < min)
			node = n.second;
	}

	DeleteNode(node);
	return (node->GetTemp());
}

void CInterferenceGraph::SetColors(int K)
{
	CInterferenceGraph graph = CInterferenceGraph(this);
	std::stack< std::pair <const Temp::CTemp*, bool> > stack;

	while ( graph.nodesCount() > 0)
	{
		//здесь надо написать нормально.
		bool isCandidate;
		const Temp::CTemp* temp = graph.simplify(K);
		isCandidate = temp == NULL;
		if (temp == NULL) temp = graph.spill();
		stack.push ( std::make_pair(temp, isCandidate) );
	}


}