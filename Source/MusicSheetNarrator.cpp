#include "StdAfx.h"
#include "MusicSheetNarrator.h"

CMusicSheetNarrator::CMusicSheetNarrator(void)
{
}


CMusicSheetNarrator::~CMusicSheetNarrator(void)
{
}

CString	CMusicSheetNarrator::GetNoteTypeName(MusicSheet::_Note & Note)
{
	CString	Text;
	
	// Grace
	if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_GRACE))
		Text += L"Grace";
	else
	{
		// Accents
		/*{
			if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_STACCATO))
				Text += L"Staccato_";

			if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_ACCENT))
				Text += L"Marcato_";

			if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_STRONG_ACCENT))
				Text += L"Martellato_";

			if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_GRUPPETTO))
				Text += L"Gruppetto_";
		}*/

		// Dot
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_DOT))
			Text += L"Dotted_";

		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_DOUBLE_DOT))
			Text += L"Double_Dotted_";

		switch (Note.Type)
		{
		case	MusicSheet::TYPE_WHOLE:		Text += L"Whole";	break;
		case	MusicSheet::TYPE_HALF:		Text += L"Half";	break;
		case	MusicSheet::TYPE_QUARTER:	Text += L"Quarter";	break;
		case	MusicSheet::TYPE_EIGHTH:	Text += L"8th";	break;
		case	MusicSheet::TYPE_SIXTEENTH:	Text += L"16th"; break;
		case	MusicSheet::TYPE_32ND:		Text += L"32nd"; break;
		case	MusicSheet::TYPE_64TH:		Text += L"64th"; break;
		case	MusicSheet::TYPE_128TH:		Text += L"128th"; break;
		case	MusicSheet::TYPE_256TH:		Text += L"256th"; break;
		case	MusicSheet::TYPE_UNTOLD:	Text += L"Whole_Bar"; break;
		default:							Text += L"UNKNOWN";	break;
		}
	}

	return Text;
}

void	CMusicSheetNarrator::GetNoteText(MusicSheet::_Note & Note, NarratedMusicSheet::Voice & Voice, bool bInTuplet, bool & bInGrace, CString * pChordLength)
{
	CString	Text, Temp;

	// Main Text
	if (Note.chStep == 'R')
	{
		Text = GetNoteTypeName(Note);
		Text += L"_Rest";
	}
	else
	{
		// Accents
		{
			if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_STACCATO))
				Text += L"Staccato; ";

			if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_ACCENT))
				Text += L"Marcato; ";

			if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_STRONG_ACCENT))
				Text += L"Martellato; ";

			if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_GRUPPETTO))
				Text += L"Gruppetto; ";
		}

		// Fermata
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_FERMATA))
			Text += L"Fermata_on_ ";

		// Accidentals
		switch (Note.chAccidental)
		{
		case 's':	Temp = L"♯"; break;
		case 'f':	Temp = L"♭"; break;
		case 'n':	Temp = L"♮"; break;
		case 'S':	Temp = L"♯♯"; break;
		case 'F':	Temp = L"♭♭"; break;
		case 0:		Temp = ""; break;
		default:
			Temp = L"UNKNOWN_ACCENT"; break;
		}
		if (Temp.GetLength())
			Text += Temp;
		
		Text += Note.chStep;

		Temp.Format(L"_%i", Note.iOctave);
		Text += Temp;

		{
			Text += L"_";
			Text += GetNoteTypeName(Note);
			if (m_bDetailedText)
				Text += L"_Note";
		}
		
    // Fingers?
    if (Note.Fingers.size()) {
      for (auto &finger: Note.Fingers) {
        if (finger == *Note.Fingers.begin())
          if (m_bDetailedText)
            Text += "; Finger_";
          else
            Text += "; ";
        else
            Text += "_";
        Text += GetFingerText(finger);
      }
    }

		// Slur-End
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_SLUR_END))
			Text += L"; End_Legato_Line";
		
		// Tied-End
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_TIED_END))
			Text += L"; End_Tied_Line";

		// Slur-Start
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_SLUR_START))
			Text += L"; Start_Legato_Line";

		// Tied-Start
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_TIED_START))
			Text += L"; Start_Tied_Line";
	}
	Voice.Text.push_back(Text);

	// Lily
	{
		Text = L"";
		// Grace
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_GRACE))
			if (!bInGrace)
			{
				Text += L"\\grace {";
				bInGrace = true;
			}

		Text += (char)tolower(Note.chStep);

		switch (Note.chAccidental)
		{
		case 's':	Text += L"is"; break;
		case 'f':	Text += L"es"; break;
		case 'S':	Text += L"isis"; break;
		case 'F':	Text += L"eses"; break;
		//case 'N':	Text += L"!"; break;
		}

		if (Note.iOctave != -1)
			if (Note.iOctave > 3)
				for (int i = 3; i < Note.iOctave; i++)
					Text += L"'";
			else
				for (int i = 3; i > Note.iOctave; i--)
					Text += L",";

		switch (Note.Type)
		{
		case	MusicSheet::TYPE_WHOLE:		Temp = L"1"; break;
		case	MusicSheet::TYPE_HALF:		Temp = L"2"; break;
		case	MusicSheet::TYPE_QUARTER:	Temp = L"4"; break;
		case	MusicSheet::TYPE_EIGHTH:	Temp = L"8"; break;
		case	MusicSheet::TYPE_SIXTEENTH:	Temp = L"16"; break;
		case	MusicSheet::TYPE_32ND:		Temp = L"32"; break;
		case	MusicSheet::TYPE_64TH:		Temp = L"64"; break;
		case	MusicSheet::TYPE_128TH:		Temp = L"128"; break;
		case	MusicSheet::TYPE_256TH:		Temp = L"256"; break;
		default:							
			if (Text == L"r")
			{
				Text = L"R1";
				Temp = L"";
			}
			else
				Temp = L"\\maxima"; break;
		}
		// Dot
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_DOT))
			Temp += L".";

		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_DOUBLE_DOT))
			Temp += L"..";

		if (pChordLength)
			*pChordLength = Temp;
		else
			Text += Temp;

		// Fermata
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_FERMATA))
			Text += L"\\fermata";
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_STACCATO))
			Text += L"\\staccato";

		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_ACCENT))
			Text += L"\\accent";

		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_STRONG_ACCENT))
			Text += L"\\accent";

		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_GRUPPETTO))
			Text += L"\\turn";
		
    for (auto& finger : Note.Fingers)
      Temp.Format(L"^\\markup {%s} ", GetFingerText(finger));

		// Texts should come after a note!
		if (Voice.PendingLilyText.GetLength())
		{
			Text += Voice.PendingLilyText;
			Voice.PendingLilyText = L"";
		}

		// Slur-End
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_SLUR_END))
			Text += L")";

		//// Tied-End
		//if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_TIED_END))
		//	Text += L"; End_Tied_Line";

		// Slur-Start
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_SLUR_START))
			Text += L"(";

		// Tied-Start
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_TIED_START))
			Text += L"~";
		
		Text += (bInTuplet || pChordLength) ? L" " : L"\r\n";		
	}
	Voice.Lily += Text;
}

void	CMusicSheetNarrator::GetSignaturesText(NarratedMusicSheet::Voice & Voice, MusicSheet::Signatures & Sigs, int iStaff, MusicSheet::Signatures * pPreviousSignature)
{
	CString	Text(L""), T1, T2, T3;
	bool	bItemChanged;

	// Clefs	
	if (iStaff < (int)Sigs.Clefs.size() && Sigs.Clefs[iStaff].Sign.GetLength())
	{
		bItemChanged = pPreviousSignature && (pPreviousSignature->Clefs[iStaff].Sign != Sigs.Clefs[iStaff].Sign || pPreviousSignature->Clefs[iStaff].iLine != Sigs.Clefs[iStaff].iLine);

		Text = CString((pPreviousSignature && !bItemChanged) ? L"*" : L"") + L"Clef";

		if (pPreviousSignature && bItemChanged)
			Text += L"_Changes_To_";
		else
			Text += L"_";
    /*  Text += L"_Changes_To: ";
    else
      Text += L": ";*/

		int iLine = Sigs.Clefs[iStaff].iLine;
		T1 = T2 = Sigs.Clefs[iStaff].Sign;

		if (T1 == L"G" && iLine == 2)
		{
			T1 = L"Treble";
			T2 = L"treble";
		}
		else if(T1 == L"G" && iLine == 1)
		{
			T1 = L"French";
			T2 = L"french";
		}
		else if (T1 == L"F" && iLine == 4)
		{
			T1 = L"Bass";
			T2 = L"bass";
		}
		else if (T1 == L"F" && iLine == 3)
		{
			T1 = L"Baritone";
			T2 = L"baritonevarF";
		}
		else if (T1 == L"F" && iLine == 5)
		{
			T1 = L"Subbass";
			T2 = L"subbass";
		}
		else if (T1 == L"C" && iLine == 3)
		{
			T1 = L"Alto";
			T2 = L"alto";
		}
		else if (T1 == L"C" && iLine == 4)
		{
			T1 = L"Tenor";
			T2 = L"tenor";
		}
		else if (T1 == L"C" && iLine == 5)
		{
			T1 = L"Baritone";
			T2 = L"baritone";
		}
		else if (T1 == L"C" && iLine == 2)
		{
			T1 = L"Mezzosoprano";
			T2 = L"mezzosoprano";
		}
		else if (T1 == L"C" && iLine == 1)
		{
			T1 = L"Soprano";
			T2 = L"soprano";
		}
		else if (T1 == L"percussion" && iLine == 1)
		{
			T1 = L"Percussion";
			T2 = L"percussion";
		}
		else if (T1 == L"TAB")
		{
			T1 = L"Tablature";
			T2 = L"tab"; // Also check LilyPondWrapper --> GetMeasureImage
		}
		
		T3.Format(L"%s_Line_%i", T1, iLine);
		Text += T3;
		Voice.Text.push_back(Text);

		T3.Format(L"\\clef %s\r\n", T2);
		Voice.Lily += T3;
	}

	// Key Signature
	if (Sigs.Key.iFifths != -1)
	{
		bItemChanged = pPreviousSignature && (pPreviousSignature->Key.iFifths != Sigs.Key.iFifths || pPreviousSignature->Key.bMajor != Sigs.Key.bMajor);
	
		Text = CString((pPreviousSignature && !bItemChanged) ? L"*" : L"") + L"Key_Signature";

		if (pPreviousSignature && bItemChanged)
			Text += L"_Changes_To_";
		else
			Text += L"_";
    /*  Text += L"_Changes_To: ";
    else
      Text += L": ";*/

		T1 = L"";
		switch (Sigs.Key.iFifths)
		{
		case 0:
			T1 = Sigs.Key.bMajor ? L"C" : L"A";
			break;
		case 7:	T1 = L"_C♯";
		case 6:	T1 = L"_E♯" + T1;
		case 5:	T1 = L"_A♯" + T1;
		case 4:	T1 = L"_D♯" + T1;
		case 3:	T1 = L"_G♯" + T1;
		case 2:	T1 = L"_C♯" + T1;
		case 1:	T1 = L"F♯" + T1;	
			break;		
		case -7:	T1 = L"_F♭";
		case -6:	T1 = L"_C♭";
		case -5:	T1 = L"_G♭";		
		case -4:	T1 = L"_D♭";			
		case -3:	T1 = L"_A♭";			
		case -2:	T1 = L"_E♭";			
		case -1:	T1 = L"B♭";
			break;
		
		default:	T1 += L"UNKNOWN_KEY";
		}
		Voice.Text.push_back(Text + T1 + L"_" + (Sigs.Key.bMajor? L"Major" : L"Minor"));

		// LILY
		switch (Sigs.Key.iFifths)
		{
		case 0:	T1 = Sigs.Key.bMajor ? L"c" : L"a"; break;
		case 1:	T1 = Sigs.Key.bMajor ? L"g" : L"e"; break;
		case 2:	T1 = Sigs.Key.bMajor ? L"d" : L"b"; break;
		case 3:	T1 = Sigs.Key.bMajor ? L"a" : L"fis"; break;
		case 4:	T1 = Sigs.Key.bMajor ? L"e" : L"cis"; break;
		case 5:	T1 = Sigs.Key.bMajor ? L"b" : L"gis"; break;
		case 6:	T1 = Sigs.Key.bMajor ? L"fis" : L"dis"; break;
		case 7:	T1 = Sigs.Key.bMajor ? L"cis" : L"dis"; break;
		case -1:	T1 = Sigs.Key.bMajor ? L"f" : L"d"; break;
		case -2:	T1 = Sigs.Key.bMajor ? L"bes" : L"g"; break;
		case -3:	T1 = Sigs.Key.bMajor ? L"ees" : L"c"; break;
		case -4:	T1 = Sigs.Key.bMajor ? L"aes" : L"f"; break;
		case -5:	T1 = Sigs.Key.bMajor ? L"des" : L"bes"; break;
		case -6:	T1 = Sigs.Key.bMajor ? L"ges" : L"ees"; break;
		case -7:	T1 = Sigs.Key.bMajor ? L"ces" : L"aes"; break;
		default:	T1 = L"";
		}

		if (T1.GetLength())
		{
			T2.Format(L"\\key %s \\%s\r\n", T1, Sigs.Key.bMajor ? L"major" : L"minor");
			Voice.Lily += T2;
		}
		
	}

	// Time Signature
	if (Sigs.Time.iBeats && Sigs.Time.iBeatType)
	{
		bItemChanged = pPreviousSignature && (pPreviousSignature->Time.iBeats != Sigs.Time.iBeats || pPreviousSignature->Time.iBeatType != Sigs.Time.iBeatType);

		Text = CString((pPreviousSignature && !bItemChanged) ? L"*" : L"") + L"Time_Signature";

		if (pPreviousSignature && bItemChanged)
			Text += L"_Changes_To_";
		else
			Text += L"_";
      /*Text += L"_Changes_To: ";
    else
      Text += L": ";*/

		T1.Format(L"%i_%i", Sigs.Time.iBeats, Sigs.Time.iBeatType);
		Text += T1;
		Voice.Text.push_back(Text);
		
		T1.Format(L"\\time %i/%i\r\n", Sigs.Time.iBeats, Sigs.Time.iBeatType);
		Voice.Lily += T1;
	}
}

bool	IsPreDirection(MusicSheet::DirectionTypes nDirType)
{
	return nDirType == MusicSheet::DIR_UNKNWON ||
		IsInRange(nDirType, MusicSheet::DIR_first_Dynamic, MusicSheet::DIR_last_Dynamic) ||
		IsInRange(nDirType, MusicSheet::DIR_first_Feeling, MusicSheet::DIR_last_Feeling);
}

vector<CString> TokenizeText(CString Text)
{
	vector<CString>	Result;
	int		iCurPos = 0;
	CString	Token = Text.Tokenize(L"|", iCurPos);
	while (Token != L"")
	{
		Result.push_back(Token);
		Token = Text.Tokenize(L"|", iCurPos);
	}
	return Result;
}

void	CMusicSheetNarrator::GetDirectionText(NarratedMusicSheet::Voice & Voice, MusicSheet::DirectionTypes nDirType, 
											  CString & BufferText, CString ExtraText, bool bAbove)
{
	CString	Text(L""), Lily(L"");

	if (IsInRange(nDirType, MusicSheet::DIR_first_TempoText, MusicSheet::DIR_last_TempoText))
		Text = L"Tempo_Text_";

	if (IsInRange(nDirType, MusicSheet::DIR_first_Dynamic, MusicSheet::DIR_last_Dynamic))
		Text += L"Dynamic_";

	switch (nDirType)
	{
	case MusicSheet::DIR_DOLCE:			Text += L"Dolce"; break;
	case MusicSheet::DIR_MESTO:			Text += L"Metso"; break;
	case MusicSheet::DIR_A_TEMPO:		Text += L"A_Tempo"; break;
	case MusicSheet::DIR_POCO_RIT:		Text += L"Poco_Ritardtando"; break;
	case MusicSheet::DIR_POCO_RALL:		Text += L"Poco_Rall"; break;
	case MusicSheet::DIR_SEMPRE:		Text += L"Sempre"; break;
	case MusicSheet::DIR_ACCEL:			Text += L"Accel"; break;
	case MusicSheet::DIR_TRILL:			Text += L"Trill"; break;
	case MusicSheet::DIR_GRUPPETTO:		Text += L"Gruppetto"; break;
	case MusicSheet::DIR_MORDENT:		Text += L"Mordent"; break;
	case MusicSheet::DIR_MODERATO:		Text += L"Moderato"; break;
	case MusicSheet::DIR_LARGO:			Text += L"Largo"; break;
	case MusicSheet::DIR_GRAVE:			Text += L"Grave"; break;
	case MusicSheet::DIR_ALLEGRO:		Text += L"Allegro"; break;
	case MusicSheet::DIR_ALLEGRATO:		Text += L"Allegrato"; break;

	case MusicSheet::DIR_DYNAMIC_P:		Text += L"Piano"; Lily = L"\\p"; break;
	case MusicSheet::DIR_DYNAMIC_PP:	Text += L"Pianissimo"; Lily = L"\\pp"; break;
	case MusicSheet::DIR_DYNAMIC_PPP:	Text += L"Pianississimo"; Lily = L"\\ppp"; break;
	case MusicSheet::DIR_DYNAMIC_MP:	Text += L"Mezzo_Piano"; Lily = L"\\mp"; break;
	case MusicSheet::DIR_DYNAMIC_MF:	Text += L"Mezzo_Forte"; Lily = L"\\mf"; break;
	case MusicSheet::DIR_DYNAMIC_F:		Text += L"Forte"; Lily = L"\\f"; break;
	case MusicSheet::DIR_DYNAMIC_FP:	Text += L"Forte_Piano"; Lily = L"\\fp"; break;
	case MusicSheet::DIR_DYNAMIC_FZ:	Text += L"Forzando"; Lily = L"\\fz"; break;
	case MusicSheet::DIR_DYNAMIC_FF:	Text += L"Fortessimo"; Lily = L"\\ff"; break;
	case MusicSheet::DIR_DYNAMIC_FFF:	Text += L"Fortessissimo"; Lily = L"\\fff"; break;
	case MusicSheet::DIR_DYNAMIC_SFZ:	Text += L"Sforzando"; Lily = L"\\sfz"; break;
	case MusicSheet::DIR_DYNAMIC_DIM:	Text += L"Diminuendo"; Lily = L"\\dim"; break;
	case MusicSheet::DIR_DYNAMIC_DECRESC:	Text += L"Decrescendo"; Lily = L"\\decresc"; break;
	case MusicSheet::DIR_DYNAMIC_CRESC:		Text += L"Cresc"; Lily = L"\\cresc"; break;
	case MusicSheet::DIR_DYNAMIC_STACCATO:	Text += L"Staccato"; Lily = L"\\staccato"; break;
	case MusicSheet::DIR_DYNAMIC_LEGATO:	Text += L"Legato"; /*Lily = L"\\legato";*/ break;

	case MusicSheet::DIR_WEDGE_DIMINUENDO:	Text += L"Start_Diminuendo"; BufferText = L"Diminuendo"; Lily = L"\\>";  break;
	case MusicSheet::DIR_WEDGE_STOP:		Text += CString(L"Stop_") + BufferText; Lily = L"\\!"; break;
	case MusicSheet::DIR_WEDGE_CRESCENDO:	Text += L"Start_Crescendo"; BufferText = L"Crescendo"; Lily = L"\\<"; break;

	case MusicSheet::DIR_OCTAVE_SHIFT_DOWN:	Text += L"Octave_Shift_Down"; break;
	case MusicSheet::DIR_OCTAVE_SHIFT_UP:	Text += L"Octave_Shift_Up"; break;
	case MusicSheet::DIR_OCTAVE_SHIFT_STOP:	Text += L"Octave_Shift_Stop"; break;

	case MusicSheet::DIR_SOUND:
	{
		/*vector<CString> Tokens = TokenizeText(ExtraText);
		if (Tokens.size() == 2)
		{
			Text += CString(L"Sound");
			if (Tokens[0] != L"(null)")
				Text += L"_Tempo_=_" + Tokens[0];
			if (Tokens[1] != L"(null)")
				Text += L"_Dynamic_=_" + Tokens[1];
		}*/
	}
	break;

	case MusicSheet::DIR_METRONOME:
	{
		vector<CString> Tokens = TokenizeText(ExtraText);
		if (Tokens.size() == 2)
		{
			CString Temp;
			Temp.Format(L"%s_note_equal_to_%s", Tokens[0], Tokens[1]);
			Text += Temp;
		}

		Tokens[0].MakeLower();
		int	iNote = 0;
		if (Tokens[0] == L"whole")
			iNote = 1;
		else if (Tokens[0] == L"whole")
			iNote = 1;
		else if (Tokens[0] == L"half")
			iNote = 2;
		else if (Tokens[0] == L"quarter")
			iNote = 4;
		else if (Tokens[0] == L"eight")
			iNote = 8;
		else if (Tokens[0] == L"sixteenth")
			iNote = 16;
		
		CString Temp = L"";
		for (TCHAR * c = Tokens[1].GetBuffer(); *c; c++)
			if (isdigit(*c) || *c == L'-' || *c == L' ')
				Temp += *c;
		Lily.Format(L"\\tempo %i = %s\r\n", iNote, Temp);
	}
	break;

	default:							
		Text += (ExtraText.GetLength() ? ExtraText: L"UNKNOWN_DIRECTION");
		break;
	}

	if (!Lily.GetLength() && Text.GetLength())
    Lily.Format(L"%s\\markup {%s} ", bAbove ? L"^" : L"_", Text);
	
	if (Lily.GetLength())
		if (IsPreDirection(nDirType) || Lily.Find(L"markup") != -1)
			Voice.PendingLilyText += Lily;
		else
			Voice.Lily += L" " + Lily;
	
  if (Text.GetLength())
    Voice.Text.push_back(Text);
}

CString	ConvertNumberToText(int i)
{
	switch (i)
	{
	case 1:	return L"First";
	case 2:	return L"Second";
	case 3:	return L"Third";
	case 4:	return L"Forth";
	default:
		CString A;
		A.Format(L"%i", i);
		return A;
	}
}

TCHAR*  CMusicSheetNarrator::GetFingerText(MusicSheet::DirectionTypes finger_type)
{
  switch (finger_type) {
    case MusicSheet::DIR_FINGER_0: return L"Open_Handed";
    case MusicSheet::DIR_FINGER_1: return L"1";
    case MusicSheet::DIR_FINGER_2: return L"2";
    case MusicSheet::DIR_FINGER_3: return L"3";
    case MusicSheet::DIR_FINGER_4: return L"4";
    case MusicSheet::DIR_FINGER_5: return L"5";

    case MusicSheet::DIR_FINGER_I: return L"I";
    case MusicSheet::DIR_FINGER_M: return L"M";
    case MusicSheet::DIR_FINGER_A: return L"A";
    case MusicSheet::DIR_FINGER_P: return L"P"; 
  }

  _RPTF1(_CRT_ERROR, "Unexpected finger type %i", finger_type);
  return L"";
}

void	CMusicSheetNarrator::GetBarLineText(NarratedMusicSheet::Voice & Voice, MusicSheet::BarLine & BL)
{
	switch (BL.nType)
	{
	case MusicSheet::BarLine::BL_START:
		Voice.Text.push_back(L"Begin_Bar_Line");
		Voice.Lily += L" \\bar \".|\"";
		return ;

	case MusicSheet::BarLine::BL_FORWARD:
		Voice.Text.push_back(L"Begin_Repeat");
		Voice.Lily += L" \\bar \".|:\"";
		return;

	case MusicSheet::BarLine::BL_VOLTA_START:
		Voice.Text.push_back(ConvertNumberToText(BL.iVoltaNumber) + L"_Volta_Start");
		#pragma message("Missing Lily")
		Voice.Lily += L" \\bar \"\"";

	case MusicSheet::BarLine::BL_END:
		Voice.Text.push_back(L"End_Bar_Line");
		Voice.Lily += L" \\bar \"|.\"";
		return;

	case MusicSheet::BarLine::BL_BACKWARD:
		Voice.Text.push_back(L"End_Repeat");
		Voice.Lily += L" \\bar \":|.\"";
		return;

	case MusicSheet::BarLine::BL_VOLTA_END:
		Voice.Text.push_back(ConvertNumberToText(BL.iVoltaNumber) + L"_Volta_End");
		Voice.Lily += L" \\bar \"\"";
		#pragma message("Missing Lily")
		return;

	default:
		Voice.Text.push_back(L"Unknown_Bar_Line");
		Voice.Lily += L" \\bar \"S\"";
		return;
	}
}

typedef struct _InlineDirection
{
	int							iVoice, iNote;
	MusicSheet::DirectionTypes	nType;
	CString					ExtraText;
	bool						bAbove;
	_InlineDirection(bool bAbove, int iVoice, int iNote, MusicSheet::DirectionTypes nType, CString ExtraText = L"")
	{
		this->bAbove = bAbove;
		this->iVoice = iVoice;
		this->iNote = iNote;
		this->nType = nType;
		this->ExtraText = ExtraText;		
	}
} InlineDirection;

NarratedMusicSheet::MeasureText	CMusicSheetNarrator::GetMeasureText(MusicSheet::_Measure * pMeasure, MusicSheet::Signatures * & pPreviousSignature)
{
	NarratedMusicSheet::MeasureText	MT;

	CString	Temp;
	unsigned	uNextSignature = 0;

	// Init all Voices
	{
		TCHAR * StyleNames[] = { L"voiceNeutralStyle", L"voiceOneStyle", L"voiceTwoStyle", L"voiceThreeStyle", L"voiceFourStyle" };

		Temp.Format(L"Measure_%i", pMeasure->iNumber + 1);
    //Temp.Format(L"Measure: %i", pMeasure->iNumber + 1);
		for ALL(pMeasure->Voices, pVoice)
		{
			NarratedMusicSheet::Voice NewVoice;
			NewVoice.iStaff = pVoice->iStaff;
			NewVoice.iVoice = 1;
			for ALL(MT.Voices, pInVoice)
				if (pInVoice->iStaff == NewVoice.iStaff)
					NewVoice.iVoice++;
			NewVoice.Text.push_back(Temp);
			if (!pVoice->Notes.size())
				NewVoice.Lily = L"!"; // Reminder that this line doesn't need to be printed.
			else 
				NewVoice.Lily.Format(L"{\r\n\\%s\r\n", StyleNames[(NewVoice.iVoice - 1) % 5]);
			MT.Voices.push_back(NewVoice);
		}
	}
	
	// Tell the first signatures before the first measure
	if (pMeasure->Signatures.size() && !pMeasure->Signatures[0].BeforeNote.second)
	{
		for ALL_INDICES(pMeasure->Voices, i)
			GetSignaturesText(
				MT.Voices[i],
				pMeasure->Signatures[uNextSignature], 
				pMeasure->Voices[i].iStaff, 
				pPreviousSignature);
		pPreviousSignature = &pMeasure->Signatures[uNextSignature++];
	}

	// Preprocess Directives
	vector<InlineDirection>	Directions;
	{
    for ALL(pMeasure->Directions, pDir) {
      _ASSERTE(!IsInRange(pDir->nType, MusicSheet::DIR_first_guitar_Finger, MusicSheet::DIR_last_guitar_Finger) &&
               !IsInRange(pDir->nType, MusicSheet::DIR_first_Finger, MusicSheet::DIR_last_Finger));

        if (IsInRange(pDir->nType, MusicSheet::DIR_first_Wedge, MusicSheet::DIR_last_Wedge) ||
            pDir->nType == MusicSheet::DIR_OCTAVE_SHIFT_UP ||
            pDir->nType == MusicSheet::DIR_OCTAVE_SHIFT_DOWN ||
            pDir->nType == MusicSheet::DIR_OCTAVE_SHIFT_STOP)
        {
          Directions.push_back(InlineDirection(pDir->bAbove, pDir->BeforeNote.first, pDir->BeforeNote.second, pDir->nType));
        }
        else if (pDir->nType == MusicSheet::DIR_UNKNWON)
        {
          CString Text;
          Text = pDir->Text.Trim(L"\r\n");
          Text.Replace(L' ', L'_');
          Text.Format(L"\"%s\"", Text);
          Directions.push_back(InlineDirection(pDir->bAbove, pDir->BeforeNote.first, pDir->BeforeNote.second, pDir->nType, Text));
#pragma message("\t-->\tUnknown Directions are no more moved to the beginning.")
        }
        else if (IsInRange(pDir->nType, MusicSheet::DIR_first_TempoText, MusicSheet::DIR_last_TempoText)
                 || pDir->nType == MusicSheet::DIR_SOUND || pDir->nType == MusicSheet::DIR_METRONOME)
        {
          // Use now.
          GetDirectionText(MT.Voices[pDir->BeforeNote.first], pDir->nType, Temp, pDir->Text, pDir->bAbove);
        }
        else if (IsInRange(pDir->nType, MusicSheet::DIR_first_Dynamic, MusicSheet::DIR_last_Dynamic) ||
                 IsInRange(pDir->nType, MusicSheet::DIR_first_Feeling, MusicSheet::DIR_last_Feeling))
        {
          MusicSheet::Voice & InVoice = pMeasure->Voices[pDir->BeforeNote.first];
          int					iCurNote = pDir->BeforeNote.second;

          // Get Position
          if (!InVoice.Notes.size())
          {
            // Use now if there is no note.
            GetDirectionText(MT.Voices[pDir->BeforeNote.first], pDir->nType, Temp, pDir->Text, pDir->bAbove);
          }
          else if (pDir->BeforeNote.second <= (int)InVoice.Notes.size())
          {
            int	iXPos = (pDir->BeforeNote.second < (int)InVoice.Notes.size()) ?
              InVoice.Notes[iCurNote].iXPos : InVoice.Notes[iCurNote - 1].iXPos;

            // Find Closest Note in all staffs and add it to the list.
            for ALL_INDICES(pMeasure->Voices, v)
              if (pMeasure->Voices[v].Notes.size())
              {
                int		n = 0;

                for ALL_INDICES(pMeasure->Voices[v].Notes, i)
                  if (abs(pMeasure->Voices[v].Notes[i].iXPos - iXPos) < // If in an chord, get the first one.
                      abs(pMeasure->Voices[v].Notes[n].iXPos - iXPos))
                    n = i;

                // If it is not under a rest, put it
                if (pMeasure->Voices[v].Notes[n].chStep != 'R')
                {
                  // If it is inside an chord, move to first:
                  while (n && (FOUND_IN_SET(pMeasure->Voices[v].Notes[n].Extras, MusicSheet::NE_CHORD_MIDDLE) ||
                               FOUND_IN_SET(pMeasure->Voices[v].Notes[n].Extras, MusicSheet::NE_CHORD_END)))
                    n--;

                  Directions.push_back(InlineDirection(pDir->bAbove, v, n, pDir->nType));
                }
                else
                  n = 1; // Skipped
              }
          }
          else
            _RPTF0(_CRT_ERROR, "Unexpected case.");
        }
        else
          _RPTF1(_CRT_ERROR, "Unexpected Direction: %i", pDir->nType);
    }
	}

	// Sort Bar Lines
	{
		for ALL_INDICES(pMeasure->BarLines, i)
			for DRANGE(j, i + 1, (int)pMeasure->BarLines.size())
				if (pMeasure->BarLines[i].nType > pMeasure->BarLines[j].nType)
					swap(pMeasure->BarLines[i], pMeasure->BarLines[j]);
	}

	// Notes
	for ALL_INDICES(pMeasure->Voices, iVoice)
	{
		MusicSheet::Voice & InVoice = pMeasure->Voices[iVoice];
		NarratedMusicSheet::Voice & OutVoice = MT.Voices[iVoice];

		CString	DirectionBufferText;

		// Start Bar Lines
		{
			for ALL(pMeasure->BarLines, pBL)
				if (pBL->nType < MusicSheet::BarLine::BL_LEFT_HAND_BARS)
					GetBarLineText(OutVoice, *pBL);
		}

		int		iInTuplet = 0;
		bool	bTubletStart = false;
		bool	bInChord = false;
		bool	bChordStart = false;
		bool	bChordEnd = false;
		bool	bTubletEnd = false;
		CString	LilyChordLength, *pLAL = NULL;
		bool	bChordIsArpeggio = false;
		bool	bInGrace = false;

		for ALL_INDICES(InVoice.Notes, i)
		{
			// Signature Change?
			if (uNextSignature < pMeasure->Signatures.size() &&
				pMeasure->Signatures[uNextSignature].BeforeNote.first == iVoice&&
				pMeasure->Signatures[uNextSignature].BeforeNote.second == i)
			{
				GetSignaturesText(OutVoice, pMeasure->Signatures[uNextSignature], InVoice.iStaff, pPreviousSignature);
				pPreviousSignature = &pMeasure->Signatures[uNextSignature++];
			}

			// Direction Before Note?
			for ALL(Directions, pDir)
				if (pDir->iVoice == iVoice && pDir->iNote == i)
					if (IsPreDirection(pDir->nType))
						GetDirectionText(OutVoice, pDir->nType, DirectionBufferText, pDir->ExtraText, pDir->bAbove);

			// Check Groupings
			// In a chord set, the first one doesn't have NE_CHORD, the rest have.
			bTubletStart = FOUND_IN_SET(InVoice.Notes[i].Extras, MusicSheet::NE_TUPLET_START);
			bTubletEnd = (iInTuplet == 1);
			bChordStart = FOUND_IN_SET(InVoice.Notes[i].Extras, MusicSheet::NE_CHORD_START);
			bInChord = FOUND_IN_SET(InVoice.Notes[i].Extras, MusicSheet::NE_CHORD_MIDDLE);
			bChordEnd = FOUND_IN_SET(InVoice.Notes[i].Extras, MusicSheet::NE_CHORD_END);

			if (bTubletStart)
			{
				OutVoice.Text.push_back(L"Tuplet[");
				iInTuplet = 0;
				while (i + iInTuplet < (int)InVoice.Notes.size() &&
					!FOUND_IN_SET(InVoice.Notes[i + iInTuplet].Extras, MusicSheet::NE_TUPLET_STOP))
					iInTuplet++;
				iInTuplet++;

				Temp.Format(L"\\tuplet %i/%i ", iInTuplet, iInTuplet - 1);
				OutVoice.Lily += Temp;
			}
			else if (bChordStart)
			{
				if (FOUND_IN_SET(InVoice.Notes[i].Extras, MusicSheet::NE_ARPEGGIATE))
				{
					OutVoice.Text.push_back(L"Arpeggiate[");
					bChordIsArpeggio = true;
				}
				else
				{
					OutVoice.Text.push_back(L"Chord[");
					bChordIsArpeggio = false;
				}
				pLAL = &LilyChordLength;
			}
						
			if (bChordStart || bTubletStart)
			{
				// If Chord is already added, just add a [ to it, otherwise add a new token for [
				/*if (OutVoice.Text.size() && 
					(	OutVoice.Text.back() == L"Chord" || 
						OutVoice.Text.back() == L"Arpeggiate" || 
						OutVoice.Text.back() == L"Tuplet"))
					OutVoice.Text.back() += L" [";
				else
					OutVoice.Text.push_back(L"[");*/
				if (bChordStart)
					OutVoice.Lily += L" < ";
				else
					OutVoice.Lily += L" { ";
			}

			GetNoteText(InVoice.Notes[i], OutVoice, iInTuplet-- > 0, bInGrace, pLAL);

			if (bInGrace)
				if (i + 1 == (int)InVoice.Notes.size() || !FOUND_IN_SET(InVoice.Notes[i + 1].Extras, MusicSheet::NE_GRACE))
				{
					OutVoice.Lily += L" } ";
					bInGrace = false;
				}

			// If current note has ARPEGGIATE and is in chord, remove the flag from the next ones. 
			if (FOUND_IN_SET(InVoice.Notes[i].Extras, MusicSheet::NE_ARPEGGIATE))
				if (bChordStart || bInChord)
					// For all next notes...
					for DRANGE(j, i + 1, (int)InVoice.Notes.size())
					{
						// If it has ARPEGGIATE, remove it.
						if (FOUND_IN_SET(InVoice.Notes[j].Extras, MusicSheet::NE_ARPEGGIATE))
							InVoice.Notes[j].Extras.erase(MusicSheet::NE_ARPEGGIATE);

						// If it is in this chord,...
						if (FOUND_IN_SET(InVoice.Notes[j].Extras, MusicSheet::NE_CHORD_END))
							break;
					}

			// Chord Continuem or End?
			if (iInTuplet > 0 || (bChordStart || bInChord))
				OutVoice.Text.push_back(L",");
			else if (bChordEnd)
			{
        OutVoice.Text.push_back(/*m_bDetailedText? (bChordIsArpeggio ? L"Arepggio_End" : L"Chord_End") : */L"]");
				OutVoice.Lily += L"> " + LilyChordLength;
				if (bChordIsArpeggio)
					OutVoice.Lily += L"\\arpeggio\r\n";
				pLAL = NULL;
				bChordEnd = false;						
			}
			else if (bTubletEnd)
			{
        OutVoice.Text.push_back(/*m_bDetailedText? L"Tuplet_End" : */L"]");
				OutVoice.Lily += L" }\r\n";
				bTubletEnd = false;
			}

			for ALL(Directions, pDir)
				if (pDir->iVoice == iVoice && pDir->iNote == i)
					if (!IsPreDirection(pDir->nType))
            GetDirectionText(OutVoice, pDir->nType, DirectionBufferText, pDir->ExtraText, pDir->bAbove);
		}

		for ALL(Directions, pDir)
			if (pDir->iVoice == iVoice && pDir->iNote == InVoice.Notes.size())
			{
				GetDirectionText(OutVoice, pDir->nType, DirectionBufferText, pDir->ExtraText, pDir->bAbove);
			}

		// Signature Change?
		if (uNextSignature < pMeasure->Signatures.size() &&
			pMeasure->Signatures[uNextSignature].BeforeNote.first == iVoice &&
			pMeasure->Signatures[uNextSignature].BeforeNote.second == (int)InVoice.Notes.size())
		{
			GetSignaturesText(OutVoice, pMeasure->Signatures[uNextSignature], InVoice.iStaff, pPreviousSignature);
			pPreviousSignature = &pMeasure->Signatures[uNextSignature++];
		}

		// End Bar Lines
		{
			for ALL(pMeasure->BarLines, pBL)
				if (pBL->nType > MusicSheet::BarLine::BL_LEFT_HAND_BARS)
					GetBarLineText(OutVoice, *pBL);
		}
	}

	// End Lilies
	for ALL(MT.Voices, pVoice)
		if (pVoice->Lily[0] != '!')
			pVoice->Lily += L"}\r\n";
		else
			pVoice->Lily = L"";

#ifdef _DEBUG
	for ALL(MT.Voices, pVoice)
		for ALL_INDICES(pVoice->Text, i)
			if (pVoice->Text[i].Left(6) == L"Chord")
				if (pVoice->Text[i].Find(L"Fermata") != -1 || (i + 1 < (int)pVoice->Text.size() && pVoice->Text[i + 1].Find(L"Fermata") != -1))
					AfxMessageBox(L"FOUND ONE CHORD CHANGE");
	//Text.Replace(L"Chord Fermata on" , L"Fermata on Chord") ;
#endif
	return MT;
}

// Converts the sheet into text
bool	CMusicSheetNarrator::ConvertToText(MusicSheet & Sheet, NarratedMusicSheet & Narration, bool bDetailed)
{
	try
	{
		m_bDetailedText = bDetailed;

		Narration.Clear();

		CString	Text;

		// Preprocess
		PreprocessSheet(Sheet);

		// Credits
		if (Sheet.Credits.size())
		{
			Text = L"";
			for ALL(Sheet.Credits, pCredit)
			{
				if (Text.GetLength())
					Text += L", ";
				Text += *pCredit;
			}
			Narration.Credits = Text;
		}

		// Movements
		for ALL(Sheet.Movements, pMovement)
		{
			NarratedMusicSheet::MovementText	PT;
			// Name
			{
        if (pMovement->Name.GetLength())
          PT.MovementName = pMovement->Name;
        else if (Sheet.Movements.size() > 1)
          PT.MovementName.Format(L"Movement_%i", (pMovement - Sheet.Movements.begin()) + 1);
				else
          PT.MovementName = L"";
			}
			MusicSheet::Signatures * pPreviousSignature = NULL;
			for ALL_INDICES(pMovement->Measures, i)
				PT.Measures.push_back(GetMeasureText(&pMovement->Measures[i], pPreviousSignature));
			Narration.Movements.push_back(PT);
		}

		PostprocessSheet(Sheet, Narration);

		return true;
	}
	catch (...)
	{
		return false;
	}
}

// Makes All necessary changes.
void	CMusicSheetNarrator::PreprocessSheet(MusicSheet & Sheet)
{
  for ALL(Sheet.Movements, pMovement)
	{
		vector<MusicSheet::Signatures>	LastSignatures;
		
		for ALL(pMovement->Measures, pMeasure)
		{
			// If Signatures are new, mark them new and keep them, otherwise copy previous ones.
			if (pMeasure->Signatures.size())
			{
				LastSignatures.clear();
				for ALL_INDICES(pMeasure->Signatures, s)
				{
					pMeasure->Signatures[s].bChangedInThisMeasure = true;
					LastSignatures.push_back(pMeasure->Signatures[s]);
					LastSignatures.back().bChangedInThisMeasure = false;
					LastSignatures.back().BeforeNote.second = 0;
				}

				while (LastSignatures.size() > 1)
					VEC_ERASE(LastSignatures, 0);
			}
			else
				pMeasure->Signatures = LastSignatures;

			// CONVERT NE_CHORD signs into NE_CHORD_START, NE_CHORD_MIDDLE, NE_CHORD_END
			for ALL_INDICES(pMeasure->Voices, v)
				for ALL_INDICES(pMeasure->Voices[v].Notes, n)
					if (FOUND_IN_SET(pMeasure->Voices[v].Notes[n].Extras, MusicSheet::NE_CHORD))
					{
						int start = n - 1;
						while (n + 1 < (int)pMeasure->Voices[v].Notes.size())
							if (!FOUND_IN_SET(pMeasure->Voices[v].Notes[n + 1].Extras, MusicSheet::NE_CHORD))
								break;
							else
								n++;
						int end = n;
						// Remove former NE_CHORDs
						for (int i = start + 1; i <= end; i++)
							pMeasure->Voices[v].Notes[i].Extras.erase(MusicSheet::NE_CHORD);
						// ADD NEW NE_CHORD_...s
						pMeasure->Voices[v].Notes[start].Extras.insert(MusicSheet::NE_CHORD_START);
						pMeasure->Voices[v].Notes[end].Extras.insert(MusicSheet::NE_CHORD_END);
						for (int i = start + 1; i < end; i++)
							pMeasure->Voices[v].Notes[i].Extras.insert(MusicSheet::NE_CHORD_MIDDLE);
					}

			// Merge Metronome Directions
			for ALL_INDICES(pMeasure->Directions, i)
				if (pMeasure->Directions[i].nType == MusicSheet::DIR_METRONOME)
					for (int j = i + 1; j < (int)pMeasure->Directions.size(); j++)
						if (pMeasure->Directions[j].nType == MusicSheet::DIR_METRONOME)
						{
							vector<CString> Tokens1 = TokenizeText(pMeasure->Directions[i].Text);
							vector<CString> Tokens2 = TokenizeText(pMeasure->Directions[j].Text);
							if (Tokens1.size() == 2 && Tokens2.size() == 2 && Tokens1[0] == Tokens2[0])
							{ 
								if (_wtoi(Tokens1[1]) > _wtoi(Tokens2[1]))
									swap(Tokens1[1], Tokens2[1]);

								CString	Temp;
								Temp.Format(L"%s|%s-%s", Tokens1[0], Tokens1[1], Tokens2[1]);
								pMeasure->Directions[i].Text = Temp;
								VEC_ERASE(pMeasure->Directions, j);
							}								
							break;
						}

			// Merge Text Directions at the same position, clean the rest
			for ALL_INDICES(pMeasure->Directions, i)
			{
				MusicSheet::Direction * pDir1 = &pMeasure->Directions[i];
				if (pDir1->nType == MusicSheet::DIR_UNKNWON)
				{
					for (MusicSheet::Direction * pDir2 = pDir1 + 1; pDir2 <= & pMeasure->Directions.back(); pDir2++)
						if (pDir2->nType == MusicSheet::DIR_UNKNWON &&
							pDir1->iStaff == pDir2->iStaff &&
							pDir1->iVoice == pDir2->iVoice &&
							pDir1->BeforeNote == pDir2->BeforeNote &&
							pDir1->bAbove == pDir2->bAbove)
						{
							pDir1->Text += L" ";
							pDir1->Text += pDir2->Text;
							VEC_ERASE(pMeasure->Directions, pDir2 - &*pMeasure->Directions.begin());
							pDir2--;
						}
					while (pDir1->Text.Find(L"  ") != -1)
						pDir1->Text.Replace(L"  ", L" ");
					pDir1->Text.Replace(L"()", L"");
					pDir1->Text.Replace(L"( )", L"");
					pDir1->Text = pDir1->Text.Trim(' ');
				}
			}

			// Remove dynamics and feelings repeated in other staffs, as they will be told for all staffs.
			for ALL(pMeasure->Directions, pDir)
				if (IsInRange(pDir->nType, MusicSheet::DIR_first_Dynamic, MusicSheet::DIR_last_Dynamic) ||
					IsInRange(pDir->nType, MusicSheet::DIR_first_Feeling, MusicSheet::DIR_last_Feeling))
					for (int j = VEC_INDEX(pDir, pMeasure->Directions) + 1; j < (int)pMeasure->Directions.size(); j++)
						if (pMeasure->Directions[j].nType == pDir->nType)
							VEC_ERASE(pMeasure->Directions, j--);

      // Convert finger directions to finger notation in notes, transfer all related to Chords to the end, and sort them there!
      map < pair<int, int>, vector<MusicSheet::DirectionTypes>> Fingers;
        
      // Assgin all finger directives to notes, transfer chord fingers to the end.
      for ALL_INDICES(pMeasure->Directions, dir_index) {
        if (IsInRange(pMeasure->Directions[dir_index].nType, MusicSheet::DIR_first_Finger, MusicSheet::DIR_last_Finger))
        {
          MusicSheet::Voice & InVoice = pMeasure->Voices[pMeasure->Directions[dir_index].BeforeNote.first];
          auto key = pMeasure->Directions[dir_index].BeforeNote;

          while (key.second < (int)InVoice.Notes.size() &&
            (FOUND_IN_SET(InVoice.Notes[key.second].Extras, MusicSheet::NE_CHORD_START) ||
             FOUND_IN_SET(InVoice.Notes[key.second].Extras, MusicSheet::NE_CHORD_MIDDLE)))
            key.second++;

          if (key.first < (int)pMeasure->Voices.size() && key.second < (int)pMeasure->Voices[key.first].Notes.size()) {
            if (Fingers.find(key) == Fingers.end()) {
              vector<MusicSheet::DirectionTypes> new_set;
              Fingers.insert(make_pair(key, new_set));
            }
            Fingers.find(key)->second.push_back(pMeasure->Directions[dir_index].nType);
          }
          else {
            _RPTF2(_CRT_ERROR, "Unexpected finger position %i,%i", key.first, key.second);
          }

          VEC_ERASE(pMeasure->Directions, dir_index--);
        }
      }

      // Sort fingers in each chord and assign them to their note.
      for (auto& item : Fingers) {
        auto& key = item.first;
        auto& fingers = item.second;

        for ALL_INDICES(fingers, i) {
          for DRANGE(j, i + 1, (int)fingers.size()) {
            if (fingers[i] > fingers[j])
              swap(fingers[i], fingers[j]);
          }
          pMeasure->Voices[key.first].Notes[key.second].Fingers.push_back(fingers[i]);
        }
      }

      // If a note has more than one finger, and its in a chord, move them all to the end or distribute them evenly.
      for (auto &voice : pMeasure->Voices)
        for ALL_INDICES(voice.Notes, iNote)
          // If a note has more than 1 finger
          if (voice.Notes[iNote].Fingers.size() > 1) {
            // And it is not last note of the chord, but in a chord
            if (NOTE_IS_CHORD_START(voice.Notes[iNote]) || NOTE_IS_CHORD_MIDDLE(voice.Notes[iNote])) {
              // Find the first note in chord.
              while (iNote && NOTE_IS_CHORD_MIDDLE(voice.Notes[iNote]))
                iNote--;

              int iChordStart = iNote;
              // Collect all chord fingers.
              vector<MusicSheet::DirectionTypes> Fingers;
              while(true)
              {
                for (auto& finger : voice.Notes[iNote].Fingers)
                  Fingers.push_back(finger);
                voice.Notes[iNote].Fingers.clear();

                if (!NOTE_IS_CHORD_END(voice.Notes[iNote]))
                  iNote++;
                else
                  break;                  
              }
              // Add them all to the end.
              sort(Fingers.begin(), Fingers.end());
              if (Fingers.size() == iNote - iChordStart + 1) {
                for (int i = iChordStart; i <= iNote; i++)
                  voice.Notes[i].Fingers.push_back(Fingers[i - iChordStart]);
              }
              else
                voice.Notes[iNote].Fingers = Fingers;
            }
          }

		} // Measure
	} // Movement
}

// Makes all post narration changes.
void	CMusicSheetNarrator::PostprocessSheet(MusicSheet & Sheet, NarratedMusicSheet & Narration)
{
	for ALL(Narration.Movements, pMovement)
	{
		set<pair<int, int>>	Voices;
		for ALL(pMovement->Measures, pMeasure)
			for ALL(pMeasure->Voices, pVoice)
			{
				Voices.insert(make_pair(pVoice->iStaff, pVoice->iVoice));

				vector<CString> & pNotes = pVoice->Text;
				for ALL_INDICES(pNotes, n)
					if (pNotes[n] == L"," && n && n + 1 < (int)pNotes.size())
					{
						pNotes[n - 1] += CString(L", ") + pNotes[n + 1];
						VEC_ERASE(pNotes, n);
						VEC_ERASE(pNotes, n--);
					}
					else if (pNotes[n] == L"[" && n + 1 < (int)pNotes.size())
					{
						pNotes[n] = pNotes[n] + pNotes[n + 1];
						VEC_ERASE(pNotes, n + 1);
						n--;
					}
					else if (pNotes[n] == L" ]" && n)
					{
						pNotes[n - 1] = pNotes[n - 1] + L" ]";
						VEC_ERASE(pNotes, n--);
					}
			}


		// Put something in Empty Voices.
		for (int v = 0; v < (int)Voices.size(); v++)
			for ALL(pMovement->Measures, pMeasure)
				if (!pMeasure->Voices[v].Text.size())
				{
					CString Text, MText;
					int	iNextFull = -1;
					for (decltype(pMeasure) pNext = pMeasure; pNext != pMovement->Measures.end(); pNext++)
						if (pNext->Voices[v].Text.size())
						{
							iNextFull = VEC_INDEX(pNext, pMovement->Measures);
							break;
						}

					if (iNextFull == -1)
						Text = L"Silence_till_end_of_part";
					else if (iNextFull == VEC_INDEX(pMeasure, pMovement->Measures) + 1)
						Text = L"Silence";
					else
						Text.Format(L"Silence_till_Measure_%i", iNextFull);

					for (decltype(pMeasure) pNext = pMeasure; pNext != pMovement->Measures.end(); pNext++)
						if (pNext->Voices[v].Text.size())
							break;
						else
						{
							MText.Format(L"Measure: %i", VEC_INDEX(pNext, pMovement->Measures) + 1);
							pNext->Voices[v].Text.push_back(MText);
							pNext->Voices[v].Text.push_back(Text);
						}
				}
	}

  // Check if movements need to be merged.
  if (!Narration.similar_movements && Sheet.Movements.size() > 1) {
    Narration.similar_movements = true;
    for (auto&movement : Sheet.Movements)
      Narration.similar_movements &= (movement.Measures.size() == Sheet.Movements[0].Measures.size());
  }
}