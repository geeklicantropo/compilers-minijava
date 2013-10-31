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
		struct comparer {
		public:
			bool operator() ( const string&, const string& );
		};
		map<string, CSymbol*, comparer> storage;
	};

};