#pragma once
#include <assert.h>
#include "InterferenceGraph.h"

CInterferenceGraphNode :: CInterferenceGraphNode(Temp::CTemp *t)
{
	temp = t;
	color = -1;
}

void CInterferenceGraphNode::AddEdge(CInterferenceGraphNode *n, CInterferenceGraphEdgeType *t)
{
	CInterferenceGraphEdge *edge = new CInterferenceGraphEdge(this, n, t);
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

CInterferenceGraphEdge::CInterferenceGraphEdge (CInterferenceGraphNode *f, CInterferenceGraphNode *s, CInterferenceGraphEdgeType *t)
{
	type = t;
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

CInterferenceGraph::CInterferenceGraph()
{} //здесь будет построение графа

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