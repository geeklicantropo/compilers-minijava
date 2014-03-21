#include <assert.h>

class Node;
class NodeList;
class Graph;

class Node {
public:
	Node() {}
	Node( Graph* g );

	Graph* GetGraph() const;
	NodeList* GetSuccs() const;
	NodeList* GetPreds() const;
	void SetSuccs( NodeList* _succs );
	void SetPreds( NodeList* _preds );

	NodeList* MergeSuccAndPred();

	int InDegree();
	int OutDegree();
	int Degree();

	bool GoesTo( const Node* n ) const;
	bool ComesFrom( const Node* n ) const;
	bool Achieve( const Node* n );
private:
	Graph* currentGraph;
	int nodeKey;
	
	NodeList* succs;
	NodeList* preds;

	NodeList* cat( NodeList* a, NodeList* b );
};

class NodeList {
public:
	NodeList( const Node* n,  NodeList* rest ) : node( n ), next ( rest ) {}
	const Node* GetNode() const;
	NodeList* GetNext() const;
	void SetNext( NodeList* n );
	int Length();
private:
	const Node* node;
	NodeList* next;
};

class Graph {
public:
	int nodeCount;
	NodeList* last;
	NodeList* nodes;
	Graph() { nodeCount = 0; }

	Node* newNode();
	bool InList( const Node* n, NodeList* list );
	void AddEdge( Node* from, Node* to );
	void RemoveEdge( Node* from, Node* to );
private:
	bool check( const Node* n );
	NodeList* remove( const Node* n, NodeList* list );
};