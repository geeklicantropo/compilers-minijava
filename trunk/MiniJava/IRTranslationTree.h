#include "Temp.h"

namespace IRTree {
	class IExpression {
	public:
		virtual ~IExpression() {}
		virtual void Accept( const IRTreeVisitor* v ) const;
		virtual const CExpList* GetChild() const {}
	};

	class IStatement {
	public:
		virtual ~IStatement() {}
		virtual void Accept( const IRTreeVisitor* v ) const;
		virtual const CStmList* GetChild() const {}
	};
};

class IRTreeVisitor {
	virtual ~IRTreeVisitor() {}
	virtual void Visit( const CConst& p ) = 0;
	virtual void Visit( const CName& p ) = 0;
	virtual void Visit( const CTemp& p ) = 0;
	virtual void Visit( const CBinop& p ) = 0;
	virtual void Visit( const CMem& p ) = 0;
	virtual void Visit( const CCall& p ) = 0;
	virtual void Visit( const CEseq& p ) = 0;
	virtual void Visit( const CMove& p ) = 0;
	virtual void Visit( const CExp& p ) = 0;
	virtual void Visit( const CJump& p ) = 0;
	virtual void Visit( const CCJump& p ) = 0;
	virtual void Visit( const CSeq& p ) = 0;
	virtual void Visit( const CLabel& p ) = 0;
	virtual void Visit( const CExpList& p ) = 0;
	virtual void Visit( const CStmList& p ) = 0;
};

enum TBinOp {
	PLUS, MINUS, MUL, DIV, AND, OR, LSHIFT, RSHIFT, ARSHIFT, XOR
};

enum TCJump {
	EQ, NE, LT, GT, LE, GE, ULT, ULE, UGT, UGE
};

class CConst : IRTree::IExpression {
public:
	CConst( int v );
	int GetValue() const;
private:
	int val;
};

class CName : IRTree::IExpression {
public:
	CName( const CLabel* _label );
	const CLabel* GetLabel() const;
private:
	const CLabel* label;
};

class CTemp : IRTree::IExpression {
public:
	CTemp( const CTemp* t );
	const CTemp* GetTemp() const;
private:
	const CTemp* temp;
};

class CBinOp : IRTree::IExpression {
public:
	CBinOp( const IRTree::IExpression* _left, const IRTree::IExpression* _right, TBinOp _binop );
	TBinOp GetBinOp( ) const;
	const IRTree::IExpression* GetLeft() const;
	const IRTree::IExpression* GetRight() const;
private:
	const IRTree::IExpression* left;
	const IRTree::IExpression* right;
	TBinOp binop;
};

class CMem : IRTree::IExpression {
public:
	CMem( const IRTree::IExpression* e );
	const IRTree::IExpression* GetExp() const;
private:
	const IRTree::IExpression* exp;
};

class CCall : IRTree::IExpression {
	
};

class CEseq : IRTree::IExpression {
};

class CMove : IRTree::IStatement {
};

class CExp : IRTree::IStatement {
};

class CJump : IRTree::IStatement {
};

class CCJump : IRTree::IStatement {
};

class CSeq : IRTree::IStatement {
};

class CLabel : IRTree::IStatement {
};

class CExpList {
public:
	CExpList( const IRTree::IExpression* e, const CExpList* n );
	const IRTree::IExpression* GetExp() const;
	const CExpList* GetNext() const;
	void SetExp( const IRTree::IExpression* e );
	void SetNext( const CExpList* n );
private:
	const IRTree::IExpression* exp;
	const CExpList* next;
};

class CStmList {
public:
	CStmList( const IRTree::IStatement* e, const CStmList* n );
	const IRTree::IStatement* GetStm() const;
	const CStmList* GetNext() const;
	void SetStm( const IRTree::IStatement* e );
	void SetNext( const CStmList* n );
private:
	const IRTree::IStatement* stm;
	const CStmList* next;
};