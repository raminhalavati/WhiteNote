#pragma once
#include "MusicSheet.h"
#include "Common.h"

class CMusicSheetNarrator
{
public:
	CMusicSheetNarrator(void);
	~CMusicSheetNarrator(void);

	// Converts the sheet into text
	bool	ConvertToText(MusicSheet & Sheet, NarratedMusicSheet & Narration, bool bDetailed=true);

	// Makes All necessary changes before Narration.
	void	PreprocessSheet(MusicSheet & Sheet);

	// Makes all post narration changes.
	void	PostprocessSheet(MusicSheet & Sheet, NarratedMusicSheet & Narration);

	NarratedMusicSheet::MeasureText	GetMeasureText(MusicSheet::Measure * pMeasure, MusicSheet::Signatures * & pPreviousSignature);

	void	GetSignaturesText(NarratedMusicSheet::Voice & Voice, MusicSheet::Signatures & Sigs, int iStaff, MusicSheet::Signatures * pPreviousSignature);

	void	GetBarLineText(NarratedMusicSheet::Voice & Voice, MusicSheet::BarLine & BL);

	void	GetDirectionText(NarratedMusicSheet::Voice & Voice, MusicSheet::DirectionTypes nDirType, CString & BufferText,
		MusicSheet::DirectionTypes nPreviousDir = MusicSheet::DIR_UNKNWON, CString ExtraText = L"", bool bAbove = true);

	void	GetNoteText(MusicSheet::Note & Note, NarratedMusicSheet::Voice & Voice, bool bInTuplet, bool &bInGrace, CString * pChordLength);

	CString	GetNoteTypeName(MusicSheet::Note & Note);

	bool		m_bProcessingGuitar;
	bool		m_bDetailedText;
};

