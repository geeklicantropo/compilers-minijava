#pragma once
#include <assert.h>
#include "Visitor.h"
#include "Interpreter.h"
#include "Symbols.h"

enum TBinaryOperation {
	AND, LESS, PLUS,  MINUS, TIMES
};

class IProgram {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IProgram();
};

class IMainClass {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IMainClass();
};

class IClassDeclare {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IClassDeclare();
};

class IClassDeclareStar {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IClassDeclareStar();
};

class IVarDeclareStar {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IVarDeclareStar();
};

class IVarDeclare {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IVarDeclare();
};

class IMethodDeclareStar {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IMethodDeclareStar();
};

class IMethodDeclare {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IMethodDeclare();
};

class IFormalList {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IFormalList();
};

class IFormalRestStar {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IFormalRestStar();
};

class IStatement {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IStatement();
};

class IStatementStar {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IStatementStar();
};

class IExpression {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IExpression();
};

class IExpList {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IExpList();
};

class CProgram : public IProgram {
public:
	int Accept(IVisitor *v) const;
	CProgram( const IMainClass* , const IClassDeclareStar* );
	
	const IMainClass* GetMainClass() const;
	const IClassDeclareStar* GetClassDeclareStar() const;

private:
	const IMainClass* const mainClass;
	const IClassDeclareStar* const classDeclareStar;
};

class CMainClass : public IMainClass {
public:
	CMainClass( const CSymbol*, const CSymbol*, const IStatement* );
	int Accept(IVisitor *v) const;
	const IStatement* GetStatement() const;
	const CSymbol* GetId() const;
	const CSymbol* GetArgsId() const;
		
private:
	const CSymbol* id;
	const CSymbol* argsId;
	const IStatement* const stm;
};

class CClassDeclareStar : public IClassDeclareStar {
public:
	CClassDeclareStar(const IClassDeclareStar*, const IClassDeclare* );
	int Accept(IVisitor *v) const;
	const IClassDeclareStar* GetClassDeclareStar() const;
	const IClassDeclare* GetClassDeclare() const;

private:
	const IClassDeclareStar* const classDeclareStar;
	const IClassDeclare* const classDeclare;
};

class CClassDeclare : public IClassDeclare {
public:
	CClassDeclare( const CSymbol*, const IVarDeclareStar*, const IMethodDeclareStar* );
	int Accept(IVisitor *v) const;
	const IVarDeclareStar* GetVarDeclareStar() const;
	const IMethodDeclareStar* GetMethodDeclareStar() const;
	const CSymbol* GetId() const;

private:
	const CSymbol* id;
	const IVarDeclareStar* const varDeclareStar;
	const IMethodDeclareStar* const methodDeclareStar;
};

class CClassDeclareExtends : public IClassDeclare {
public:
	CClassDeclareExtends( const CSymbol*, const CSymbol*, const IVarDeclareStar*, 
		const IMethodDeclareStar* );
	int Accept(IVisitor *v) const;
	const IMethodDeclareStar* GetMethodDeclareStar() const;
	const IVarDeclareStar* GetVarDeclareStar() const;
	const CSymbol* GetId() const;
	const CSymbol* GetExtendsId() const;

private:
	const CSymbol* id;
	const CSymbol* extendsId;
	const IVarDeclareStar* const varDeclareStar;
	const IMethodDeclareStar* const methodDeclareStar;
};

class CVarDeclareStar : public IVarDeclareStar {
public:
	CVarDeclareStar( const IVarDeclareStar*, const IVarDeclare* );
	int Accept(IVisitor *v) const;
	const IVarDeclareStar* GetVarDeclareStar() const;
	const IVarDeclare* GetVarDeclare() const;

private:
	const IVarDeclareStar* const varDeclareStar;
	const IVarDeclare* const varDeclare;
};

class CVarDeclare : public IVarDeclare {
public:
	CVarDeclare( const CSymbol* , const CSymbol* );
	int Accept(IVisitor *v) const;
	const CSymbol* GetType() const;
	const CSymbol* GetId() const;

private:
	const CSymbol* id;
	const CSymbol* typeId;
};

class CMethodDeclareStar : public IMethodDeclareStar {
public:
	CMethodDeclareStar( const IMethodDeclareStar*, const IMethodDeclare* );
	int Accept(IVisitor *v) const;
	const IMethodDeclareStar* GetMethodDeclareStar() const ;
	const IMethodDeclare* GetMethodDeclare() const;

private:
	const IMethodDeclareStar* const methodDeclareStar;
	const IMethodDeclare* const methodDeclare;
};

class CMethodDeclare : public IMethodDeclare {
public:
	CMethodDeclare( const CSymbol*, const CSymbol*, const IFormalList*, const IVarDeclareStar*, 
		const IStatementStar*, const IExpression* );
	int Accept(IVisitor *v) const;
	const CSymbol* GetType() const;
	const CSymbol* GetId() const;
	const IFormalList*  GetFormalList() const;
	const IVarDeclareStar* GetVarDeclareStar() const;
	const IStatementStar* GetStatementStar() const;
	const IExpression* GetExpression() const;

private:
	const CSymbol* dataType;
	const CSymbol* id;
	const IFormalList* const formalList;
	const IVarDeclareStar* const varDeclareStar;
	const IStatementStar* const statementStar;
	const IExpression* const expression;
};

class CFormalList : public IFormalList {
public:
	CFormalList( const CSymbol*, const CSymbol*, const IFormalRestStar* );
	int Accept(IVisitor *v) const;
	const CSymbol* GetType() const;
	const CSymbol* GetId() const;
	const IFormalRestStar* GetFormalRestStar() const;

private:
	const CSymbol* dataType;
	const CSymbol* id;
	const IFormalRestStar* const formalRestStar;
};

class CFormalRestStar : public IFormalRestStar {
public:
	CFormalRestStar( const CSymbol*, const CSymbol*, const IFormalRestStar* );
	int Accept(IVisitor *v) const;
	const CSymbol* GetType() const;
	const CSymbol* GetId() const;
	const IFormalRestStar* GetFormalRestStar() const;

private:
	const CSymbol* dataType;
	const CSymbol* id;
	const IFormalRestStar* const formalRestStar;
};

class CStatementStar : public IStatementStar {
public:
	CStatementStar(const IStatement*, const IStatementStar* );
	int Accept(IVisitor *v) const;
	const IStatementStar* GetStatementStar() const;
	const IStatement* GetStatement() const;

private:
	const IStatementStar* const statementStar;
	const IStatement* const statement;
};

class CStatement : public IStatement {
public:
	CStatement( const IStatementStar* );
	int Accept(IVisitor *v) const;
	const IStatementStar* GetStatementStar() const;

private:
	const IStatementStar* const statementStar;
};

class CStatementIf : public IStatement {
public:
	CStatementIf( const IExpression*, const IStatement*, const IStatement* );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	const IStatement* GetStatementIf() const;
	const IStatement* GetStatementElse() const;

private:
	const IExpression* const expression;
	const IStatement* const statementIf;
	const IStatement* const statementElse;
};

class CStatementWhile : public IStatement {
public:
	CStatementWhile( const IExpression*, const IStatement* );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	const IStatement* GetStatement() const;

private:
	const IExpression* const expression;
	const IStatement* const statement;
};

class CStatementSysOut : public IStatement {
public:
	CStatementSysOut( const IExpression* );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;

private:
	const IExpression* const expression;
};

class CStatementAssignment : public IStatement {
public:
	CStatementAssignment( const CSymbol*, const IExpression* );
	int Accept(IVisitor *v) const;
	const CSymbol* GetId() const;
	const IExpression* GetExpression() const;

private:
	const CSymbol* id;
	const IExpression* const expression;
};

class CStatementArrayAssignment : public IStatement {
public:
	CStatementArrayAssignment( const CSymbol*, const IExpression*, const IExpression* );
	int Accept(IVisitor *v) const;
	const CSymbol* GetId() const;
	const IExpression* GetExpressionArray() const;
	const IExpression* GetExpression() const;

private:
	const CSymbol* id;
	const IExpression* const expressionArray;
	const IExpression* const expression;
};

class CExpressionBinOp : public IExpression {
public:
	CExpressionBinOp( const IExpression*, const TBinaryOperation, const IExpression* );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpressionFirst() const;
	const TBinaryOperation GetOperation() const;
	const IExpression* GetExpressionSecond() const;

private:
	const IExpression* const expressionFirst;
	const TBinaryOperation operation;
	const IExpression* const expressionSecond;
};

class CExpressionArray : public IExpression {
public:
	CExpressionArray( const IExpression*, const IExpression*);
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression1() const;
	const IExpression* GetExpression2() const;

private:
	const IExpression* const expression1;
	const IExpression* const expression2;
};

class CExpressionLength : public IExpression {
public:
	CExpressionLength( const IExpression* );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;

private:
	const IExpression* const expression;
};

class CExpressionCallMethod : public IExpression {
public:
	CExpressionCallMethod( const IExpression*, const CSymbol*, const IExpList* );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	const CSymbol* const GetId() const;
	const IExpList* const GetExpList() const;

private:
	const IExpression* const expression;
	const CSymbol* id;
	const IExpList* const expList;
};

class CExpressionNumber : public IExpression {
public:
	CExpressionNumber( const CSymbol* );
	int Accept(IVisitor *v) const;
	const CSymbol* GetNumber() const;

private:
	const CSymbol* number;
};

class CExpressionBool : public IExpression {
public:
	CExpressionBool( const bool );
	int Accept(IVisitor *v) const;
	const bool GetValue() const;

private:
	const bool value;
};

class CExpressionVar : public IExpression {
public:
	CExpressionVar( const CSymbol* );
	int Accept(IVisitor *v) const;
	const CSymbol* GetId() const;

private:
	const CSymbol* id;
};

class CExpressionThis : public IExpression {
public:
	CExpressionThis();
	int Accept(IVisitor *v) const;

private:
};

class CExpressionNewInt : public IExpression {
public:
	CExpressionNewInt( const IExpression* );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;

private:
	const IExpression* const expression;
};

class CExpressionNewId : public IExpression {
public:
	CExpressionNewId( const CSymbol* );
	int Accept(IVisitor *v) const;
	const CSymbol* GetId() const;

private:
	const CSymbol* id;
};

class CExpressionNegation : public IExpression {
public:
	CExpressionNegation( const IExpression* );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;

private:
	const IExpression* const expression;
};

class CExpression : public IExpression {
public:
	CExpression( const IExpression* );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;

private:
	const IExpression* const expression;
};

class CExpList : public IExpList {
public:
	CExpList( const IExpression* );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;

private:
	const IExpression* const expression;
};

class CExpListNext : public IExpList {
public:
	CExpListNext( const IExpList*, const IExpression* );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	const IExpList* GetExpList() const;

private:
	const IExpression* const expression;
	const IExpList* const expList;
};