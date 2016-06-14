#include "StdAfx.h"
#include "MusicSheetNarrator.h"

#define EOS "; "
CMusicSheetNarrator::CMusicSheetNarrator(void)
{
}


CMusicSheetNarrator::~CMusicSheetNarrator(void)
{
}

CStringA	CMusicSheetNarrator::GetNoteTypeName(MusicSheet::_Note & Note)
{
	CStringA	Text("");
	
	// Grace
	if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_GRACE))
		Text += "Grace";
	else
	{
		// Accents
		{
			if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_STACCATO))
				Text += "Staccato_";

			if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_ACCENT))
				Text += "Marcato_";

			if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_STRONG_ACCENT))
				Text += "Martellato_";

			if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_GRUPPETTO))
				Text += "Gruppetto_";
		}

		switch (Note.Type)
		{
		case	MusicSheet::TYPE_WHOLE:		Text += "Whole";	break;
		case	MusicSheet::TYPE_HALF:		Text += "Half";	break;
		case	MusicSheet::TYPE_QUARTER:	Text += "Quarter";	break;
		case	MusicSheet::TYPE_EIGHTH:	Text += "Eighth";	break;
		case	MusicSheet::TYPE_SIXTEENTH:	Text += "Sixteenth"; break;
		case	MusicSheet::TYPE_32ND:		Text += "32nd"; break;
		case	MusicSheet::TYPE_64TH:		Text += "64th"; break;
		case	MusicSheet::TYPE_128TH:		Text += "128th"; break;
		case	MusicSheet::TYPE_256TH:		Text += "256th"; break;
		case	MusicSheet::TYPE_UNTOLD:	Text += "Whole_Bar"; break;
		default:							Text += "UNKNOWN";	break;
		}

		// Dot
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_DOT))
			Text += "_Dotted";

		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_DOUBLE_DOT))
			Text += "_Double_Dotted";
	}

	return Text;
}

void	CMusicSheetNarrator::GetNoteText(MusicSheet::_Note & Note, NarratedMusicSheet::Voice & Voice, bool bInTuplet, CStringA * pChordLength)
{
	CStringA	Text, Temp;

	// Main Text
	if (Note.chStep == 'R')
	{
		Text = GetNoteTypeName(Note);
		Text += "_Rest";
	}
	else
	{
		// Fermata
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_FERMATA))
			Text = "Fermata_on_ ";
		else
			Text = "";

		Text += Note.chStep;

		switch (Note.chAccidental)
		{
		case 'S':	Text += "#"; break;
		case 'F':	Text += "b"; break;
		case 'N':	Text += "n"; break;
		case 0:	break;
		default:	Text += "UNKNOWN_ACCENT"; break;
		}

		if (Note.bAccdidentalDouble)
			Text += "2";

		Temp.Format("_%i", Note.iOctave);
		Text += Temp;

		//if (!bInTuplet)
		{
			Text += L"_";
			Text += GetNoteTypeName(Note);
			Text += L"_Note";
		}
		/*else
			Text += L" "*/;

		// Slur-End
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_SLUR_END))
			Text += "; End_Legato_Line";
		
		// Tied-End
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_TIED_END))
			Text += "; End_Tied_Line";

		// Slur-Start
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_SLUR_START))
			Text += "; Start_Legato_Line";

		// Tied-Start
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_TIED_START))
			Text += "; Start_Tied_Line";
	}
	Voice.Text.push_back(Text);

	// Lily
	{
		Text = (char)tolower(Note.chStep);
		for (int i = 0; i < (Note.bAccdidentalDouble ? 2 : 1); i++)
			switch (Note.chAccidental)
			{
			case 'S':	Text += "is"; break;
			case 'F':	Text += "es"; break;
			//case 'N':	Text += "!"; break;
			}

		if (Note.iOctave != -1)
			if (Note.iOctave > 3)
				for (int i = 3; i < Note.iOctave; i++)
					Text += "'";
			else
				for (int i = 3; i > Note.iOctave; i--)
					Text += ",";

		switch (Note.Type)
		{
		case	MusicSheet::TYPE_WHOLE:		Temp = "1"; break;
		case	MusicSheet::TYPE_HALF:		Temp = "2"; break;
		case	MusicSheet::TYPE_QUARTER:	Temp = "4"; break;
		case	MusicSheet::TYPE_EIGHTH:	Temp = "8"; break;
		case	MusicSheet::TYPE_SIXTEENTH:	Temp = "16"; break;
		case	MusicSheet::TYPE_32ND:		Temp = "32"; break;
		case	MusicSheet::TYPE_64TH:		Temp = "64"; break;
		case	MusicSheet::TYPE_128TH:		Temp = "128"; break;
		case	MusicSheet::TYPE_256TH:		Temp = "256"; break;
		default:							Temp = "\\maxima"; break;
		}		
		// Dot
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_DOT))
			Temp += ".";

		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_DOUBLE_DOT))
			Temp += "..";

		if (pChordLength)
			*pChordLength = Temp;
		else
			Text += Temp;

		// Fermata
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_FERMATA))
			Text += "\\fermata";
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_STACCATO))
			Text += "\\staccato";

		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_ACCENT))
			Text += "\\accent";

		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_STRONG_ACCENT))
			Text += "\\accent";

		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_GRUPPETTO))
			Text += "\\turn";
		
		// Grace
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_GRACE))
			_RPTF1(_CRT_WARN, "Lily misses %s", "Grace");

		// Texts should come after a note!
		if (Voice.PendingLilyText.GetLength())
		{
			Text += Voice.PendingLilyText;
			Voice.PendingLilyText = "";
		}

		// Slur-End
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_SLUR_END))
			Text += ")";

		//// Tied-End
		//if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_TIED_END))
		//	Text += "; End_Tied_Line";

		// Slur-Start
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_SLUR_START))
			Text += "(";

		// Tied-Start
		if (FOUND_IN_SET(Note.Extras, MusicSheet::NE_TIED_START))
			Text += "~";
	}
	Voice.Lily += Text + "\r\n";
}

void	CMusicSheetNarrator::GetSignaturesText(MusicSheet::Signatures & Sigs, int iStaff, MusicSheet::Signatures * pPreviousSignature, NarratedMusicSheet::Voice & Voice)
{
	CStringA	Text(""), T1, T2, T3;
	bool	bItemChanged;

	// Clefs	
	if (iStaff < (int)Sigs.Clefs.size() && Sigs.Clefs[iStaff].Sign.GetLength())
	{
		bItemChanged = pPreviousSignature && (pPreviousSignature->Clefs[iStaff].Sign != Sigs.Clefs[iStaff].Sign || pPreviousSignature->Clefs[iStaff].iLine != Sigs.Clefs[iStaff].iLine);

		Text = CStringA((pPreviousSignature && !bItemChanged) ? "*" : "") + "Clef";

		if (pPreviousSignature && bItemChanged)
			Text += "_Changes_To: ";
		else
			Text += ": ";

		int iLine = Sigs.Clefs[iStaff].iLine;
		T1 = T2 = Sigs.Clefs[iStaff].Sign;

		if (T1 == "G" && iLine == 2)
		{
			T1 = "Treble";
			T2 = "treble";
		}
		else if(T1 == "G" && iLine == 1)
		{
			T1 = "French";
			T2 = "french";
		}
		else if (T1 == "F" && iLine == 4)
		{
			T1 = "Bass";
			T2 = "bass";
		}
		else if (T1 == "F" && iLine == 3)
		{
			T1 = "Baritone";
			T2 = "baritonevarF";
		}
		else if (T1 == "F" && iLine == 5)
		{
			T1 = "Subbass";
			T2 = "subbass";
		}
		else if (T1 == "C" && iLine == 3)
		{
			T1 = "Alto";
			T2 = "alto";
		}
		else if (T1 == "C" && iLine == 4)
		{
			T1 = "Tenor";
			T2 = "tenor";
		}
		else if (T1 == "C" && iLine == 5)
		{
			T1 = "Baritone";
			T2 = "baritone";
		}
		else if (T1 == "C" && iLine == 2)
		{
			T1 = "Mezzosoprano";
			T2 = "mezzosoprano";
		}
		else if (T1 == "C" && iLine == 1)
		{
			T1 = "Soprano";
			T2 = "soprano";
		}
		else if (T1 == "percussion" && iLine == 1)
		{
			T1 = "Percussion";
			T2 = "percussion";
		}
		else if (T1 == "TAB" && iLine == 1)
		{
			T1 = "Tablature";
			T2 = "\\new TabStaff {\\clef tab }";
		}
		// If cound not translate it, show text and line, otherwise the new name only.
		//if (T1 == Sigs.Clefs[iStaff].Sign)
			T3.Format("%s_Line_%i", T1, iLine);
		//else
		//	T3 = T1;
		Text += T3;
		Voice.Text.push_back(Text);

		T3.Format("\\clef %s\r\n", T2);
		Voice.Lily += T3;
	}

	// Key Signature
	if (Sigs.Key.iFifths != -1)
	{
		bItemChanged = pPreviousSignature && (pPreviousSignature->Key.iFifths != Sigs.Key.iFifths || pPreviousSignature->Key.bMajor != Sigs.Key.bMajor);
	
		Text = CStringA((pPreviousSignature && !bItemChanged) ? "*" : "" ) + "Key_Signature";

		if (pPreviousSignature && bItemChanged)
			Text += "_Changes_To: ";
		else
			Text += ": ";

		T1 = "";
		switch (Sigs.Key.iFifths)
		{
		case 0:
			T1 = Sigs.Key.bMajor ? "C" : "A";
			break;
		case 7:	T1 = "_#C";
		case 6:	T1 = "_E#" + T1;
		case 5:	T1 = "_A#" + T1;
		case 4:	T1 = "_D#" + T1;
		case 3:	T1 = "_G#" + T1;
		case 2:	T1 = "_C#" + T1;
		case 1:	T1 = "F#" + T1;	
			break;		
		case -7:	T1 = "_Fb";
		case -6:	T1 = "_Cb";
		case -5:	T1 = "_Gb";		
		case -4:	T1 = "_Db";			
		case -3:	T1 = "_Ab";			
		case -2:	T1 = "_Eb";			
		case -1:	T1 = "Bb";
			break;
		
		default:	T1 += "UNKNOWN_KEY";
		}

		Voice.Text.push_back(Text + T1 + "_" + (Sigs.Key.bMajor? "Major" : "Minor"));

		// LILY
		switch (Sigs.Key.iFifths)
		{
		case 0:	T1 = Sigs.Key.bMajor ? "c" : "a"; break;
		case 1:	T1 = Sigs.Key.bMajor ? "g" : "e"; break;
		case 2:	T1 = Sigs.Key.bMajor ? "d" : "b"; break;
		case 3:	T1 = Sigs.Key.bMajor ? "a" : "fis"; break;
		case 4:	T1 = Sigs.Key.bMajor ? "e" : "cis"; break;
		case 5:	T1 = Sigs.Key.bMajor ? "b" : "gis"; break;
		case 6:	T1 = Sigs.Key.bMajor ? "fis" : "dis"; break;
		case 7:	T1 = Sigs.Key.bMajor ? "cis" : "dis"; break;
		case -1:	T1 = Sigs.Key.bMajor ? "f" : "d"; break;
		case -2:	T1 = Sigs.Key.bMajor ? "bes" : "g"; break;
		case -3:	T1 = Sigs.Key.bMajor ? "ees" : "c"; break;
		case -4:	T1 = Sigs.Key.bMajor ? "aes" : "f"; break;
		case -5:	T1 = Sigs.Key.bMajor ? "des" : "bes"; break;
		case -6:	T1 = Sigs.Key.bMajor ? "ges" : "ees"; break;
		case -7:	T1 = Sigs.Key.bMajor ? "ces" : "aes"; break;
		default:	T1 = "";
		}

		if (T1.GetLength())
		{
			T2.Format("\\key %s \\%s\r\n", T1, Sigs.Key.bMajor ? "major" : "minor");
			Voice.Lily += T2;
		}
		
	}

	// Time Signature
	if (Sigs.Time.iBeats && Sigs.Time.iBeatType)
	{
		bItemChanged = pPreviousSignature && (pPreviousSignature->Time.iBeats != Sigs.Time.iBeats || pPreviousSignature->Time.iBeatType != Sigs.Time.iBeatType);

		Text = CStringA((pPreviousSignature && !bItemChanged) ? "*" : "") + "Time_Signature";

		if (pPreviousSignature && bItemChanged)
			Text += "_Changes_To: ";
		else
			Text += ": ";

		T1.Format("%i_%i", Sigs.Time.iBeats, Sigs.Time.iBeatType);
		Text += T1;
		Voice.Text.push_back(Text);
		
		T1.Format("\\time %i/%i\r\n", Sigs.Time.iBeats, Sigs.Time.iBeatType);
		Voice.Lily += T1;
	}
}

bool	IsPreDirection(MusicSheet::DirectionTypes nDirType)
{
	return nDirType == MusicSheet::DIR_SOUND || nDirType == MusicSheet::DIR_METRONOME ||
		IsInRange(nDirType, MusicSheet::DIR_first_Dynamic, MusicSheet::DIR_last_Dynamic) ||
		IsInRange(nDirType, MusicSheet::DIR_first_Feeling, MusicSheet::DIR_last_Feeling);
}

vector<CStringA> TokenizeText(CStringA Text)
{
	vector<CStringA>	Result;
	int		iCurPos = 0;
	CStringA	Token = Text.Tokenize("|", iCurPos);
	while (Token != L"")
	{
		Result.push_back(Token);
		Token = Text.Tokenize("|", iCurPos);
	}
	return Result;
}

CStringA	CMusicSheetNarrator::GetDirectionText(MusicSheet::DirectionTypes nDirType, CStringA & BufferText, MusicSheet::DirectionTypes nPreviousDir, CStringA ExtraText)
{
	CStringA	Text("");

	if (IsInRange(nDirType, MusicSheet::DIR_first_TempoText, MusicSheet::DIR_last_TempoText))
		Text = "Tempo_Text: ";

	if (IsInRange(nDirType, MusicSheet::DIR_first_Dynamic, MusicSheet::DIR_last_Dynamic))
		Text += "Dynamic: ";

	switch (nDirType)
	{
	case MusicSheet::DIR_DOLCE:			Text += "Dolce"; break;
	case MusicSheet::DIR_MESTO:			Text += "Metso"; break;
	case MusicSheet::DIR_A_TEMPO:		Text += "A_Tempo"; break;
	case MusicSheet::DIR_POCO_RIT:		Text += "Poco_Ritardtando"; break;
	case MusicSheet::DIR_POCO_RALL:		Text += "Poco_Rall"; break;
	case MusicSheet::DIR_SEMPRE:		Text += "Sempre"; break;
	case MusicSheet::DIR_ACCEL:			Text += "Accel"; break;
	case MusicSheet::DIR_TRILL:			Text += "Trill"; break;
	case MusicSheet::DIR_GRUPPETTO:		Text += "Gruppetto"; break;
	case MusicSheet::DIR_MORDENT:		Text += "Mordent"; break;
	case MusicSheet::DIR_MODERATO:		Text += "Moderato"; break;
	case MusicSheet::DIR_LARGO:			Text += "Largo"; break;
	case MusicSheet::DIR_GRAVE:			Text += "Grave"; break;
	case MusicSheet::DIR_ALLEGRO:		Text += "Allegro"; break;
	case MusicSheet::DIR_ALLEGRATO:		Text += "Allegrato"; break;

	case MusicSheet::DIR_DYNAMIC_P:		Text += "Piano"; break;
	case MusicSheet::DIR_DYNAMIC_PP:	Text += "Pianissimo"; break;
	case MusicSheet::DIR_DYNAMIC_PPP:	Text += "Pianississimo"; break;
	case MusicSheet::DIR_DYNAMIC_MP:	Text += "Mezzo_Piano"; break;
	case MusicSheet::DIR_DYNAMIC_MF:	Text += "Mezzo_Forte"; break;
	case MusicSheet::DIR_DYNAMIC_F:		Text += "Forte"; break;
	case MusicSheet::DIR_DYNAMIC_FP:	Text += "Forte_Piano"; break;
	case MusicSheet::DIR_DYNAMIC_FZ:	Text += "Forzando"; break;
	case MusicSheet::DIR_DYNAMIC_FF:	Text += "Fortessimo"; break;
	case MusicSheet::DIR_DYNAMIC_FFF:	Text += "Fortessissimo"; break;
	case MusicSheet::DIR_DYNAMIC_SFZ:	Text += "Sforzando"; break;
	case MusicSheet::DIR_DYNAMIC_DIM:	Text += "Diminuendo"; break;
	case MusicSheet::DIR_DYNAMIC_DECRESC:	Text += "Decrescendo"; break;
	case MusicSheet::DIR_DYNAMIC_CRESC:		Text += "Cresc"; break;
	case MusicSheet::DIR_DYNAMIC_STACCATO:	Text += "Staccato"; break;
	case MusicSheet::DIR_DYNAMIC_LEGATO:	Text += "Legato"; break;

	case MusicSheet::DIR_WEDGE_DIMINUENDO:	Text += "Start_Diminuendo"; BufferText = "Diminuendo"; break;
	case MusicSheet::DIR_WEDGE_STOP:		Text += CStringA("Stop_") + BufferText; break;
	case MusicSheet::DIR_WEDGE_CRESCENDO:	Text += "Start_Crescendo"; BufferText = "Crescendo"; break;

	case MusicSheet::DIR_FINGER_0:			Text += "Open_Handed"; break;
	case MusicSheet::DIR_FINGER_1:			Text += IsInRange(nPreviousDir, MusicSheet::DIR_first_Finger, MusicSheet::DIR_last_Finger) ? "1" : "Finger_1"; break;
	case MusicSheet::DIR_FINGER_2:			Text += IsInRange(nPreviousDir, MusicSheet::DIR_first_Finger, MusicSheet::DIR_last_Finger) ? "2" : "Finger_2"; break;
	case MusicSheet::DIR_FINGER_3:			Text += IsInRange(nPreviousDir, MusicSheet::DIR_first_Finger, MusicSheet::DIR_last_Finger) ? "3" : "Finger_3"; break;
	case MusicSheet::DIR_FINGER_4:			Text += IsInRange(nPreviousDir, MusicSheet::DIR_first_Finger, MusicSheet::DIR_last_Finger) ? "4" : "Finger_4"; break;
	case MusicSheet::DIR_FINGER_5:			Text += IsInRange(nPreviousDir, MusicSheet::DIR_first_Finger, MusicSheet::DIR_last_Finger) ? "5" : "Finger_5"; break;

	case MusicSheet::DIR_FINGER_I:			Text += "I"; break;
	case MusicSheet::DIR_FINGER_M:			Text += "M"; break;
	case MusicSheet::DIR_FINGER_A:			Text += "A"; break;
	case MusicSheet::DIR_FINGER_P:			Text += "P"; break;

	case MusicSheet::DIR_OCTAVE_SHIFT_DOWN:	Text += "Octave_Shift_Down"; break;
	case MusicSheet::DIR_OCTAVE_SHIFT_UP:	Text += "Octave_Shift_Up"; break;
	case MusicSheet::DIR_OCTAVE_SHIFT_STOP:	Text += "Octave_Shift_Stop"; break;

	case MusicSheet::DIR_SOUND:
	{
		vector<CStringA> Tokens = TokenizeText(ExtraText);
		if (Tokens.size() == 2)
		{
			Text += CStringA("Sound");
			if (Tokens[0] != "(null)")
				Text += "_Tempo_=_" + Tokens[0];
			if (Tokens[1] != "(null)")
				Text += "_Dynamic_=_" + Tokens[1];
		}
	}
	break;

	case MusicSheet::DIR_METRONOME:
	{
		vector<CStringA> Tokens = TokenizeText(ExtraText);
		if (Tokens.size() == 2)
		{
			CStringA Temp;
			Temp.Format("Metronome_%s_=_%s", Tokens[0], Tokens[1]);
			Text += Temp;
		}
	}
	break;

	default:								
		Text += "UNKNOWN_DIRECTION"; break;
	}

	return Text;
}

CStringA	ConvertNumberToText(int i)
{
	switch (i)
	{
	case 1:	return "First";
	case 2:	return "Second";
	case 3:	return "Third";
	case 4:	return "Forth";
	default:
		CStringA A;
		A.Format("%i", i);
		return A;
	}
}

CStringA	CMusicSheetNarrator::GetBarLineText(MusicSheet::BarLine & BL)
{
	switch (BL.nType)
	{
	case MusicSheet::BarLine::BL_START:
		return "Begin_Bar_Line";

	case MusicSheet::BarLine::BL_FORWARD:
		return "Begin_Repeat";

	case MusicSheet::BarLine::BL_VOLTA_START:
		return ConvertNumberToText(BL.iVoltaNumber) + "_Volta_Start";

	case MusicSheet::BarLine::BL_END:
		return "End_Bar_Line";

	case MusicSheet::BarLine::BL_BACKWARD:
		return "End_Repeat";

	case MusicSheet::BarLine::BL_VOLTA_END:
		return ConvertNumberToText(BL.iVoltaNumber) + "_Volta_End";

	default:
		return "Unknown_Bar_Line";
	}
}

typedef struct _InlineDirection
{
	int							iVoice, iNote;
	MusicSheet::DirectionTypes	nType;
	CStringA					ExtraText;
	_InlineDirection(int iVoice, int iNote, MusicSheet::DirectionTypes nType, CStringA ExtraText = "")
	{
		this->iVoice = iVoice;
		this->iNote = iNote;
		this->nType = nType;
		this->ExtraText = ExtraText;
	}
} InlineDirection;

NarratedMusicSheet::MeasureText	CMusicSheetNarrator::GetMeasureText(MusicSheet::_Measure * pMeasure, MusicSheet::Signatures * & pPreviousSignature)
{
	NarratedMusicSheet::MeasureText	MT;

	CStringA	Temp;
	unsigned	uNextSignature = 0;

	// Init all Voices
	{
		Temp.Format("Measure: %i", pMeasure->iNumber + 1);
		for ALL(pMeasure->Voices, pVoice)
		{
			NarratedMusicSheet::Voice NewVoice;
			NewVoice.iStaff = pVoice->iStaff;
			NewVoice.iVoice = 1;
			NewVoice.Lily = MT.Voices.size() ? "\\new Staff\r\n" : "";
			NewVoice.Lily += "{\r\n";

			for ALL(MT.Voices, pInVoice)
				if (pInVoice->iStaff == NewVoice.iStaff)
					NewVoice.iVoice++;
			NewVoice.Text.push_back(Temp);
			MT.Voices.push_back(NewVoice);
		}
	}
	
	// Tell the first signatures before the first measure
	if (pMeasure->Signatures.size() && !pMeasure->Signatures[0].BeforeNote.second)
	{
		for ALL_INDICES(pMeasure->Voices, i)
			GetSignaturesText(
				pMeasure->Signatures[uNextSignature], 
				pMeasure->Voices[i].iStaff, 
				pPreviousSignature, 
				MT.Voices[i]);
		pPreviousSignature = &pMeasure->Signatures[uNextSignature++];
	}

	// Preprocess Directives
	vector<InlineDirection>	Directions;
	{
		// Reorder Fingers
		{
			// Transfer Chord Fingers to the end.
			for ALL(pMeasure->Directions, pDir)
				if (IsInRange(pDir->nType, MusicSheet::DIR_first_Finger, MusicSheet::DIR_last_Finger))
				{
					MusicSheet::Voice & InVoice	= pMeasure->Voices[pDir->BeforeNote.first];
					int					iNote		= pDir->BeforeNote.second;

					if (iNote >= (int)InVoice.Notes.size())
						continue;

					bool	bInChord = (FOUND_IN_SET(InVoice.Notes[iNote].Extras, MusicSheet::NE_CHORD));

					if (!bInChord && iNote + 1 < (int)InVoice.Notes.size())
						bInChord = (FOUND_IN_SET(InVoice.Notes[iNote + 1].Extras, MusicSheet::NE_CHORD));

					if (bInChord)
						while (++iNote < (int)InVoice.Notes.size())
							if (FOUND_IN_SET(InVoice.Notes[iNote].Extras, MusicSheet::NE_CHORD))
								pDir->BeforeNote.second++;
							else
								break;
				}

			// Sort Fingers in one Chord
			for ALL(pMeasure->Directions, pDir)
				if (IsInRange(pDir->nType, MusicSheet::DIR_first_Finger, MusicSheet::DIR_last_Finger))
				{
					vector<MusicSheet::DirectionTypes>	Fingers;

					for (DECLEQ(pNexts, pDir); pNexts < pMeasure->Directions.end(); pNexts++)
						if (IsInRange(pNexts->nType, MusicSheet::DIR_first_Finger, MusicSheet::DIR_last_Finger) && 
							pNexts->iStaff == pDir->iStaff && pNexts->BeforeNote == pDir->BeforeNote)
							Fingers.push_back(pNexts->nType);
						else
							break;

					if (Fingers.size() > 1)
					{
						if (pDir->iStaff)
						{
							for ALL_INDICES(Fingers, i)
								for DRANGE(j, i + 1, (int)Fingers.size())
									if (Fingers[i] < Fingers[j])
										swap(Fingers[i], Fingers[j]);
						}
						else
						{
							for ALL_INDICES(Fingers, i)
								for DRANGE(j, i + 1, (int)Fingers.size())
									if (Fingers[i] > Fingers[j])
										swap(Fingers[i], Fingers[j]);
						}

						for ALL_INDICES(Fingers, i)
							(pDir++)->nType = Fingers[i];

						if (pDir == pMeasure->Directions.end())
							break;
					}
				}
		}

		CStringA	GuitarFingers = "";

		for ALL(pMeasure->Directions, pDir)
			if (pDir->nType == MusicSheet::DIR_UNKNWON)
			{
				// Use now.

				// Check if its fingers for guitar
				if (m_bProcessingGuitar)
				{
					CStringA	TempText = pDir->Text;

					TempText.MakeUpper();
					for RANGE(i, 5)
						TempText.Remove("IMAP "[i]);

					if (!TempText.GetLength())
					{
						pDir->Text.Remove(' ');
						GuitarFingers += pDir->Text;
						pDir->Text = "";
					}
				}

				if (pDir->Text.GetLength())
				{
					MT.Voices[0].Text.push_back(pDir->Text);	//MT.Voices[0].Text.push_back(CStringA("\"") + pDir->Text + CStringA("\""));
					Temp.Format("%s\\markup {%s} ", pDir->bAbove ? "^" : "_", pDir->Text);
					MT.Voices[0].PendingLilyText += Temp;
				}
			}
			else if (m_bProcessingGuitar && IsInRange(pDir->nType, MusicSheet::DIR_first_guitar_Finger, MusicSheet::DIR_last_guitar_Finger))
				GuitarFingers += GetDirectionText(pDir->nType, Temp);
			else if (IsInRange(pDir->nType, MusicSheet::DIR_first_TempoText, MusicSheet::DIR_last_TempoText))
			{
				// Use now.
				MT.Voices[0].Text.push_back(GetDirectionText(pDir->nType, Temp));
				// TODO: ----> LILY
			}
			else if (IsInRange(pDir->nType, MusicSheet::DIR_first_Dynamic, MusicSheet::DIR_last_Dynamic) ||
				IsInRange(pDir->nType, MusicSheet::DIR_first_Feeling, MusicSheet::DIR_last_Feeling))
			{
				MusicSheet::Voice & InVoice = pMeasure->Voices[pDir->BeforeNote.first];
				int					iCurNote = pDir->BeforeNote.second;

				// Get Position
				if (pDir->BeforeNote.second <= (int)InVoice.Notes.size())
				{			
					int	iXPos = (pDir->BeforeNote.second < (int)InVoice.Notes.size()) ?
						InVoice.Notes[iCurNote].iXPos : InVoice.Notes[iCurNote - 1].iXPos;

					// Find Closest Note in all staffs and add it to the list.
					for ALL_INDICES(pMeasure->Voices, v)
					{
						int		n = 0;

						for ALL_INDICES(pMeasure->Voices[v].Notes, i)
							if (abs(pMeasure->Voices[v].Notes[i].iXPos - iXPos) < // If in an chord, get the first one.
								abs(pMeasure->Voices[v].Notes[n].iXPos - iXPos))
								n = i;

						// If it is not under a rest, put it
						if (pMeasure->Voices[v].Notes[n].chStep != 'R')
						{
							//// If it is inside an chord, move to last:
							//if (FOUND_IN_SET(pMeasure->Voices[v].Notes[n].Extras, MusicSheet::NE_CHORD) ||
							//	(n < (int)pMeasure->Voices[v].Notes.size() - 1 &&
							//	FOUND_IN_SET(pMeasure->Voices[v].Notes[n + 1].Extras, MusicSheet::NE_CHORD)))
							//	while (n < (int)pMeasure->Voices[v].Notes.size() - 1 &&
							//		FOUND_IN_SET(pMeasure->Voices[v].Notes[n + 1].Extras, MusicSheet::NE_CHORD))
							//		n++;
							// If it is inside an chord, move to first:
							while (n && FOUND_IN_SET(pMeasure->Voices[v].Notes[n].Extras, MusicSheet::NE_CHORD))
									n--;

							Directions.push_back(InlineDirection(v, n, pDir->nType));
						}
						else
							n = 1; // Skipped
					}
				}
				else
					_RPTF0(_CRT_ERROR, "Unexpected case.");

			}
			else if (IsInRange(pDir->nType, MusicSheet::DIR_first_Finger, MusicSheet::DIR_last_Finger))
				Directions.push_back(InlineDirection(pDir->BeforeNote.first, pDir->BeforeNote.second, pDir->nType));
			else if (IsInRange(pDir->nType, MusicSheet::DIR_first_Wedge, MusicSheet::DIR_last_Wedge))
				Directions.push_back(InlineDirection(pDir->BeforeNote.first, pDir->BeforeNote.second, pDir->nType));
			else if (pDir->nType == MusicSheet::DIR_OCTAVE_SHIFT_UP || pDir->nType == MusicSheet::DIR_OCTAVE_SHIFT_DOWN || pDir->nType == MusicSheet::DIR_OCTAVE_SHIFT_STOP)
				Directions.push_back(InlineDirection(pDir->BeforeNote.first, pDir->BeforeNote.second, pDir->nType));
			else if (pDir->nType == MusicSheet::DIR_SOUND || pDir->nType == MusicSheet::DIR_METRONOME)
			{
				for ALL_INDICES(pMeasure->Voices, v)
					Directions.push_back(InlineDirection(v, pDir->BeforeNote.second, pDir->nType, pDir->Text));
#pragma message("Probably you should convert positions to put it for all voices")
			}
			else
				_RPTF1(_CRT_ERROR, "Unexpected Direction: %i", pDir->nType);

			if (GuitarFingers.GetLength())
				MT.Voices[0].Text.push_back(CStringA("Arpeggiate_\"") + GuitarFingers + "\"_");
				// TODO: ----> LILY
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

		CStringA	DirectionBufferText;

		// Start Bar Lines
		{
			for ALL(pMeasure->BarLines, pBL)
				if (pBL->nType < MusicSheet::BarLine::BL_LEFT_HAND_BARS)
					OutVoice.Text.push_back(GetBarLineText(*pBL));
					// TODO: ----> LILY
		}

		int			iInTuplet = 0;
		bool		bEndChord = false;
		bool		bEndTuplet = false;
		CStringA	LilyChordLength, *pLAL = NULL;
		bool		bChordIsArpeggio;
		
		for ALL_INDICES(InVoice.Notes, i)
		{
			// Signature Change?
			if (uNextSignature < pMeasure->Signatures.size() &&
				pMeasure->Signatures[uNextSignature].BeforeNote.first == iVoice&&
				pMeasure->Signatures[uNextSignature].BeforeNote.second == i)
			{
				GetSignaturesText(pMeasure->Signatures[uNextSignature], InVoice.iStaff, pPreviousSignature, OutVoice);
				pPreviousSignature = &pMeasure->Signatures[uNextSignature++];
			}

			// Direction Before Note?
			MusicSheet::DirectionTypes nLastDirection = MusicSheet::DIR_UNKNWON;
			for ALL(Directions, pDir)
				if (pDir->iVoice == iVoice && pDir->iNote == i)
					if (IsPreDirection(pDir->nType))
					{
						OutVoice.Text.push_back(GetDirectionText(pDir->nType, DirectionBufferText, nLastDirection, pDir->ExtraText));
						// TODO: ----> LILY
						nLastDirection = pDir->nType;
					}

			// Chord Start?
			bool	bInChord = FOUND_IN_SET(InVoice.Notes[i].Extras, MusicSheet::NE_CHORD);
			bool	bChordStart = (!bInChord &&
				i + 1 < (int)InVoice.Notes.size() &&
				FOUND_IN_SET(InVoice.Notes[i + 1].Extras, MusicSheet::NE_CHORD));

			if (bChordStart)
			{
				if (FOUND_IN_SET(InVoice.Notes[i].Extras, MusicSheet::NE_ARPEGGIATE))
				{
					OutVoice.Text.push_back("Arpeggiate");
					bChordIsArpeggio = true;
				}
				else
				{
					OutVoice.Text.push_back("Chord");
					bChordIsArpeggio = false;
				}
				bEndChord = true;
				pLAL = &LilyChordLength;
			}

			if (FOUND_IN_SET(InVoice.Notes[i].Extras, MusicSheet::NE_TUPLET))
			{
				OutVoice.Text.push_back("Tuplet");
				OutVoice.Lily += "\\tuplet 3/2 ";
				iInTuplet = 3;
			}

			if (bChordStart || iInTuplet == 3)
			{
				OutVoice.Text.push_back("[");
				if (bChordStart)
					OutVoice.Lily += " < ";
				else
					OutVoice.Lily += " { ";
			}

			bEndTuplet = (iInTuplet == 1);
			GetNoteText(InVoice.Notes[i], OutVoice, iInTuplet-- > 0, pLAL);

			// If current note has ARPEGGIATE and is in chord, remove the flag from the next ones. 
			if (FOUND_IN_SET(InVoice.Notes[i].Extras, MusicSheet::NE_ARPEGGIATE))
				if (bChordStart || bInChord)
					// For all next notes...
					for DRANGE(j, i + 1, (int)InVoice.Notes.size())
						// If it is in this chord,...
						if (FOUND_IN_SET(InVoice.Notes[j].Extras, MusicSheet::NE_CHORD))
						{
							// If it has ARPEGGIATE, remove it.
							if (FOUND_IN_SET(InVoice.Notes[j].Extras, MusicSheet::NE_ARPEGGIATE))
								InVoice.Notes[j].Extras.erase(MusicSheet::NE_ARPEGGIATE);
						}
						else
							break;

			// Chord Continuem or End?
			if (iInTuplet <= 0 && (bChordStart || bInChord))
				if ((i + 1 < (int)InVoice.Notes.size() && FOUND_IN_SET(InVoice.Notes[i + 1].Extras, MusicSheet::NE_CHORD)))
					OutVoice.Text.push_back(",");
				else
					if (i + 1 < (int)InVoice.Notes.size())
					{
						OutVoice.Text.push_back(" ]");
						OutVoice.Lily += " > " + LilyChordLength;
						if (bChordIsArpeggio)
							OutVoice.Lily += "\\arpeggio";
						pLAL = NULL;
						bEndChord = false;						
					}

			if (bEndTuplet)
			{
				OutVoice.Text.push_back(" ]");
				OutVoice.Lily += " } ";
				bEndTuplet = false;
			}

			nLastDirection = MusicSheet::DIR_UNKNWON;
			for ALL(Directions, pDir)
				if (pDir->iVoice == iVoice && pDir->iNote == i)
					if (!IsPreDirection(pDir->nType))
				{
					OutVoice.Text.push_back(GetDirectionText(pDir->nType, DirectionBufferText, nLastDirection, pDir->ExtraText));
					// TODO: ----> LILY
					nLastDirection = pDir->nType;
				}
		}

		if (bEndChord)
		{
			OutVoice.Text.push_back(" ]");
			OutVoice.Lily += " > " + LilyChordLength;
			if (bChordIsArpeggio)
				OutVoice.Lily += "\\arpeggio";
			pLAL = NULL;
		}

		MusicSheet::DirectionTypes nLastDirection = MusicSheet::DIR_UNKNWON;
		for ALL(Directions, pDir)
			if (pDir->iVoice == iVoice && pDir->iNote == InVoice.Notes.size())
			{
				OutVoice.Text.push_back(GetDirectionText(pDir->nType, DirectionBufferText, nLastDirection));
				// TODO: ----> LILY
				nLastDirection = pDir->nType;
			}

		// Signature Change?
		if (uNextSignature < pMeasure->Signatures.size() &&
			pMeasure->Signatures[uNextSignature].BeforeNote.first == iVoice &&
			pMeasure->Signatures[uNextSignature].BeforeNote.second == (int)InVoice.Notes.size())
		{
			GetSignaturesText(pMeasure->Signatures[uNextSignature], InVoice.iStaff, pPreviousSignature, OutVoice);
			pPreviousSignature = &pMeasure->Signatures[uNextSignature++];
		}

		// End Bar Lines
		{
			for ALL(pMeasure->BarLines, pBL)
				if (pBL->nType > MusicSheet::BarLine::BL_LEFT_HAND_BARS)
					OutVoice.Text.push_back(GetBarLineText(*pBL));
					// TODO: ----> LILY
		}
	}

	// End Lilies
	for ALL(MT.Voices, pVoice)
		pVoice->Lily += "}\r\n";

#ifdef _DEBUG
	for ALL(MT.Voices, pVoice)
		for ALL_INDICES(pVoice->Text, i)
			if (pVoice->Text[i].Left(6) == "Chord")
				if (pVoice->Text[i].Find("Fermata") != -1 || (i + 1 < (int)pVoice->Text.size() && pVoice->Text[i + 1].Find("Fermata") != -1))
					AfxMessageBox(L"FOUND ONE CHORD CHANGE");
	//Text.Replace( "Chord Fermata on" , "Fermata on Chord" ) ;
#endif
	return MT;
}

// Converts the sheet into text
bool	CMusicSheetNarrator::ConvertToText(MusicSheet & Sheet, NarratedMusicSheet & Narration)
{
	try
	{
		Narration.Clear();

		CStringA	Text;

		m_bProcessingGuitar = false;

		// Preprocess
		PreprocessSheet(Sheet);

		// Credits
		if (Sheet.Credits.size())
		{
			Text = "";
			for ALL(Sheet.Credits, pCredit)
			{
				if (Text.GetLength())
					Text += ", ";
				Text += *pCredit;
			}
			Narration.Credits = CA2W(Text);
		}

		// Parts
		for ALL(Sheet.Parts, pPart)
		{
			NarratedMusicSheet::PartText	PT;
			// Name
			{
				if (Sheet.Parts.size() > 1)
					Text.Format("Part: %i", (pPart - Sheet.Parts.begin()) + 1);
				else
					Text = "";
				if (pPart->Name.GetLength())
				{
					if (Text.GetLength())
						Text += ", ";
					Text += pPart->Name;
				}
				PT.PartName = CA2W(Text);

				m_bProcessingGuitar = pPart->Name.MakeUpper() == "GUITAR";
			}
			MusicSheet::Signatures * pPreviousSignature = NULL;
			for ALL_INDICES(pPart->Measures, i)
				PT.Measures.push_back(GetMeasureText(&pPart->Measures[i], pPreviousSignature));
			Narration.Parts.push_back(PT);
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
	// Propagate Accidentals...
	for ALL(Sheet.Parts, pPart)
	{
		vector<MusicSheet::Signatures>	LastSignatures;
		
		for ALL(pPart->Measures, pMeasure)
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

			for ALL(pMeasure->Voices, pVoice)
				for ALL(pVoice->Notes, pNote)
				{
					if (pNote->chAccidental == 'S' ||
						pNote->chAccidental == 'F')
						for (decltype(pVoice->Notes.begin()) pNote2 = pNote + 1; pNote2 < pVoice->Notes.end(); pNote2++)
							if (pNote2->chStep == pNote->chStep && pNote2->iOctave == pNote->iOctave)
								if (pNote2->chAccidental)
									break;
								else
									pNote2->chAccidental = pNote->chAccidental;
				}
		}
	}
}

// Makes all post narration changes.
void	CMusicSheetNarrator::PostprocessSheet(MusicSheet & Sheet, NarratedMusicSheet & Narration)
{
	for ALL(Narration.Parts, pPart)
	{
		set<pair<int, int>>	Voices;
		for ALL(pPart->Measures, pMeasure)
			for ALL(pMeasure->Voices, pVoice)
			{
				Voices.insert(make_pair(pVoice->iStaff, pVoice->iVoice));

				vector<CStringA> & pNotes = pVoice->Text;
				for ALL_INDICES(pNotes, n)
					if (pNotes[n] == "," && n && n + 1 < (int)pNotes.size())
					{
						pNotes[n - 1] += CStringA(", ") + pNotes[n + 1];
						VEC_ERASE(pNotes, n);
						VEC_ERASE(pNotes, n--);
					}
					else if (pNotes[n] == "[" && n + 1 < (int)pNotes.size())
					{
						pNotes[n] = pNotes[n] + pNotes[n + 1];
						VEC_ERASE(pNotes, n + 1);
						n--;
					}
					else if (pNotes[n] == " ]" && n)
					{
						pNotes[n - 1] = pNotes[n - 1] + " ]";
						VEC_ERASE(pNotes, n--);
					}
			}
		// Sort Voices
		for ALL(pPart->Measures, pMeasure)
			for (int v = 0; v < (int)pMeasure->Voices.size(); v++)
				for (int w = v + 1; w < (int)pMeasure->Voices.size(); w++)
					if (pMeasure->Voices[v].iStaff > pMeasure->Voices[w].iStaff ||
						(pMeasure->Voices[v].iStaff == pMeasure->Voices[w].iStaff && pMeasure->Voices[v].iVoice > pMeasure->Voices[w].iVoice))
					{
						NarratedMusicSheet::Voice V = pMeasure->Voices[w];
						VEC_ERASE(pMeasure->Voices, w);
						VEC_INSERT(pMeasure->Voices, v, V);
					}

		// Check if all measures have all voices
		NarratedMusicSheet::Voice	EmptyVoice;
		EmptyVoice.iStaff = -1;
		for ALL(pPart->Measures, pMeasure)
		{
			int i = 0;
			for each(pair<int,int> v in Voices)
			{
				if ((int)pMeasure->Voices.size() <= i || pMeasure->Voices[i].iStaff != v.first || pMeasure->Voices[i].iVoice != v.second)
				{
					EmptyVoice.iStaff = v.first;
					EmptyVoice.iVoice = v.second;
					pMeasure->Voices.insert(pMeasure->Voices.begin() + i, EmptyVoice);
				}
				i++;
			}
		}

		// If something added, fill up the empty ones.
		if (EmptyVoice.iStaff != -1)
			for (int v = 0; v < (int)Voices.size(); v++)
				for ALL(pPart->Measures, pMeasure)
					if (!pMeasure->Voices[v].Text.size())
					{
						CStringA Text, MText;
						int	iNextFull = -1;
						for (decltype(pMeasure) pNext = pMeasure; pNext != pPart->Measures.end(); pNext++)
							if (pNext->Voices[v].Text.size())
							{
								iNextFull = VEC_INDEX(pNext, pPart->Measures);
								break;
							}

						if (iNextFull == -1)
							Text = "Silence_till_end_of_part";
						else if (iNextFull == VEC_INDEX(pMeasure, pPart->Measures) + 1)
							Text = "Silence";
						else
							Text.Format("Silence_till_Measure_%i", iNextFull);

						for (decltype(pMeasure) pNext = pMeasure; pNext != pPart->Measures.end(); pNext++)
							if (pNext->Voices[v].Text.size())
								break;
							else
							{
								MText.Format("Measure: %i", VEC_INDEX(pNext, pPart->Measures) + 1);
								pNext->Voices[v].Text.push_back(MText);
								pNext->Voices[v].Text.push_back(Text);
							}
					}
	}
}