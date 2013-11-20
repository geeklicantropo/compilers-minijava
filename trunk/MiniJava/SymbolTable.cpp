#include "SymbolTable.h"

CTypeInfo::CTypeInfo( const CSymbol* _type ) : type( _type ) {}

const CSymbol* CTypeInfo::GetType() {
	return type;
}

CClassDescription* CSymbolTable::AddClass( const CSymbol* className ) {
	map< const CSymbol*, CClassDescription* >::iterator it = classes.find( className );
	if( it != classes.end() )
		return NULL;
	return classes[className] = new CClassDescription( this, className );
}

CClassDescription* CSymbolTable::AddClass( const CSymbol* className, const CSymbol* baseName ) {
	map< const CSymbol*, CClassDescription* >::iterator it = classes.find( className );
	if( it != classes.end() )
		return NULL;
	return classes[className] = new CClassDescription( this, className, baseName );
}

CClassDescription* CSymbolTable::LookUpClass( const CSymbol* className ) {
	map< const CSymbol*, CClassDescription* >::iterator it = classes.find( className );
	if ( it == classes.end() )
		return NULL;
	return it->second;
}

CClassDescription::CClassDescription( CSymbolTable* _symbolTable, const CSymbol* _name ) 
	: symbolTable( _symbolTable), name( _name )
{
	baseClass = 0;
}

CClassDescription::CClassDescription( CSymbolTable* _symbolTable, const CSymbol* _name, const CSymbol* _base ) 
	: symbolTable( _symbolTable ), name( _name ), baseClass( _base ) {}

CVarDescription* CClassDescription::AddField( const CSymbol* _name, CTypeInfo* _type ) {
	map< const CSymbol*, CVarDescription* >::iterator it = fields.find( _name );
	if ( it != fields.end() )
		return NULL;
	return fields[_name] = new CVarDescription( _name, _type );
}

CMethodDescription* CClassDescription::AddMethod( const CSymbol* _name, CTypeInfo* _type ) {
	map< const CSymbol*, CMethodDescription* >::iterator it = methods.find( _name );
	if ( it != methods.end() )
		return NULL;
	return methods[_name] = new CMethodDescription( this, _name, _type );
}

CVarDescription* CClassDescription::LookUpField( const CSymbol* field ) {
	map< const CSymbol*, CVarDescription* >::iterator it = fields.find( field );
	if ( it == fields.end() )
		return NULL;
	return it->second;
}

CMethodDescription* CClassDescription::LookUpMethod( const CSymbol* method ) {
	map< const CSymbol*, CMethodDescription* >::iterator it = methods.find( method );
	if ( it == methods.end() )
		return NULL;
	return it->second;
}

CVarDescription* CClassDescription::LookUp( const CSymbol* var ) {
	CVarDescription* var1 = LookUpField( var );

	if ( var1 == NULL )
	{
		if ( baseClass != 0 )
		{
			return symbolTable->LookUpClass( baseClass )->LookUp( var );
		}
		else
			return NULL;
	}
	else
		return var1;
}

CVarDescription::CVarDescription( const CSymbol* _name, CTypeInfo* _type ) : name( _name ), type( _type ) {}

const CSymbol* CVarDescription::GetName() {
	return name;
}

const CSymbol* CVarDescription::GetType() {
	return type->GetType();
}

CMethodDescription::CMethodDescription( CClassDescription* _currentClass, const CSymbol* _name, CTypeInfo* _returnType ) 
	: currentClass( _currentClass ), name( _name ), returnType( _returnType ) {}

CVarDescription* CMethodDescription::AddPapam( CVarDescription* param ) {
	map< const CSymbol*, CVarDescription* >::iterator it = params.find( param->GetName() );
	if ( it != params.end() )
		return NULL;
	return params[param->GetName()] = new CVarDescription( param->GetName(), new CTypeInfo( param->GetType() ) );
}

CVarDescription* CMethodDescription::AddLocal( CVarDescription* local ) {
	map< const CSymbol*, CVarDescription* >::iterator it = locals.find( local->GetName() );
	if ( it != locals.end() )
		return NULL;
	return locals[local->GetName()] = new CVarDescription( local->GetName(), new CTypeInfo( local->GetType() ) );
}

const CSymbol* CMethodDescription::GetName() {
	return name;
}

const CSymbol* CMethodDescription::GetType() {
	return returnType->GetType();
}

CVarDescription* CMethodDescription::LookUpParam( const CSymbol* param ) {
	map< const CSymbol*, CVarDescription* >::iterator it = params.find( param );
	if ( it == params.end() )
		return NULL;
	return it->second;
}

CVarDescription* CMethodDescription::LookUpLocal( const CSymbol* local ) {
	map< const CSymbol*, CVarDescription* >::iterator it = locals.find( local );
	if ( it == locals.end() )
		return NULL;
	return it->second;
}

CVarDescription* CMethodDescription::LookUp( const CSymbol* variable ) {
	CVarDescription* var1 = LookUpLocal( variable );
	CVarDescription* var2 = LookUpParam( variable );
	CVarDescription* var3 = currentClass->LookUp( variable );
	if ( var1 == NULL )
	{
		if ( var2 == NULL )
		{
			if ( var3 == NULL )
				return NULL;
			else
				return var3;
		}
		else
			return var2;
	}
	else
		return var1;
}