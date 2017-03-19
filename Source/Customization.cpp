// Customization.cpp : implementation file
//

#include "stdafx.h"
#include "WhiteNote.h"
#include "Customization.h"
#include "afxdialogex.h"


// CCustomization dialog

IMPLEMENT_DYNAMIC(CCustomization, CDialogEx)

CCustomization::CCustomization(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CUSTOMIZATION, pParent)
{

}

CCustomization::~CCustomization()
{
}

void CCustomization::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_REPEAT_SIGNATURES, m_AlwaysShowSignatures);
  DDX_Control(pDX, IDC_SHOW_DETAILED_TEXT, m_ShowDetailedText);
  DDX_Control(pDX, IDC_PLAY_NAVIGATIONAL_SOUNDS, m_PlayNavigationalSounds);
  DDX_Control(pDX, IDC_USE_UNICODE_CHARACTERS, m_UseUnicodeCharacters);
  DDX_Control(pDX, IDC_PAGE_SIZE, m_PageSize);
  DDX_Control(pDX, IDC_SHOW_PERSIAN_NUMBERS_WITH_LETTERS, m_PersianNumbers);
}


BEGIN_MESSAGE_MAP(CCustomization, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCustomization::OnBnClickedOk)
END_MESSAGE_MAP()


// CCustomization message handlers


BOOL CCustomization::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_AlwaysShowSignatures.SetCheck(m_Values.bAlwaysShowSignatures);
	m_ShowDetailedText.SetCheck(m_Values.bShowDetailedText);
	m_PlayNavigationalSounds.SetCheck(m_Values.bPlayNavigationalSounds);
	m_UseUnicodeCharacters.SetCheck(m_Values.bUseUnicodeCharacters);
  m_PersianNumbers.SetCheck(m_Values.bLettersForPersianNumbers);
	CString Temp;
	Temp.Format(L"%i", m_Values.iPageSize);
	m_PageSize.SetWindowText(Temp);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CCustomization::OnBnClickedOk()
{
	m_Values.bAlwaysShowSignatures = m_AlwaysShowSignatures.GetCheck() == BST_CHECKED;
	m_Values.bShowDetailedText = m_ShowDetailedText.GetCheck() == BST_CHECKED;
	m_Values.bPlayNavigationalSounds = m_PlayNavigationalSounds.GetCheck() == BST_CHECKED;
	m_Values.bUseUnicodeCharacters = m_UseUnicodeCharacters.GetCheck() == BST_CHECKED;
  m_Values.bLettersForPersianNumbers = m_PersianNumbers.GetCheck() == BST_CHECKED;
	CString Temp;
	m_PageSize.GetWindowText(Temp);
	m_Values.iPageSize = _wtoi(Temp);

	if (m_Values.iPageSize < 1)
	{
		AfxMessageBox(L"Page size must be more than 1.", MB_ICONERROR);
		return;
	}

	CDialogEx::OnOK();
}
