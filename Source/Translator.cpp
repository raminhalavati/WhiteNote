#include "stdafx.h"
#include "Translator.h"
#include "Common.h"
#include <fstream>

CTranslator::CTranslator()
{
	m_Language = L"EN";

	m_NoteWords.push_back("whole");
	m_NoteWords.push_back("half");
	m_NoteWords.push_back("quarter");
	m_NoteWords.push_back("eighth");
	m_NoteWords.push_back("sixteenth");
	m_NoteWords.push_back("32nd");
	m_NoteWords.push_back("64th");
	m_NoteWords.push_back("128th");
	m_NoteWords.push_back("256th");
	m_NoteWords.push_back("whole_bar");
}


CTranslator::~CTranslator()
{
}

void CTranslator::SetLanguage(CString Language)
{
	m_Language = Language;
	
	m_Words.insert(make_pair("c", L"دو"));
	m_Words.insert(make_pair("d", L"رِ"));
	m_Words.insert(make_pair("e", L"می"));
	m_Words.insert(make_pair("f", L"فا"));
	m_Words.insert(make_pair("g", L"سُل"));
	m_Words.insert(make_pair("a", L"لا"));
	m_Words.insert(make_pair("b", L"سی"));

	m_Words.insert(make_pair("cn", L"دو_بِکار"));
	m_Words.insert(make_pair("dn", L"رِ_بِکار"));
	m_Words.insert(make_pair("en", L"می_بِکار"));
	m_Words.insert(make_pair("fn", L"فا_بِکار"));
	m_Words.insert(make_pair("gn", L"سُل_بِکار"));
	m_Words.insert(make_pair("an", L"لا_بِکار"));
	m_Words.insert(make_pair("bn", L"سی_بِکار"));

	m_Words.insert(make_pair("cb", L"دو_بِمُل"));
	m_Words.insert(make_pair("db", L"رِ_بِمُل"));
	m_Words.insert(make_pair("eb", L"می_بِمُل"));
	m_Words.insert(make_pair("fb", L"فا_بِمُل"));
	m_Words.insert(make_pair("gb", L"سُل_بِمُل"));
	m_Words.insert(make_pair("ab", L"لا_بِمُل"));
	m_Words.insert(make_pair("bb", L"سی_بِمُل"));
	
	m_Words.insert(make_pair("c#", L"دو_دیِز"));
	m_Words.insert(make_pair("d#", L"رِ_دیِز"));
	m_Words.insert(make_pair("e#", L"می_دیِز"));
	m_Words.insert(make_pair("f#", L"فا_دیِز"));
	m_Words.insert(make_pair("g#", L"سُل_دیِز"));
	m_Words.insert(make_pair("a#", L"لا_دیِز"));
	m_Words.insert(make_pair("b#", L"سی_دیِز"));


	m_Words.insert(make_pair("whole", L"گرد"));
	m_Words.insert(make_pair("half", L"سفید"));
	m_Words.insert(make_pair("quarter", L"سیاه"));
	m_Words.insert(make_pair("eighth", L"چنگ"));
	m_Words.insert(make_pair("sixteenth", L"دو_لا_چنگ"));
	m_Words.insert(make_pair("32nd", L"سه_لا_چنگ"));
	m_Words.insert(make_pair("64th", L"چهار_لا_چنگ"));
	m_Words.insert(make_pair("128th", L"پنج_لا_چنگ"));
	m_Words.insert(make_pair("256th", L"شش_لا_چنگ"));
	m_Words.insert(make_pair("whole_bar", L"تمام_میزان"));
	
	m_Words.insert(make_pair("a_tempo", L"اِتِمپو"));
	m_Words.insert(make_pair("acceL", L"آجلراندو"));
	m_Words.insert(make_pair("allegro", L"آلِگرو"));
	m_Words.insert(make_pair("allegrato", L"آلِگرتو"));
	m_Words.insert(make_pair("dolce", L"دولچه"));
	m_Words.insert(make_pair("grave", L"گریو"));
	m_Words.insert(make_pair("gruppetto", L"گروپتو"));
	m_Words.insert(make_pair("largo", L"لارگو"));
	m_Words.insert(make_pair("marcato", L"مارکاتو"));
	m_Words.insert(make_pair("martelato", L"مارتلاتو"));
	m_Words.insert(make_pair("metso", L"متسو"));
	m_Words.insert(make_pair("mordent", L"موردنت"));
	m_Words.insert(make_pair("moderato", L"مودراتو"));
	m_Words.insert(make_pair("poco_ralL", L"پوکورال"));
	m_Words.insert(make_pair("poco_ritardtando", L"پوکوریت"));
	m_Words.insert(make_pair("rinforzando", L"رینفورزاندو"));
	m_Words.insert(make_pair("sempre", L"سمپره"));
	m_Words.insert(make_pair("trilL", L"تریل"));

	m_Words.insert(make_pair("crescendo", L"کرشندو"));
	m_Words.insert(make_pair("decrescendo", L"دیکرشندو"));
	m_Words.insert(make_pair("diminuendo", L"دیمینو_اندو"));
	m_Words.insert(make_pair("legato", L"لِگاتو"));
	m_Words.insert(make_pair("forte", L"فورته"));
	m_Words.insert(make_pair("fortessimo", L"فورتیسیمو"));
	m_Words.insert(make_pair("fortessissimo", L"فورته_فورتیسیمو"));
	m_Words.insert(make_pair("forte_piano", L"فورته_پیانو"));
	m_Words.insert(make_pair("forzando", L"فورزاندو"));
	m_Words.insert(make_pair("mezzo_forte", L"متزو_فورته"));
	m_Words.insert(make_pair("mezzo_piano", L"متزو_پیانو"));
	m_Words.insert(make_pair("piano", L"پیانو"));
	m_Words.insert(make_pair("pianissimo", L"پیانیسیمو"));
	m_Words.insert(make_pair("pianississimo", L"پیانو_پیانیسیمو"));
	m_Words.insert(make_pair("sforzando", L"اسفورزاندو"));
	m_Words.insert(make_pair("stacatto", L"استکاتو"));

	m_Words.insert(make_pair("chord", L"آکورد"));
	m_Words.insert(make_pair("arpeggiate", L"آرپژ"));
	m_Words.insert(make_pair("bass", L"باس"));
	m_Words.insert(make_pair("begin_bar_line", L"دولا_خط_شروع"));
	m_Words.insert(make_pair("begin_repeat", L"دو_نقطه_تکرار_سمت_راست"));
	m_Words.insert(make_pair("change_to", L"تغییر_به"));
	m_Words.insert(make_pair("changes_to", L"تغییر_میکند_به"));
	m_Words.insert(make_pair("clarinet", L"کلارینت"));
	m_Words.insert(make_pair("clef_bass", L"کلید_فا"));
	m_Words.insert(make_pair("clef_tab", L"کلید_تَبلِچِر"));
	m_Words.insert(make_pair("clef_treble", L"کلید_سُل"));
	m_Words.insert(make_pair("clef_french", L"کلید_ویلن_فرانسه"));
	m_Words.insert(make_pair("clef_baritone", L"کلید_باریتن"));
	m_Words.insert(make_pair("clef_subbass", L"کلید_باس_پایین"));
	m_Words.insert(make_pair("clef_alto", L"کلید_آلتو"));
	m_Words.insert(make_pair("clef_tenor", L"کلید_تِنور"));
	m_Words.insert(make_pair("clef_mezzosoprano", L"کلید_متسو_سوپرانو"));
	m_Words.insert(make_pair("clef_soprano", L"کلید_سوپرانو"));
	m_Words.insert(make_pair("clef_percussion", L"کلید_خنثی"));
	m_Words.insert(make_pair("clef_tablature", L"کلید_تَبلِچِر"));
	m_Words.insert(make_pair("cresc.", L"کرشندو"));
	m_Words.insert(make_pair("staccato", L"استاکاتو"));
	m_Words.insert(make_pair("dynamic", L"دینامیک"));
	m_Words.insert(make_pair("dotted", L"نقطه_دار"));
	m_Words.insert(make_pair("double_dotted", L"دو_نقطه_دار"));
	m_Words.insert(make_pair("equal_to", L"مساوی_با"));
	m_Words.insert(make_pair("fermata", L"علامت_فرمات"));
	m_Words.insert(make_pair("finger", L"انگشت"));
	m_Words.insert(make_pair("fingers", L"انگشتان"));
	m_Words.insert(make_pair("first_volta_end", L"پایان_ولت_یک"));
	m_Words.insert(make_pair("first_volta_start", L"شروع_ولت_یک"));
	m_Words.insert(make_pair("for_instrument", L"برای_ساز"));
	m_Words.insert(make_pair("key_signature", L"علامت_سر_کلید"));
	m_Words.insert(make_pair("end_bar_line", L"دولا_خط_پایان"));
	m_Words.insert(make_pair("end_repeat", L"دو_نقطه_تکرار_سمت_چپ"));
	m_Words.insert(make_pair("end_tied_line", L"پایان_خط_اتحاد"));
	m_Words.insert(make_pair("end_legato_line", L"پایان_خط_لگاتو"));
	m_Words.insert(make_pair("has_comments", L"توضیح_دارد"));
	m_Words.insert(make_pair("left_hand", L"دست_چپ"));
	m_Words.insert(make_pair("line", L"خط"));
	m_Words.insert(make_pair("major", L"ماژور"));
	m_Words.insert(make_pair("measure", L"میزان"));
	m_Words.insert(make_pair("measure_end", L"پایان_میزان"));
	m_Words.insert(make_pair("metronome", L"مترونوم"));
	m_Words.insert(make_pair("movement", L"موومان"));
	m_Words.insert(make_pair("minor", L"مینور"));
	m_Words.insert(make_pair("note", L"نت"));
	m_Words.insert(make_pair("octave", L"اکتاو"));
	m_Words.insert(make_pair("octave_shift_down", L"شروع اکتاو پایین تر"));
	m_Words.insert(make_pair("octave_shift_up", L"شروع اکتاو بالا تر"));
	m_Words.insert(make_pair("octave_shift_stop", L"پایان تغییر اکتاو "));
	m_Words.insert(make_pair("open_handed", L"دست_باز"));
	m_Words.insert(make_pair("on", L"روی"));
	m_Words.insert(make_pair("part", L"قطعه"));
	m_Words.insert(make_pair("rest", L"سکوت"));
	m_Words.insert(make_pair("right_hand", L"دست_راست"));
	m_Words.insert(make_pair("silence", L"سکوت"));
	m_Words.insert(make_pair("silence_till_end_of_part", L"سکوت تا پایان قطعه"));
	m_Words.insert(make_pair("staff", L"خط_حامل"));
	m_Words.insert(make_pair("second_volta_end", L"پایان_ولت_دو"));
	m_Words.insert(make_pair("second_volta_start", L"شروع_ولت_دو"));
	m_Words.insert(make_pair("sound", L"صدا"));
	m_Words.insert(make_pair("start", L"شروع"));
	m_Words.insert(make_pair("start_legato_line", L"شروع_خط_لگاتو"));
	m_Words.insert(make_pair("start_tied_line", L"شروع_خط_اتحاد"));
	m_Words.insert(make_pair("stop", L"پایان"));
	m_Words.insert(make_pair("tab", L"تب"));
	m_Words.insert(make_pair("tempo", L"تِمپو"));
	m_Words.insert(make_pair("tilL", L"تا"));	
	m_Words.insert(make_pair("treble", L"سُل"));
	m_Words.insert(make_pair("tuplet", L"تِریوله"));
	m_Words.insert(make_pair("time_signature", L"میزان_نما"));
	m_Words.insert(make_pair("voice", L"صدا"));
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
	Statement.MakeLower();
	if (m_Language == L"EN")
		return CString(CA2W(Statement));

	CStringA		Prefix = "";
	CStringA		Postfix = "";
	CStringA		Codes = "*:[],\r\n\"";
	
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
		bool	bFirst = true;
		while (m_Context.size())
		{
			if (bFirst)
			{
				Tokens[0] = m_Context.top() + Tokens[0];
				bFirst = false;
			}
			else
				Tokens.insert(Tokens.begin(), m_Context.top());
			m_Context.pop();			
		}
		// Don't convert original statement if it exists
		Statement = L"";
	}

	if (EQ(1) && Tokens[0] == "clef")
	{
		m_Context.push("clef_");\
		Postfix = L"";
	}
	else if (EQ(3) && Tokens[0] == "clef" && Tokens[1] == "changes" && Tokens[2] == "to")
	{
		m_Context.push("change");
		m_Context.push("to");
		m_Context.push("clef_");
		Postfix = L"";
	}
	else if (H_(Statement))
		Outs.push_back(T_(Statement));
	else if (EQ(2) && Tokens[1] == "rest")
	{
		Outs.push_back(T(1));
		Outs.push_back(T(0));
	}
	else
		if (EQ(4) && Tokens.back() == "note")
		{
			Outs.push_back(T(3));
			Outs.push_back(T(0));
			Outs.push_back(T(2));
			Outs.push_back(T_("octave"));
			Outs.push_back(D(1));
		}
		else if (EQ(5) && Tokens.back() == "note")
		{
			Outs.push_back(T(4));
			Outs.push_back(T(0));
			Outs.push_back(T(2));
			Outs.push_back(T(3));
			Outs.push_back(T_("octave"));
			Outs.push_back(D(1));
		}
//		else if (EQ(2) && Tokens[0] == "Dynamic")
//			Outs.push_back(T_(Statement));
		else
		{
			// See if there is a "note" in the text
			int	iNote = VEC_INDEX(find(Tokens.begin(), Tokens.end(), "note"), Tokens);
			if (iNote != (int)Tokens.size())
				if (iNote > 0 && find(m_NoteWords.begin(), m_NoteWords.end(), Tokens[iNote - 1]) != m_NoteWords.end())
					swap(Tokens[iNote - 1], Tokens[iNote]);
			
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