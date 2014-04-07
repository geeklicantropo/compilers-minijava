#pragma once

#include "Graph.h"
#include "Temp.h"
#include "CodeGeneration.h"
#include <map>

class IFlowGraph : public CGraph {
public:
	virtual ~IFlowGraph() {}

	virtual const Temp::CTempList* def( const CNode* node ) = 0;
	virtual const Temp::CTempList* use( const CNode* node ) = 0;
	virtual bool isMove( const CNode* node ) = 0;
};

class AssemFlowGraph: public IFlowGraph {
public:
	AssemFlowGraph( CodeGeneration::IInstructionList* instructions );

	virtual const Temp::CTempList* def( const CNode* node );
	virtual const Temp::CTempList* use( const CNode* node );
	virtual bool isMove( const CNode* node );

	CodeGeneration::IInstruction* instruction( const CNode* node );
private:
	std::map<const Temp::CLabel*, const CodeGeneration::IInstruction*> labelToInstructionTable;
	std::map<const CNode*, const CodeGeneration::IInstruction*> nodeToInstructionTable;
	std::map<const CodeGeneration::IInstruction*, CNode*> instructionToNodeTable;
};