#include "SymbolTable.h"

CClassDescription* CSymbolTable::AddClass( CSymbol* className ) {
	if ( classes.find( className ) == classes.end() )
		return NULL;
	return classes.at( className ) = new CClassDescription( className );
}

void CSymbolTable::DeleteClass( CSymbol* className ) {
	classes.erase( className );
}

map<CSymbol*, CClassDescription*> CSymbolTable::GetClasses() {
	return classes;
}

CClassDescription::CClassDescription( CSymbol* _name ) {
	name = _name;
}

CVarDescription* CClassDescription::AddField( CSymbol* _name, CTypeInfo _type ) {
	if ( fields.find( _name ) == fields.end() )
		return NULL;
	return fields.at( _name ) = new CVarDescription( _name, _type );
}

CMethodDescription* CClassDescription::AddMethod( CSymbol* _name, CTypeInfo _type ) {
	if ( methods.find( _name ) == methods.end() )
		return NULL;
	return methods.at( _name ) = new CMethodDescription( _name, _type );
}

void CClassDescription::DeleteField( CSymbol* _name ) {
	fields.erase( _name );
}

void CClassDescription::DeleteMethod( CSymbol* _name ) {
	methods.erase( _name );
}

map<CSymbol*, CVarDescription*> CClassDescription::GetFields() {
	return fields;
}

map<CSymbol*, CMethodDescription*> CClassDescription::GetMethods() {
	return methods;
}