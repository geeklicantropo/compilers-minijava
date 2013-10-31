#pragma once
#include <assert.h>
#include "Visitor.h"
#include "Interpreter.h"

enum TDataTypes {
	INT, BOOLEAN, INTARRAY
};

enum TBinaryOperation {
	AND, LESS, PLUS,  MINUS, TIMES
};

struct CValue {
	TDataTypes Type;
	int Int;
	bool Bool;
	int* IntArrayPointer;
	CValue( TDataTypes type, int _int, bool _bool, int* intArrayPointer ) : 
		Type( type ), Int( _int ), Bool( _bool ), IntArrayPointer( intArrayPointer )
	{
	}
};

struct CVariable {
	char Name[256];
	CValue Value;
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
	CMainClass( const char*, const char*, const IStatement* );
	int Accept(IVisitor *v) const;
	const IStatement* GetStatement() const;
	const char* GetId() const;
	const char* GetArgsId() const;
		
private:
	const char* const id;
	const char* const argsId;
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
	CClassDeclare( const char*, const IVarDeclareStar*, const IMethodDeclareStar* );
	int Accept(IVisitor *v) const;
	const IVarDeclareStar* GetVarDeclareStar() const;
	const IMethodDeclareStar* GetmethodDeclareStar() const;
	const char* GetId() const;

private:
	const char* const id;
	const IVarDeclareStar* const varDeclareStar;
	const IMethodDeclareStar* const methodDeclareStar;
};

class CClassDeclareExtends : public IClassDeclare {
public:
	CClassDeclareExtends( const char*, const char*, const IVarDeclareStar*, 
		const IMethodDeclareStar* );
	int Accept(IVisitor *v) const;
	const IMethodDeclareStar* GetMethodDeclareStar() const;
	const IVarDeclareStar* GetVarDeclareStar() const;
	const char* GetId() const;
	const char* GetExtendsId() const;

private:
	const char* const id;
	const char* const extendsId;
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
	CVarDeclare( const TDataTypes, const char* );
	int Accept(IVisitor *v) const;
	const TDataTypes GetDataType() const;
	const char* GetId() const;

private:
	const TDataTypes dataType;
	const char* const id;
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
	CMethodDeclare( const TDataTypes, const char*, const IFormalList*, const IVarDeclareStar*, 
		const IStatementStar*, const IExpression* );
	int Accept(IVisitor *v) const;
	const TDataTypes GetDataType() const;
	const char* GetId() const;
	const IFormalList*  GetFormalList() const;
	const IVarDeclareStar* GetVarDeclareStar() const;
	const IStatementStar* GetStatementStar() const;
	const IExpression* GetExpression() const;

private:
	const TDataTypes dataType;
	const char* const id;
	const IFormalList* const formalList;
	const IVarDeclareStar* const varDeclareStar;
	const IStatementStar* const statementStar;
	const IExpression* const expression;
};

class CFormalList : public IFormalList {
public:
	CFormalList( const TDataTypes, const char*, const IFormalRestStar* );
	int Accept(IVisitor *v) const;
	const TDataTypes GetDataType() const;
	const char* GetId() const;
	const IFormalRestStar* GetFormalRestStar() const;

private:
	const TDataTypes dataType;
	const char* const id;
	const IFormalRestStar* const formalRestStar;
};

class CFormalRestStar : public IFormalRestStar {
public:
	CFormalRestStar( const TDataTypes, const char*, const IFormalRestStar* );
	int Accept(IVisitor *v) const;
	const TDataTypes GetDataType() const;
	const char* GetId() const;
	const IFormalRestStar* GetFormalRestStar() const;

private:
	const TDataTypes dataType;
	const char* const id;
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
	CStatementAssignment( const char*, const IExpression* );
	int Accept(IVisitor *v) const;
	const char* GetId() const;
	const IExpression* GetExpression() const;

private:
	const char* const id;
	const IExpression* const expression;
};

class CStatementArrayAssignment : public IStatement {
public:
	CStatementArrayAssignment( const char*, const IExpression*, const IExpression* );
	int Accept(IVisitor *v) const;
	const char* GetId() const;
	const IExpression* GetExpressionArray() const;
	const IExpression* GetExpression() const;

private:
	const char* const id;
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
	CExpressionCallMethod( const IExpression*, const char*, const IExpList* );
	int Accept(IVisitor *v) const;
	const IExpression* GetExpression() const;
	const char* const GetId() const;
	const IExpList* const GetExpList() const;

private:
	const IExpression* const expression;
	const char* const id;
	const IExpList* const expList;
};

class CExpressionNumber : public IExpression {
public:
	CExpressionNumber( const int );
	int Accept(IVisitor *v) const;
	const int GetNumber() const;

private:
	const int number;
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
	CExpressionVar( const char* );
	int Accept(IVisitor *v) const;
	const char* GetId() const;

private:
	const char* const id;
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
	CExpressionNewId( const char* );
	int Accept(IVisitor *v) const;
	const char* GetId() const;

private:
	const char* const id;
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