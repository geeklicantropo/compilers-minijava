#include "Temp.h"

class IRTreeVisitor;
class CConst;
class CName; 
class CTemp; 
class CBinOp;
class CMem;
class CCall; 
class CEseq; 
class CMove; 
class CExp;
class CJump;
class CCJump;
class CSeq;
class CLabel;
class CExpList;
class CStmList;

namespace IRTree {
	class IExpression {
	public:
		virtual ~IExpression() {}
		virtual void Accept( const IRTreeVisitor* v ) const = 0;
		virtual const CExpList* GetChild() const = 0;
	};

	class IStatement {
	public:
		virtual ~IStatement() {}
		virtual void Accept( const IRTreeVisitor* v ) const = 0;
		virtual const CStmList* GetChild() const = 0;
	};
};

class IRTreeVisitor {
	virtual ~IRTreeVisitor() {}
	virtual void Visit( const CConst& p ) = 0;
	virtual void Visit( const CName& p ) = 0;
	virtual void Visit( const CTemp& p ) = 0;
	virtual void Visit( const CBinOp& p ) = 0;
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

	void Accept( const IRTreeVisitor* v ) const;
	const CExpList* GetChild() const;
private:
	int val;
};

class CName : IRTree::IExpression {
public:
	CName( const CLabel* _label );
	const CLabel* GetLabel() const;
	
	void Accept( const IRTreeVisitor* v ) const;
	const CExpList* GetChild() const;
private:
	const CLabel* label;
};

class CTemp : IRTree::IExpression {
public:
	CTemp( const Temp::CTemp* t );
	const Temp::CTemp* GetTemp() const;

	void Accept( const IRTreeVisitor* v ) const;
	const CExpList* GetChild() const;
private:
	const Temp::CTemp* temp;
};

class CBinOp : IRTree::IExpression {
public:
	CBinOp( const IRTree::IExpression* _left, const IRTree::IExpression* _right, TBinOp _binop );
	TBinOp GetBinOp( ) const;
	const IRTree::IExpression* GetLeft() const;
	const IRTree::IExpression* GetRight() const;

	void Accept( const IRTreeVisitor* v ) const;
	const CExpList* GetChild() const;
private:
	const IRTree::IExpression* left;
	const IRTree::IExpression* right;
	TBinOp binop;
};

class CMem : IRTree::IExpression {
public:
	CMem( const IRTree::IExpression* e );
	const IRTree::IExpression* GetExp() const;

	void Accept( const IRTreeVisitor* v ) const;
	const CExpList* GetChild() const;
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
