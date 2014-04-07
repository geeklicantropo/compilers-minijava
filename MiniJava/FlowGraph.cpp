#include "FlowGraph.h"


const Temp::CTempList* AssemFlowGraph::def( const CNode* node )
{
	return nodeToInstructionTable[node]->DefinedVars();
}

const Temp::CTempList* AssemFlowGraph::use( const CNode* node )
{
	return nodeToInstructionTable[node]->UsedVars();
}

bool AssemFlowGraph::isMove( const CNode* node )
{
	const CodeGeneration::CMove* move = dynamic_cast <const CodeGeneration::CMove*> ( nodeToInstructionTable[node] );
	return ( move != 0 );
}

AssemFlowGraph::AssemFlowGraph( CodeGeneration::IInstructionList* instructions )
{
	// Составим словарь с возможностью получать по лейблу соответствующую инструкцию.
	const CodeGeneration::IInstruction* currentInstr = 0;
	for( const CodeGeneration::IInstructionList* p = instructions; p != 0; p = instructions->GetNext() ) {
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
	for( const CodeGeneration::IInstructionList* p = instructions; p != 0; p = instructions->GetNext() ) {
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
	for( const CodeGeneration::IInstructionList* p = instructions; p != 0; p = instructions->GetNext() ) {
		currentInstr = p->GetInstr();
		CNode* currentNode = instructionToNodeTable[currentInstr];
		const Temp::CLabelList* labelsOut = currentInstr->JumpTargets()->GetLabels();
		const Temp::CLabel* currentLabel = 0;
		for( const Temp::CLabelList* l = labelsOut; l != 0; l = labelsOut->Next() ) {
			currentLabel = l->Label();
			const CodeGeneration::IInstruction* instrOut = labelToInstructionTable[currentLabel];
			CNode* nodeOut = instructionToNodeTable[instrOut];
			AddEdge( currentNode, nodeOut );
		}
	}
	// Граф построен.
}