
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
	bool	m_bNewVersionExists;
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
};

extern CWhiteNoteApp theApp;
