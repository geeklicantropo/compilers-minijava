#pragma once
#include <assert.h>
#include "Temp.h"
#include "Graph.h"
#include "FlowGraph.h"
#include <vector>

class CInterferenceGraphEdge;
class CInterferenceGraphNode;
class CInterferenceGraph;

class CInterferenceGraphEdge
{
private:
	CInterferenceGraphNode *first;
	CInterferenceGraphNode *second;
	bool isMove;
public:
	CInterferenceGraphEdge (CInterferenceGraphNode *f, CInterferenceGraphNode *s, bool isMove_);
	CInterferenceGraphNode* getFirst();
	CInterferenceGraphNode* getSecond();
	bool IsMove();

};

class CInterferenceGraphNode
{
private:
	int color; //для раскрашивания графа
	const Temp::CTemp *temp;

	std::map<CInterferenceGraphNode*, CInterferenceGraphEdge*> edgeMap;

	//vector<CInterferenceGraphEdge*> edgeArray;
public:
	CInterferenceGraphNode(const Temp::CTemp *t);
	void AddEdge(CInterferenceGraphNode *n, bool isMove);
	void DeleteEdge(CInterferenceGraphNode *n, bool isMove);
	bool ExistEdge(CInterferenceGraphNode *n, bool isMove);
	void DeleteAllEdges();
	const Temp::CTemp* GetTemp();
};

class CInterferenceGraph
{

private:
	vector<CInterferenceGraphNode*> nodeArray;
	std::map<const Temp::CTemp*, CInterferenceGraphNode*> nodeMap;
	//vector<CInterferenceGraphEdge> edgeArray;

public:
	CInterferenceGraph(CNodeList* flowNodes, AssemFlowGraph* flowGraph);
	void AddNode(const Temp::CTemp *t);
	void AddEdge(CInterferenceGraphEdge *e);

	void DeleteEdge(CInterferenceGraphEdge *e);
	void DeleteNode(CInterferenceGraphNode *n);

	bool existEdge(CInterferenceGraphEdge *e);

	CInterferenceGraphNode* getNode(const Temp::CTemp *t);
};