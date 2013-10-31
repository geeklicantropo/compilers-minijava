#include <map>
#include <string.h>
using namespace std;


class CSymbol {
public:
	CSymbol(string);
	string getString();
	static const CSymbol* CSymbolGet( string );
private:
	string s;
	static struct comparer {
	public:
		bool operator() ( const string&, const string& );
	};
	static map<string, CSymbol*, comparer> storage;
};