#pragma once
#include "Symbols.h"
#include "miniJava.h"
#include <map>
#include <string>
#include <vector>

using std::map;

class CSymbolTable;
class CClassDescription;
class CVarDescription;
class CMethodDescription;

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
	map<const CSymbol*, size_t> orderedFields;
	map<const CSymbol*, CMethodDescription*> methods;

	CSymbolTable* symbolTable;
public:
	CClassDescription( CSymbolTable* _symbolTable, const CSymbol* _name);
	CClassDescription( CSymbolTable* _symbolTable, const CSymbol* _name, const CSymbol* _base );

	CVarDescription* AddField( const CSymbol* _name, const CTypeInfo* type );
	CMethodDescription* AddMethod( const CSymbol* _name, const CTypeInfo* returnType );

	CVarDescription* LookUpField( const CSymbol* field );
	CMethodDescription* LookUpMethod( const CSymbol* method );

	CVarDescription* LookUp( const CSymbol* var );

	const CSymbol* GetName() const;

	size_t SizeOf() const;
	size_t GetFieldOffset( const CSymbol* name ) const;
};

class CVarDescription {
private:
	const CSymbol* name;
	const CTypeInfo* type;
public:
	CVarDescription( const CSymbol* _name, const CTypeInfo* _type );

	const CSymbol* GetName() const;
	const CTypeInfo* GetType() const;
};

class CMethodDescription {
private:
	const CSymbol* name;
	map<const CSymbol*, CVarDescription*> params;
	vector<CVarDescription*> orderedParams;
	const CTypeInfo* returnType;
	map<const CSymbol*, CVarDescription*> locals;

	CClassDescription* currentClass;
public:
	CMethodDescription( CClassDescription* _currentClass, const CSymbol* _name, const CTypeInfo* _returnType );
	
	CVarDescription* AddPapam( CVarDescription* param );
	CVarDescription* AddLocal( CVarDescription* local );

	const CSymbol* GetName() const;
	const CTypeInfo* GetType() const;
	vector<CVarDescription*> GetOrderedParams() const;
	int GetParamsNumber() const;
	
	CVarDescription* LookUpParam( const CSymbol* param );
	CVarDescription* LookUpLocal( const CSymbol* local );

	CVarDescription* LookUp( const CSymbol* variable );
};
