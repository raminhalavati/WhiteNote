#pragma once
#include "MusicSheet.h"
#include "Common.h"
#include <set>

class CLilyPondWrapper
{
public:
	CLilyPondWrapper();
	~CLilyPondWrapper();

	bool					m_bReady;
	NarratedMusicSheet *	m_pNarration;
	CString					m_CacheFolderRoot;
	CString					m_FileCacheFolder;
	CString					m_LilyPondPath;
	HWND					m_hWndParent;
	
	set<pair<int, int>>		m_FailedItems;
	
	struct _ThreadData
	{
		bool	bRunnung;
		bool	bStop;
		bool	bAllDone;
		int		iProgress;
	} m_ThreadData;

	// Initializes LilyPond image wrapper.
	void Initialize(CString LilyPondPath, NarratedMusicSheet * pSheet, CString FilePath);
	// Deletes cache for current image or all sheets.
	void DeleteCache(bool bAllSheets);
	// Checks to see if the checksum of the given sheet and cached images match, otherwise delets all images and writes the new checksum.
	void VerifyCheckSum();
	// Returns requested measures image, either from buffer or new creation.
	bool GetMeasureImage(int iPartNo, int iMeasureNo, CImage & Image, bool bForceRecheck=false);
	// Removes temporary LilyPond files.
	void CleanTempFiles();
	// Creats all images for all parts and measures.
	void CreateAllImages();
	// Image Creation Thread
	static void __cdecl ImageCreationThread(void * pParam);
};

