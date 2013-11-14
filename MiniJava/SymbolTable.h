#pragma once
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
	const CSymbol* type;
	//bool isStandart;
public:
	CTypeInfo ( const CSymbol* _type );
	
	const CSymbol* GetType();
};

class CSymbolTable {
private:
	map<const CSymbol*, CClassDescription*> classes;
public:
	CClassDescription* AddClass( const CSymbol* className );
	CClassDescription* AddClass( const CSymbol* className, const CSymbol* baseName );

	CClassDescription* LookUpClass( const CSymbol* className );
};

class CClassDescription {
private:
	const CSymbol* name;
	const CSymbol* baseClass;
	map<const CSymbol*, CVarDescription*> fields;
	map<const CSymbol*, CMethodDescription*> methods;
public:
	CClassDescription(const CSymbol* _name);
	CClassDescription(const CSymbol* _name, const CSymbol* _base);

	CVarDescription* AddField( const CSymbol* _name, CTypeInfo* type );
	CMethodDescription* AddMethod( const CSymbol* _name, CTypeInfo* returnType );

	CVarDescription* LookUpField( const CSymbol* field );
	CMethodDescription* LookUpMethod( const CSymbol* method );
};

class CVarDescription {
private:
	const CSymbol* name;
	CTypeInfo* type;
public:
	CVarDescription( const CSymbol* _name, CTypeInfo* _type );

	const CSymbol* GetName();
	const CSymbol* GetType();
};

class CMethodDescription {
private:
	const CSymbol* name;
	map<const CSymbol*, CVarDescription*> params;
	CTypeInfo* returnType;
	map<const CSymbol*, CVarDescription*> locals;
public:
	CMethodDescription( const CSymbol* _name, CTypeInfo* _returnType );
	
	CVarDescription* AddPapam( CVarDescription* param );
	CVarDescription* AddLocal( CVarDescription* local );

	const CSymbol* GetName();
	const CSymbol* GetType();
	
	CVarDescription* LookUpParam( CSymbol* param );
	CVarDescription* LookUpLocal( CSymbol* local );
};
