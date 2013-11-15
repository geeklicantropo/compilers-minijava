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

	CSymbolTable* symbolTable;
public:
	CClassDescription( CSymbolTable* _symbolTable, const CSymbol* _name);
	CClassDescription( CSymbolTable* _symbolTable, const CSymbol* _name, const CSymbol* _base );

	CVarDescription* AddField( const CSymbol* _name, CTypeInfo* type );
	CMethodDescription* AddMethod( const CSymbol* _name, CTypeInfo* returnType );

	CVarDescription* LookUpField( const CSymbol* field );
	CMethodDescription* LookUpMethod( const CSymbol* method );

	CVarDescription* LookUp( const CSymbol* var );
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

	CClassDescription* currentClass;
public:
	CMethodDescription( CClassDescription* _currentClass, const CSymbol* _name, CTypeInfo* _returnType );
	
	CVarDescription* AddPapam( CVarDescription* param );
	CVarDescription* AddLocal( CVarDescription* local );

	const CSymbol* GetName();
	const CSymbol* GetType();
	
	CVarDescription* LookUpParam( const CSymbol* param );
	CVarDescription* LookUpLocal( const CSymbol* local );

	CVarDescription* LookUp( const CSymbol* variable );
};
