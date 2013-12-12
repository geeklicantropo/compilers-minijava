#pragma once
#include "IRTranslationTree.h"

class IRTreePrinter: public IRTreeVisitor
{
public:
	IRTreePrinter() {}
	void Visit( const CConst& p );
	void Visit( const CName& p );
	void Visit( const CTemp& p );
	void Visit( const CBinOp& p );
	void Visit( const CMem& p );
	void Visit( const CCall& p );
	void Visit( const CEseq& p );
	void Visit( const CMove& p );
	void Visit( const CExp& p );
	void Visit( const CJump& p );
	void Visit( const CCJump& p );
	void Visit( const CSeq& p );
	void Visit( const CLabel& p );
	void Visit( const CExpList& p );
	void Visit( const CStmList& p );
};
