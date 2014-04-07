#pragma once
#include <assert.h>

class CNode;
class CNodeList;
class CGraph;

class CNode {
public:
	CNode() {}
	CNode( CGraph* g );

	CGraph* GetGraph() const;
	CNodeList* GetSuccs() const;
	CNodeList* GetPreds() const;
	void SetSuccs( CNodeList* _succs );
	void SetPreds( CNodeList* _preds );

	CNodeList* MergeSuccAndPred();

	int InDegree();
	int OutDegree();
	int Degree();

	bool GoesTo( const CNode* n ) const;
	bool ComesFrom( const CNode* n ) const;
	bool Adjacent( const CNode* n );
private:
	CGraph* currentGraph;
	int nodeKey;
	
	CNodeList* succs;
	CNodeList* preds;

	CNodeList* cat( CNodeList* a, CNodeList* b );
};

class CNodeList {
public:
	CNodeList( const CNode* n,  CNodeList* rest ) : node( n ), next ( rest ) {}
	const CNode* GetNode() const;
	CNodeList* GetNext() const;
	void SetNext( CNodeList* n );
	int Length();
private:
	const CNode* node;
	CNodeList* next;
};

class CGraph {
public:
	int nodeCount;
	CNodeList* last;
	CNodeList* nodes;
	CGraph() { nodeCount = 0; }

	CNode* newNode();
	bool InList( const CNode* n, CNodeList* list );
	void AddEdge( CNode* from, CNode* to );
	void RemoveEdge( CNode* from, CNode* to );
private:
	bool check( const CNode* n );
	CNodeList* remove( const CNode* n, CNodeList* list );
};