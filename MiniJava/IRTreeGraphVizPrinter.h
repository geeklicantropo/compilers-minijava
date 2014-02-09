#pragma once
#include "IRTranslationTree.h"
#include <stack>
#include <vector>
#include <fstream>


class IRTreeGraphVizPrinter: public IRTree::IRTreeVisitor
{
public:
	IRTreeGraphVizPrinter( ofstream& _out, vector<string>& _labels );
	void Visit( const IRTree::CConst& p );
	void Visit( const IRTree::CName& p );
	void Visit( const IRTree::CTemp& p );
	void Visit( const IRTree::CBinOp& p );
	void Visit( const IRTree::CMem& p );
	void Visit( const IRTree::CCall& p );
	void Visit( const IRTree::CEseq& p );
	void Visit( const IRTree::CMove& p );
	void Visit( const IRTree::CExp& p );
	void Visit( const IRTree::CJump& p );
	void Visit( const IRTree::CCJump& p );
	void Visit( const IRTree::CSeq& p );
	void Visit( const IRTree::CLabel& p );
	void Visit( const IRTree::CExpList& p );
	void Visit( const IRTree::CStmList& p );
private:
	ofstream& out;
	vector<string>& labels;
	stack<int> st;
	int id;
};
