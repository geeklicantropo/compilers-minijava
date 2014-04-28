#pragma once
#include <assert.h>
#include "InterferenceGraph.h"
#include <fstream>

CInterferenceGraphNode :: CInterferenceGraphNode(const Temp::CTemp *t, int _id)
{
	temp = t;
	color = -1;
	id = _id;
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

int CInterferenceGraphNode::GetId()
{
	return id;
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

std::map<CInterferenceGraphNode*, CInterferenceGraphEdge*> CInterferenceGraphNode::GetEdgeMap()
{
	return edgeMap;
}

bool CInterferenceGraphEdge::IsMove()
{
	return isMove;
}

void CInterferenceGraph::AddNode(const Temp::CTemp *t)
{
	if ( nodeMap.find( t ) == nodeMap.end() ) 
		nodeMap.insert( pair<const Temp::CTemp*, CInterferenceGraphNode*> (t , new CInterferenceGraphNode(t, nodesCount() )) );
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

int CInterferenceGraph::nodesCount()
{
	return nodeMap.size();
}

CInterferenceGraph::CInterferenceGraph(CNodeList* flowNodes, AssemFlowGraph* flowGraph)
{ 
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

	while (flowNodes != NULL) //добавление всех вершин
	{	
		
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

void CInterferenceGraph::WriteGraph(string path)
{
	ofstream out(path);
	
	out << "digraph G {" << endl;

	for ( auto n : nodeMap )
	{
		for ( auto e : n.second->GetEdgeMap() )
		{
			out << n.second->GetId() << "->" << e.second->getSecond()->GetId() << endl;
		}
	}

	for ( auto n : nodeMap )
	{
		out << n.second->GetId() << "[label=\"" << n.second->GetTemp()->getName() << "\"]" << endl;
	}

	out << "}" ;
}