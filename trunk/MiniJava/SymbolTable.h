#include "Symbols.h"
#include <map>
#include <string>

using std::map;

class CSymbolTable {
private:
	map<CSymbol*, CClassDescription*> classes;
public:
	CClassDescription* AddClass( CSymbol* className );
	void DeleteClass( CSymbol* className );

	map<CSymbol*, CClassDescription*> GetClasses();	
};

class CClassDescription {
private:
	CSymbol* name;
	map<CSymbol*, CVarDescription*> fields;
	map<CSymbol*, CMethodDescription*> methods;
public:
	CClassDescription(CSymbol* _name);

	CVarDescription* AddField( CSymbol* _name, CTypeInfo type );
	CMethodDescription* AddMethod( CSymbol* _name, CTypeInfo returnType );

	void DeleteField( CSymbol* _name );
	void DeleteMethod( CSymbol* _name );

	map<CSymbol*, CVarDescription*> GetFields();
	map<CSymbol*, CMethodDescription*> GetMethods();
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
	map<CSymbol*, CVarDescription*> params;
	CTypeInfo returnType;
	map<CSymbol*, CVarDescription*> locals;
public:
	CMethodDescription( CSymbol* name_, CTypeInfo returnType_ );
	
	CVarDescription* AddPapam( CVarDescription* param );
	CVarDescription* AddLocal( CVarDescription* local );

	CVarDescription* DeletePapam( CSymbol* paramName );
	CVarDescription* DeleteLocal( CSymbol* lokalName );

	CSymbol* GetName();
	CSymbol* GetType();
	map<CSymbol*, CVarDescription*> GetParams();
	map<CSymbol*, CVarDescription*> GetLocals();
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
