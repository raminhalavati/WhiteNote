// CommentDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WhiteNote.h"
#include "CommentDialog.h"
#include "afxdialogex.h"


// CCommentDialog dialog

IMPLEMENT_DYNAMIC(CCommentDialog, CDialog)

CCommentDialog::CCommentDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_COMMENTDIALOG, pParent)
{

}

CCommentDialog::~CCommentDialog()
{
}

void CCommentDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT, m_TextBox);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
}


BEGIN_MESSAGE_MAP(CCommentDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CCommentDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CCommentDialog message handlers


BOOL CCommentDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_TextBox.SetWindowTextW(m_Text);
	if (!m_bEditable)
	{
		m_TextBox.SetReadOnly(true);
		m_Cancel.ShowWindow(SW_HIDE);
	}
	SetWindowText(m_Title);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CCommentDialog::OnBnClickedOk()
{
	m_TextBox.GetWindowTextW(m_Text);
	CDialog::OnOK();
}

CString	ShowComment(bool bEditable, CString Text, int iMeasure)
{
	CCommentDialog Dlg;
	Dlg.m_Text = Text;
	Dlg.m_bEditable = bEditable;
	Dlg.m_Title = bEditable ? L"Add/Edit " : L"";

	Text.Format(L"Comments for Measure %i", iMeasure + 1);
	Dlg.m_Title += Text;

	if (Dlg.DoModal() == IDOK)
		return Dlg.m_Text;
	else
		return L"-1";
}