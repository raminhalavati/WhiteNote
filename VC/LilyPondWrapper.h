#pragma once
#include "MusicSheet.h"
#include "Common.h"
#include <set>

class CLilyPondWrapper
{
public:
	CLilyPondWrapper();
	~CLilyPondWrapper();

	NarratedMusicSheet *	m_pNarration;
	CString					m_CacheFolderRoot;
	CString					m_FileCacheFolder;
	CString					m_LilyPondPath;

	bool					m_bThreadRunning;
	bool					m_bStopThread;
	pair<int,int>			m_ThreadCurrent;
	set<pair<int, int>>		m_FailedItems;

	// Initializes LilyPond image wrapper.
	void Initialize(CString LilyPondPath, NarratedMusicSheet * pSheet, CString FilePath, bool bAutoRefresh);
	// Deletes cache for current image or all sheets.
	void DeleteCache(bool bAllSheets);
	// Checks to see if the checksum of the given sheet and cached images match, otherwise delets all images and writes the new checksum.
	void VerifyCheckSum();
	// Returns requested measures image, either from buffer or new creation.
	bool GetMeasureImage(int iPartNo, int iMeasureNo, CImage & Image, bool bCalledFromThread=false, bool bForceRecheck=false);
	// Removes temporary LilyPond files.
	void CleanTempFiles();
	// Prepare Buffer
	static void __cdecl BufferBuilder(void * pParam);	
};

