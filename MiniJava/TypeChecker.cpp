#include "TypeChecker.h"


int Visit( const CProgram* n ) {return 0;}
int Visit( const CMainClass* n ) {return 0;}
int Visit( const CClassDeclareStar* n ) {return 0;}
int Visit( const CClassDeclare* n ) {return 0;}
int Visit( const CClassDeclareExtends* n ) {return 0;}
int Visit( const CVarDeclareStar* n ) {return 0;}
int Visit( const CVarDeclare* n ) {return 0;}
int Visit( const CMethodDeclare* n ) {return 0;}
int Visit( const CMethodDeclareStar* n ) {return 0;}
int Visit( const CFormalList* n ) {return 0;}
int Visit( const CFormalRestStar* n ) {return 0;}
int Visit( const CStatement* n ) {return 0;}
int Visit( const CStatementStar* n ) {return 0;}
int Visit( const CStatementIf* n ) {return 0;}
int Visit( const CStatementWhile* n ) {return 0;}
int Visit( const CStatementSysOut* n ) {return 0;}
int Visit( const CStatementAssignment* n ) {return 0;}
int Visit( const CStatementArrayAssignment* n ) {return 0;}
int Visit( const CExpressionBinOp* n ) {return 0;}
int Visit( const CExpressionArray* n ) {return 0;}
int Visit( const CExpressionLength* n ) {return 0;}
int Visit( const CExpressionCallMethod* n ) {return 0;}
int Visit( const CExpressionNumber* n ) {return 0;}
int Visit( const CExpressionBool* n ) {return 0;}
int Visit( const CExpressionVar* n ) {return 0;}
int Visit( const CExpressionThis* n ) {return 0;}
int Visit( const CExpressionNewInt* n ) {return 0;}
int Visit( const CExpressionNewId* n ) {return 0;}
int Visit( const CExpressionNegation* n ) {return 0;}
int Visit( const CExpression* n ) {return 0;}
int Visit( const CExpList* n ) {return 0;}
int Visit( const CExpListNext* n ) {return 0;}