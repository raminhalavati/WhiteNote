// TextOutputOptions.cpp : implementation file
//

#include "stdafx.h"
#include "WhiteNote.h"
#include "TextOutputOptions.h"
#include "afxdialogex.h"


// CTextOutputOptions dialog

IMPLEMENT_DYNAMIC(CTextOutputOptions, CDialogEx)

CTextOutputOptions::CTextOutputOptions(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TEXTOUTPUTOPTIONS, pParent)
{

}

CTextOutputOptions::~CTextOutputOptions()
{
}

void CTextOutputOptions::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REPEAT_SIGNATURES, m_RepeatSignatures);
	DDX_Control(pDX, ID_WIZFINISH, m_GroupBy);
}


BEGIN_MESSAGE_MAP(CTextOutputOptions, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTextOutputOptions::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTextOutputOptions::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTextOutputOptions message handlers
BOOL CTextOutputOptions::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_GroupBy.InsertString(0, L"Measure by Measure");
	m_GroupBy.InsertString(1, L"Voice by Voice");	
	Serialize(true);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// Serializes settings.
void CTextOutputOptions::Serialize(bool bLoad)
{
	if (bLoad)
	{
		m_GroupBy.SetCurSel(theApp.GetProfileInt(L"TextOptions", L"Group By", 0));
		m_RepeatSignatures.SetCheck(m_Options.bRepeatSignatures = theApp.GetProfileInt(L"TextOptions", L"RepeatSignatures", 0) != 0);
	}
	else
	{
		theApp.WriteProfileInt(L"TextOptions", L"Group By", m_GroupBy.GetCurSel());
		theApp.WriteProfileInt(L"TextOptions", L"RepeatSignatures", m_Options.bRepeatSignatures = (m_RepeatSignatures.GetCheck() != 0));

		if (m_GroupBy.GetCurSel() == 0)
			m_Options.chGroupBy = 'm';
		else
			m_Options.chGroupBy = 'v';
	}
}

void CTextOutputOptions::OnBnClickedOk()
{
	Serialize(false);

	CDialogEx::OnOK();
}


void CTextOutputOptions::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
