#pragma once
#include "IRTranslationTree.h"
#include "Frame.h"

class CCodeFragment {
public:
	CCodeFragment( const CFrame* f, const IRTree::IExpression* t, const CCodeFragment* n );
	
	void SetNext( const CCodeFragment* n );
	const CFrame* GetFrame() const;
	const IRTree::IExpression* GetIRTree() const;
	const CCodeFragment* GetNext() const;
private:
	const CFrame* frame;
	const IRTree::IExpression* irtree;
	const CCodeFragment* next;
};