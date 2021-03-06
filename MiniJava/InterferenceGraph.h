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
	int id;

	int color; //��� ������������� �����
	const Temp::CTemp *temp;
	bool freezed;
	bool candidate;

	std::map<CInterferenceGraphNode*, CInterferenceGraphEdge*> edgeMap;

	//vector<CInterferenceGraphEdge*> edgeArray;
public:
	CInterferenceGraphNode(const Temp::CTemp *t, int _id);
	void AddEdge(CInterferenceGraphNode *n, bool isMove);
	void DeleteEdge(CInterferenceGraphNode *n, bool isMove);
	bool ExistEdge(CInterferenceGraphNode *n, bool isMove);
	void DeleteAllEdges();
	const Temp::CTemp* GetTemp();
	int GetId();

	std::map<CInterferenceGraphNode*, CInterferenceGraphEdge*> GetEdgeMap();
	bool HasMoveEdge();
	bool IsFreezed();
	void SetFreeze(bool f);

	int GetColor();
	void SetColor(int c);

	void SetCandidate(bool c);
	bool IsCandidate();

	std::vector<const Temp::CTemp*> innerTemps;
};

class CInterferenceGraph
{

private:
	int nextId;

	const CFrame* frame;
	CInterferenceGraphNode* fpNode;

	std::map<const Temp::CTemp*, CInterferenceGraphNode*> nodeMap;
	std::map<const Temp::CTemp*, bool> onStack;

	CInterferenceGraphNode* simplify(int K); //�������� ����� �� ������
 	CInterferenceGraphNode* coalesce(int k);
	bool freeze(int k);
	CInterferenceGraphNode* spill();
	void select();

	void addRegisterClique (int k);
public:
	CInterferenceGraph(CNodeList* flowNodes, AssemFlowGraph* flowGraph, const CFrame* fr, std::map<const Temp::CTemp*, bool> onStack_);
	CInterferenceGraph(CInterferenceGraph* graph);
	void AddNode(const Temp::CTemp *t);
	void AddNode(CInterferenceGraphNode *n);
	void AddEdge(CInterferenceGraphEdge *e);

	CInterferenceGraphNode* PopEdge(CInterferenceGraphEdge *e, int k);

	void DeleteEdge(CInterferenceGraphEdge *e);
	void DeleteNode(CInterferenceGraphNode *n);

	bool existEdge(CInterferenceGraphEdge *e);

	CInterferenceGraphNode* getNode(const Temp::CTemp *t);

	int nodesCount();

	void WriteGraph(string path, bool enableColors, int k);

	void SetColors(int K);

	CodeGeneration::IInstructionList* UpdateInstructionList( CodeGeneration::IInstructionList* instrList, int K, const CFrame* frame, AssemFlowGraph& );
	void CleanColor();

	bool IsColored(int K);

	bool OnStack(const Temp::CTemp* t);
	std::map<const Temp::CTemp*, bool> GetOnStack () { return onStack; }	

	std::map<const Temp::CTemp*, int> GetColorMap ();
};