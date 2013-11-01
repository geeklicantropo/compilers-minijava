#include "Interpreter.h"
#include "miniJava.h"
#include <iostream>

int CInterpreter::Visit( const CProgram* n )
{
	n->GetMainClass()->Accept( this );
	if ( n->GetClassDeclareStar() != NULL ) n->GetClassDeclareStar()->Accept( this );
	return 0;
}

int CInterpreter::Visit( const CMainClass* n )  
{ 
	std::cout << "class " << n->GetId()->getString() << "{" << std::endl << "public static void main( String[] " << n->GetArgsId()->getString() << ")" << std::endl << "{" << std::endl << "\t"; 
	n->GetStatement()->Accept( this );
	std::cout << std::endl << "}" << std::endl << "}" << std::endl;
	return 0; 
}

int CInterpreter::Visit( const CClassDeclareStar* n )  {
	if ( n->GetClassDeclareStar() != NULL ) n->GetClassDeclareStar()->Accept( this );
	n->GetClassDeclare()->Accept( this );
	return 0; 
}

int CInterpreter::Visit( const CClassDeclare* n )  { 
	std::cout << "class" << " " << n->GetId()->getString() << std::endl << "{" << std::endl << "\t";
	if ( n->GetVarDeclareStar() != NULL ) n->GetVarDeclareStar()->Accept( this );
	if ( n->GetmethodDeclareStar() != NULL ) n->GetmethodDeclareStar()->Accept( this );
	std::cout << std::endl << "}" << std::endl;
	return 0; 
}

int CInterpreter::Visit( const CClassDeclareExtends* n )  
{
	std::cout << "class" << " " << n->GetId()->getString().c_str() << "extends" << n->GetExtendsId()->getString() << std::endl << "{" << std::endl << "\t";
	if( n->GetVarDeclareStar() != 0) n->GetVarDeclareStar()->Accept( this );
	if( n->GetMethodDeclareStar() != 0)  n->GetMethodDeclareStar()->Accept( this );
	std::cout << std::endl << "}" << std::endl;
	return 0; 
}

int CInterpreter::Visit( const CVarDeclareStar* n )  
{
	if( n->GetVarDeclareStar() != 0 ) n->GetVarDeclareStar()->Accept( this );
	n->GetVarDeclare()->Accept( this );
	return 0; 
}	

int CInterpreter::Visit( const CVarDeclare* n )  
{ 
	TDataTypes type = n->GetDataType();
	switch( type ) {
		case INT:
			std::cout << "int ";
			break;
		case BOOLEAN:
			std::cout << "boolean ";
			break;
		case INTARRAY:
			std::cout << "int[] ";
			break;
	}
	std::cout << n->GetId()->getString() << ";" << std::endl;
	return 0; 
}

int CInterpreter::Visit( const CMethodDeclare* n )  
{ 
	TDataTypes type = n->GetDataType();
	std::cout << "public ";
	switch( type ) {
		case INT:
			std::cout << "int ";
			break;
		case BOOLEAN:
			std::cout << "boolean ";
			break;
		case INTARRAY:
			std::cout << "int[] ";
			break;
	}
	std::cout << n->GetId()->getString();
	std::cout << "(";
	if (n->GetFormalList() != NULL) n->GetFormalList()->Accept( this );
	std::cout << ")" << std::endl << "{" << std::endl << "\t";
	if (n->GetVarDeclareStar() != NULL) n->GetVarDeclareStar()->Accept( this );
	if (n->GetStatementStar() != NULL) n->GetStatementStar()->Accept( this );
	std::cout << "return ";
	n->GetExpression()->Accept( this );
	std::cout << ";" << std::endl << "}";
	return 0; 
}

int CInterpreter::Visit( const CMethodDeclareStar* n ) 
{
	if (n->GetMethodDeclareStar() != NULL) n->GetMethodDeclareStar()->Accept( this );
	n->GetMethodDeclare()->Accept( this );
	return 0; 
}

int CInterpreter::Visit( const CFormalList* n )  
{
	std::cout << n->GetDataType() << " " << n->GetId()->getString() << " ";
	if (n->GetFormalRestStar() != NULL) n->GetFormalRestStar()->Accept( this );
	return 0; 
}

int CInterpreter::Visit( const CFormalRestStar* n )  { 
	std::cout << ", " << n->GetDataType() << " " << n->GetId()->getString() << " ";
	if (n->GetFormalRestStar() != NULL) n->GetFormalRestStar()->Accept( this );
	return 0; 
}

int CInterpreter::Visit( const CStatement* n )  
{ 
	std::cout << "{ ";
	if (n->GetStatementStar() != NULL) n->GetStatementStar()->Accept( this );
	std::cout << std::endl << "}";
	return 0; 
}

int CInterpreter::Visit( const CStatementStar* n )  { 
	n->GetStatement()->Accept( this );
	if (n->GetStatementStar() != NULL) n->GetStatementStar()->Accept( this );
	return 0; 
}

int CInterpreter::Visit( const CStatementIf* n )  
{
	std::cout << "if( "; 
	n->GetExpression()->Accept( this );
	std::cout << " ) " << std::endl;
	n->GetStatementIf()->Accept( this );
	std::cout << "else" << std::endl;
	n->GetStatementElse()->Accept( this );
	return 0; 
}

int CInterpreter::Visit( const CStatementWhile* n )  
{ 
	std::cout << "while( ";
	n->GetExpression()->Accept( this );
	std::cout << " ) ";
	n ->GetStatement()->Accept( this );
	return 0; 
}

int CInterpreter::Visit( const CStatementSysOut* n )  { 
	std::cout << "System.out.println( ";
	n->GetExpression()->Accept( this );
	std::cout << " ); " << std::endl;
	return 0; 
}

int CInterpreter::Visit( const CStatementAssignment* n )  
{
	std::cout << n->GetId()->getString() << " = ";
	n->GetExpression()->Accept( this );
	std::cout << ";" << std::endl;
	return 0; 
}

int CInterpreter::Visit( const CStatementArrayAssignment* n )  
{
	//ID '[' Expression ']' '=' Expression ';' {
	std::cout << n->GetId()->getString() << "[";
	n->GetExpressionArray()->Accept( this );
	std::cout << "] = ";
	n->GetExpression()->Accept( this );
	std::cout << ";" << std::endl;
	return 0; 
}

int CInterpreter::Visit( const CExpressionBinOp* n )
{
	n->GetExpressionFirst()->Accept( this );
	TBinaryOperation type =  n->GetOperation();
	switch( type ) {
		case PLUS:
			std::cout << " + ";
			break;
		case MINUS:
			std::cout << " - ";
			break;
		case TIMES:
			std::cout << " * ";
			break;
		case LESS:
			std::cout << " < ";
			break;
		case AND:
			std::cout << " & ";
			break;
	}
	n->GetExpressionSecond()->Accept( this );
	return 0; 
}

int CInterpreter::Visit( const CExpressionArray* n )  
{ 
	n->GetExpression1()->Accept( this );
	std::cout << "[ ";
	n->GetExpression2()->Accept( this );
	std::cout << "]";
	return 0; 
}

int CInterpreter::Visit( const CExpressionLength* n )
{
	n->GetExpression()->Accept( this );
	std::cout << ".length";
	return 0; 
}

int CInterpreter::Visit( const CExpressionCallMethod* n )  
{
	//Expression '.' ID '(' ExpList ')' {
	n->GetExpression()->Accept( this );
	std::cout << "." << n->GetId()->getString() << "( ";
	n->GetExpList()->Accept( this );
	std::cout << " )"; 
	return 0; 
}

int CInterpreter::Visit( const CExpressionNumber* n )  
{ 
	std::cout << n->GetNumber()->getString();
	return 0; 
}

int CInterpreter::Visit( const CExpressionBool* n ) 
{
	std::cout << n->GetValue();
	return 0; 
}

int CInterpreter::Visit( const CExpressionVar* n )  
{
	std::cout << n->GetId()->getString();
	return 0; 
}

int CInterpreter::Visit( const CExpressionThis* n )  
{ 
	std::cout << "this";
	return 0; 
}

int CInterpreter::Visit( const CExpressionNewInt* n )
{ 
	//NEW INT_T '[' Expression ']' {
	std::cout << "new int[ ";
	n->GetExpression()->Accept( this );
	std::cout << " ]"; 
	return 0; 
}

int CInterpreter::Visit( const CExpressionNewId* n ) 
{
	std::cout << "new " << n->GetId()->getString() << "( )";
	return 0; 
}

int CInterpreter::Visit( const CExpressionNegation* n )  
{
	std::cout << "!";
	n->GetExpression()->Accept( this );
	return 0; 
}

int CInterpreter::Visit( const CExpression* n )  
{
	std::cout << "( ";
	n->GetExpression()->Accept( this );
	std::cout << " )"; 
	return 0; 
}

int CInterpreter::Visit( const CExpList* n )  
{ 
	n->GetExpression()->Accept( this );
	return 0; 
}

int CInterpreter::Visit( const CExpListNext* n ) 
{
	n->GetExpression()->Accept( this );
	std::cout << ",";
	n->GetExpList()->Accept( this );
	return 0; 
}