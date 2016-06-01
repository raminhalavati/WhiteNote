#pragma once
#include <vector>
#include <stack>
#include <map>
using namespace std;

class CTranslator
{
public:
	CTranslator();
	~CTranslator();
	CString	m_Language;

	void SetLanguage(CString Language);

	// Tokenizes given string into a vector of strings
	inline vector<CStringA> Tokenize(CStringA Text, CStringA Separators)
	{
		vector<CStringA>	Output;
		int	iPos = 0;
		do
		{
			Output.push_back(Text.Tokenize(Separators, iPos));
		} while (Output.back().GetLength());
		
		Output.pop_back();
		return Output;
	}

	// Translates string into selected language.
	CString TranslateText(CStringA Text);
	
	// Translates an isolated statement.
	CString TranslateStatement(CStringA Statement);

	// Translates a single word
	inline CString	TranslateWord(CStringA Word);

	map<CStringA, CString>	m_Words;
	stack<CStringA>			m_Context;
};

