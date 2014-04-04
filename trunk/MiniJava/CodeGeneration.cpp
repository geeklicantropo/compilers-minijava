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

string IInstruction::GetAssemblerCode() const
{
	return asmCode;
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
	reverseList();
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
	const IRTree::CMem* dstMem = dynamic_cast<const IRTree::CMem*>( stm->GetDst() );
	const IRTree::CMem* srcMem = dynamic_cast<const IRTree::CMem*>( stm->GetSrc() );
	const IRTree::CTemp* temp = dynamic_cast<const IRTree::CTemp*>( stm->GetDst() );
	const IRTree::CCall* call = dynamic_cast<const IRTree::CCall*>( stm->GetSrc() );
	if( dstMem != 0 ) {
		const IRTree::CBinOp* binop = dynamic_cast<const IRTree::CBinOp*>( dstMem->GetExp() );
		const IRTree::CConst* c = dynamic_cast<const IRTree::CConst*>( dstMem->GetExp() );
		if( binop != 0 ) {
			const IRTree::CConst* right = dynamic_cast<const IRTree::CConst*>( binop->GetRight() );
			const IRTree::CConst* left = dynamic_cast<const IRTree::CConst*>( binop->GetLeft() );
			if( binop->GetBinOp() == IRTree::TBinOp::PLUS && right != 0 ) {
				//MOVE(MEM(BINOP(PLUS, a, CONST)), d)
				string s = "STORE M['s0+" + to_string(right->GetValue()) + "] <- 's1\n";
				Temp::CTempList* list = new Temp::CTempList( munchExp( binop->GetLeft() ),
					new Temp::CTempList( munchExp( stm->GetSrc() ), 0 ) );
				emit( new COper( s, 0, list ) );
			} else if( binop->GetBinOp() == IRTree::TBinOp::PLUS && left != 0 ) {
				//MOVE(MEM(BINOP(PLUS, CONST, a)), d)
				string s = "STORE M['s0+" + to_string(left->GetValue()) + "] <- 's1\n";
				Temp::CTempList* list = new Temp::CTempList( munchExp( binop->GetRight() ),
					new Temp::CTempList( munchExp( stm->GetSrc() ), 0 ) );
				emit( new COper( s, 0, list ) );
			}
		} else if( srcMem != 0 ) {
			//MOVE(MEM(a), MEM(a))
			string s = "MOVE M['s0] <- M['s1]\n";
			Temp::CTempList* list = new Temp::CTempList( munchExp( dstMem->GetExp() ),
				new Temp::CTempList( munchExp( srcMem->GetExp() ), 0 ) );
			emit( new COper( s, 0, list ) );
		} else if( c != 0 ) {
			//MOVE(MEM(CONST), d)
			string s = "STORE M[r0+" + to_string( c->GetValue() ) + "] <- 's0\n";
			emit( new COper( s, 0, new Temp::CTempList( munchExp( stm->GetSrc() ), 0 ) ) );
		} else {
			//MOVE(MEM(a),d)
			string s = "STORE M['s0] <- 's1\n";
			Temp::CTempList* list = new Temp::CTempList( munchExp( dstMem->GetExp() ),
				new Temp::CTempList( munchExp( stm->GetSrc() ), 0 ) );
			emit( new COper( s, 0, list ) );
		}
	} else if( temp != 0 && call != 0 ) {
		//MOVE(TEMP, CALL )
		const Temp::CTemp* t = munchExp( call->GetExp() );
		Temp::CTempList* args = munchArgs( call->GetArgs() );
		string s = "CALL 's0\n";
		emit( new COper( s, 0, new Temp::CTempList( t, args ) ) );
		s = "ADD 'd0 <- 'r0 + rv\n";
		emit( new COper( s, new Temp::CTempList( temp->GetTemp(), 0 ), 0 ) );
	} else if( temp != 0 ) {
		//MOVE(TEMP,e)
		string s = "ADD 'd0 <- 's0 + r0\n";
		emit( new COper( s, new Temp::CTempList( temp->GetTemp(), 0 ), new Temp::CTempList( munchExp( stm->GetSrc() ), 0 ) ) );
	} else {
		assert( false );
	}
}

void CCodeGenerator::munchStm( const IRTree::CSeq* stm )
{
	munchStm( stm->GetLeft() );
	munchStm( stm->GetRight() );
}

void CCodeGenerator::munchStm( const IRTree::CLabel* stm )
{
	emit( new CodeGeneration::CLabel( stm->GetLabel()->Name() + ":\n", stm->GetLabel() ) );
}

void CCodeGenerator::munchStm( const IRTree::CJump* stm )
{
	emit( new CodeGeneration::COper( "JMP 'l0\n", 0, 0, stm->GetTargets() ) );
}

void CCodeGenerator::munchStm( const IRTree::CCJump* stm )
{
	string name;
    Temp::CTemp* resultExp = new Temp::CTemp();
	IRTree::IStatement* resultExpTree = new IRTree::CMove( new IRTree::CTemp( resultExp ), 
		new IRTree::CBinOp( IRTree::TBinOp::MINUS, stm->GetLeft(), stm->GetRight() ) );
    munchStm( resultExpTree );
    
    switch( stm->GetRelop() ) {
		case IRTree::TCJump::EQ:
			name = "CJmpEQ "; 
			break;
		case IRTree::TCJump::NE:
			name = "CJmpNE ";
			break;
		case IRTree::TCJump::GT:
			name = "CJmpGT ";
			break;
		case IRTree::TCJump::LT:
			name = "CJmpLT ";
			break;
		case IRTree::TCJump::LE: 
			name = "CJmpLE ";
			break;
		case IRTree::TCJump::GE:
			name = "CJmpGE ";
			break;
		case IRTree::TCJump::ULT: 
			name = "CJmpULT ";
			break;
		case IRTree::TCJump::ULE:
			name = "CJmpULE ";
			break;
		case IRTree::TCJump::UGT: 
			name = "CJmpUGT ";
			break;
		case IRTree::TCJump::UGE:
			name = "CJmpUGE ";
			break;
		default:
			assert(false);
    }
    name += "'s0 'l0\n";
    Temp::CLabelList* possibleLabels = new Temp::CLabelList( stm->GetTrueLabel(), 
		new Temp::CLabelList( stm->GetFalseLabel(), 0) );
	emit( new CodeGeneration::COper( name, 0, new Temp::CTempList( resultExp, 0 ), possibleLabels ) );
}

void CCodeGenerator::munchStm( const IRTree::CExp* stm )
{
	const IRTree::CCall* exp = dynamic_cast<const IRTree::CCall*>( stm->GetExp() );
	if( exp != 0 ) {
		const Temp::CTemp* r = munchExp( exp->GetExp() ); 
		Temp::CTempList* l = munchArgs( exp->GetArgs() );
		emit( new CodeGeneration::COper( "CALL 's0\n", 0, new const Temp::CTempList( r, l ) ) );
		return;
	}
	assert(false);
}

const Temp::CTemp* CCodeGenerator::munchExp( const IRTree::CMem* exp ) 
{
	const IRTree::CBinOp* binExp= dynamic_cast<const IRTree::CBinOp*>( exp->GetExp() );
	const IRTree::CConst* constExp = dynamic_cast<const IRTree::CConst*>( exp->GetExp() );
	if( binExp != 0 ) {
		const IRTree::CConst* leftExp = dynamic_cast<const IRTree::CConst*>( binExp->GetLeft());
		const IRTree::CConst* rightExp = dynamic_cast<const IRTree::CConst*>( binExp->GetRight());
		if( binExp->GetBinOp() == IRTree::TBinOp::PLUS && leftExp != 0 ) {
			// MEM(BINOP(PLUS,CONST(i),e1))
			Temp::CTemp* r = new Temp::CTemp();
			string name = "LOAD 'd0 <- M['s0+" + to_string( leftExp->GetValue() ) + "]\n";
			emit( new CodeGeneration::COper( name,
				new const Temp::CTempList( r, 0 ), new const Temp::CTempList( munchExp( binExp->GetRight() ), 0) ) );
			return r;
		}

		if( binExp->GetBinOp() == IRTree::TBinOp::PLUS && rightExp != 0 ) {
			//MEM(BINOP(PLUS,e1,CONST(i)))
			Temp::CTemp* r = new Temp::CTemp();
			string name = "LOAD 'd0 <- M['s0+" + to_string( rightExp->GetValue() ) + "]\n";
			emit( new CodeGeneration::COper( name,
				new const Temp::CTempList( r, 0 ), new const Temp::CTempList( munchExp( binExp->GetRight() ), 0) ) );
			return r;
		}
	}
	if( constExp != 0 ) {
		Temp::CTemp* r = new Temp::CTemp();
		string name = "LOAD 'd0 <- M['r0+" + to_string( constExp->GetValue() ) + "]\n";
		emit( new CodeGeneration::COper( name, new Temp::CTempList(r, 0),  0) );
		return r;
	}

	Temp::CTemp* r = new Temp::CTemp();
	emit( new CodeGeneration::COper( "LOAD 'd0 <- M['s0+0]\n",
		new Temp::CTempList( r, 0 ), new Temp::CTempList( munchExp( exp->GetExp() ), 0) ) );
	return r;
}

const Temp::CTemp* CCodeGenerator::munchExp( const IRTree::CBinOp* exp ) 
{
	if( exp->GetBinOp() == IRTree::TBinOp::PLUS ) {
		const IRTree::CConst* leftExp = dynamic_cast<const IRTree::CConst*>( exp->GetLeft() );
		const IRTree::CConst* rightExp = dynamic_cast<const IRTree::CConst*>( exp->GetRight() );
		if( leftExp != 0 ) {
			Temp::CTemp* r = new Temp::CTemp();
			string s = "ADDI 'd0 <- 's0+";
			s += to_string( leftExp->GetValue() );
			s += "\n";
			emit( new CodeGeneration::COper( s,
				new Temp::CTempList( r, 0 ), new Temp::CTempList( munchExp(exp->GetRight() ), 0) ) );
			return r;
		}

		if( rightExp != 0 ) {
			Temp::CTemp* r = new Temp::CTemp();
			string s = "ADDI 'd0 <- 's0+";
			s += to_string( rightExp->GetValue() );
			s += "\n";
			emit( new CodeGeneration::COper( s,
			new Temp::CTempList(r, 0), new Temp::CTempList( munchExp( exp->GetLeft() ), 0) ) );
			return r;
		}

		Temp::CTemp* r = new Temp::CTemp();
		emit( new CodeGeneration::COper("ADD 'd0 <- 's0+'s1\n",
		new Temp::CTempList(r, 0), new Temp::CTempList( munchExp(exp->GetLeft() ), new Temp::CTempList( munchExp(exp->GetRight() ), 0) ) ) );
		return r;
	}

	if( exp->GetBinOp() == IRTree::TBinOp::MINUS ) {
		const IRTree::CConst* leftExp = dynamic_cast<const IRTree::CConst*>( exp->GetLeft() );
		const IRTree::CConst* rightExp = dynamic_cast<const IRTree::CConst*>( exp->GetRight() );
		if( leftExp != 0 ) {
			Temp::CTemp* r = new Temp::CTemp();
			string s = "SUBI 'd0 <- 's0-";
			s += to_string( leftExp->GetValue() );
			s += "\n";
			emit( new CodeGeneration::COper( s,
				new Temp::CTempList( r, 0 ), new Temp::CTempList( munchExp(exp->GetRight() ), 0) ) );
			return r;
		}

		if( rightExp != 0 ) {
			Temp::CTemp* r = new Temp::CTemp();
			string s = "SUBI 'd0 <- 's0-";
			s += to_string( rightExp->GetValue() );
			s += "\n";
			emit( new CodeGeneration::COper( s,
			new Temp::CTempList( r, 0 ), new Temp::CTempList( munchExp( exp->GetLeft() ), 0) ) );
			return r;
		}

		Temp::CTemp* r = new Temp::CTemp();
		emit( new CodeGeneration::COper("SUB 'd0 <- 's0-'s1\n",
		new const Temp::CTempList( r, 0 ), new Temp::CTempList( munchExp(exp->GetLeft() ), new Temp::CTempList( munchExp(exp->GetRight() ), 0) ) ) );
		return r;
	}

	if( exp->GetBinOp() == IRTree::TBinOp::MUL ) {
		Temp::CTemp* r = new Temp::CTemp();
		emit( new CodeGeneration::COper("MUL 'd0 <- 's0*'s1\n",
		new const Temp::CTempList( r, 0 ), new Temp::CTempList( munchExp(exp->GetLeft() ), new Temp::CTempList( munchExp(exp->GetRight() ), 0) ) ) );
		return r;
	}

	if( exp->GetBinOp() == IRTree::TBinOp::DIV ) {
		Temp::CTemp* r = new Temp::CTemp();
		emit( new CodeGeneration::COper("DIV 'd0 <- 's0/'s1\n",
		new const Temp::CTempList( r, 0 ), new Temp::CTempList( munchExp(exp->GetLeft() ), new Temp::CTempList( munchExp(exp->GetRight() ), 0) ) ) );
		return r;
	}
}

const Temp::CTemp* CCodeGenerator::munchExp( const IRTree::CConst* exp ) 
{
	Temp::CTemp* r = new Temp::CTemp();
	string name = "ADDI 'd0 <- 'r0+" + to_string( exp->GetValue() ) + "\n";
	emit( new CodeGeneration::COper( name, new Temp::CTempList( r, 0 ), 0) );
	return r;
}

const Temp::CTemp* CCodeGenerator::munchExp( const IRTree::CTemp* exp ) 
{
	return exp->GetTemp();
}

const Temp::CTemp* CCodeGenerator::munchExp( const IRTree::CName* exp ) 
{
	return new Temp::CTemp( CSymbol::CSymbolGet( exp->GetLabel()->Name() ) );
}

void CCodeGenerator::reverseList()
{
	IInstructionList* curr = head;
	IInstructionList* prev = 0;
	while( curr != 0 ) {
		IInstructionList* next = curr->GetNext();
		curr->SetNext( prev );
		prev = curr;
		curr = next;
	}
	last = head;
	head = prev;
}

IInstructionList* CCodeGenerator::GetHead() const
{
	return head;
}