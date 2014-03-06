#pragma once
#include "IRTranslationTree.h"

class StmExpList
{
public:
	IRTree::CExpList* exps;
	IRTree::IStatement* stm;
	StmExpList( IRTree::IStatement* _stm, IRTree::CExpList* _exps ) : stm( _stm ), exps( _exps ) {}
};

