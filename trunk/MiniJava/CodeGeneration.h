#pragma once
#include <string>
#include <Temp.h>

using namespace std;

namespace CodeGeneration {
	
	class CTargets 
	{
	public:
		CTargets( const Temp::CLabelList* list ) {
			labels = list;
		}
		const Temp::CLabelList* GetLabels() const {
			return labels; 
		}
	private:
		const Temp::CLabelList* labels;
	};

	class IInstruction
	{
	public:
		virtual const Temp::CTempList* UsedVars() const = 0;
		virtual const Temp::CTempList* DefinedVars() const = 0;
		virtual const CTargets* JumpTargets() const = 0;
		string Format( const Temp::CTempMap* varsMapping ) const;
	private:
		const Temp::CTemp* nthTemp(const Temp::CTempList* list, int i) const ;
		const Temp::CLabel* nthLabel(const Temp::CLabelList* list, int i) const;
		string asmCode;
	};

	class IInstructionList 
	{
	public:
		IInstructionList ( const IInstruction* _head, const IInstructionList* _tail ) : head( _head ), tail( _tail ) {}
	private:
		const IInstruction* head;
		const IInstructionList* tail;
	};

	class LABEL : public IInstruction
	{
	public:
		LABEL( const string _assem, const Temp::CLabel* _lable ) : asmCode( _assem ), lable( _lable ) {}
		virtual const Temp::CTempList* UsedVars() const { return 0; }
		virtual const Temp::CTempList* DefinedVars() const { return 0; }
		virtual const CTargets* JumpTargets() const { return 0; }
	private:
		const string asmCode;
		const Temp::CLabel* lable;
	};

	class MOVE : public IInstruction
	{
	public:
		MOVE(string _assem, const Temp::CTemp* _dst, const Temp::CTemp* _src): asmCode( _assem ), dst( _dst ), src( _src ) {}
		virtual const Temp::CTempList* UsedVars() const { return new Temp::CTempList(src, 0); }
		virtual const Temp::CTempList* DefinedVars() const { return new Temp::CTempList(dst, 0); }
		virtual const CTargets* JumpTargets() const { return 0; }
	private:
		string asmCode;
		const Temp::CTemp* dst;
		const Temp::CTemp* src;
	};

	class OPER : public IInstruction
	{
	public:
		OPER( string _assem, const Temp::CTempList* _dst, const Temp::CTempList* _src ): asmCode( _assem ), dst( _dst ), src( _src ), jump(0) {}
		OPER( string _assem, const Temp::CTempList* _dst, const Temp::CTempList* _src, const Temp::CLabelList* _jump ) : asmCode( _assem ), dst( _dst ), src( _src ), jump(new CTargets( _jump )) {}
		virtual const Temp::CTempList* UsedVars() const { return src; }
		virtual const Temp::CTempList* DefinedVars() const { return dst; }
		virtual const CTargets* JumpTargets() const { jump; }
	private:
		string asmCode;
		const Temp::CTempList* dst;
		const Temp::CTempList* src;
		const CTargets* jump;
	};
}