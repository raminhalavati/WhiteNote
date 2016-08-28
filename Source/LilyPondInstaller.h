#pragma once
#include "afxwin.h"


// CLilyPondInstaller dialog

class CLilyPondInstaller : public CDialog
{
	DECLARE_DYNAMIC(CLilyPondInstaller)

public:
	CLilyPondInstaller(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLilyPondInstaller();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LILYPONDINSTALLER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedNoDontRemind();
	afx_msg void OnBnClickedNoDontRemindSoon();
	CEdit m_Message;
	virtual BOOL OnInitDialog();
};
