#include <assert.h>

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

class CMainClass;
class IClassDeclare;
class CClassDeclareStar;
class IStatement;
class IExpression;
class CMethodDeclare;
class CMethodDeclareStar;
class CVarDeclare;
class CVarDeclareStar;
class CFormalList;
class CFormalRestStar;
class CFormalRest;
class CStatementStar;

class CProgram {
public:
	CProgram( const CMainClass* _mainClass, const CClassDeclareStar* _classDeclareStar ) :
	mainClass( _mainClass), classDeclareStar( _classDeclareStar )
	{
		assert( mainClass != 0 );
		assert( classDeclareStar != 0 );
	}

private:
	const CMainClass* const mainClass;
	const CClassDeclareStar* const classDeclareStar;
};

class CMainClass {
public:
	CMainClass( const char* _id, const char* _argsId, const IStatement* _stm ) :
		id( _id ), argsId( _argsId ), stm( _stm )
	{
		assert( id != 0 );
		assert( argsId != 0 );
		assert( stm != 0 );
	};
private:
	const char* const id;
	const char* const argsId;
	const IStatement* const stm;
};

class IClassDeclare {
public:
	~IClassDeclare();
};

class CClassDeclareStar {
public:
	CClassDeclareStar(const CClassDeclareStar* classDecStar, const IClassDeclare* classDec ) :
	classDeclareStar( classDecStar ), classDeclare( classDec )
	{
		assert( classDeclare != 0 );
	}

private:
	const CClassDeclareStar* const classDeclareStar;
	const IClassDeclare* const classDeclare;
};

class CClassDeclare : public IClassDeclare {
public:
	CClassDeclare( const char* _id, const CVarDeclareStar* _varDeclareStar, const CMethodDeclareStar* _methodDeclareStar) :
		id( _id ), varDeclareStar( _varDeclareStar), methodDeclareStar( _methodDeclareStar )
	{
		assert( id != 0 );
		assert( varDeclareStar != 0 );
		assert( methodDeclareStar != 0 );
	}

private:
	const char* const id;
	const CVarDeclareStar* const varDeclareStar;
	const CMethodDeclareStar* const methodDeclareStar;
};

class CClassDeclareExtends : public IClassDeclare {
public:
	CClassDeclareExtends( const char* _id, const char* _extendsId, const CVarDeclareStar* _varDeclareStar, const CMethodDeclareStar* _methodDeclareStar) :
		id( _id ), extendsId( _extendsId ), varDeclareStar( _varDeclareStar), methodDeclareStar( _methodDeclareStar )
	{
		assert( id != 0 );
		assert( extendsId != 0 );
		assert( varDeclareStar != 0 );
		assert( methodDeclareStar != 0 );
	}
private:
	const char* const id;
	const char* const extendsId;
	const CVarDeclareStar* const varDeclareStar;
	const CMethodDeclareStar* const methodDeclareStar;
};

class CVarDeclareStar
{
public:
	CVarDeclareStar( const CVarDeclareStar* varDecStar, const CVarDeclare* varDec ) :
		varDeclareStar( varDecStar ), varDeclare( varDec ) 
	{
		assert( varDeclare != 0);
	}
private:
	const CVarDeclareStar* const varDeclareStar;
	const CVarDeclare* const varDeclare;
};

class CVarDeclare
{
public:
	CVarDeclare( const TDataTypes* _dataType, const char* _id ) :
		dataType (_dataType ), id( _id )
	{
		assert( dataType != 0 );
		assert( id != _id );
	}

private:
	const TDataTypes* const dataType;
	const char* const id;
};

class CMethodDeclareStar
{
public:
	CMethodDeclareStar( const CMethodDeclareStar* methodDecStar, const CMethodDeclare* methodDec ) :
		methodDeclareStar( methodDecStar ), methodDeclare( methodDec )
	{
		assert( methodDeclare != 0 );
	}

private:
	const CMethodDeclareStar* const methodDeclareStar;
	const CMethodDeclare* const methodDeclare;
};

class CMethodDeclare
{
public:
	CMethodDeclare( const TDataTypes* _dataType, const char* _id, const CFormalList* _formalList, const CVarDeclareStar* varDecStar, 
		const CStatementStar* _statementStar, const IExpression* _expression ) :
	dataType( _dataType ), id( _id ), formalList( _formalList ), varDeclareStar( varDecStar ), statementStar( _statementStar ),
		expression( _expression )
	{
		assert( dataType != 0 );
		assert( id != 0 );
		assert( formalList != 0 );
		assert( varDeclareStar != 0 );
		assert( statementStar != 0 );
		assert( expression != 0 );
	}
private:
	const TDataTypes* const dataType;
	const char* const id;
	const CFormalList* const formalList;
	const CVarDeclareStar* const varDeclareStar;
	const CStatementStar* const statementStar;
	const IExpression* const expression;
};

class CFormalList
{
public:
	CFormalList( const TDataTypes* _dataType, const char* _id, const CFormalRestStar* _formalRestStar ) :
		dataType( _dataType ), id( _id ), formalRestStar( _formalRestStar )
	{
		assert( dataType != 0 );
		assert( id != 0 );
		assert( formalRestStar != 0 );
	}
private:
	const TDataTypes* const dataType;
	const char* const id;
	const CFormalRestStar* const formalRestStar;
};

class CFormalRestStar
{
public:
	CFormalRestStar( const CFormalRestStar* _formalRestStar, const CFormalRest* _formalRest ) :
		formalRestStar( _formalRestStar ), formalRest( _formalRest )
	{
		assert( formalRest != 0 );
	}
private:
	const CFormalRestStar* const formalRestStar;
	const CFormalRest* const formalRest;
};

class CFormalRest
{
public:
	CFormalRest( const TDataTypes* _dataType, const char* _id ) :
		dataType( _dataType ), id( _id )
	{
		assert( dataType != 0 );
		assert( id != 0 );
	}
private:
	const TDataTypes* const dataType;
	const char* const id;
};

class IStatement
{
public:
	~IStatement();
};

class CStatementStar
{
public:
	CStatementStar(const IStatement* _statement, const CStatementStar* _statementStar) :
		statement( _statement ), statementStar( _statementStar )
	{
		assert( statement != 0 );
	}
private:
	const CStatementStar* const statementStar;
	const IStatement* const statement;
};

class CStatement : public IStatement
{
public:
	CStatement( const CStatementStar* _statementStar ) :
		statementStar( _statementStar )
	{
		assert( statementStar != 0 );
	}
private:
	const CStatementStar* const statementStar;
};

class CStatementIf : public IStatement
{
public:
	CStatementIf( const IExpression* _expression, const IStatement* _statementIf, const IStatement* _statementElse ) :
		expression( _expression ), statementIf( _statementIf ), statementElse( _statementElse )
	{
		assert( expression != 0 );
		assert( statementIf != 0 );
		assert( statementElse != 0 );
	}
private:
	const IExpression* const expression;
	const IStatement* const statementIf;
	const IStatement* const statementElse;
};

class CStatementWhile : public IStatement
{
public:
	CStatementWhile( const IExpression* _expression, const IStatement* _statement ) :
		expression( _expression ), statement( _statement )
	{
		assert( expression != 0 );
		assert( statement != 0 );
	}
private:
	const IExpression* const expression;
	const IStatement* const statement;
};

class CStatementSysOut : public IStatement
{
public:
	CStatementSysOut( const IExpression* _expression ) :
		expression( _expression )
	{
		assert( expression != 0 );
	}
private:
	const IExpression* const expression;
};

class CStatementAssignment : public IStatement
{
public:
	CStatementAssignment( const char* _id, const IExpression* _expression ) :
		id( _id ), expression( _expression )
	{
		assert( id != 0 );
		assert( expression != 0 );
	}

private:
	const char* const id;
	const IExpression* const expression;
};

class CStatementArrayAssignment : public IStatement
{
public:
	CStatementArrayAssignment( const char* _id, const IExpression* _expressionArray, const IExpression* _expression ) :
		id( _id ), expressionArray( _expressionArray ), expression( _expression )
	{
		assert( id != 0 );
		assert( expressionArray != 0 );
		assert( expression != 0 );
	}

private:
	const char* const id;
	const IExpression* const expressionArray;
	const IExpression* const expression;
};

