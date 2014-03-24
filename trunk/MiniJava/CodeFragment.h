#pragma once
#include "IRTranslationTree.h"
#include "Frame.h"

class CCodeFragment {
public:
	CCodeFragment( const CFrame* f, const IRTree::IStatement* t, const CCodeFragment* n );
	
	void SetNext( const CCodeFragment* n );
	const CFrame* GetFrame() const;
	const IRTree::IStatement* GetIRTree() const;
	const CCodeFragment* GetNext() const;
private:
	const CFrame* frame;
	const IRTree::IStatement* irtree;
	const CCodeFragment* next;

};