#include "Symbols.h"
#include <vector>
#include <string>

using std::vector;

class CSymbolTable {
private:
	vector<CClassDescription*> classes;
public:
	CClassDescription* AddClass( CSymbol* className );

	vector<CClassDescription*> GetClasses();	
};

class CClassDescription {
private:
	vector<CVarDescription*> fields;
	vector<CMethodDescription*> methods;
public:
	CVarDescription* AddField( CTypeInfo type, CSymbol* name );
	CMethodDescription* AddMethod( CTypeInfo returnType, CSymbol* name );

	vector<CVarDescription*> GetFields();
	vector<CMethodDescription*> GetMethods();
};

class CVarDescription {
private:
	CSymbol* name;
	CTypeInfo type;
public:
	CVarDescription( CSymbol* name_, CTypeInfo type_ );

	CSymbol* GetName();
	CSymbol* GetType();
};

class CMethodDescription {
private:
	CSymbol* name;
	vector<CVarDescription*> params;
	CTypeInfo returnType;
	vector<CVarDescription*> locals;
public:
	CMethodDescription( CSymbol* name_, CTypeInfo returnType_ );
	
	CVarDescription* AddPapam( CVarDescription* param );
	CVarDescription* AddLocal( CVarDescription* local );

	CSymbol* GetName();
	CSymbol* GetType();
	vector<CVarDescription*> GetParams();
	vector<CVarDescription*> GetLocals();
};

class CTypeInfo
{
private:
	CSymbol* type;
	//bool isStandart;
public:
	CTypeInfo ( CSymbol* type_ );
	
	CSymbol* GetType();
};
