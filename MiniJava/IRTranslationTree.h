#pragma once
#include "Temp.h"

namespace IRTree {

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

	class IExpression {
	public:
		virtual ~IExpression() {}
		virtual void Accept( IRTreeVisitor* v ) const = 0;
		virtual const CExpList* GetChild() const = 0;
	};
	
	class IStatement {
	public:
		virtual ~IStatement() {}
		virtual void Accept( IRTreeVisitor* v ) const = 0;
		virtual const CExpList* GetChild() const = 0;
	};

	class IRTreeVisitor {
	public:
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

	class CConst : public IRTree::IExpression {
	public:
		CConst( int v );
		int GetValue() const;
	
		void Accept( IRTreeVisitor* v ) const;
		const CExpList* GetChild() const;
	private:
		int val;
	};

	class CName : public IRTree::IExpression {
	public:
		CName( const Temp::CLabel* l );
		const Temp::CLabel* GetLabel() const;
		
		void Accept( IRTreeVisitor* v ) const;
		const CExpList* GetChild() const;
	private:
		const Temp::CLabel* label;
	};

	class CTemp : public IRTree::IExpression {
	public:
		CTemp( const Temp::CTemp* t );
		const Temp::CTemp* GetTemp() const;

		void Accept( IRTreeVisitor* v ) const;
		const CExpList* GetChild() const;
	private:
		const Temp::CTemp* temp;
	};

	class CBinOp : public IRTree::IExpression {
	public:
		CBinOp( TBinOp _binop, const IRTree::IExpression* _left, const IRTree::IExpression* _right );
		TBinOp GetBinOp( ) const;
		const IRTree::IExpression* GetLeft() const;
		const IRTree::IExpression* GetRight() const;

		void Accept( IRTreeVisitor* v ) const;
		const CExpList* GetChild() const;
	private:
		const IRTree::IExpression* left;
		const IRTree::IExpression* right;
		TBinOp binop;
	};

	class CMem : public IRTree::IExpression {
	public:
		CMem( const IRTree::IExpression* e );
		const IRTree::IExpression* GetExp() const;

		void Accept( IRTreeVisitor* v ) const;
		const CExpList* GetChild() const;
	private:
		const IRTree::IExpression* exp;
	};

	class CCall : public IRTree::IExpression {
	public:
		CCall( const IRTree::IExpression* f, const CExpList* a );
		const IRTree::IExpression* GetExp() const;
		const CExpList* GetArgs() const;

		void Accept( IRTreeVisitor* v ) const;
		const CExpList* GetChild() const;
	private:
		const IRTree::IExpression* func;
		const CExpList* args;
	};

	class CEseq : public IRTree::IExpression {
	public:
		CEseq( const IRTree::IStatement* s, const IRTree::IExpression* e );
		const IRTree::IStatement* GetStm() const;
		const IRTree::IExpression* GetExp() const;

		void Accept( IRTreeVisitor* v ) const;
		const CExpList* GetChild() const;
	private:
		const IRTree::IStatement* stm;
		const IRTree::IExpression* exp;
	};

	class CMove : public IRTree::IStatement {
	public:
		CMove( const IRTree::IExpression* d, const IRTree::IExpression* s );
		const IRTree::IExpression* GetDst() const;
		const IRTree::IExpression* GetSrc() const;
	
		void Accept( IRTreeVisitor* v ) const;
		const CExpList* GetChild() const;
	private:
		const IRTree::IExpression* dst;
		const IRTree::IExpression* src;
	};

	class CExp : public IRTree::IStatement {
	public:
		CExp( const IRTree::IExpression* e );
		const IRTree::IExpression* GetExp() const;

		void Accept( IRTreeVisitor* v ) const;
		const CExpList* GetChild() const;
	private:
		const IRTree::IExpression* exp;
	};

	class CJump : public IRTree::IStatement {
	public:
		CJump( const IRTree::IExpression* e, const Temp::CLabelList* t );
		explicit CJump( const Temp::CLabel* t );
		const IRTree::IExpression* GetExp() const;
		const Temp::CLabelList* GetTargets() const;	
			
		void Accept( IRTreeVisitor* v ) const;
		const CExpList* GetChild() const;
	private:
		const IRTree::IExpression* exp;
		const Temp::CLabelList* targets;
	};

	class CCJump : public IRTree::IStatement {
	public:
		CCJump( TCJump r, const IRTree::IExpression* _left, const IRTree::IExpression* _right, const Temp::CLabel* _iftrue, const Temp::CLabel* _iffalse );
		TCJump GetRelop() const;
		const IRTree::IExpression* GetLeft() const;
		const IRTree::IExpression* GetRight() const;
		const Temp::CLabel* GetTrueLabel() const;
		const Temp::CLabel* GetFalseLabel() const;

		void Accept( IRTreeVisitor* v ) const;
		const CExpList* GetChild() const;
	private:
		TCJump relop;
		const IRTree::IExpression* left;
		const IRTree::IExpression* right;
		const Temp::CLabel* iftrue;
		const Temp::CLabel* iffalse;
	};

	class CSeq : public IRTree::IStatement {
	public:
		CSeq( const IRTree::IStatement* l, const IRTree::IStatement* r );
		const IRTree::IStatement* GetLeft() const;
		const IRTree::IStatement* GetRight() const;

		void Accept( IRTreeVisitor* v ) const;
		const CExpList* GetChild() const;
	private:
		const IRTree::IStatement* left;
		const IRTree::IStatement* right;
	};

	class CLabel : public IRTree::IStatement {
	public:
		CLabel( const Temp::CLabel* l );
		const Temp::CLabel* GetLabel() const;

		void Accept( IRTreeVisitor* v ) const;
		const CExpList* GetChild() const;
	private:
		const Temp::CLabel* label;
	};

	class CExpList {
	public:
		CExpList( const IRTree::IExpression* e, const CExpList* n );
		const IRTree::IExpression* GetExp() const;
		const CExpList* GetNext() const;
		void SetExp( const IRTree::IExpression* e );
		void SetNext( const CExpList* n );
	
		void Accept( IRTreeVisitor* v ) const;
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
	
		void Accept( IRTreeVisitor* v ) const;
	private:
		const IRTree::IStatement* stm;
		const CStmList* next;
	};
}