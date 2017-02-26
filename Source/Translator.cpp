#include "stdafx.h"
#include "Translator.h"
#include "Common.h"
#include <fstream>

CTranslator::CTranslator()
{
	m_Language = L"EN";

	m_NoteWords.push_back(L"whole");
	m_NoteWords.push_back(L"half");
	m_NoteWords.push_back(L"quarter");
	m_NoteWords.push_back(L"8th");
	m_NoteWords.push_back(L"16th");
	m_NoteWords.push_back(L"32nd");
	m_NoteWords.push_back(L"64th");
	m_NoteWords.push_back(L"128th");
	m_NoteWords.push_back(L"256th");
	m_NoteWords.push_back(L"whole_bar");
}


CTranslator::~CTranslator()
{
}

void CTranslator::SetLanguage(CString Language)
{
	m_Language = Language;
	
	m_Words.insert(make_pair(L"c", L"دو"));
	m_Words.insert(make_pair(L"d", L"رِ"));
	m_Words.insert(make_pair(L"e", L"می"));
	m_Words.insert(make_pair(L"f", L"فا"));
	m_Words.insert(make_pair(L"g", L"سُل"));
	m_Words.insert(make_pair(L"a", L"لا"));
	m_Words.insert(make_pair(L"b", L"سی"));

	m_Words.insert(make_pair(L"♮c", L"دو_بِکار"));
	m_Words.insert(make_pair(L"♮d", L"رِ_بِکار"));
	m_Words.insert(make_pair(L"♮e", L"می_بِکار"));
	m_Words.insert(make_pair(L"♮f", L"فا_بِکار"));
	m_Words.insert(make_pair(L"♮g", L"سُل_بِکار"));
	m_Words.insert(make_pair(L"♮a", L"لا_بِکار"));
	m_Words.insert(make_pair(L"♮b", L"سی_بِکار"));

	m_Words.insert(make_pair(L"♭c", L"دو_بِمُل"));
	m_Words.insert(make_pair(L"♭d", L"رِ_بِمُل"));
	m_Words.insert(make_pair(L"♭e", L"می_بِمُل"));
	m_Words.insert(make_pair(L"♭f", L"فا_بِمُل"));
	m_Words.insert(make_pair(L"♭g", L"سُل_بِمُل"));
	m_Words.insert(make_pair(L"♭a", L"لا_بِمُل"));
	m_Words.insert(make_pair(L"♭b", L"سی_بِمُل"));
	
	m_Words.insert(make_pair(L"♯c", L"دو_دیِز"));
	m_Words.insert(make_pair(L"♯d", L"رِ_دیِز"));
	m_Words.insert(make_pair(L"♯e", L"می_دیِز"));
	m_Words.insert(make_pair(L"♯f", L"فا_دیِز"));
	m_Words.insert(make_pair(L"♯g", L"سُل_دیِز"));
	m_Words.insert(make_pair(L"♯a", L"لا_دیِز"));
	m_Words.insert(make_pair(L"♯b", L"سی_دیِز"));

	m_Words.insert(make_pair(L"♭♭c", L"دو_دوبل_بِمُل"));
	m_Words.insert(make_pair(L"♭♭d", L"رِ_دوبل_بِمُل"));
	m_Words.insert(make_pair(L"♭♭e", L"می_دوبل_بِمُل"));
	m_Words.insert(make_pair(L"♭♭f", L"فا_دوبل_بِمُل"));
	m_Words.insert(make_pair(L"♭♭g", L"سُل_دوبل_بِمُل"));
	m_Words.insert(make_pair(L"♭♭a", L"لا_دوبل_بِمُل"));
	m_Words.insert(make_pair(L"♭♭b", L"سی_دوبل_بِمُل"));

	m_Words.insert(make_pair(L"♯♯c", L"دو_دوبل_دیِز"));
	m_Words.insert(make_pair(L"♯♯d", L"رِ_دوبل_دیِز"));
	m_Words.insert(make_pair(L"♯♯e", L"می_دوبل_دیِز"));
	m_Words.insert(make_pair(L"♯♯f", L"فا_دوبل_دیِز"));
	m_Words.insert(make_pair(L"♯♯g", L"سُل_دوبل_دیِز"));
	m_Words.insert(make_pair(L"♯♯a", L"لا_دوبل_دیِز"));
	m_Words.insert(make_pair(L"♯♯b", L"سی_دوبل_دیِز"));

	m_Words.insert(make_pair(L"whole", L"گرد"));
	m_Words.insert(make_pair(L"half", L"سفید"));
	m_Words.insert(make_pair(L"quarter", L"سیاه"));
	m_Words.insert(make_pair(L"8th", L"چنگ"));
	m_Words.insert(make_pair(L"16th", L"دو_لا_چنگ"));
	m_Words.insert(make_pair(L"32nd", L"سه_لا_چنگ"));
	m_Words.insert(make_pair(L"64th", L"چهار_لا_چنگ"));
	m_Words.insert(make_pair(L"128th", L"پنج_لا_چنگ"));
	m_Words.insert(make_pair(L"256th", L"شش_لا_چنگ"));
	m_Words.insert(make_pair(L"whole_bar", L"تمام_میزان"));
	
	m_Words.insert(make_pair(L"a_tempo", L"اِتِمپو"));
	m_Words.insert(make_pair(L"acceL", L"آجلراندو"));
	m_Words.insert(make_pair(L"allegro", L"آلِگرو"));
	m_Words.insert(make_pair(L"allegrato", L"آلِگرتو"));
	m_Words.insert(make_pair(L"dolce", L"دولچه"));
	m_Words.insert(make_pair(L"grave", L"گریو"));
	m_Words.insert(make_pair(L"gruppetto", L"گروپتو"));
	m_Words.insert(make_pair(L"largo", L"لارگو"));
	m_Words.insert(make_pair(L"marcato", L"مارکاتو"));
	m_Words.insert(make_pair(L"martelato", L"مارتلاتو"));
	m_Words.insert(make_pair(L"metso", L"متسو"));
	m_Words.insert(make_pair(L"mordent", L"موردنت"));
	m_Words.insert(make_pair(L"moderato", L"مودراتو"));
	m_Words.insert(make_pair(L"poco_ralL", L"پوکورال"));
	m_Words.insert(make_pair(L"poco_ritardtando", L"پوکوریت"));
	m_Words.insert(make_pair(L"rinforzando", L"رینفورزاندو"));
	m_Words.insert(make_pair(L"sempre", L"سمپره"));
	m_Words.insert(make_pair(L"trilL", L"تریل"));

	m_Words.insert(make_pair(L"crescendo", L"کرشندو"));
	m_Words.insert(make_pair(L"decrescendo", L"دیکرشندو"));
	m_Words.insert(make_pair(L"diminuendo", L"دیمینو_اندو"));
	m_Words.insert(make_pair(L"legato", L"لِگاتو"));
	m_Words.insert(make_pair(L"forte", L"فورته"));
	m_Words.insert(make_pair(L"fortessimo", L"فورتیسیمو"));
	m_Words.insert(make_pair(L"fortessissimo", L"فورته_فورتیسیمو"));
	m_Words.insert(make_pair(L"forte_piano", L"فورته_پیانو"));
	m_Words.insert(make_pair(L"forzando", L"فورزاندو"));
	m_Words.insert(make_pair(L"mezzo_forte", L"متزو_فورته"));
	m_Words.insert(make_pair(L"mezzo_piano", L"متزو_پیانو"));
	m_Words.insert(make_pair(L"piano", L"پیانو"));
	m_Words.insert(make_pair(L"pianissimo", L"پیانیسیمو"));
	m_Words.insert(make_pair(L"pianississimo", L"پیانو_پیانیسیمو"));
	m_Words.insert(make_pair(L"sforzando", L"اسفورزاندو"));
	m_Words.insert(make_pair(L"stacatto", L"استکاتو"));

  m_Words.insert(make_pair(L"arpeggiate", L"آرپژ"));
  m_Words.insert(make_pair(L"arpeggiate_end", L"پایان_آرپژ"));
  m_Words.insert(make_pair(L"chord", L"آکورد"));
  m_Words.insert(make_pair(L"chord_end", L"پایان_آکورد"));
	m_Words.insert(make_pair(L"bass", L"باس"));
	m_Words.insert(make_pair(L"begin_bar_line", L"دولا_خط_شروع"));
	m_Words.insert(make_pair(L"begin_repeat", L"دو_نقطه_تکرار_سمت_راست"));
	m_Words.insert(make_pair(L"change_to", L"تغییر_به"));
	m_Words.insert(make_pair(L"changes_to", L"تغییر_میکند_به"));
	m_Words.insert(make_pair(L"clarinet", L"کلارینت"));
	m_Words.insert(make_pair(L"clef_bass", L"کلید_فا"));
	m_Words.insert(make_pair(L"clef_tab", L"کلید_تَبلِچِر"));
	m_Words.insert(make_pair(L"clef_treble", L"کلید_سُل"));
	m_Words.insert(make_pair(L"clef_french", L"کلید_ویلن_فرانسه"));
	m_Words.insert(make_pair(L"clef_baritone", L"کلید_باریتن"));
	m_Words.insert(make_pair(L"clef_subbass", L"کلید_باس_پایین"));
	m_Words.insert(make_pair(L"clef_alto", L"کلید_آلتو"));
	m_Words.insert(make_pair(L"clef_tenor", L"کلید_تِنور"));
	m_Words.insert(make_pair(L"clef_mezzosoprano", L"کلید_متسو_سوپرانو"));
	m_Words.insert(make_pair(L"clef_soprano", L"کلید_سوپرانو"));
	m_Words.insert(make_pair(L"clef_percussion", L"کلید_خنثی"));
	m_Words.insert(make_pair(L"clef_tablature", L"کلید_تَبلِچِر"));
  //m_Words.insert(make_pair(L"clef_bass", L"کلید: فا"));
  //m_Words.insert(make_pair(L"clef_tab", L"کلید: تَبلِچِر"));
  //m_Words.insert(make_pair(L"clef_treble", L"کلید: سُل"));
  //m_Words.insert(make_pair(L"clef_french", L"کلید: ویلن_فرانسه"));
  //m_Words.insert(make_pair(L"clef_baritone", L"کلید: باریتن"));
  //m_Words.insert(make_pair(L"clef_subbass", L"کلید: باس_پایین"));
  //m_Words.insert(make_pair(L"clef_alto", L"کلید: آلتو"));
  //m_Words.insert(make_pair(L"clef_tenor", L"کلید: تِنور"));
  //m_Words.insert(make_pair(L"clef_mezzosoprano", L"کلید: متسو_سوپرانو"));
  //m_Words.insert(make_pair(L"clef_soprano", L"کلید: سوپرانو"));
  //m_Words.insert(make_pair(L"clef_percussion", L"کلید: خنثی"));
  //m_Words.insert(make_pair(L"clef_tablature", L"کلید: تَبلِچِر"));
	m_Words.insert(make_pair(L"cresc.", L"کرشندو"));
	m_Words.insert(make_pair(L"staccato", L"استاکاتو"));
	m_Words.insert(make_pair(L"dynamic", L"دینامیک"));
	m_Words.insert(make_pair(L"dotted", L"نقطه_دار"));
	m_Words.insert(make_pair(L"double", L"دو")); // in double dotted
	m_Words.insert(make_pair(L"equal_to", L"مساوی_با"));
	m_Words.insert(make_pair(L"fermata", L"علامت_فرمات"));
	m_Words.insert(make_pair(L"finger", L"انگشت"));
	m_Words.insert(make_pair(L"fingers", L"انگشتان"));
	m_Words.insert(make_pair(L"first_volta_end", L"پایان_ولت_یک"));
	m_Words.insert(make_pair(L"first_volta_start", L"شروع_ولت_یک"));
	m_Words.insert(make_pair(L"for_instrument", L"برای_ساز"));
	m_Words.insert(make_pair(L"key_signature", L"علامت_سر_کلید"));
	m_Words.insert(make_pair(L"end_bar_line", L"دولا_خط_پایان"));
	m_Words.insert(make_pair(L"end_repeat", L"دو_نقطه_تکرار_سمت_چپ"));
	m_Words.insert(make_pair(L"end_tied_line", L"پایان_خط_اتحاد"));
	m_Words.insert(make_pair(L"end_legato_line", L"پایان_خط_لگاتو"));
	m_Words.insert(make_pair(L"has_comments", L"توضیح_دارد"));
	m_Words.insert(make_pair(L"left_hand", L"دست_چپ"));
	m_Words.insert(make_pair(L"line", L"خط"));
	m_Words.insert(make_pair(L"major", L"ماژور"));
	m_Words.insert(make_pair(L"measure", L"میزان"));
	m_Words.insert(make_pair(L"measure_end", L"پایان_میزان"));
	m_Words.insert(make_pair(L"metronome", L"مترونوم"));
	m_Words.insert(make_pair(L"movement", L"موومان"));
	m_Words.insert(make_pair(L"minor", L"مینور"));
	m_Words.insert(make_pair(L"note", L"نت"));
	m_Words.insert(make_pair(L"octave", L"اکتاو"));
	m_Words.insert(make_pair(L"octave_shift_down", L"شروع اکتاو پایین تر"));
	m_Words.insert(make_pair(L"octave_shift_up", L"شروع اکتاو بالا تر"));
	m_Words.insert(make_pair(L"octave_shift_stop", L"پایان تغییر اکتاو "));
	m_Words.insert(make_pair(L"open_handed", L"دست_باز"));
	m_Words.insert(make_pair(L"on", L"روی"));
	m_Words.insert(make_pair(L"part", L"قطعه"));
	m_Words.insert(make_pair(L"rest", L"سکوت"));
	m_Words.insert(make_pair(L"right_hand", L"دست_راست"));
	m_Words.insert(make_pair(L"silence", L"سکوت"));
	m_Words.insert(make_pair(L"silence_till_end_of_part", L"سکوت تا پایان قطعه"));
	m_Words.insert(make_pair(L"staff", L"خط_حامل"));
	m_Words.insert(make_pair(L"second_volta_end", L"پایان_ولت_دو"));
	m_Words.insert(make_pair(L"second_volta_start", L"شروع_ولت_دو"));
	m_Words.insert(make_pair(L"sound", L"صدا"));
	m_Words.insert(make_pair(L"start", L"شروع"));
	m_Words.insert(make_pair(L"start_legato_line", L"شروع_خط_لگاتو"));
	m_Words.insert(make_pair(L"start_tied_line", L"شروع_خط_اتحاد"));
	m_Words.insert(make_pair(L"stop", L"پایان"));
	m_Words.insert(make_pair(L"tab", L"تب"));
	m_Words.insert(make_pair(L"tempo", L"تِمپو"));
	m_Words.insert(make_pair(L"tilL", L"تا"));	
	m_Words.insert(make_pair(L"treble", L"سُل"));
	m_Words.insert(make_pair(L"tuplet", L"تِریوله"));
  m_Words.insert(make_pair(L"tuplet_end", L"پایان_تِریوله"));
  m_Words.insert(make_pair(L"time_signature", L"میزان_نما"));
	m_Words.insert(make_pair(L"voice", L"صدا"));
}

// Translates string into selected language.
CString CTranslator::TranslateText(CString Text)
{
	if (m_Language == L"EN")
		return Text;

	CString	Output = L"";

	vector<CString>	Tokens = Tokenize(Text, L";");
	
	for each(CString Statement in Tokens)
	{
		m_Context.empty();

		vector<CString>	SubTokens = Tokenize(Statement.Trim(L' '), L" ");

		if (!SubTokens.size())
			continue;
		for each (CString SubStatement in SubTokens)
		{
			CString Translation = TranslateStatement(SubStatement);

			if (Translation.GetLength())
				Output += Translation + L" ";
		}

		if (Output.GetLength())
			Output = Output.Left(Output.GetLength() - 1);
		Output += L"; ";
	}

	if (Output.GetLength() > 3 && Output.Right(3) == "\n; ")
		Output = Output.Left(Output.GetLength() - 2);
	return Output;
}

#define EQ(X) ((int)Tokens.size()==X)
#define IS(X,Y) (Tokens[X] == Y)
#define T(X) TranslateWord(Tokens[X])
#define T_(X) TranslateWord(X)
#define H(X) (m_Words.find(Tokens[X]) != m_Words.end())
#define H_(X) (m_Words.find(X) != m_Words.end())
#define D(X) (Tokens[X])
#define D_(X) (X)


// Translates a single word
CString	CTranslator::TranslateWord(CString Word)
{
	if (H_(Word))
		return m_Words[Word];
	else
	{
		// Number?
		bool	bIsNumber = true;
		for (TCHAR * pch = Word.GetBuffer(); *pch; pch++)
			if (!iswdigit(*pch) && *pch != '.')
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
			return Word;
		}
	}
}

// Translates an isolated statement.
CString CTranslator::TranslateStatement(CString Statement)
{
	Statement.MakeLower();
	if (m_Language == L"EN")
		return Statement;
	
	CString		Prefix;
	CString		Postfix;
	CString		Codes(L"*:[],\r\n\"");
	
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

	vector<CString>	Tokens = Tokenize(Statement, L"_");
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
		m_Context.push(L"clef_");\
		Postfix = L"";
	}
	else if (EQ(3) && Tokens[0] == "clef" && Tokens[1] == "changes" && Tokens[2] == "to")
	{
		m_Context.push(L"change");
		m_Context.push(L"to");
		m_Context.push(L"clef_");
		Postfix = L"";
	}
	else if (H_(Statement))
		Outs.push_back(T_(Statement));
	// Change order if it's a rest rescription.
	else if (Tokens.size() > 1 && Tokens.back() == "rest")
	{
		// The original order is:	[[double] dotted] eight rest
		// It should become:		rest eight [[double] dotted]
		// Exception:				whole bar rest
		int len = (int)Tokens.size();
		if (len == 3 && Tokens[0] == L"whole" && Tokens[1] == L"bar") {
			Outs.push_back(T_(L"rest"));
			Outs.push_back(T_(L"whole_bar"));
		}
		else {
			Outs.push_back(T(len - 1));
			Outs.push_back(T(len - 2));
			if (len > 2)
				Outs.push_back(T(0));
			if (len > 3)
				Outs.push_back(T(1));
		}
	}
	else
		// Change order if it's a note description
		if (Tokens.size() > 3 && Tokens.back() == "note")
		{
			// The original order is:		D 5 [[double] dotted] eight note
			// it should be changed to:		note D eight [[double] dotted] octave 5
			int len = (int)Tokens.size();
			Outs.push_back(T(len - 1));
			Outs.push_back(T(0));
			Outs.push_back(T(len - 2));
			if (Tokens.size() > 4)
				Outs.push_back(T(2));
			if (Tokens.size() > 5)
				Outs.push_back(T(3));
			Outs.push_back(T_(L"octave"));
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
					CString	Combination;
					for (int j = 0; j < i; j++)
						Combination += Tokens[j] + (j < i - 1 ? L"_" : L"");
			
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
			
			for each(CString Word in Tokens)
				Outs.push_back(T_(Word));
		}
	CString	Output = L"";
	if (Outs.size())
		for each (CString Word in Outs)
			Output += Word + L"_";
	else
		Output = L"_";

	Output = Prefix + Output.Left(Output.GetLength() - 1) + Postfix;
	return Output;
}