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
class CMethodDeclareStar;
class CVarDeclareStar;

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
		assert( classDeclareStar != 0);
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