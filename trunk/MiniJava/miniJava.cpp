#include <cstdio>
#include "miniJava.h"
#include "Visitor.h"
#include "string"
#include <ostream>

using namespace std;


void ErrorMessage( ostream& out, const string& msg, int line ) 
{
	out << "Error in line " << line << ": " << msg << endl;
}

CProgram::CProgram( const IMainClass* _mainClass, const IClassDeclareStar* _classDeclareStar, int _location ) :
	mainClass( _mainClass), classDeclareStar( _classDeclareStar ), location(_location)
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

int CProgram::GetLocation() const 
{
	return location;
}

CMainClass::CMainClass( const CSymbol* _id, const CSymbol* _argsId, const IStatement* _stm, int _location ) :
	id( _id ), argsId( _argsId ), stm( _stm ), location( _location )
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

int CMainClass::GetLocation() const 
{
	return location;
}

CClassDeclareStar::CClassDeclareStar(const IClassDeclareStar* classDecStar, const IClassDeclare* classDec, int _location ) :
	classDeclareStar( classDecStar ), classDeclare( classDec ), location( _location )
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

int CClassDeclareStar::GetLocation() const 
{
	return location;
}

CClassDeclare::CClassDeclare( const CSymbol* _id, const IVarDeclareStar* _varDeclareStar, const IMethodDeclareStar* _methodDeclareStar, int _location ) :
	id( _id ), varDeclareStar( _varDeclareStar), methodDeclareStar( _methodDeclareStar ), location( _location )
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

const IMethodDeclareStar* CClassDeclare::GetMethodDeclareStar() const
{
	return methodDeclareStar;
}

int CClassDeclare::GetLocation() const 
{
	return location;
}

CClassDeclareExtends::CClassDeclareExtends( const CSymbol* _id, const CSymbol* _extendsId, const IVarDeclareStar* _varDeclareStar, 
		const IMethodDeclareStar* _methodDeclareStar, int _location ) :
	id( _id ), extendsId( _extendsId ), varDeclareStar( _varDeclareStar), methodDeclareStar( _methodDeclareStar ), location( _location )
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

int CClassDeclareExtends::GetLocation() const 
{
	return location;
}


CVarDeclareStar::CVarDeclareStar( const IVarDeclareStar* varDecStar, const IVarDeclare* varDec, int _location ) :
		varDeclareStar( varDecStar ), varDeclare( varDec ), location( _location )
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

int CVarDeclareStar::GetLocation() const 
{
	return location;
}

CVarDeclare::CVarDeclare( const CSymbol* _typeId, const CSymbol* _id, int _location ) :
		typeId (_typeId ), id( _id ), location( _location )
	{
		assert( id != 0 );
		assert( typeId != 0 );
	}

const CSymbol* CVarDeclare::GetType() const
{
	return typeId;
}

const CSymbol* CVarDeclare::GetId() const
{
	return id;
}

int CVarDeclare::GetLocation() const 
{
	return location;
}

CMethodDeclareStar::CMethodDeclareStar( const IMethodDeclareStar* methodDecStar, const IMethodDeclare* methodDec, int _location ) :
	methodDeclareStar( methodDecStar ), methodDeclare( methodDec ), location( _location )
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

int CMethodDeclareStar::GetLocation() const 
{
	return location;
}

CMethodDeclare::CMethodDeclare( const CSymbol* _dataType, const CSymbol* _id, const IFormalList* _formalList, const IVarDeclareStar* varDecStar, 
		const IStatementStar* _statementStar, const IExpression* _expression, int _location ) :
	dataType( _dataType ), id( _id ), formalList( _formalList ), varDeclareStar( varDecStar ), statementStar( _statementStar ),
	expression( _expression ), location( _location )
	{
		//
		assert( id != 0 );
		assert( expression != 0 );
		assert( dataType != 0 );
	}
const CSymbol* CMethodDeclare::GetType() const
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

int CMethodDeclare::GetLocation() const 
{
	return location;
}

CFormalList::CFormalList( const CSymbol* _dataType, const CSymbol* _id, const IFormalRestStar* _formalRestStar, int _location ) :
	dataType( _dataType ), id( _id ), formalRestStar( _formalRestStar ), location( _location )
	{
		assert( id != 0 );
	}

const CSymbol* CFormalList::GetType() const
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

int CFormalList::GetLocation() const 
{
	return location;
}

CFormalRestStar::CFormalRestStar( const CSymbol* _dataType, const CSymbol* _id, const IFormalRestStar* _formalRestStar, int _location ) :
	dataType( _dataType ), id( _id ), formalRestStar( _formalRestStar ), location( _location )
	{
		
		assert( id != 0 );
	}

const CSymbol* CFormalRestStar::GetType() const
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

int CFormalRestStar::GetLocation() const 
{
	return location;
}

CStatementStar::CStatementStar(const IStatement* _statement, const IStatementStar* _statementStar, int _location ) :
	statement( _statement ), statementStar( _statementStar ), location( _location )
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

int CStatementStar::GetLocation() const 
{
	return location;
}

CStatement::CStatement( const IStatementStar* _statementStar, int _location ) :
	statementStar( _statementStar ), location( _location )
	{
	}

const IStatementStar* CStatement::GetStatementStar() const
{
	return statementStar;
}

int CStatement::GetLocation() const 
{
	return location;
}

CStatementIf::CStatementIf( const IExpression* _expression, const IStatement* _statementIf, const IStatement* _statementElse, int _location ) :
	expression( _expression ), statementIf( _statementIf ), statementElse( _statementElse ), location( _location )
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

int CStatementIf::GetLocation() const 
{
	return location;
}

CStatementWhile::CStatementWhile( const IExpression* _expression, const IStatement* _statement, int _location ) :
	expression( _expression ), statement( _statement ), location( _location )
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

int CStatementWhile::GetLocation() const 
{
	return location;
}

CStatementSysOut::CStatementSysOut( const IExpression* _expression, int _location ) :
	expression( _expression ), location( _location )
	{
		assert( expression != 0 );
	}

const IExpression* CStatementSysOut::GetExpression() const
{
	return expression;
}

int CStatementSysOut::GetLocation() const 
{
	return location;
}

CStatementAssignment::CStatementAssignment( const CSymbol* _id, const IExpression* _expression, int _location ) :
	id( _id ), expression( _expression ), location( _location )
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

int CStatementAssignment::GetLocation() const 
{
	return location;
}

CStatementArrayAssignment::CStatementArrayAssignment( const CSymbol* _id, const IExpression* _expressionArray, const IExpression* _expression, int _location ) :
	id( _id ), expressionArray( _expressionArray ), expression( _expression ), location( _location )
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

int CStatementArrayAssignment::GetLocation() const 
{
	return location;
}

CExpressionBinOp::CExpressionBinOp( const IExpression* _expressionFirst, const TBinaryOperation _operation, const IExpression* _expressionSecond, int _location ) :
	expressionFirst( _expressionFirst ), operation( _operation ), expressionSecond( _expressionSecond ), location( _location )
	{
		assert( expressionFirst != 0 );
		assert( operation >= 0 );
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

int CExpressionBinOp::GetLocation() const 
{
	return location;
}

CExpressionArray::CExpressionArray( const IExpression* _expression1, const IExpression* _expression2, int _location ) :
	expression1( _expression1 ), expression2( _expression2 ), location( _location )
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

int CExpressionArray::GetLocation() const 
{
	return location;
}

CExpressionLength::CExpressionLength( const IExpression* _expression, int _location ) :
	expression( _expression ), location( _location )
	{
		assert( expression != 0 );
	}

const IExpression* CExpressionLength::GetExpression() const
{
	return expression;	
}

int CExpressionLength::GetLocation() const 
{
	return location;
}

CExpressionCallMethod::CExpressionCallMethod( const IExpression* _expression, const CSymbol* _id, const IExpList* _expList, int _location ) :
	expression( _expression ), id( _id ), expList( _expList ), location( _location )
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

int CExpressionCallMethod::GetLocation() const 
{
	return location;
}

CExpressionNumber::CExpressionNumber( const CSymbol* _number, int _location ) : number( _number ), location( _location ) {}

const CSymbol* CExpressionNumber::GetNumber() const
{
	return number;
}

int CExpressionNumber::GetLocation() const 
{
	return location;
}

CExpressionBool::CExpressionBool( const bool _value, int _location ) : value( _value ), location( _location ) {}

const bool CExpressionBool::GetValue() const
{
	return value;
}

int CExpressionBool::GetLocation() const 
{
	return location;
}

CExpressionVar::CExpressionVar( const CSymbol* _id, int _location ) : id( _id ), location( _location )
	{
		assert( id != 0 );
	}

const CSymbol* CExpressionVar::GetId() const
{
	return id;
}

int CExpressionVar::GetLocation() const 
{
	return location;
}

CExpressionThis::CExpressionThis( int _location ) : location( _location ) {}

int CExpressionThis::GetLocation() const 
{
	return location;
}

CExpressionNewInt::CExpressionNewInt( const IExpression* _expression, int _location ) :
		expression( _expression ), location( _location ) {}

const IExpression* CExpressionNewInt::GetExpression() const
{
	return expression;
}

int CExpressionNewInt::GetLocation() const 
{
	return location;
}

CExpressionNewId::CExpressionNewId( const CSymbol* _id, int _location ) : id( _id ), location( _location ) {}

const CSymbol* CExpressionNewId::GetId() const
{
	return id;
}

int CExpressionNewId::GetLocation() const 
{
	return location;
}

CExpressionNegation::CExpressionNegation( const IExpression* _expression, int _location ) : expression( _expression ), location( _location ) {}

const IExpression* CExpressionNegation::GetExpression() const 
{
	return expression;
}

int CExpressionNegation::GetLocation() const 
{
	return location;
}

CExpression::CExpression( const IExpression* _expression, int _location ) : expression( _expression ), location( _location ) {}

const IExpression* CExpression::GetExpression() const
{
	return expression;
}

int CExpression::GetLocation() const 
{
	return location;
}

CExpList::CExpList( const IExpression* _expression, int _location ) :
		expression( _expression ), location( _location ) {}

const IExpression* CExpList::GetExpression() const
{
	return expression;
}

int CExpList::GetLocation() const 
{
	return location;
}

CExpListNext::CExpListNext( const IExpList* _expList, const IExpression* _expression, int _location ) :
		expList( _expList ), expression( _expression ), location( _location ) {}

const IExpression* CExpListNext::GetExpression() const
{
	return expression;
}
const IExpList* CExpListNext::GetExpList() const
{
	return expList;
}

int CExpListNext::GetLocation() const 
{
	return location;
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