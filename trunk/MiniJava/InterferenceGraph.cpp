#pragma once
#include <assert.h>
#include "InterferenceGraph.h"
#include <fstream>
#include <stack>
#include "Symbols.h"
#include "Frame.h"


CInterferenceGraphNode :: CInterferenceGraphNode(const Temp::CTemp *t, int _id)
{
	temp = t;
	color = -1;
	id = _id;
	freezed = false;
	edgeMap.clear();
}

void CInterferenceGraphNode::AddEdge(CInterferenceGraphNode *n, bool isMove)
{
	if ( edgeMap.find(n) != edgeMap.end() && edgeMap.find(n)->second->IsMove() == isMove ) return;
	else edgeMap.insert ( make_pair(n, new CInterferenceGraphEdge(this, n, isMove) ) );

}

bool CInterferenceGraphNode::ExistEdge(CInterferenceGraphNode *n, bool isMove)
{
	return ( edgeMap.find(n) != edgeMap.end() && edgeMap.find(n)->second->IsMove() == isMove );
}

void CInterferenceGraphNode::DeleteAllEdges()
{
	for ( auto e : edgeMap )
	{
		e.second->getSecond()->DeleteEdge(this, false);
		e.second->getSecond()->DeleteEdge(this, true);
	}
}

void CInterferenceGraphNode::DeleteEdge(CInterferenceGraphNode *n, bool isMove)
{
	if ( edgeMap.find(n) != edgeMap.end() && edgeMap.find(n)->second->IsMove() == isMove )
		edgeMap.erase( edgeMap.find(n) );
}

const Temp::CTemp* CInterferenceGraphNode::GetTemp()
{
	return temp;
}

int CInterferenceGraphNode::GetId()
{
	return id;
}

std::map<CInterferenceGraphNode*, CInterferenceGraphEdge*> CInterferenceGraphNode::GetEdgeMap()
{
	return edgeMap;
}

bool CInterferenceGraphNode::HasMoveEdge()
{
	for (auto e : edgeMap)
		if (e.second->IsMove()) 
			return true;
	return false;
}

bool CInterferenceGraphNode::IsFreezed()
{
	return freezed;
}

void CInterferenceGraphNode::SetFreeze(bool f)
{
	freezed = f;
}

int CInterferenceGraphNode::GetColor()
{
	return color;
}

void CInterferenceGraphNode::SetColor(int c)
{
	color = c;
}

CInterferenceGraphEdge::CInterferenceGraphEdge (CInterferenceGraphNode *f, CInterferenceGraphNode *s, bool _isMove)
{
	isMove = _isMove;
	first = f;
	second = s;
}
	
CInterferenceGraphNode* CInterferenceGraphEdge::getFirst()
{
	return first;
}
	
CInterferenceGraphNode* CInterferenceGraphEdge::getSecond()
{
	return second;
}

bool CInterferenceGraphEdge::IsMove()
{
	return isMove;
}

void CInterferenceGraph::AddNode(const Temp::CTemp *t)
{
	if ( nodeMap.find( t ) == nodeMap.end() ) 
		nodeMap.insert( pair<const Temp::CTemp*, CInterferenceGraphNode*> (t , new CInterferenceGraphNode(t, nextId++ )) );
}

void CInterferenceGraph::AddNode(CInterferenceGraphNode *n)
{
	const Temp::CTemp *t = n -> GetTemp();
	if ( nodeMap.find( t ) == nodeMap.end() ) 
		nodeMap.insert( make_pair (t , n) );
}

void CInterferenceGraph::AddEdge(CInterferenceGraphEdge *e)
{
	e->getFirst() ->AddEdge( e->getSecond(), e->IsMove() );
	e->getSecond()->AddEdge( e->getFirst(),  e->IsMove() );
}

void CInterferenceGraph::DeleteEdge(CInterferenceGraphEdge *e)
{
	e->getFirst()->DeleteEdge(e->getSecond(), e->IsMove() );
	e->getSecond()->DeleteEdge(e->getFirst(), e->IsMove() );
}

void CInterferenceGraph::DeleteNode(CInterferenceGraphNode *n)
{
	n->DeleteAllEdges();

	if ( nodeMap.find( n->GetTemp() ) != nodeMap.end() )
		nodeMap.erase( nodeMap.find( n->GetTemp() ) );

}

bool CInterferenceGraph::existEdge(CInterferenceGraphEdge *e)
{
	return e->getFirst()->ExistEdge(e->getSecond(), e->IsMove() );
}

CInterferenceGraphNode* CInterferenceGraph::getNode(const Temp::CTemp *t)
{
	if ( nodeMap.find( t ) != nodeMap.end() ) 
		return nodeMap[t];
	else
		return NULL;

}

int CInterferenceGraph::nodesCount()
{
	return nodeMap.size();
}

CInterferenceGraphNode* CInterferenceGraph::PopEdge(CInterferenceGraphEdge* edge, int k)
{
	CInterferenceGraphNode* fst = edge->getFirst();
	CInterferenceGraphNode* snd = edge->getSecond();

	string name = fst->GetTemp()->getName() + "&" + snd->GetTemp()->getName();
	const Temp::CTemp* temp = new Temp::CTemp( CSymbol::CSymbolGet(name) );
	
	CInterferenceGraphNode* node = new CInterferenceGraphNode( temp, nextId++ );

	for ( auto n : nodeMap )
	{
		if ( fst->GetTemp() != n.second->GetTemp() && snd->GetTemp() != n.second->GetTemp() )
		{
			if ( fst->ExistEdge(n.second, true ) )
				node -> AddEdge( n.second, true );
			if ( fst->ExistEdge(n.second, false) ) 
				node -> AddEdge( n.second, false );
			if ( snd->ExistEdge(n.second, true ) ) 
				node -> AddEdge( n.second, true );
			if ( snd->ExistEdge(n.second, false) )
				node -> AddEdge( n.second, false );
		}
	}

	if (node->GetEdgeMap().size() < k)
	{
		for (auto e : node->GetEdgeMap() )
		{
			e.second->getSecond()->AddEdge( node, e.second->IsMove() );
		}

		AddNode(node);

		if ( fst->innerTemps.empty() ) node->innerTemps.push_back(fst->GetTemp());
		else node->innerTemps.insert( node->innerTemps.end(), fst->innerTemps.begin(), fst->innerTemps.begin() );

		if ( snd->innerTemps.empty() ) node->innerTemps.push_back(snd->GetTemp());
		else node->innerTemps.insert( node->innerTemps.end(), snd->innerTemps.begin(), snd->innerTemps.begin() );

		DeleteNode(fst);
		DeleteNode(snd);
		return node;
	}

	nextId--;
	return NULL;
}

CInterferenceGraph::CInterferenceGraph(CNodeList* flowNodes, AssemFlowGraph* flowGraph)
{ 
	nextId = 1;
	nodeMap.clear();

	CNodeList* flowNodes1 = flowNodes;

	while (flowNodes != NULL)
	{
		for ( auto temp : flowGraph->GetUseSet(flowNodes->GetNode()) )
		{
			AddNode(temp);
		}
		flowNodes = flowNodes->GetNext();
	}

	flowNodes = flowNodes1;
	while (flowNodes != NULL)
	{
		for ( auto temp : flowGraph->GetDefSet(flowNodes->GetNode()) )
		{
			AddNode(temp);
		}
		flowNodes = flowNodes->GetNext();
	}

	flowNodes = flowNodes1;
	while (flowNodes != NULL) //добавление всех вершин
	{	
		
		if ( flowGraph->IsMove(flowNodes->GetNode()) )
		{
			const Temp::CTemp* dst = ( dynamic_cast <const CodeGeneration::CMove*> ( flowGraph->GetInstruction( flowNodes->GetNode() ) ) ) -> GetDst();
			const Temp::CTemp* src = ( dynamic_cast <const CodeGeneration::CMove*> ( flowGraph->GetInstruction( flowNodes->GetNode() ) ) ) -> GetSrc();
			
			//if ( getNode( src ) == NULL ) AddNode( src );
			//if ( getNode( dst ) == NULL ) AddNode( dst );

			if (src != dst)
			//if (src != dst && getNode( src ) != NULL && getNode( dst ) != NULL)
			{
				getNode( dst )->AddEdge( getNode( src ), true );
				getNode( src )->AddEdge( getNode( dst ), true );
			}

			for ( auto to : flowGraph->GetLiveOut( flowNodes->GetNode() ) )
			{
				if (to != src && to != dst)
				//if (to != src && to != dst && getNode( src ) != NULL && getNode( dst ) != NULL && getNode( to ) != NULL)
				{
					getNode( dst )->AddEdge( getNode( to ), false );
					getNode( to )->AddEdge( getNode( dst ), false );
				}
			}

		} else
		{
			for ( auto from : flowGraph->GetDefSet( flowNodes->GetNode() ) )
			{
				//if ( getNode( from ) == NULL ) AddNode( from );
				//if ( getNode( from ) != NULL ) 
				{
					for ( auto to : flowGraph->GetLiveOut( flowNodes->GetNode() ) )
					{
				
						//if ( getNode( to ) == NULL ) AddNode( to );
						//if (to != from && getNode( to ) != NULL)
						if (to != from)
						{
							getNode( from )->AddEdge( getNode( to ), false );
							getNode( to )->AddEdge( getNode( from ), false );
						}
					}
				}
			}
		}
		flowNodes = flowNodes->GetNext();
	}

} 

void CInterferenceGraph::WriteGraph(string path, bool enableColors, int k)
{
	ofstream out(path);
	
	out << "graph G {" << endl;

	vector<string> color;
	color.push_back("red");
	color.push_back("orange");
	color.push_back("yellow");
	color.push_back("green");
	color.push_back("blue");
	color.push_back("violet");
	color.push_back("purple");
	color.push_back("maroon");
	color.push_back("navy");
	
	std::map<CInterferenceGraphNode*, bool> used;
	for ( auto n : nodeMap )
	{
		used.insert( make_pair (n.second, false) );
	}

	for ( auto n : nodeMap )
	{
		for ( auto e : n.second->GetEdgeMap() )
		{
			if ( !used[e.second->getSecond()] ) 
			{
				out << n.second->GetId() << "--" << e.second->getSecond()->GetId();
				if ( e.second->IsMove() ) 
					out << " [style=dotted]"; 
				out << endl;
			}
		}
		used[n.second] = true;
	}

	for ( auto n : nodeMap )
	{
		out << n.second->GetId() << " [label=\"" << n.second->GetTemp()->getName();
		
		if ( enableColors && n.second->GetColor() < k && n.second->GetColor() < color.size() )
			out << "\",fillcolor=\"" << color [n.second->GetColor()] << "\",style=\"filled";
		
		if ( enableColors && n.second->GetColor() < k && n.second->GetColor() >= color.size() )
			out << "\",fillcolor=\"" << "white" << "\",style=\"filled";
		
		if ( enableColors && n.second->GetColor() == k)
			out << "\",fillcolor=\"" << "grey" << "\",style=\"filled";
		
		out << "\"]" << endl;
	}

	out << "}" ;
}

CInterferenceGraph::CInterferenceGraph(CInterferenceGraph* graph)
{
	nextId = graph->nextId;

	for ( auto n : graph->nodeMap )
	{
		nodeMap.insert ( make_pair ( n.first, new CInterferenceGraphNode(n.first, n.second->GetId() ) ) );
	}
	for ( auto n : graph->nodeMap )
	{
		for ( auto e : n.second->GetEdgeMap() )
		{
			AddEdge(
				new CInterferenceGraphEdge(
					getNode( e.second->getFirst() ->GetTemp() ),
					getNode( e.second->getSecond()->GetTemp() ),
					e.second->IsMove() ) );
		}
	}
}

CInterferenceGraphNode* CInterferenceGraph::simplify(int K)
{
	for (auto n : nodeMap)
	{
		if (n.second->GetEdgeMap().size() < K && !n.second->HasMoveEdge() ||
			n.second->GetEdgeMap().size() < K &&  n.second->HasMoveEdge() && n.second->IsFreezed() )
		{
			DeleteNode(n.second);
			return n.second;
		}

	}
	return NULL;
}

CInterferenceGraphNode* CInterferenceGraph::coalesce(int k)
{
	for (auto n : nodeMap)
	{
		for ( auto e : n.second->GetEdgeMap() )
		{
			if ( e.second->IsMove() && !e.second->getFirst()->IsFreezed() && !e.second->getSecond()->IsFreezed() )
			{
				CInterferenceGraphNode* node = PopEdge ( e.second, k );
				if (node != NULL)
				{
					DeleteNode(node);
					return node;
				}
			}
		}
	}
	return NULL;
}

bool CInterferenceGraph::freeze(int k)
{
	for (auto n : nodeMap)
	{
		if ( !n.second->IsFreezed() && n.second->GetEdgeMap().size() < k && n.second->HasMoveEdge() )
		{
			n.second->SetFreeze(true);
			return true;
		}
	}
	return false;
}

CInterferenceGraphNode* CInterferenceGraph::spill()
{
	int min = 100000;
	CInterferenceGraphNode* node = NULL;

	for (auto n : nodeMap)
	{
		if (n.second->GetEdgeMap().size() < min)
			node = n.second;
	}

	DeleteNode(node);
	return node;
}

void CInterferenceGraph::addRegisterClique (int k)
{
	vector <CInterferenceGraphNode*> regNodes;
	for (int i = 1; i <= k; i++)
	{
		string name = "R" + to_string(i);
		const Temp::CTemp* temp = new Temp::CTemp( CSymbol::CSymbolGet(name) );
		CInterferenceGraphNode* node = new CInterferenceGraphNode( temp, nextId++ );
		regNodes.push_back(node);
		AddNode(node);
	}
	for ( auto n1 : regNodes )
		for ( auto n2 : regNodes )
			if (n1 != n2)
			{
				n1->AddEdge(n2, false);
				n2->AddEdge(n1, false);
			}
}

void CInterferenceGraph::SetColors(int K)
{
	addRegisterClique(K);

	CInterferenceGraph graph = CInterferenceGraph(this);
	//std::stack< const Temp::CTemp* > stack;
	std::stack<CInterferenceGraphNode*> stack;
	while ( graph.nodesCount() > 0 )
	{
		bool simplifyResult, coalesceResult, freezeResult;

		CInterferenceGraphNode* node = graph.simplify(K);
		simplifyResult = node != NULL;
		if ( simplifyResult ) stack.push( node );
		if (graph.nodesCount() == 0) continue;

		node = graph.coalesce(K);
		coalesceResult = node != NULL;
		if ( coalesceResult ) stack.push( node );
		if (graph.nodesCount() == 0) continue;

		if (!simplifyResult && !coalesceResult) freezeResult = graph.freeze(K);

		if (!simplifyResult && !coalesceResult && !freezeResult) stack.push( graph.spill() );
		
		//stack.push ( temp );
	}

	while (!stack.empty())
	{
		CInterferenceGraphNode* node = stack.top();
		const Temp::CTemp* temp = node->GetTemp();
		stack.pop();

		std::vector<bool> posColor(K + 1, true);
		
		if ( node->innerTemps.empty() )
		{
			for ( auto e : getNode ( temp ) -> GetEdgeMap() )
			{
				if ( e.second->getSecond()->GetColor() != -1 && e.second->getSecond()->GetColor() != K && !e.second->IsMove() )
					posColor[ e.second->getSecond()->GetColor() ] = false;
			}
			
			int c = -1;
		
			for (int i = 0; i <= K && c == -1; i++)
				if (posColor[i]) c = i;

			getNode(temp)->SetColor(c);
		}
		else
		{
			for ( auto t : node->innerTemps )
			{
				for ( auto e : getNode ( t ) -> GetEdgeMap() )
				{
					if ( e.second->getSecond()->GetColor() != -1 && e.second->getSecond()->GetColor() != K )
						posColor[ e.second->getSecond()->GetColor() ] = false;
				}
			}
		
			int c = -1;
		
			for (int i = 0; i <= K && c == -1; i++)
				if (posColor[i]) c = i;

			for (auto t : node->innerTemps)
				getNode(t)->SetColor(c);
		}

	}
}

CodeGeneration::IInstructionList*  CInterferenceGraph::UpdateInstructionList( CodeGeneration::IInstructionList* instrList, int K, const CFrame* frame, AssemFlowGraph& afg )
{
	CodeGeneration::IInstructionList* newInstrList = 0;
	CodeGeneration::IInstructionList* head = 0;
	for( auto n : nodeMap ) {
		if( n.second->GetColor() == K ) {
			const Temp::CTemp* currentTemp = n.second->GetTemp();
			while( instrList != 0 ) {
				const CodeGeneration::IInstruction* currentInstr = instrList->GetInstr();
				if( head == 0 ) {
					newInstrList = new CodeGeneration::IInstructionList( currentInstr, 0 );
					head = newInstrList;
				} else {
					newInstrList->SetNext( new CodeGeneration::IInstructionList( currentInstr, 0 ) );
					newInstrList = newInstrList->GetNext();
				}
				
				const Temp::CTempList* varList = currentInstr->DefinedVars();
				std::set<const Temp::CTemp*> defSet, usedSet;
				for( const Temp::CTempList* p = varList; p != 0; p = p->Next() ) {
					const Temp::CTemp* currentVar = p->Temp();
					defSet.insert( currentVar );
				}
				varList = currentInstr->UsedVars();
				for( const Temp::CTempList* p = varList; p != 0; p = p->Next() ) {
					const Temp::CTemp* currentVar = p->Temp();
					usedSet.insert( currentVar );
				}
				for( const Temp::CTemp* temp : defSet ) {
					if( temp == n.second->GetTemp() ) {
						int offset = frame->GetOffSet();
						string s = "STORE M['s0+" + to_string( offset ) + "] <- 's1\n";
						Temp::CTempList* list = new Temp::CTempList( frame->GetFP(), new Temp::CTempList( temp, 0 ) );
						newInstrList->SetNext( new CodeGeneration::IInstructionList( new CodeGeneration::COper( s, 0, list ), 0 ));
						newInstrList = newInstrList->GetNext();
					}
				}

				for( const Temp::CTemp* temp : usedSet) {
					if( temp == n.second->GetTemp() ) {
						int offset = frame->GetOffSet();
						string s = "LOAD M['s0+" + to_string( offset ) + "] <- 's1\n";
						Temp::CTempList* list = new Temp::CTempList( frame->GetFP(), new Temp::CTempList( temp, 0 ) );
						newInstrList->SetNext( new CodeGeneration::IInstructionList( new CodeGeneration::COper( s, 0, list ), 0 ));
						newInstrList = newInstrList->GetNext();
					}
				}
				instrList = instrList->GetNext();
			}
		}
	}
	return head;
}

bool CInterferenceGraph::IsColored(int K)
{
	for (auto n : nodeMap)
	{
		if (n.second->GetColor() == K) 
			return false;
	}
	return true;
}

void CInterferenceGraph::CleanColor( )
{
	for( auto n : nodeMap ) {
		n.second->SetColor(0);	
	}
}
