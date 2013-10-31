#include <map>
#include <string.h>
using namespace std;

namespace Symbols {

	class CSymbol {
	public:
		CSymbol(string);
		string getString(); 
	private:
		string s;
	};

	class CSymbolStorage {
	public:
		CSymbolStorage();
		const CSymbol* CSymbolGet( string );
	private:
		map<string, CSymbol*> storage;
	};

};