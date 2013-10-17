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


class IProgram {
public:
	~IProgram();
};

class IMainClass {
public:
	~IMainClass();
};

class IClassDeclare {
public:
	~IClassDeclare();
};

class IClassDeclareStar {
public:
	~IClassDeclareStar();
};

class IVarDeclareStar {
public:
	~IVarDeclareStar();
};

class IVarDeclare {
public:
	~IVarDeclare();
};

class IMethodDeclareStar {
public:
	~IMethodDeclareStar();
};

class IMethodDeclare {
public:
	~IMethodDeclare();
};

class IFormalList {
public:
	~IFormalList();
};

class IFormalRestStar {
public:
	~IFormalRestStar();
};

class IStatement {
public:
	~IStatement();
};

class IStatementStar {
public:
	~IStatementStar();
};

class IExpression {
public:
	~IExpression();
};

class IExpList {
public:
	~IExpList();
};
