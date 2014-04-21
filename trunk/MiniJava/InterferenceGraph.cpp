#pragma once
#include <assert.h>
#include "InterferenceGraph.h"

CInterferenceGraphNode :: CInterferenceGraphNode(const Temp::CTemp *t)
{
	temp = t;
	color = -1;
}

void CInterferenceGraphNode::AddEdge(CInterferenceGraphNode *n, bool _isMove)
{
	for(int i = 0; i < edgeArray.size(); i++)
		if (edgeArray[i]->getSecond() == n && edgeArray[i]->IsMove() == _isMove) return;
	
	CInterferenceGraphEdge *edge = new CInterferenceGraphEdge(this, n, _isMove);
	edgeArray.push_back(edge);
}

bool CInterferenceGraphNode::ExistEdge(CInterferenceGraphNode *n)
{
	for(int i = 0; i < edgeArray.size(); i++)
		if (edgeArray[i]->getSecond() == n) return true;
	return false;
}

void CInterferenceGraphNode::DeleteAllEdges()
{
	for(int i = 0; i < edgeArray.size(); i++)
		edgeArray[i]->getSecond()->DeleteEdge(this);
}

void CInterferenceGraphNode::DeleteEdge(CInterferenceGraphNode *n)
{
	for(int i = 0; i < edgeArray.size(); i++)
		if (edgeArray[i]->getSecond() == n) 
		{
			edgeArray.erase( edgeArray.begin() + i );
			return;
		}
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
	for (int i = 0; i < nodeArray.size(); i++)
		if ( nodeArray[i]->GetTemp() == t) 
			return;
	AddNode ( new CInterferenceGraphNode(t) );
}

void CInterferenceGraph::AddNode(CInterferenceGraphNode *n)
{
	nodeArray.push_back(n);
}

void CInterferenceGraph::AddEdge(CInterferenceGraphEdge *e)
{
	//если понадобится, можно реализовать
}

void CInterferenceGraph::DeleteEdge(CInterferenceGraphEdge *e)
{
	e->getFirst()->DeleteEdge(e->getSecond());
	e->getSecond()->DeleteEdge(e->getFirst());
}

void CInterferenceGraph::DeleteNode(CInterferenceGraphNode *n)
{
	n->DeleteAllEdges();
	for(int i = 0; i < nodeArray.size(); i++)
		if (nodeArray[i] == n) 
		{
			nodeArray.erase( nodeArray.begin() + i );
			return;
		}
}

bool CInterferenceGraph::existEdge(CInterferenceGraphEdge *e)
{
	return e->getFirst()->ExistEdge(e->getSecond());
}

CInterferenceGraphNode* CInterferenceGraph::getNode(const Temp::CTemp *t)
{
	for (int i = 0; i < nodeArray.size(); i++)
		if ( nodeArray[i]->GetTemp() == t) 
			return nodeArray[i];
	return NULL;
}

CInterferenceGraph::CInterferenceGraph(CNodeList* flowNodes, AssemFlowGraph* flowGraph)
{ 
	nodeArray.resize(0);
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
