#include "SymbolTable.h"

CTypeInfo::CTypeInfo( const CSymbol* _type ) : type( _type ) {}

const CSymbol* CTypeInfo::GetType() {
	return type;
}

CClassDescription* CSymbolTable::AddClass( const CSymbol* className ) {
	if( classes.find( className ) == classes.end() )
		return NULL;
	return classes.at( className ) = new CClassDescription( className );
}

CClassDescription* CSymbolTable::AddClass( const CSymbol* className, const CSymbol* baseName ) {
	if( classes.find( className ) == classes.end() )
		return NULL;
	return classes.at( className ) = new CClassDescription( className, baseName );
}

CClassDescription* CSymbolTable::LookUpClass( const CSymbol* className ) {
	if( classes.find( className ) == classes.end() )
		return NULL;
	return classes[className];
}

CClassDescription::CClassDescription( const CSymbol* _name ) : name( _name )
{
	baseClass = 0;
}

CClassDescription::CClassDescription( const CSymbol* _name, const CSymbol* _base ) : name( _name ), baseClass( _base ) {}

CVarDescription* CClassDescription::AddField( const CSymbol* _name, CTypeInfo* _type ) {
	if ( fields.find( _name ) == fields.end() )
		return NULL;
	return fields.at( _name ) = new CVarDescription( _name, _type );
}

CMethodDescription* CClassDescription::AddMethod( const CSymbol* _name, CTypeInfo* _type ) {
	if ( methods.find( _name ) == methods.end() )
		return NULL;
	return methods.at( _name ) = new CMethodDescription( _name, _type );
}

CVarDescription* CClassDescription::LookUpField( const CSymbol* field ) {
	if ( fields.find( field ) == fields.end() )
		return NULL;
	return fields[field];
}

CMethodDescription* CClassDescription::LookUpMethod( const CSymbol* method ) {
	if ( methods.find( method ) == methods.end() )
		return NULL;
	return methods[method];
}

CVarDescription::CVarDescription( const CSymbol* _name, CTypeInfo* _type ) : name( _name ), type( _type ) {}

const CSymbol* CVarDescription::GetName() {
	return name;
}

const CSymbol* CVarDescription::GetType() {
	return type->GetType();
}

CMethodDescription::CMethodDescription( const CSymbol* _name, CTypeInfo* _returnType ) : name( _name ), returnType( _returnType ) {}

CVarDescription* CMethodDescription::AddPapam( CVarDescription* param ) {
	if ( params.find( param->GetName() ) == params.end() )
		return NULL;
	return params.at( param->GetName() ) = new CVarDescription( param->GetName(), new CTypeInfo( param->GetType() ) );
}

CVarDescription* CMethodDescription::AddLocal( CVarDescription* local ) {
	if ( locals.find( local->GetName() ) == locals.end() )
		return NULL;
	return locals.at( local->GetName() ) = new CVarDescription( local->GetName(), new CTypeInfo( local->GetType() ) );
}

const CSymbol* CMethodDescription::GetName() {
	return name;
}

const CSymbol* CMethodDescription::GetType() {
	return returnType->GetType();
}

CVarDescription* CMethodDescription::LookUpParam( CSymbol* param ) {
	if ( params.find( param ) == params.end() )
		return NULL;
	return params[param];
}

CVarDescription* CMethodDescription::LookUpLocal( CSymbol* local ) {
	if (locals.find( local ) == locals.end() )
		return NULL;
	return locals[local];
}