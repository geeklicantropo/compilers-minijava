#include "SymbolTable.h"

CTypeInfo::CTypeInfo( CSymbol* _type ) : type( _type ) {}

CSymbol* CTypeInfo::GetType() {
	return type;
}

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

CClassDescription::CClassDescription( CSymbol* _name ) : name( _name ) {}

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

CVarDescription::CVarDescription( CSymbol* _name, CTypeInfo _type ) : name( _name ), type( _type ) {}

CSymbol* CVarDescription::GetName() {
	return name;
}

CSymbol* CVarDescription::GetType() {
	return type.GetType();
}

CMethodDescription::CMethodDescription( CSymbol* _name, CTypeInfo _returnType ) : name( _name ), returnType( _returnType ) {}

CVarDescription* CMethodDescription::AddPapam( CVarDescription* param ) {
	if ( params.find( param->GetName() ) == params.end() )
		return NULL;
	return params.at( param->GetName() ) = new CVarDescription( param->GetName(), param->GetType() );
}

CVarDescription* CMethodDescription::AddLocal( CVarDescription* local ) {
	if ( locals.find( local->GetName() ) == locals.end() )
		return NULL;
	return locals.at( local->GetName() ) = new CVarDescription( local->GetName(), local->GetType() );
}

void CMethodDescription::DeletePapam( CSymbol* paramName ) {
	params.erase( paramName );
}

void CMethodDescription::DeleteLocal( CSymbol* localName ) {
	locals.erase( localName );
}

CSymbol* CMethodDescription::GetName() {
	return name;
}

CSymbol* CMethodDescription::GetType() {
	return returnType.GetType();
}

map<CSymbol*, CVarDescription*> CMethodDescription::GetParams() {
	return params;
}

map<CSymbol*, CVarDescription*> CMethodDescription::GetLocals() {
	return locals;
}