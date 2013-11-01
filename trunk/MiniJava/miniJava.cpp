#include <cstdio>
#include "miniJava.h"
#include "Visitor.h"
#include "iostream"

using namespace std;

CProgram::CProgram( const IMainClass* _mainClass, const IClassDeclareStar* _classDeclareStar ) :
	mainClass( _mainClass), classDeclareStar( _classDeclareStar )
	{
		assert( mainClass != 0 );
	}

const IMainClass* CProgram::GetMainClass() const
{
	return mainClass;
}

const IClassDeclareStar* CProgram::GetClassDeclareStar() const
{
	return classDeclareStar;
}

CMainClass::CMainClass( const CSymbol* _id, const CSymbol* _argsId, const IStatement* _stm ) :
	id( _id ), argsId( _argsId ), stm( _stm )
	{
		assert( id != 0 );
		assert( argsId != 0 );
		assert( stm != 0 );
	}
const CSymbol* CMainClass::GetId() const
{
	return id;
}

const CSymbol* CMainClass::GetArgsId() const
{
	return argsId;
}

const IStatement* CMainClass::GetStatement() const
{
	return stm;
}

CClassDeclareStar::CClassDeclareStar(const IClassDeclareStar* classDecStar, const IClassDeclare* classDec ) :
	classDeclareStar( classDecStar ), classDeclare( classDec )
	{
		assert( classDeclare != 0 );
	}

const IClassDeclareStar* CClassDeclareStar::GetClassDeclareStar() const
{
	return classDeclareStar;
}

const IClassDeclare* CClassDeclareStar::GetClassDeclare() const
{
	return classDeclare;
}

CClassDeclare::CClassDeclare( const CSymbol* _id, const IVarDeclareStar* _varDeclareStar, const IMethodDeclareStar* _methodDeclareStar) :
	id( _id ), varDeclareStar( _varDeclareStar), methodDeclareStar( _methodDeclareStar )
	{
		assert( id != 0 );
	}

const CSymbol* CClassDeclare::GetId() const
{
	return id;
}

const IVarDeclareStar* CClassDeclare::GetVarDeclareStar() const
{
	return varDeclareStar;
}

const IMethodDeclareStar* CClassDeclare::GetmethodDeclareStar() const
{
	return methodDeclareStar;
}

CClassDeclareExtends::CClassDeclareExtends( const CSymbol* _id, const CSymbol* _extendsId, const IVarDeclareStar* _varDeclareStar, 
		const IMethodDeclareStar* _methodDeclareStar) :
	id( _id ), extendsId( _extendsId ), varDeclareStar( _varDeclareStar), methodDeclareStar( _methodDeclareStar )
	{
		assert( id != 0 );
		assert( extendsId != 0 );
	}

const CSymbol* CClassDeclareExtends::GetId() const 
{
	return id;
}

const CSymbol* CClassDeclareExtends::GetExtendsId() const
{
	return extendsId;
}

const IMethodDeclareStar* CClassDeclareExtends::GetMethodDeclareStar() const
{
	return methodDeclareStar;
}

const IVarDeclareStar* CClassDeclareExtends::GetVarDeclareStar() const
{
	return varDeclareStar;
}

CVarDeclareStar::CVarDeclareStar( const IVarDeclareStar* varDecStar, const IVarDeclare* varDec ) :
		varDeclareStar( varDecStar ), varDeclare( varDec ) 
	{
		assert( varDeclare != 0);
	}

const IVarDeclareStar* CVarDeclareStar::GetVarDeclareStar() const
{
	return varDeclareStar;
}

const IVarDeclare* CVarDeclareStar::GetVarDeclare() const
{
	return varDeclare;
}

CVarDeclare::CVarDeclare( const TDataTypes _dataType, const CSymbol* _id ) :
		dataType (_dataType ), id( _id )
	{
		assert( id != 0 );
	}

const TDataTypes CVarDeclare::GetDataType() const
{
	return dataType;
}

const CSymbol* CVarDeclare::GetId() const
{
	return id;
}

CMethodDeclareStar::CMethodDeclareStar( const IMethodDeclareStar* methodDecStar, const IMethodDeclare* methodDec ) :
	methodDeclareStar( methodDecStar ), methodDeclare( methodDec )
	{
		assert( methodDeclare != 0 );
	}

const IMethodDeclareStar* CMethodDeclareStar::GetMethodDeclareStar() const 
{
	return methodDeclareStar;
}

const IMethodDeclare* CMethodDeclareStar::GetMethodDeclare() const 
{
	return methodDeclare;
}

CMethodDeclare::CMethodDeclare( const TDataTypes _dataType, const CSymbol* _id, const IFormalList* _formalList, const IVarDeclareStar* varDecStar, 
		const IStatementStar* _statementStar, const IExpression* _expression ) :
	dataType( _dataType ), id( _id ), formalList( _formalList ), varDeclareStar( varDecStar ), statementStar( _statementStar ),
	expression( _expression )
	{
		//
		assert( id != 0 );
		assert( expression != 0 );
	}
const TDataTypes CMethodDeclare::GetDataType() const
{
	return dataType;	
}
const CSymbol* CMethodDeclare::GetId() const
{
	return id;
}
const IFormalList*  CMethodDeclare::GetFormalList() const
{
	return formalList;
}
const IVarDeclareStar* CMethodDeclare::GetVarDeclareStar() const
{
	return varDeclareStar;
}
const IStatementStar* CMethodDeclare::GetStatementStar() const
{
	return statementStar;
}
const IExpression* CMethodDeclare::GetExpression() const
{
	return expression;
}

CFormalList::CFormalList( const TDataTypes _dataType, const CSymbol* _id, const IFormalRestStar* _formalRestStar ) :
	dataType( _dataType ), id( _id ), formalRestStar( _formalRestStar )
	{
		assert( id != 0 );
	}

const TDataTypes CFormalList::GetDataType() const
{
	return dataType;
}

const CSymbol* CFormalList::GetId() const
{
	return id;
}

const IFormalRestStar* CFormalList::GetFormalRestStar() const
{
	return formalRestStar;
}

CFormalRestStar::CFormalRestStar( const TDataTypes _dataType, const CSymbol* _id, const IFormalRestStar* _formalRestStar ) :
	dataType( _dataType ), id( _id ), formalRestStar( _formalRestStar )
	{
		
		assert( id != 0 );
	}

const TDataTypes CFormalRestStar::GetDataType() const
{
	return dataType;
}

const CSymbol* CFormalRestStar::GetId() const
{
	return id;
}

const IFormalRestStar* CFormalRestStar::GetFormalRestStar() const
{
	return formalRestStar;
}

CStatementStar::CStatementStar(const IStatement* _statement, const IStatementStar* _statementStar) :
	statement( _statement ), statementStar( _statementStar )
	{
		assert( statement != 0 );
	}

const IStatementStar* CStatementStar::GetStatementStar() const
{
	return statementStar;
}
const IStatement* CStatementStar::GetStatement() const
{
	return statement;
}

CStatement::CStatement( const IStatementStar* _statementStar ) :
	statementStar( _statementStar )
	{
	}

const IStatementStar* CStatement::GetStatementStar() const
{
	return statementStar;
}

CStatementIf::CStatementIf( const IExpression* _expression, const IStatement* _statementIf, const IStatement* _statementElse ) :
	expression( _expression ), statementIf( _statementIf ), statementElse( _statementElse )
	{
		assert( expression != 0 );
		assert( statementIf != 0 );
		assert( statementElse != 0 );
	}

const IExpression* CStatementIf::GetExpression() const
{
	return expression;
}

const IStatement* CStatementIf::GetStatementIf() const
{
	return statementIf;
}

const IStatement* CStatementIf::GetStatementElse() const
{
	return statementElse;
}

CStatementWhile::CStatementWhile( const IExpression* _expression, const IStatement* _statement ) :
	expression( _expression ), statement( _statement )
	{
		assert( expression != 0 );
		assert( statement != 0 );
	}
const IExpression* CStatementWhile::GetExpression() const
{
	return expression;
}
const IStatement* CStatementWhile::GetStatement() const
{
	return statement;
}

CStatementSysOut::CStatementSysOut( const IExpression* _expression ) :
	expression( _expression )
	{
		assert( expression != 0 );
	}

const IExpression* CStatementSysOut::GetExpression() const
{
	return expression;
}

CStatementAssignment::CStatementAssignment( const CSymbol* _id, const IExpression* _expression ) :
	id( _id ), expression( _expression )
	{
		assert( id != 0 );
		assert( expression != 0 );
	}

const CSymbol* CStatementAssignment::GetId() const
{
	return id;
}

const IExpression* CStatementAssignment::GetExpression() const
{
	return expression;
}

CStatementArrayAssignment::CStatementArrayAssignment( const CSymbol* _id, const IExpression* _expressionArray, const IExpression* _expression ) :
	id( _id ), expressionArray( _expressionArray ), expression( _expression )
	{
		assert( id != 0 );
		assert( expressionArray != 0 );
		assert( expression != 0 );
	}

const CSymbol* CStatementArrayAssignment::GetId() const
{
	return id;
}

const IExpression* CStatementArrayAssignment::GetExpressionArray() const
{
	return expressionArray;
}

const IExpression* CStatementArrayAssignment::GetExpression() const
{
	return expression;
}

CExpressionBinOp::CExpressionBinOp( const IExpression* _expressionFirst, const TBinaryOperation _operation, const IExpression* _expressionSecond ) :
	expressionFirst( _expressionFirst ), operation( _operation ), expressionSecond( _expressionSecond )
	{
		assert( expressionFirst != 0 );
		assert( operation != 0 );
		assert( expressionSecond != 0 );
	}

const IExpression* CExpressionBinOp::GetExpressionFirst() const
{
	return expressionFirst;
}

const TBinaryOperation CExpressionBinOp::GetOperation() const
{
	return operation;
}

const IExpression* CExpressionBinOp::GetExpressionSecond() const
{
	return expressionSecond;
}

CExpressionArray::CExpressionArray( const IExpression* _expression1, const IExpression* _expression2 ) :
	expression1( _expression1 ), expression2( _expression2 )
	{
		assert( expression1 != 0 );
		assert( expression2 != 0 );
	}

const IExpression* CExpressionArray::GetExpression1() const
{
	return expression1;
}

const IExpression* CExpressionArray::GetExpression2() const
{
	return expression2;
}

CExpressionLength::CExpressionLength( const IExpression* _expression ) :
	expression( _expression )
	{
		assert( expression != 0 );
	}

const IExpression* CExpressionLength::GetExpression() const
{
	return expression;	
}

CExpressionCallMethod::CExpressionCallMethod( const IExpression* _expression, const CSymbol* _id, const IExpList* _expList ) :
	expression( _expression ), id( _id ), expList( _expList )
	{
		assert( expression != 0 );
		assert( id != 0 );
	}

const IExpression* CExpressionCallMethod::GetExpression() const
{
	return expression;
}

const CSymbol* const CExpressionCallMethod::GetId() const
{
	return id;
}

const IExpList* const CExpressionCallMethod::GetExpList() const 
{
	return expList;
}

CExpressionNumber::CExpressionNumber( const CSymbol* _number ) : number( _number ) {}

const CSymbol* CExpressionNumber::GetNumber() const
{
	return number;
}

CExpressionBool::CExpressionBool( const bool _value ) : value( _value ) {}

const bool CExpressionBool::GetValue() const
{
	return value;
}

CExpressionVar::CExpressionVar( const CSymbol* _id ) : id( _id )
	{
		assert( id != 0 );
	}

const CSymbol* CExpressionVar::GetId() const
{
	return id;
}

CExpressionThis::CExpressionThis() {}

CExpressionNewInt::CExpressionNewInt( const IExpression* _expression ) :
		expression( _expression ) {}

const IExpression* CExpressionNewInt::GetExpression() const
{
	return expression;
}

CExpressionNewId::CExpressionNewId( const CSymbol* _id ) : id( _id ) {}

const CSymbol* CExpressionNewId::GetId() const
{
	return id;
}

CExpressionNegation::CExpressionNegation( const IExpression* _expression ) : expression( _expression ) {}

const IExpression* CExpressionNegation::GetExpression() const 
{
	return expression;
}

CExpression::CExpression( const IExpression* _expression ) : expression( _expression ) {}

const IExpression* CExpression::GetExpression() const
{
	return expression;
}

CExpList::CExpList( const IExpression* _expression ) :
		expression( _expression ) {}

const IExpression* CExpList::GetExpression() const
{
	return expression;
}

CExpListNext::CExpListNext( const IExpList* _expList, const IExpression* _expression ) :
		expList( _expList ), expression( _expression ) {}

const IExpression* CExpListNext::GetExpression() const
{
	return expression;
}
const IExpList* CExpListNext::GetExpList() const
{
	return expList;
}


int CProgram::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CMainClass::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CClassDeclareStar::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CClassDeclare::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CClassDeclareExtends::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this );
}

int CVarDeclareStar::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this );
}

int CVarDeclare::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this );
}

int CMethodDeclareStar::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CMethodDeclare::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CFormalList::Accept( IVisitor *v ) const 
{ return v->Visit( this ); }

int CFormalRestStar::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CStatementStar::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CStatement::Accept( IVisitor *v ) const 
{ return v->Visit( this ); }

int CStatementIf::Accept(IVisitor *v) const 
{ 
	return v->Visit( this ); 
}

int CStatementWhile::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); }

int CStatementSysOut::Accept( IVisitor *v ) const 
{ return v->Visit( this ); }

int CStatementAssignment::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CStatementArrayAssignment::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CExpressionBinOp::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CExpressionArray::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CExpressionLength::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CExpressionCallMethod::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CExpressionNumber::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CExpressionBool::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CExpressionVar::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CExpressionThis::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CExpressionNewInt::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CExpressionNewId::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CExpressionNegation::Accept( IVisitor *v ) const
{ 
	return v->Visit( this ); 
}

int CExpression::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CExpList::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}

int CExpListNext::Accept( IVisitor *v ) const 
{ 
	return v->Visit( this ); 
}