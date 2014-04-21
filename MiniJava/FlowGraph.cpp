#include "FlowGraph.h"


const std::set<const Temp::CTemp*> AssemFlowGraph::GetDefSet( const CNode* node )
{
	const Temp::CTempList* varList = nodeToInstructionTable[node]->DefinedVars();
	std::set<const Temp::CTemp*> result;
	for( const Temp::CTempList* p = varList; p != 0; p = p->Next() ) {
		const Temp::CTemp* currentVar = p->Temp();
		result.insert( currentVar );
	}
	return result;
}

const std::set<const Temp::CTemp*> AssemFlowGraph::GetUseSet( const CNode* node )
{
	const Temp::CTempList* varList = nodeToInstructionTable[node]->UsedVars();
	std::set<const Temp::CTemp*> result;
	for( const Temp::CTempList* p = varList; p != 0; p = p->Next() ) {
		const Temp::CTemp* currentVar = p->Temp();
		result.insert( currentVar );
	}
	return result;
}

bool AssemFlowGraph::IsMove( const CNode* node )
{
	const CodeGeneration::CMove* move = dynamic_cast <const CodeGeneration::CMove*> ( nodeToInstructionTable[node] );
	return ( move != 0 );
}

AssemFlowGraph::AssemFlowGraph( CodeGeneration::IInstructionList* instructions )
{
	// Составим словарь с возможностью получать по лейблу соответствующую инструкцию.
	const CodeGeneration::IInstruction* currentInstr = 0;
	for( const CodeGeneration::IInstructionList* p = instructions; p != 0; p = p->GetNext() ) {
		currentInstr = p->GetInstr();
		const CodeGeneration::CLabel* label = dynamic_cast<const CodeGeneration::CLabel*>( currentInstr );
		if( label != 0 ) {
			labelToInstructionTable.insert( std::pair<const Temp::CLabel*, const CodeGeneration::IInstruction*>( label->GetLable(), currentInstr ) );
		}
	}
	CNode* prevNode = 0;
	CNode* currentNode = 0;
	// Добавляем в граф вершины по одной и сразу проводим рёбра от предыдущей вершины к следующей, если у неё 
	// пуст список JumpTargets.
	for( const CodeGeneration::IInstructionList* p = instructions; p != 0; p = p->GetNext() ) {
		currentInstr = p->GetInstr();
		prevNode = currentNode;
		currentNode = new CNode( this );
		nodeToInstructionTable.insert( std::pair<const CNode*, const CodeGeneration::IInstruction*>( currentNode, currentInstr ) );
		instructionToNodeTable.insert( std::pair<const CodeGeneration::IInstruction*, CNode*>( currentInstr, currentNode ) );
		if( prevNode != 0 ) {
			const CodeGeneration::IInstruction* prevInstr = nodeToInstructionTable[prevNode];
			if( prevInstr->JumpTargets() == 0 ) {
				AddEdge( prevNode, currentNode );
			}
		}
	}
	// Теперь добавим в граф все рёбра в вершины из списков JumpTargets для каждой инструкции.
	for( const CodeGeneration::IInstructionList* p = instructions; p != 0; p = p->GetNext() ) {
		currentInstr = p->GetInstr();
		CNode* currentNode = instructionToNodeTable[currentInstr];
		const CodeGeneration::CTargets* jt = currentInstr->JumpTargets();
		if( jt != 0 ) {
			const Temp::CLabelList* labelsOut = jt->GetLabels();
			const Temp::CLabel* currentLabel = 0;
			for( const Temp::CLabelList* l = labelsOut; l != 0; l = labelsOut->Next() ) {
				currentLabel = l->Label();
				const CodeGeneration::IInstruction* instrOut = labelToInstructionTable[currentLabel];
				CNode* nodeOut = instructionToNodeTable[instrOut];
				AddEdge( currentNode, nodeOut );
			}
		}
	}
	// Граф построен.
	
	CNodeList* revNodeList = this->GetNodes();
	revNodeList->Reverse();
	int i = 0;
	for( CNodeList* n = revNodeList; n != 0; n = n->GetNext() ) {
		liveIn[n->GetNode()] = this->GetUseSet( n->GetNode() );
	}
	bool isAnythingChanged = true;
	while( isAnythingChanged ) {
		isAnythingChanged = false;
		for( CNodeList* p = revNodeList; p != 0; p = p->GetNext() ) {
			const CNode* currentNode = p->GetNode();
			CNodeList* predList = currentNode->GetPreds();
			for( CNodeList* pp = predList; pp != 0; pp = pp->GetNext() ) {
				const CNode* currentPred = pp->GetNode();
				for( std::set<const Temp::CTemp*>::iterator it = liveIn[currentNode].begin(); it != liveIn[currentNode].end(); it++ ) {
					if( liveOut[currentPred].insert( *it ).second ) {
						isAnythingChanged = true;
					}
				}
			}
			for( std::set<const Temp::CTemp*>::iterator it = liveOut[currentNode].begin(); it != liveOut[currentNode].end(); it++ ) {
				std::set<const Temp::CTemp*> currentDef = GetDefSet( currentNode );
				if( currentDef.find( *it ) == currentDef.end() ) {
					if( liveIn[currentNode].insert( *it ).second ) {
						isAnythingChanged = true;
					}
				}
			}
		}
	}
}

const CodeGeneration::IInstruction* AssemFlowGraph::GetInstruction( const CNode* node )
{
	return nodeToInstructionTable[node];
}

std::set<const Temp::CTemp*> AssemFlowGraph::GetLiveIn( const CNode* node )
{
	return liveIn[node];
}

std::set<const Temp::CTemp*> AssemFlowGraph::GetLiveOut( const CNode* node )
{
	return liveOut[node];
}