#pragma once

#include "Graph.h"
#include "Temp.h"
#include "CodeGeneration.h"
#include <map>
#include <set>

class IFlowGraph : public CGraph {
public:
	virtual ~IFlowGraph() {}

	virtual const std::set<Temp::CTemp> GetDefSet( const CNode* node ) = 0;
	virtual const std::set<Temp::CTemp> GetUseSet( const CNode* node ) = 0;
	virtual bool IsMove( const CNode* node ) = 0;
};

class AssemFlowGraph: public IFlowGraph {
public:
	AssemFlowGraph( CodeGeneration::IInstructionList* instructions );

	virtual const std::set<Temp::CTemp> GetDefSet( const CNode* node );
	virtual const std::set<Temp::CTemp> GetUseSet( const CNode* node );
	virtual bool IsMove( const CNode* node );

	CodeGeneration::IInstruction* instruction( const CNode* node );
private:
	std::map<const Temp::CLabel*, const CodeGeneration::IInstruction*> labelToInstructionTable;
	std::map<const CNode*, const CodeGeneration::IInstruction*> nodeToInstructionTable;
	std::map<const CodeGeneration::IInstruction*, CNode*> instructionToNodeTable;

	std::map<const CNode*, std::set<Temp::CTemp> > liveIn;
	std::map<const CNode*, std::set<Temp::CTemp> > liveOut;
};