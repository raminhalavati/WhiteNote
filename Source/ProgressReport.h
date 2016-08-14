#pragma once
#include "afxcmn.h"
#include "resource.h"

// CProgressReport dialog

class CProgressReport : public CDialog
{
	DECLARE_DYNAMIC(CProgressReport)

public:
	CProgressReport(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgressReport();

// Dialog Data
	enum { IDD = IDD_PROGRESSREPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

	CProgressCtrl m_Progress;

	bool	* m_pbStop;
	bool	* m_pbRunning;
	int		* m_piProgress;
	
};
