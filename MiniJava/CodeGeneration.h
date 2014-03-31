#pragma once
#include <string>
#include "Temp.h"
#include "CodeFragment.h"

using namespace std;

namespace CodeGeneration {
	
	class CTargets  {
	public:
		CTargets( const Temp::CLabelList* list ) : labels( list ) {}
		const Temp::CLabelList* GetLabels() const
		{
			return labels; 
		}
	private:
		const Temp::CLabelList* labels;
	};

	class IInstruction {
	public:
		virtual const Temp::CTempList* UsedVars() const = 0;
		virtual const Temp::CTempList* DefinedVars() const = 0;
		virtual const CTargets* JumpTargets() const = 0;
		string Format( const Temp::CTempMap* varsMapping ) const;
	private:
		const Temp::CTemp* getAt(const Temp::CTempList* list, int i) const ;
		const Temp::CLabel* getAt(const Temp::CLabelList* list, int i) const;
		string asmCode;
	};

	class IInstructionList {
	public:
		IInstructionList( const IInstruction* i, IInstructionList* n ) : instr( i ), next( n ) {}
		const IInstruction* GetInstr() const;
		IInstructionList* GetNext() const;
		void SetInstr( const IInstruction* i );
		void SetNext( IInstructionList* n );
	private:
		const IInstruction* instr;
		IInstructionList* next;
	};

	class CLabel : public IInstruction {
	public:
		CLabel( const string _assem, const Temp::CLabel* _lable ) : asmCode( _assem ), lable( _lable ) {}
		virtual const Temp::CTempList* UsedVars() const { return 0; }
		virtual const Temp::CTempList* DefinedVars() const { return 0; }
		virtual const CTargets* JumpTargets() const { return 0; }
	private:
		const string asmCode;
		const Temp::CLabel* lable;
	};

	class CMove : public IInstruction {
	public:
		CMove(string _assem, const Temp::CTemp* _dst, const Temp::CTemp* _src): asmCode( _assem ), dst( _dst ), src( _src ) {}
		virtual const Temp::CTempList* UsedVars() const { return new Temp::CTempList(src, 0); }
		virtual const Temp::CTempList* DefinedVars() const { return new Temp::CTempList(dst, 0); }
		virtual const CTargets* JumpTargets() const { return 0; }
	private:
		string asmCode;
		const Temp::CTemp* dst;
		const Temp::CTemp* src;
	};

	class COper : public IInstruction {
	public:
		COper( string _assem, const Temp::CTempList* _dst, const Temp::CTempList* _src ): asmCode( _assem ), dst( _dst ), src( _src ), jump(0) {}
		COper( string _assem, const Temp::CTempList* _dst, const Temp::CTempList* _src, const Temp::CLabelList* _jump ) : asmCode( _assem ), dst( _dst ), src( _src ), jump(new CTargets( _jump )) {}
		virtual const Temp::CTempList* UsedVars() const { return src; }
		virtual const Temp::CTempList* DefinedVars() const { return dst; }
		virtual const CTargets* JumpTargets() const { jump; }
	private:
		string asmCode;
		const Temp::CTempList* dst;
		const Temp::CTempList* src;
		const CTargets* jump;
	};

	class CCodeGenerator {
	public:
		CCodeGenerator( const CFrame* fr, const IRTree::IStatement* tr );
		
	private:
		const CFrame* frame;
		const IRTree::IStatement* tree;
		IInstructionList* head;
		IInstructionList* last;

		void munchStm( const IRTree::IStatement* stm );
		const Temp::CTemp* munchExp( const IRTree::IExpression* exp );

		void munchStm( const IRTree::CMove* stm );
		void munchStm( const IRTree::CSeq* stm );
		void munchStm( const IRTree::CLabel* stm );
		void munchStm( const IRTree::CJump* stm );
		void munchStm( const IRTree::CCJump* stm );
		void munchStm( const IRTree::CExp* stm );
	
		const Temp::CTemp* munchExp( const IRTree::CMem* exp );
		const Temp::CTemp* munchExp( const IRTree::CBinOp* exp );
		const Temp::CTemp* munchExp( const IRTree::CConst* exp );
		const Temp::CTemp* munchExp( const IRTree::CTemp* exp );
		const Temp::CTemp* munchExp( const IRTree::CName* exp );

		void emit( IInstruction* instr );

		Temp::CTempList* munchArgs( const IRTree::CExpList* args );

	};
}