#pragma once
#include <assert.h>
#include "Visitor.h"
#include "Interpreter.h"
#include "Symbols.h"

enum TType {
	BOOL, INT, INTARRAY, USERTYPE
};

class CTypeInfo {
private:
	const CSymbol* userType;
	TType type;
public:
	CTypeInfo ( const CSymbol* n );
	CTypeInfo ( TType t );
	TType GetType() const;
	const CSymbol* GetUserType() const;
};

enum TBinaryOperation {
	AND, LESS, EQUALS, PLUS,  MINUS, TIMES
};

void ErrorMessage( ostream& out, const string& msg, int line );

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
	~IExpression() {};
};

class IExpList {
public:
	virtual int Accept(IVisitor *v) const = 0;
	~IExpList();
};

class CProgram : public IProgram {
public:
	int Accept(IVisitor *v) const;
	CProgram( const IMainClass* , const IClassDeclareStar*, int _location );
	
	const IMainClass* GetMainClass() const;
	const IClassDeclareStar* GetClassDeclareStar() const;
	int GetLocation() const;

private:
	const IMainClass* const mainClass;
	const IClassDeclareStar* const classDeclareStar;
	int location;
};

class CMainClass : public IMainClass {
public:
	CMainClass( const CSymbol*, const CSymbol*, const IStatement*, int _location );
	int Accept(IVisitor *v) const;
	const IStatement* GetStatement() const;
	const CSymbol* GetId() const;
	const CSymbol* GetArgsId() const;
	int GetLocation() const;
		
private:
	const CSymbol* id;
	const CSymbol* argsId;
	const IStatement* const stm;
	int location;
};

class CClassDeclareStar : public IClassDeclareStar {
public:
	CClassDeclareStar( const IClassDeclareStar*, const IClassDeclare*, int _location );
	int Accept(IVisitor *v) const;
	const IClassDeclareStar* GetClassDeclareStar() const;
	const IClassDeclare* GetClassDeclare() const;
	int GetLocation() const;


private:
	const IClassDeclareStar* const classDeclareStar;
	const IClassDeclare* const classDeclare;
	int location;
};

class CClassDeclare : public IClassDeclare {
public:
	CClassDeclare( const CSymbol*, const IVarDeclareStar*, const IMethodDeclareStar*, int _location );
	int Accept(IVisitor *v) const;
	const IVarDeclareStar* GetVarDeclareStar() const;
	const IMethodDeclareStar* GetMethodDeclareStar() const;
	const CSymbol* GetId() const;
	int GetLocation() const;

private:
	const CSymbol* id;
	const IVarDeclareStar* const varDeclareStar;
	const IMethodDeclareStar* const methodDeclareStar;
	int location;
};

class CClassDeclareExtends : public IClassDeclare {
public:
	CClassDeclareExtends( const CSymbol*, const CSymbol*, const IVarDeclareStar*, 
		const IMethodDeclareStar*, int _location );
	int Accept(IVisitor *v) const;
	const IMethodDeclareStar* GetMethodDeclareStar() const;
	const IVarDeclareStar* GetVarDeclareStar() const;
	const CSymbol* GetId() const;
	const CSymbol* GetExtendsId() const;
	int GetLocation() const;

private:
	const CSymbol* id;
	const CSymbol* extendsId;
	const IVarDeclareStar* const varDeclareStar;
	const IMethodDeclareStar* const methodDeclareStar;
	int location;
};

class CVarDeclareStar : public IVarDeclareStar {
public:
	CVarDeclareStar( const IVarDeclareStar*, const IVarDeclare*, int _location );
	int Accept(IVisitor *v) const;
	const IVarDeclareStar* GetVarDeclareStar() const;
	const IVarDeclare* GetVarDeclare() const;
	int GetLocation() const;

private:
	const IVarDeclareStar* const varDeclareStar;
	const IVarDeclare* const varDeclare;
	int location;
};

class CVarDeclare : public IVarDeclare {
public:
	CVarDeclare( const CTypeInfo* , const CSymbol*, int _location );
	int Accept(IVisitor *v) const;
	const CTypeInfo* GetType() const;
	const CSymbol* GetId() const;
	int GetLocation() const;

private:
	const CSymbol* id;
	const CTypeInfo* type;
	int location;
};

class CMethodDeclareStar : public IMethodDeclareStar {
public:
	CMethodDeclareStar( const IMethodDeclareStar*, const IMethodDeclare*, int _location );
	int Accept(IVisitor *v) const;
	const IMethodDeclareStar* GetMethodDeclareStar() const ;
	const IMethodDeclare* GetMethodDeclare() const;
	int GetLocation() const;

private:
	const IMethodDeclareStar* const methodDeclareStar;
	const IMethodDeclare* const methodDeclare;
	int location;
};

class CMethodDeclare : public IMethodDeclare {
public:
	CMethodDeclare( const CTypeInfo*, const CSymbol*, const IFormalList*, const IVarDeclareStar*, 
		const IStatementStar*, const IExpression*, int _location );
	int Accept(IVisitor *v) const;
	const CTypeInfo* GetType() const;
	const CSymbol* GetId() const;
	const IFormalList*  GetFormalList() const;
	const IVarDeclareStar* GetVarDeclareStar() const;
	const IStatementStar* GetStatementStar() const;
	const IExpression* GetExpression() const;
	int GetLocation() const;

private:
	const CTypeInfo* type;
	const CSymbol* id;
	const IFormalList* const formalList;
	const IVarDeclareStar* const varDeclareStar;
	const IStatementStar* const statementStar;
	const IExpression* const expression;
	int location;
};

class CFormalList : public IFormalList {
public:
	CFormalList( const CTypeInfo*, const CSymbol*, const IFormalRestStar*, int _location );
	int Accept(IVisitor *v) const;
	const CTypeInfo* GetType() const;
	const CSymbol* GetId() const;
	const IFormalRestStar* GetFormalRestStar() const;
	int GetLocation() const;

private:
	const CTypeInfo* type;
	const CSymbol* id;
	const IFormalRestStar* const formalRestStar;
	int location;
};

class CFormalRestStar : public IFormalRestStar {
public:
	CFormalRestStar( const CTypeInfo*, const CSymbol*, const IFormalRestStar*, int _location );
	int Accept(IVisitor *v) const;
	const CTypeInfo* GetType() const;
	const CSymbol* GetId() const;
	const IFormalRestStar* GetFormalRestStar() const;
	int GetLocation() const;

private:
	const CTypeInfo* type;
	const CSymbol* id;
	const IFormalRestStar* const formalRestStar;
	int location;
};

class CStatementStar : public IStatementStar {
public:
	CStatementStar(const IStatement*, const IStatementStar*, int _location );
	int Accept(IVisitor *v) const;
	const IStatementStar* GetStatementStar() const;
	const IStatement* GetStatement() const;
	int GetLocation() const;

private:
	const IStatementStar* const statementStar;
	const IStatement* const statement;
	int location;
};

class CStatement : public IStatement {
public:
	CStatement( const IStatementStar*, int _location );
	int Accept(IVisitor *v) const;
	const IStatementStar* GetStatementStar() const;
	int GetLocation() const;

private:
	const IStatementStar* const statementStar;
	int location;
};

class CStatementIf : public IStatement {
public:
	CStatementIf( const IExpression*, const IStatement*, const IStatement*, int _location );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	const IStatement* GetStatementIf() const;
	const IStatement* GetStatementElse() const;
	int GetLocation() const;

private:
	const IExpression* const expression;
	const IStatement* const statementIf;
	const IStatement* const statementElse;
	int location;
};

class CStatementWhile : public IStatement {
public:
	CStatementWhile( const IExpression*, const IStatement*, int _location );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	const IStatement* GetStatement() const;
	int GetLocation() const;

private:
	const IExpression* const expression;
	const IStatement* const statement;
	int location;
};

class CStatementSysOut : public IStatement {
public:
	CStatementSysOut( const IExpression*, int _location );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	int GetLocation() const;

private:
	const IExpression* const expression;
	int location;
};

class CStatementAssignment : public IStatement {
public:
	CStatementAssignment( const CSymbol*, const IExpression*, int _location );
	int Accept(IVisitor *v) const;
	const CSymbol* GetId() const;
	const IExpression* GetExpression() const;
	int GetLocation() const;

private:
	const CSymbol* id;
	const IExpression* const expression;
	int location;
};

class CStatementArrayAssignment : public IStatement {
public:
	CStatementArrayAssignment( const CSymbol*, const IExpression*, const IExpression*, int _location );
	int Accept(IVisitor *v) const;
	const CSymbol* GetId() const;
	const IExpression* GetExpressionArray() const;
	const IExpression* GetExpression() const;
	int GetLocation() const;

private:
	const CSymbol* id;
	const IExpression* const expressionArray;
	const IExpression* const expression;
	int location;
};

class CExpressionBinOp : public IExpression {
public:
	CExpressionBinOp( const IExpression*, const TBinaryOperation, const IExpression*, int _location );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpressionFirst() const;
	const TBinaryOperation GetOperation() const;
	const IExpression* GetExpressionSecond() const;
	int GetLocation() const;

private:
	const IExpression* const expressionFirst;
	const TBinaryOperation operation;
	const IExpression* const expressionSecond;
	int location;
};

class CExpressionArray : public IExpression {
public:
	CExpressionArray( const IExpression*, const IExpression*, int _location );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression1() const;
	const IExpression* GetExpression2() const;
	int GetLocation() const;

private:
	const IExpression* const expression1;
	const IExpression* const expression2;
	int location;
};

class CExpressionLength : public IExpression {
public:
	CExpressionLength( const IExpression*, int _location );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	int GetLocation() const;

private:
	const IExpression* const expression;
	int location;
};

class CExpressionCallMethod : public IExpression {
public:
	CExpressionCallMethod( const IExpression*, const CSymbol*, const IExpList*, int _location );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	const CSymbol* const GetId() const;
	const IExpList* const GetExpList() const;
	int GetLocation() const;

private:
	const IExpression* const expression;
	const CSymbol* id;
	const IExpList* const expList;
	int location;
};

class CExpressionNumber : public IExpression {
public:
	CExpressionNumber( int, int _location );
	int Accept(IVisitor *v) const;
	int GetNumber() const;
	int GetLocation() const;

private:
	int number;
	int location;
};

class CExpressionBool : public IExpression {
public:
	CExpressionBool( const bool, int _location );
	int Accept(IVisitor *v) const;
	const bool GetValue() const;
	int GetLocation() const;

private:
	const bool value;
	int location;
};

class CExpressionVar : public IExpression {
public:
	CExpressionVar( const CSymbol*, int _location );
	int Accept(IVisitor *v) const;
	const CSymbol* GetId() const;
	int GetLocation() const;

private:
	const CSymbol* id;
	int location;
};

class CExpressionThis : public IExpression {
public:
	CExpressionThis( int _location );
	int Accept(IVisitor *v) const;
	int GetLocation() const;

private:
	int location;
};

class CExpressionNewInt : public IExpression {
public:
	CExpressionNewInt( const IExpression*, int _location );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	int GetLocation() const;

private:
	const IExpression* const expression;
	int location;
};

class CExpressionNewId : public IExpression {
public:
	CExpressionNewId( const CSymbol*, int _location );
	int Accept(IVisitor *v) const;
	const CSymbol* GetId() const;
	int GetLocation() const;

private:
	const CSymbol* id;
	int location;
};

class CExpressionNegation : public IExpression {
public:
	CExpressionNegation( const IExpression*, int _location );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	int GetLocation() const;

private:
	const IExpression* const expression;
	int location;
};

class CExpression : public IExpression {
public:
	CExpression( const IExpression*, int _location );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	int GetLocation() const;

private:
	const IExpression* const expression;
	int location;
};

class CExpList : public IExpList {
public:
	CExpList( const IExpression*, int _location );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	int GetLocation() const;

private:
	const IExpression* const expression;
	int location;
};

class CExpListNext : public IExpList {
public:
	CExpListNext( const IExpList*, const IExpression*, int _location );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	const IExpList* GetExpList() const;
	int GetLocation() const;

private:
	const IExpression* const expression;
	const IExpList* const expList;
	int location;
};