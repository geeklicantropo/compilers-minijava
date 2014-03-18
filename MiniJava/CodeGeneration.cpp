#include "CodeGeneration.h"
#include <string>

const Temp::CTemp* CodeGeneration::IInstruction::nthTemp( const Temp::CTempList* list, int i ) const
{
	if(i == 0) return list->Temp();
    else return nthTemp( list->Next(), i-1 );
}

const Temp::CLabel* CodeGeneration::IInstruction::nthLabel( const Temp::CLabelList* list, int i ) const 
{
	if(i == 0) return list->Label();
	else return nthLabel( list->Next(), i-1 );	
}

string CodeGeneration::IInstruction::Format( const Temp::CTempMap* m ) const
{
	const Temp::CTempList* dst = DefinedVars();
	const Temp::CTempList* src = UsedVars();
    const CTargets* jumps = JumpTargets();
	const Temp::CLabelList* list = jumps->GetLabels();
    string result;
    int length = asmCode.length();
	int n;
    for( int i = 0; i < length; i++ ) {
		if( asmCode[i] == '`')
			switch( asmCode[++i] ) {
			case 's': 
				n = atoi( (const char*)asmCode[++i] );
				result.append( m->TempMap( nthTemp( src, n ) ));
				break;
			case 'd':
				n = atoi( (const char*)asmCode[++i] );
				result.append( m->TempMap( nthTemp( dst, n ) ));
				break;
			case 'j': 
				n = atoi( (const char*)asmCode[++i] );
				result.append( nthLabel(list, n)->Name() );
				break;
			case '`': 
				result.append("`"); 
				break;
			default: 
				printf( "bad Assem format" );
		   }
		   else result.append( (const char*) asmCode[i] );
	}

    return result;
}