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
	inline vector<CString> Tokenize(CString Text, CString Separators)
	{
		vector<CString>	Output;
		int	iPos = 0;
		do
		{
			Output.push_back(Text.Tokenize(Separators, iPos));
		} while (Output.back().GetLength());
		
		Output.pop_back();
		return Output;
	}

	// Translates string into selected language.
	CString TranslateText(CString Text);
	
	// Translates an isolated statement.
	CString TranslateStatement(CString Statement);

	// Translates a single word
	inline CString	TranslateWord(CString Word);

	map<CString, CString>	m_Words;
	vector<CString>			m_NoteWords;
	stack<CString>			m_Context;
};

