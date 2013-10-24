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

class CProgram : public IProgram {
public:
	CProgram( const IMainClass* , const IClassDeclareStar* );

private:
	const IMainClass* const mainClass;
	const IClassDeclareStar* const classDeclareStar;
};

class CMainClass : public IMainClass {
public:
	CMainClass( const char*, const char*, const IStatement* );
		
private:
	const char* const id;
	const char* const argsId;
	const IStatement* const stm;
};

class CClassDeclareStar : public IClassDeclareStar {
public:
	CClassDeclareStar(const IClassDeclareStar*, const IClassDeclare* );

private:
	const IClassDeclareStar* const classDeclareStar;
	const IClassDeclare* const classDeclare;
};

class CClassDeclare : public IClassDeclare {
public:
	CClassDeclare( const char*, const IVarDeclareStar*, const IMethodDeclareStar* );

private:
	const char* const id;
	const IVarDeclareStar* const varDeclareStar;
	const IMethodDeclareStar* const methodDeclareStar;
};

class CClassDeclareExtends : public IClassDeclare {
public:
	CClassDeclareExtends( const char*, const char*, const IVarDeclareStar*, 
		const IMethodDeclareStar* );

private:
	const char* const id;
	const char* const extendsId;
	const IVarDeclareStar* const varDeclareStar;
	const IMethodDeclareStar* const methodDeclareStar;
};

class CVarDeclareStar : public IVarDeclareStar {
public:
	CVarDeclareStar( const IVarDeclareStar*, const IVarDeclare* );
	
private:
	const IVarDeclareStar* const varDeclareStar;
	const IVarDeclare* const varDeclare;
};

class CVarDeclare : public IVarDeclare {
public:
	CVarDeclare( const TDataTypes, const char* );

private:
	const TDataTypes dataType;
	const char* const id;
};

class CMethodDeclareStar : public IMethodDeclareStar {
public:
	CMethodDeclareStar( const IMethodDeclareStar*, const IMethodDeclare* );

private:
	const IMethodDeclareStar* const methodDeclareStar;
	const IMethodDeclare* const methodDeclare;
};

class CMethodDeclare : public IMethodDeclare {
public:
	CMethodDeclare( const TDataTypes, const char*, const IFormalList*, const IVarDeclareStar*, 
		const IStatementStar*, const IExpression* );

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

private:
	const TDataTypes dataType;
	const char* const id;
	const IFormalRestStar* const formalRestStar;
};

class CFormalRestStar : public IFormalRestStar {
public:
	CFormalRestStar( const TDataTypes, const char*, const IFormalRestStar* );

private:
	const TDataTypes dataType;
	const char* const id;
	const IFormalRestStar* const formalRestStar;
};

class CStatementStar : public IStatementStar {
public:
	CStatementStar(const IStatement*, const IStatementStar* );

private:
	const IStatementStar* const statementStar;
	const IStatement* const statement;
};

class CStatement : public IStatement {
public:
	CStatement( const IStatementStar* );

private:
	const IStatementStar* const statementStar;
};

class CStatementIf : public IStatement {
public:
	CStatementIf( const IExpression*, const IStatement*, const IStatement* );

private:
	const IExpression* const expression;
	const IStatement* const statementIf;
	const IStatement* const statementElse;
};

class CStatementWhile : public IStatement {
public:
	CStatementWhile( const IExpression*, const IStatement* );

private:
	const IExpression* const expression;
	const IStatement* const statement;
};

class CStatementSysOut : public IStatement {
public:
	CStatementSysOut( const IExpression* );

private:
	const IExpression* const expression;
};

class CStatementAssignment : public IStatement {
public:
	CStatementAssignment( const char*, const IExpression* );

private:
	const char* const id;
	const IExpression* const expression;
};

class CStatementArrayAssignment : public IStatement {
public:
	CStatementArrayAssignment( const char*, const IExpression*, const IExpression* );

private:
	const char* const id;
	const IExpression* const expressionArray;
	const IExpression* const expression;
};

class CExpressionBinOp : public IExpression {
public:
	CExpressionBinOp( const IExpression*, const TBinaryOperation, const IExpression* );
private:
	const IExpression* const expressionFirst;
	const TBinaryOperation operation;
	const IExpression* const expressionSecond;
};

class CExpressionArray : public IExpression {
public:
	CExpressionArray( const IExpression*, const IExpression*);

private:
	const IExpression* const expression1;
	const IExpression* const expression2;
};

class CExpressionLength : public IExpression {
public:
	CExpressionLength( const IExpression* );

private:
	const IExpression* const expression;
};

class CExpressionCallMethod : public IExpression {
public:
	CExpressionCallMethod( const IExpression*, const char*, const IExpList* );

private:
	const IExpression* const expression;
	const char* const id;
	const IExpList* const expList;
};

class CExpressionNumber : public IExpression {
public:
	CExpressionNumber( const int );

private:
	const int number;
};

class CExpressionBool : public IExpression {
public:
	CExpressionBool( const bool );

private:
	const bool value;
};

class CExpressionVar : public IExpression {
public:
	CExpressionVar( const char* );

private:
	const char* const id;
};

class CExpressionThis : public IExpression {
public:
	CExpressionThis();

private:
};

class CExpressionNewInt : public IExpression {
public:
	CExpressionNewInt( const IExpression* );

private:
	const IExpression* const expression;
};

class CExpressionNewId : public IExpression {
public:
	CExpressionNewId( const char* );

private:
	const char* const id;
};

class CExpressionNegation : public IExpression {
public:
	CExpressionNegation( const IExpression* );

private:
	const IExpression* const expression;
};

class CExpression : public IExpression {
public:
	CExpression( const IExpression* );

private:
	const IExpression* const expression;
};

class CExpList : public IExpList {
public:
	CExpList( const IExpression* );

private:
	const IExpression* const expression;
};

class CExpListNext : public IExpList {
public:
	CExpListNext( const IExpList*, const IExpression* );

private:
	const IExpression* const expression;
	const IExpList* const expList;
};

class IVisitor {
public:
	~IVisitor();
	virtual int Visit( const CProgram* n ) = 0;
	virtual int Visit( const CMainClass* n ) = 0;
	virtual int Visit( const CClassDeclareStar* n ) = 0;
	virtual int Visit( const CClassDeclare* n ) = 0;
	virtual int Visit( const CClassDeclareExtends* n ) = 0;
	virtual int Visit( const CVarDeclareStar* n ) = 0;
	virtual int Visit( const CVarDeclare* n ) = 0;
	virtual int Visit( const CMethodDeclare* n ) = 0;
	virtual int Visit( const CMethodDeclareStar* n ) = 0;
	virtual int Visit( const CFormalList* n ) = 0;
	virtual int Visit( const CFormalRestStar* n ) = 0;
	virtual int Visit( const CStatement* n ) = 0;
	virtual int Visit( const CStatementStar* n ) = 0;
	virtual int Visit( const CStatementIf* n ) = 0;
	virtual int Visit( const CStatementWhile* n ) = 0;
	virtual int Visit( const CStatementSysOut* n ) = 0;
	virtual int Visit( const CStatementAssignment* n ) = 0;
	virtual int Visit( const CStatementArrayAssignment* n ) = 0;
	virtual int Visit( const CExpressionBinOp* n ) = 0;
	virtual int Visit( const CExpressionArray* n ) = 0;
	virtual int Visit( const CExpressionLength* n ) = 0;
	virtual int Visit( const CExpressionCallMethod* n ) = 0;
	virtual int Visit( const CExpressionNumber* n ) = 0;
	virtual int Visit( const CExpressionBool* n ) = 0;
	virtual int Visit( const CExpressionVar* n ) = 0;
	virtual int Visit( const CExpressionNewInt* n ) = 0;
	virtual int Visit( const CExpressionNewId* n ) = 0;
	virtual int Visit( const CExpressionNegation* n ) = 0;
	virtual int Visit( const CExpression* n ) = 0;
	virtual int Visit( const CExpList* n ) = 0;
	virtual int Visit( const CExpListNext* n ) = 0;
};

class CInterpreter: public IVisitor {
public:
	int Visit( const CProgram* n );
	int Visit( const CMainClass* n );
	int Visit( const CClassDeclareStar* n );
	int Visit( const CClassDeclare* n );
	int Visit( const CClassDeclareExtends* n );
	int Visit( const CVarDeclareStar* n );
	int Visit( const CVarDeclare* n );
	int Visit( const CMethodDeclare* n );
	int Visit( const CMethodDeclareStar* n );
	int Visit( const CFormalList* n );
	int Visit( const CFormalRestStar* n );
	int Visit( const CStatement* n );
	int Visit( const CStatementStar* n );
	int Visit( const CStatementIf* n );
	int Visit( const CStatementWhile* n );
	int Visit( const CStatementSysOut* n );
	int Visit( const CStatementAssignment* n );
	int Visit( const CStatementArrayAssignment* n );
	int Visit( const CExpressionBinOp* n );
	int Visit( const CExpressionArray* n );
	int Visit( const CExpressionLength* n );
	int Visit( const CExpressionCallMethod* n );
	int Visit( const CExpressionNumber* n );
	int Visit( const CExpressionBool* n );
	int Visit( const CExpressionVar* n );
	int Visit( const CExpressionNewInt* n );
	int Visit( const CExpressionNewId* n );
	int Visit( const CExpressionNegation* n );
	int Visit( const CExpression* n );
	int Visit( const CExpList* n );
	int Visit( const CExpListNext* n );
};