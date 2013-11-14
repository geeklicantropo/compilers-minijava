#include "Symbols.h"
#include <map>
#include <string>

using std::map;

class CSymbolTable;
class CClassDescription;
class CVarDescription;
class CMethodDescription;

class CTypeInfo
{
private:
	CSymbol* type;
	//bool isStandart;
public:
	CTypeInfo ( CSymbol* _type );
	
	CSymbol* GetType();
};

class CSymbolTable {
private:
	map<CSymbol*, CClassDescription*> classes;
public:
	CClassDescription* AddClass( CSymbol* className );

	CClassDescription* LookUpClass( CSymbol* className );
};

class CClassDescription {
private:
	const CSymbol* name;
	map<CSymbol*, CVarDescription*> fields;
	map<CSymbol*, CMethodDescription*> methods;
public:
	CClassDescription(const CSymbol* _name);

	CVarDescription* AddField( CSymbol* _name, CTypeInfo type );
	CMethodDescription* AddMethod( CSymbol* _name, CTypeInfo returnType );

	CVarDescription* LookUpField( CSymbol* field );
	CMethodDescription* LookUpMethod( CSymbol* method );
};

class CVarDescription {
private:
	CSymbol* name;
	CTypeInfo type;
public:
	CVarDescription( CSymbol* _name, CTypeInfo _type );

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
	CMethodDescription( CSymbol* _name, CTypeInfo _returnType );
	
	CVarDescription* AddPapam( CVarDescription* param );
	CVarDescription* AddLocal( CVarDescription* local );

	CSymbol* GetName();
	CSymbol* GetType();
	
	CVarDescription* LookUpParam( CSymbol* param );
	CVarDescription* LookUpLocal( CSymbol* local );
};
