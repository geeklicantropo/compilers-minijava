#include "CodeGeneration.h"
#include <assert.h>
#include <string>

using namespace CodeGeneration;

const Temp::CTemp* IInstruction::getAt( const Temp::CTempList* list, int i ) const
{
	if( i == 0 ) 
		return list->Temp();
    else 
		return getAt( list->Next(), i - 1 );
}

const Temp::CLabel* IInstruction::getAt( const Temp::CLabelList* list, int i ) const 
{
	if( i == 0 ) 
		return list->Label();
	else 
		return getAt( list->Next(), i - 1 );	
}

string IInstruction::Format( const Temp::CTempMap* m ) const
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
				result.append( m->TempMap( getAt( src, n ) ));
				break;
			case 'd':
				n = atoi( (const char*)asmCode[++i] );
				result.append( m->TempMap( getAt( dst, n ) ));
				break;
			case 'j': 
				n = atoi( (const char*)asmCode[++i] );
				result.append( getAt(list, n)->Name() );
				break;
			case '`': 
				result.append("`"); 
				break;
			default:
				assert( false );
				printf( "bad Assem format" );
		   }
		   else result.append( (const char*) asmCode[i] );
	}

    return result;
}

const IInstruction* IInstructionList::GetInstr() const
{
	return instr;
}

IInstructionList* IInstructionList::GetNext() const
{
	return next;
}

void IInstructionList::SetInstr( const IInstruction* i )
{
	instr = i;
}

void IInstructionList::SetNext( IInstructionList* n )
{
	next = n;
}

CCodeGenerator::CCodeGenerator( const CFrame* fr, const IRTree::IStatement* tr ) :
	tree( tr ), frame( fr )
{
	head = last = 0;
	munchStm( tr );
}

Temp::CTempList* CCodeGenerator::munchArgs( const IRTree::CExpList* args )
{
	if( args == 0 )
		return 0;

	Temp::CTemp* r = new Temp::CTemp();
	IRTree::CMove* move = new IRTree::CMove( new IRTree::CTemp( r ), args->GetExp() );
	munchStm( move );
	return new Temp::CTempList( r, munchArgs( args->GetNext() ) );
}

void CCodeGenerator::munchStm( const IRTree::IStatement* stm )
{
	const IRTree::CMove* move = dynamic_cast<const IRTree::CMove*>( stm );
	const IRTree::CSeq* seq = dynamic_cast<const IRTree::CSeq*>( stm );
	const IRTree::CLabel* label = dynamic_cast<const IRTree::CLabel*>( stm );
	const IRTree::CJump* jump = dynamic_cast<const IRTree::CJump*>( stm );
	const IRTree::CCJump* cjump = dynamic_cast<const IRTree::CCJump*>( stm );
	const IRTree::CExp* exp = dynamic_cast<const IRTree::CExp*>( stm );
	
	if( move != 0 ) {
		munchStm( move );
	} else if( seq != 0 ) {
		munchStm( seq );
	} else if( label != 0 ) {
		munchStm( label );
	} else if( jump != 0 ) {
		munchStm( jump );
	} else if( cjump != 0 ) {
		munchStm( cjump );
	} else if( exp != 0 ) {
		munchStm( exp );
	} else {
		assert( false );
	}
}

const Temp::CTemp* CCodeGenerator::munchExp( const IRTree::IExpression* exp )
{
	const IRTree::CMem* mem = dynamic_cast<const IRTree::CMem*>( exp );
	const IRTree::CBinOp* binop = dynamic_cast<const IRTree::CBinOp*>( exp );
	const IRTree::CConst* c = dynamic_cast<const IRTree::CConst*>( exp );
	const IRTree::CTemp* temp = dynamic_cast<const IRTree::CTemp*>( exp );
	const IRTree::CName* name = dynamic_cast<const IRTree::CName*>( exp );

	if( mem != 0 ) {
		return munchExp( mem );
	} else if( binop != 0 ) {
		return munchExp( binop );
	} else if( c != 0 ) {
		return munchExp( c );
	} else if( temp != 0 ) {
		return munchExp( temp );
	} else if( name != 0 ) {
		return munchExp( name );
	} else {
		assert( false );
		return 0;
	}
}

void CCodeGenerator::emit( IInstruction* instr )
{
	IInstructionList* newElement = new IInstructionList( instr, 0 );
	if( last != 0 ) {
		last->SetNext( newElement );
		last = newElement;
	} else {
		last = newElement;
		head = newElement;
	}
}

void CCodeGenerator::munchStm( const IRTree::CMove* stm )
{
}

void CCodeGenerator::munchStm( const IRTree::CSeq* stm )
{
}

void CCodeGenerator::munchStm( const IRTree::CLabel* stm )
{
	emit( new CodeGeneration::CLabel( stm->GetLabel()->Name() + ":\n", stm->GetLabel() ) );
}

void CCodeGenerator::munchStm( const IRTree::CJump* stm )
{
}

void CCodeGenerator::munchStm( const IRTree::CCJump* stm )
{
}

void CCodeGenerator::munchStm( const IRTree::CExp* stm )
{
}

const Temp::CTemp* CCodeGenerator::munchExp( const IRTree::CMem* exp ) 
{
	return 0;
}

const Temp::CTemp* CCodeGenerator::munchExp( const IRTree::CBinOp* exp ) 
{
	return 0;
}

const Temp::CTemp* CCodeGenerator::munchExp( const IRTree::CConst* exp ) 
{
	return 0;
}

const Temp::CTemp* CCodeGenerator::munchExp( const IRTree::CTemp* exp ) 
{
	return 0;
}

const Temp::CTemp* CCodeGenerator::munchExp( const IRTree::CName* exp ) 
{
	return 0;
}

