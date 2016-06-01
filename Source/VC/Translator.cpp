#include "stdafx.h"
#include "Translator.h"
#include "Common.h"
#include <fstream>

CTranslator::CTranslator()
{
	m_Language = L"EN";
}


CTranslator::~CTranslator()
{
}

void CTranslator::SetLanguage(CString Language)
{
	m_Language = Language;
	
	m_Words.insert(make_pair("C", L"دو"));
	m_Words.insert(make_pair("D", L"رِ"));
	m_Words.insert(make_pair("E", L"می"));
	m_Words.insert(make_pair("F", L"فا"));
	m_Words.insert(make_pair("G", L"سُل"));
	m_Words.insert(make_pair("A", L"لا"));
	m_Words.insert(make_pair("B", L"سی"));

	m_Words.insert(make_pair("Cn", L"دو_بِکار"));
	m_Words.insert(make_pair("Dn", L"رِ_بِکار"));
	m_Words.insert(make_pair("En", L"می_بِکار"));
	m_Words.insert(make_pair("Fn", L"فا_بِکار"));
	m_Words.insert(make_pair("Gn", L"سُل_بِکار"));
	m_Words.insert(make_pair("An", L"لا_بِکار"));
	m_Words.insert(make_pair("Bn", L"سی_بِکار"));

	m_Words.insert(make_pair("Cb", L"دو_بِمُل"));
	m_Words.insert(make_pair("Db", L"رِ_بِمُل"));
	m_Words.insert(make_pair("Eb", L"می_بِمُل"));
	m_Words.insert(make_pair("Fb", L"فا_بِمُل"));
	m_Words.insert(make_pair("Gb", L"سُل_بِمُل"));
	m_Words.insert(make_pair("Ab", L"لا_بِمُل"));
	m_Words.insert(make_pair("Bb", L"سی_بِمُل"));
	
	m_Words.insert(make_pair("C#", L"دو_دیِز"));
	m_Words.insert(make_pair("D#", L"رِ_دیِز"));
	m_Words.insert(make_pair("E#", L"می_دیِز"));
	m_Words.insert(make_pair("F#", L"فا_دیِز"));
	m_Words.insert(make_pair("G#", L"سُل_دیِز"));
	m_Words.insert(make_pair("A#", L"لا_دیِز"));
	m_Words.insert(make_pair("B#", L"سی_دیِز"));


	m_Words.insert(make_pair("Whole", L"گرد"));
	m_Words.insert(make_pair("Half", L"سفید"));
	m_Words.insert(make_pair("Quarter", L"سیاه"));
	m_Words.insert(make_pair("Eighth", L"چنگ"));
	m_Words.insert(make_pair("Sixteenth", L"دو_لا_چنگ"));
	m_Words.insert(make_pair("Thirty_Second", L"سه_لا_چنگ"));
	m_Words.insert(make_pair("Whole_Bar", L"تمام_میزان"));
	
	m_Words.insert(make_pair("A_Tempo", L"اِتِمپو"));
	m_Words.insert(make_pair("Accel", L"آجلراندو"));
	m_Words.insert(make_pair("Allegro", L"آلِگرو"));
	m_Words.insert(make_pair("Allegrato", L"آلِگرتو"));
	m_Words.insert(make_pair("Dolce", L"دولچه"));
	m_Words.insert(make_pair("Grave", L"گریو"));
	m_Words.insert(make_pair("Gruppetto", L"گروپتو"));
	m_Words.insert(make_pair("Largo", L"لارگو"));
	m_Words.insert(make_pair("Marcato", L"مارکاتو"));
	m_Words.insert(make_pair("Martelato", L"مارتلاتو"));
	m_Words.insert(make_pair("Metso", L"متسو"));
	m_Words.insert(make_pair("Mordent", L"موردنت"));
	m_Words.insert(make_pair("Moderato", L"مودراتو"));
	m_Words.insert(make_pair("Poco_Rall", L"پوکورال"));
	m_Words.insert(make_pair("Poco_Ritardtando", L"پوکوریت"));
	m_Words.insert(make_pair("Rinforzando", L"رینفورزاندو"));
	m_Words.insert(make_pair("Sempre", L"سمپره"));
	m_Words.insert(make_pair("Trill", L"تریل"));

	m_Words.insert(make_pair("Crescendo", L"کرشندو"));
	m_Words.insert(make_pair("Decrescendo", L"دیکرشندو"));
	m_Words.insert(make_pair("Diminuendo", L"دیمینو_اندو"));
	m_Words.insert(make_pair("Legato", L"لِگاتو"));
	m_Words.insert(make_pair("F", L"فورته"));
	m_Words.insert(make_pair("FF", L"فورتیسیمو"));
	m_Words.insert(make_pair("FFF", L"فورته_فورتیسیمو"));
	m_Words.insert(make_pair("FP", L"فورته_پیانو"));
	m_Words.insert(make_pair("FZ", L"فورزانتو"));
	m_Words.insert(make_pair("MF", L"متزو_فورته"));
	m_Words.insert(make_pair("MP", L"متزو_پیانو"));
	m_Words.insert(make_pair("P", L"پیانو"));
	m_Words.insert(make_pair("PP", L"پیانیسیمو"));
	m_Words.insert(make_pair("PPP", L"پیانو_پیانیسیمو"));
	m_Words.insert(make_pair("SFZ", L"اسفورزاندو"));
	m_Words.insert(make_pair("Stacatto", L"استکاتو"));

	m_Words.insert(make_pair("Accord", L"آکورد"));
	m_Words.insert(make_pair("Arpeggiate", L"آرپژ"));
	m_Words.insert(make_pair("Bass", L"باس"));
	m_Words.insert(make_pair("Begin_Repeat", L"دو_نقطه_تکرار_سمت_راست"));
	m_Words.insert(make_pair("Clarinet", L"کلارینت"));
	m_Words.insert(make_pair("Clef_Bass", L"کلید فا"));
	m_Words.insert(make_pair("Clef_Treble", L"کلید سُل"));
	m_Words.insert(make_pair("Dynamic", L"دینامیک"));
	m_Words.insert(make_pair("Dotted", L"نقطه_دار"));
	m_Words.insert(make_pair("Double_Dotted", L"دو_نقطه_دار"));
	m_Words.insert(make_pair("Fermata", L"علامت_فرمات_"));
	m_Words.insert(make_pair("Finger", L"انگشت"));
	m_Words.insert(make_pair("Fingers", L"انگشتان"));
	m_Words.insert(make_pair("First_Volta_End", L"پایان_ولت_یک"));
	m_Words.insert(make_pair("First_Volta_Start", L"شروع_ولت_یک"));
	m_Words.insert(make_pair("For_Instrument", L"برای_ساز"));
	m_Words.insert(make_pair("Key_Signature", L"علامت_سر_کلید"));
	m_Words.insert(make_pair("Key_Signature_Changes_To", L"علامت_سر_کلید_تغییر_میکند_به"));
	m_Words.insert(make_pair("End_Bar_Line", L"دولا_خط_پایان"));
	m_Words.insert(make_pair("End_Repeat", L"دو_نقطه_تکرار_سمت_چپ"));
	m_Words.insert(make_pair("End_Tied_Line", L"پایان_خط_اتحاد"));
	m_Words.insert(make_pair("End_Legato_Line", L"پایان_خط_لگاتو"));
	m_Words.insert(make_pair("Left_Hand", L"دست_چپ"));
	m_Words.insert(make_pair("Line", L"خط"));
	m_Words.insert(make_pair("Major", L"ماژور"));
	m_Words.insert(make_pair("Measure", L"میزان"));
	m_Words.insert(make_pair("Measure_End", L"پایان_میزان"));
	m_Words.insert(make_pair("Movement", L"موومان"));
	m_Words.insert(make_pair("Minor", L"مینور"));
	m_Words.insert(make_pair("Note", L"نت"));
	m_Words.insert(make_pair("Octave", L"اکتاو"));
	m_Words.insert(make_pair("Open_Handed", L"دست_باز"));
	m_Words.insert(make_pair("on", L"روی"));
	m_Words.insert(make_pair("Part", L"قطعه"));
	m_Words.insert(make_pair("Rest", L"سکوت"));
	m_Words.insert(make_pair("Right_Hand", L"دست_راست"));
	m_Words.insert(make_pair("Silence", L"سکوت"));
	m_Words.insert(make_pair("Silence_till_end_of_part", L"سکوت تا پایان قطعه"));
	m_Words.insert(make_pair("Staff", L"خط_حامل"));
	m_Words.insert(make_pair("Second_Volta_End", L"پایان_ولت_دو"));
	m_Words.insert(make_pair("Second_Volta_Start", L"شروع_ولت_دو"));
	m_Words.insert(make_pair("Start", L"شروع"));
	m_Words.insert(make_pair("Start_Legato_Line", L"شروع_خط_لگاتو"));
	m_Words.insert(make_pair("Start_Tied_Line", L"شروع_خط_اتحاد"));
	m_Words.insert(make_pair("Stop", L"پایان"));
	m_Words.insert(make_pair("Tempo", L"تِمپو"));
	m_Words.insert(make_pair("till", L"تا"));
	m_Words.insert(make_pair("Treble", L"سُل"));
	m_Words.insert(make_pair("Tuplet", L"تِریوله"));
	m_Words.insert(make_pair("Time_Signature", L"میزان_نما"));
	m_Words.insert(make_pair("Voice", L"صدا"));
}

// Translates string into selected language.
CString CTranslator::TranslateText(CStringA Text)
{
	if (m_Language == L"EN")
		return CString(CA2W(Text));

	CString	Output = L"";

	vector<CStringA>	Tokens = Tokenize(Text, ";");
	
	for each(CStringA Statement in Tokens)
	{
		m_Context.empty();

		vector<CStringA>	SubTokens = Tokenize(Statement.Trim(L' '), " ");

		if (!SubTokens.size())
			continue;
		for each (CStringA SubStatement in SubTokens)
		{
			CString Translation = TranslateStatement(SubStatement);

			if (Translation.GetLength())
			Output += Translation + L" ";
		}

		if (Output.GetLength())
			Output = Output.Left(Output.GetLength() - 1);
		Output += L"; ";
	}

	return Output;
}

#define EQ(X) ((int)Tokens.size()==X)
#define IS(X,Y) (Tokens[X] == Y)
#define T(X) TranslateWord(Tokens[X])
#define T_(X) TranslateWord(X)
#define H(X) (m_Words.find(Tokens[X]) != m_Words.end())
#define H_(X) (m_Words.find(X) != m_Words.end())
#define D(X) CString(CA2W(Tokens[X]))
#define D_(X) CString(CA2W(X))


// Translates a single word
CString	CTranslator::TranslateWord(CStringA Word)
{
	if (H_(Word))
		return m_Words[Word];
	else
	{
		// Number?
		bool	bIsNumber = true;
		for (char * pch = Word.GetBuffer(); *pch; pch++)
			if (!isdigit(*pch) && *pch != '.')
			{
				bIsNumber = false;
				break;
			}
		if (bIsNumber)
			return D_(Word);
		else
		{
#ifdef _DEBUG
			std::ofstream OutFile("L:\\Words.txt", std::ios::app);
			OutFile << (char *)Word.GetBuffer();
			OutFile << "\n";
			OutFile.close();
#endif		
			return CString(CA2W(Word));
		}
	}
}

// Translates an isolated statement.
CString CTranslator::TranslateStatement(CStringA Statement)
{
	if (m_Language == L"EN")
		return CString(CA2W(Statement));

	CStringA		Prefix = "";
	CStringA		Postfix = "";
	CStringA		Codes = "*:[],\r\n";
	
	while (Statement.GetLength() && Codes.Find(Statement[0]) != -1)
	{
		Prefix += Statement[0];
		Statement = Statement.Right(Statement.GetLength() - 1);
	}
	while (Statement.GetLength() && Codes.Find(Statement[Statement.GetLength() - 1]) != -1)
	{
		Postfix.Insert(0, Statement[Statement.GetLength() - 1]) ;
		Statement = Statement.Left(Statement.GetLength() - 1);
	}	

	vector<CStringA>	Tokens = Tokenize(Statement, "_");
	vector<CString>		Outs;

	if (m_Context.size())
	{
		if (m_Context.top() == "Clef")
		{
			m_Context.pop();
			if (Tokens.size())
			{
				Outs.push_back(T_("Clef_" + Tokens[0]));
				Tokens.erase(Tokens.begin());
			}			
		}
	}

	if (EQ(1) && Tokens[0] == "Clef")
	{
		m_Context.push(Tokens[0]);
		Postfix = L"";
	}
	else if (H_(Statement))
		Outs.push_back(T_(Statement));
	else if (EQ(2) && Tokens[1] == "Rest")
	{
		Outs.push_back(T(1));
		Outs.push_back(T(0));
	}
	else
		if (EQ(4) && Tokens.back() == "Note")
		{
			Outs.push_back(T(3));
			Outs.push_back(T(0));
			Outs.push_back(T(2));
			Outs.push_back(T_("Octave"));
			Outs.push_back(D(1));
		}
		else if (EQ(5) && Tokens.back() == "Note")
		{
			Outs.push_back(T(4));
			Outs.push_back(T(0));
			Outs.push_back(T(2));
			Outs.push_back(T(3));
			Outs.push_back(T_("Octave"));
			Outs.push_back(D(1));
		}
//		else if (EQ(2) && Tokens[0] == "Dynamic")
//			Outs.push_back(T_(Statement));
		else
		{
			while (Tokens.size())
				for (int i = (int)Tokens.size(); i > 0; i--)
				{
					CStringA	Combination;
					for (int j = 0; j < i; j++)
						Combination += Tokens[j] + (j < i - 1 ? "_" : "");
			
					if (i == 1)
					{
						Outs.push_back(TranslateWord(Tokens[0]));
						VEC_ERASE(Tokens, 0);
						break;
					}
					else if (H_(Combination))
					{
						Outs.push_back(T_(Combination));
						while (i--)
							VEC_ERASE(Tokens, 0);
						break;
					}
				}
			
			for each(CStringA Word in Tokens)
				Outs.push_back(T_(Word));
		}
	CString	Output = L"";
	if (Outs.size())
		for each (CString Word in Outs)
			Output += Word + L"_";
	else
		Output = L"_";

	Output = CString(CA2W(Prefix)) + Output.Left(Output.GetLength() - 1) + CString(CA2W(Postfix));
	return Output;
}



