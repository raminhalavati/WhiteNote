
// WhiteNote.h : main header file for the WhiteNote application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CWhiteNoteApp:
// See WhiteNote.cpp for the implementation of this class
//

class CWhiteNoteApp : public CWinApp
{
public:
	CWhiteNoteApp();

	CString	m_Path;
	CString	m_FileVersion;
	CString m_WebsiteMessage;
	CString m_LatestFileLocation;
	bool	m_bNewVersionExists;
	bool	m_bTerminateToUpdate;
// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnHelpHelp();
	afx_msg void OnHelpCheckforupdate();
	// Checks for update.
	bool UpdateCheck(bool bForceCheck=false);	
public:
	// Opens a file/website in browser
	void OpenInBrowser(CString Path);
	// Is called when a newer version is available.
	int NewVersionAvailable();
	virtual BOOL OnIdle(LONG lCount);
};

extern CWhiteNoteApp theApp;
