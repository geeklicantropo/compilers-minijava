#pragma once
#include "IRTranslationTree.h"

class StmExpList
{
public:
	IRTree::IExpression* exp;
	IRTree::IStatement* stm;
	StmExpList( IRTree::IStatement* _stm, IRTree::IExpression* _exp ) : stm( _stm ), exp( _exp ) {}
};