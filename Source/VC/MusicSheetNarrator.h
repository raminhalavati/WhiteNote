#pragma once
#include "MusicSheet.h"
#include "Common.h"

class CMusicSheetNarrator
{
public:
	CMusicSheetNarrator(void);
	~CMusicSheetNarrator(void);

	// Converts the sheet into text
	bool	ConvertToText(MusicSheet & Sheet, NarratedMusicSheet & Narration);

	// Makes All necessary changes before Narration.
	void	PreprocessSheet(MusicSheet & Sheet);

	// Makes all post narration changes.
	void	PostprocessSheet(MusicSheet & Sheet, NarratedMusicSheet & Narration);

	NarratedMusicSheet::MeasureText	GetMeasureText(MusicSheet::Measure * pMeasure, MusicSheet::Signatures * & pPreviousSignature);

	void		GetSignaturesText(MusicSheet::Signatures & Sigs, int iStaff, MusicSheet::Signatures * pPreviousSignature, vector<CStringA> &Texts);

	CStringA	GetBarLineText(MusicSheet::BarLine & BL);

	CStringA	GetDirectionText(MusicSheet::DirectionTypes nDirType, CStringA & BufferText, MusicSheet::DirectionTypes nPreviousDir = MusicSheet::DIR_UNKNWON, CStringA ExtraText = "");

	CStringA	GetNoteText(MusicSheet::Note & Note, bool bInTuplet = false);

	CStringA	GetNoteTypeName(MusicSheet::Note & Note);

	bool		m_bProcessingGuitar;
};

