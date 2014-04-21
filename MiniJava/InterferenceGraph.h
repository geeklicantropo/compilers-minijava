#pragma once
#include <assert.h>
#include "Temp.h"
#include <vector>

class CInterferenceGraphEdge;
class CInterferenceGraphEdgeType;
class CInterferenceGraphNode;
class CInterferenceGraph;

class CInterferenceGraphEdge
{
private:
	CInterferenceGraphNode *first;
	CInterferenceGraphNode *second;
	CInterferenceGraphEdgeType *type;
public:
	CInterferenceGraphEdge (CInterferenceGraphNode *f, CInterferenceGraphNode *s, CInterferenceGraphEdgeType *t);
	CInterferenceGraphNode* getFirst();
	CInterferenceGraphNode* getSecond();

};

class CInterferenceGraphEdgeType
{
//здесь предполагается хранить тип инструкции, которой соответствует ребро. енамчик
private:
	
public:
};

class CInterferenceGraphNode
{
private:
	int color; //для раскрашивания графа
	Temp::CTemp *temp;
	vector<CInterferenceGraphEdge*> edgeArray;
public:
	CInterferenceGraphNode(Temp::CTemp *t);
	void AddEdge(CInterferenceGraphNode *n, CInterferenceGraphEdgeType *t);
	void DeleteEdge(CInterferenceGraphNode *n);
	void DeleteAllEdges();
	bool ExistEdge(CInterferenceGraphNode *n);
};

class CInterferenceGraph
{

private:
	vector<CInterferenceGraphNode*> nodeArray;
	//vector<CInterferenceGraphEdge> edgeArray;

public:
	CInterferenceGraph();
	void AddNode(CInterferenceGraphNode *n);
	void AddEdge(CInterferenceGraphEdge *e);

	void DeleteEdge(CInterferenceGraphEdge *e);
	void DeleteNode(CInterferenceGraphNode *n);

	bool existEdge(CInterferenceGraphEdge *e);
};