#include "FlowGraph.h"

CodeGeneration::IInstruction* CInstrNode::GetInstraction() const
{
	return instraction;
}

const Temp::CTempList* FlowGraph::def( const CInstrNode* node )
{
	return node->GetInstraction()->DefinedVars();
}

const Temp::CTempList* FlowGraph::use( const CInstrNode* node )
{
	return node->GetInstraction()->UsedVars();
}

bool FlowGraph::isMove( const CInstrNode* node )
{
	CodeGeneration::CMove* move = dynamic_cast<CodeGeneration::CMove*>( node->GetInstraction() );
	if ( move != 0 )
	{		
		return true;
	}
	return false;
}