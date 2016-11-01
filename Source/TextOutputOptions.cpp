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
	DDX_Control(pDX, IDC_VOICES_FIRST, m_VoicesFirst);
	DDX_Control(pDX, IDC_MEASURES_FIRST, m_MeasuresFirst);
	DDX_Control(pDX, IDC_REPEAT_SIGNATURES, m_RepeatSignatures);
}


BEGIN_MESSAGE_MAP(CTextOutputOptions, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTextOutputOptions::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTextOutputOptions::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_VOICES_FIRST, &CTextOutputOptions::OnBnClickedVoicesFirst)
	ON_BN_CLICKED(IDC_MEASURES_FIRST, &CTextOutputOptions::OnBnClickedMeasuresFirst)
END_MESSAGE_MAP()


// CTextOutputOptions message handlers
BOOL CTextOutputOptions::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	Serialize(true);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// Serializes settings.
void CTextOutputOptions::Serialize(bool bLoad)
{
	if (bLoad)
	{
		m_VoicesFirst.SetCheck(m_Options.bAllVoicesFirst = theApp.GetProfileInt(L"TextOptions", L"AllVoicesFirst", 1) != 0);
		m_MeasuresFirst.SetCheck(m_Options.bAllMeasuresFirst = theApp.GetProfileInt(L"TextOptions", L"AllMeasuresFirst", 0) != 0);
		m_RepeatSignatures.SetCheck(m_Options.bRepeatSignatures = theApp.GetProfileInt(L"TextOptions", L"RepeatSignatures", 0) != 0);
	}
	else
	{
		theApp.WriteProfileInt(L"TextOptions", L"AllVoicesFirst", m_Options.bAllVoicesFirst = (m_VoicesFirst.GetCheck() != 0));
		theApp.WriteProfileInt(L"TextOptions", L"AllMeasuresFirst", m_Options.bAllMeasuresFirst = (m_MeasuresFirst.GetCheck() != 0));
		theApp.WriteProfileInt(L"TextOptions", L"RepeatSignatures", m_Options.bRepeatSignatures = (m_RepeatSignatures.GetCheck() != 0));
	}
}

void CTextOutputOptions::OnBnClickedOk()
{
	Serialize(false);
	if (!m_Options.bAllVoicesFirst && !m_Options.bAllMeasuresFirst)
	{
		AfxMessageBox(L"You should choose one of voice or measure priorities.", MB_ICONERROR);
		return;
	}

	CDialogEx::OnOK();
}


void CTextOutputOptions::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CTextOutputOptions::OnBnClickedVoicesFirst()
{
	m_VoicesFirst.SetCheck(1);
	m_MeasuresFirst.SetCheck(0);
}


void CTextOutputOptions::OnBnClickedMeasuresFirst()
{
	m_MeasuresFirst.SetCheck(1);
	m_VoicesFirst.SetCheck(0);
}
