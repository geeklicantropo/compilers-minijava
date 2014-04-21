#pragma once
#include <assert.h>
#include "InterferenceGraph.h"

CInterferenceGraphNode :: CInterferenceGraphNode(const Temp::CTemp *t)
{
	temp = t;
	color = -1;
}

void CInterferenceGraphNode::AddEdge(CInterferenceGraphNode *n, bool isMove)
{
	/*
	for(int i = 0; i < edgeArray.size(); i++)
		if (edgeArray[i]->getSecond() == n && edgeArray[i]->IsMove() == isMove) return;
	
	CInterferenceGraphEdge *edge = new CInterferenceGraphEdge(this, n, isMove);
	edgeArray.push_back(edge);
	*/

	if ( edgeMap.find(n) != edgeMap.end() && edgeMap.find(n)->second->IsMove() == isMove ) return;
	else edgeMap.insert ( make_pair(n, new CInterferenceGraphEdge(this, n, isMove) ) );

}

bool CInterferenceGraphNode::ExistEdge(CInterferenceGraphNode *n, bool isMove)
{
	/*
	for(int i = 0; i < edgeArray.size(); i++)
		if (edgeArray[i]->getSecond() == n) return true;
	return false;
	*/

	return ( edgeMap.find(n) != edgeMap.end() && edgeMap.find(n)->second->IsMove() == isMove );
}

void CInterferenceGraphNode::DeleteAllEdges()
{
	for ( auto e : edgeMap )
	{
		e.second->getSecond()->DeleteEdge(this, false);
		e.second->getSecond()->DeleteEdge(this, true);
	}
	
/*	for(int i = 0; i < edgeArray.size(); i++)
	{
		edgeArray[i]->getSecond()->DeleteEdge(this, false);
		edgeArray[i]->getSecond()->DeleteEdge(this, true);
	}
*/
}

void CInterferenceGraphNode::DeleteEdge(CInterferenceGraphNode *n, bool isMove)
{
/*	for(int i = 0; i < edgeArray.size(); i++)
		if (edgeArray[i]->getSecond() == n && edgeArray[i]->IsMove() == isMove) 
		{
			edgeArray.erase( edgeArray.begin() + i );
			return;
		}
*/
	if ( edgeMap.find(n) != edgeMap.end() && edgeMap.find(n)->second->IsMove() == isMove )
		edgeMap.erase( edgeMap.find(n) );
}

const Temp::CTemp* CInterferenceGraphNode::GetTemp()
{
	return temp;
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
		nodeMap.insert( pair<const Temp::CTemp*, CInterferenceGraphNode*> (t , new CInterferenceGraphNode(t)) );
}

void CInterferenceGraph::AddEdge(CInterferenceGraphEdge *e)
{
	//если понадобится, можно реализовать
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

CInterferenceGraph::CInterferenceGraph(CNodeList* flowNodes, AssemFlowGraph* flowGraph)
{ 
	nodeMap.clear();

	while (flowNodes != NULL) //добавление всех вершин
	{	
		std::set<const Temp::CTemp*> fGraph = flowGraph->GetUseSet(flowNodes->GetNode());

		for ( auto temp : fGraph )
		{
			AddNode(temp);
		}

		if ( flowGraph->IsMove(flowNodes->GetNode()) )
		{
			const Temp::CTemp* dst = ( dynamic_cast <const CodeGeneration::CMove*> ( flowGraph->GetInstruction( flowNodes->GetNode() ) ) ) -> GetDst();
			const Temp::CTemp* src = ( dynamic_cast <const CodeGeneration::CMove*> ( flowGraph->GetInstruction( flowNodes->GetNode() ) ) ) -> GetSrc();
			
			getNode( dst )->AddEdge( getNode( src ), true );
			getNode( src )->AddEdge( getNode( dst ), true );

			for ( auto to : flowGraph->GetLiveOut( flowNodes->GetNode() ) )
			{
				if (to != src)
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
					getNode( from )->AddEdge( getNode( to ), false );
					getNode( to )->AddEdge( getNode( from ), false );
				}
			}
		}
		flowNodes = flowNodes->GetNext();
	}

} 
