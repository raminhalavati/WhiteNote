// ProgressReport.cpp : implementation file
//

#include "stdafx.h"
#include "WhiteNote.h"
#include "ProgressReport.h"
#include "afxdialogex.h"


// CProgressReport dialog

IMPLEMENT_DYNAMIC(CProgressReport, CDialog)

CProgressReport::CProgressReport(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressReport::IDD, pParent)
{
	m_pbStop = NULL;
	m_pbRunning = NULL;
	m_piProgress = NULL;
}

CProgressReport::~CProgressReport()
{
}

void CProgressReport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
}


BEGIN_MESSAGE_MAP(CProgressReport, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CProgressReport message handlers

BOOL CProgressReport::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetTimer(1, 1000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CProgressReport::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID != SC_CLOSE)
		CDialog::OnSysCommand(nID, lParam);
	else
	{
		if (*m_pbRunning && AfxMessageBox(L"Stop creating images?", MB_YESNO | MB_ICONQUESTION) != IDYES)
			return;

		*m_pbStop = true;
		while (*m_pbRunning)
			Sleep(100);
	}
}


void CProgressReport::OnTimer(UINT_PTR nIDEvent)
{
	m_Progress.SetPos(*m_piProgress);
	if (!*m_pbRunning)
		OnCancel();
	else
		CDialog::OnTimer(nIDEvent);
}
