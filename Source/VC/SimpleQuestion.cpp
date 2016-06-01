// SimpleQuestion.cpp : implementation file
//

#include "stdafx.h"
#include "WhiteNote.h"
#include "SimpleQuestion.h"
#include "afxdialogex.h"


// CSimpleQuestion dialog

IMPLEMENT_DYNAMIC(CSimpleQuestion, CDialog)

CSimpleQuestion::CSimpleQuestion(CString Question, CString Answer, CWnd* pParent /*=NULL*/)
	: CDialog(CSimpleQuestion::IDD, pParent)
{
	m_QuestionValue = Question;
	m_AnswerValue = Answer;
}

CSimpleQuestion::~CSimpleQuestion()
{
}

void CSimpleQuestion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_QUESTION, m_Question);
	DDX_Control(pDX, IDC_EDIT1, m_Answer);
}


BEGIN_MESSAGE_MAP(CSimpleQuestion, CDialog)
	ON_BN_CLICKED(IDOK, &CSimpleQuestion::OnBnClickedOk)
END_MESSAGE_MAP()


// CSimpleQuestion message handlers

BOOL CSimpleQuestion::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Question.SetWindowText(m_QuestionValue);
	m_Answer.SetWindowText(m_AnswerValue);
	m_Answer.SetSel(-1, -1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSimpleQuestion::OnBnClickedOk()
{
	m_Answer.GetWindowText(m_AnswerValue);
	CDialog::OnOK();
}

CString	AskQuestion(CString Question, CString Answer)
{
	CSimpleQuestion	SQ(Question, Answer);
	return (SQ.DoModal() == IDOK) ? SQ.m_AnswerValue : L"";
}

int	AskQuestion(CString Question, int iAnswer)
{
	CString	Temp;
	Temp.Format(L"%i", iAnswer);
	Temp = AskQuestion(Question, Temp);
	return Temp.GetLength() ? _wtoi(Temp) : -1;
}