#include "Graph.h"
#include "Temp.h"
#include "CodeGeneration.h"

class CInstrNode:Node {
public:
	CInstrNode( CodeGeneration::IInstruction* _instraction ) : instraction( _instraction ) {}
	CodeGeneration::IInstruction* GetInstraction() const;
private:
	CodeGeneration::IInstruction* instraction;
};

class FlowGraph:Graph {
public:
	const Temp::CTempList* def( const CInstrNode* node );
	const Temp::CTempList* use( const CInstrNode* node );
	bool isMove( const CInstrNode* node );
};

class AssemFlowGraph:FlowGraph {
public:
	CodeGeneration::IInstruction* instraction( const CInstrNode* node );
	AssemFlowGraph( CodeGeneration::IInstructionList* instractions );
};