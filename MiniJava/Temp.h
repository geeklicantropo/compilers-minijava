#pragma once
#include "Symbols.h"
#include <string>
#include <set>

using namespace std;
namespace Temp {
	class CLabel
	{
	public:
		CLabel();
		explicit CLabel( string& s );
		explicit CLabel( const CSymbol* s );

		const string Name() const;
	private:
		static int nextUniqueId;
		string name;
	};

	class CLabelList
	{
	public:
		CLabelList(const CLabel* _label, CLabelList* _next) : label(_label), next(_next) {}
		const CLabel* Label() const { return label; }
		CLabelList* Next() const { return next; }
	private:
		const CLabel* label;
		CLabelList* next;
	};

	class CTemp 
	{
	public:
		CTemp();
		explicit CTemp( const CSymbol* symbol );

		const string& Name() const;
		string getName() const { return name; }
		bool operator< ( const CTemp& other ) const { return getName() < other.getName(); }
	private:
		static int nextUniqueId;
		string name;
	};

	class CTempList
	{
	public:
		CTempList(const CTemp* _temp, CTempList* _next) : temp(_temp), next(_next) {}
		const CTemp* Temp() const { return temp; }
		CTempList* Next() const { return next; }
	private:
		const CTemp* temp;
		CTempList* next;
	};

	class CTempMap 
	{
	public:
		CTempMap() {}
		string TempMap( const CTemp* temp ) const {
			return temp->getName();
		}
	};
};