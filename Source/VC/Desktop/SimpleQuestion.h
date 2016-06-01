#pragma once
#include "afxwin.h"


// CSimpleQuestion dialog

class CSimpleQuestion : public CDialog
{
	DECLARE_DYNAMIC(CSimpleQuestion)

public:
	CSimpleQuestion(CString Question, CString Answer, CWnd* pParent = NULL);   // standard constructor
	virtual ~CSimpleQuestion();

// Dialog Data
	enum { IDD = IDD_SIMPLEQUESTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CEdit m_Question;
	CEdit m_Answer;
	CString	m_QuestionValue;
	CString	m_AnswerValue;
};

CString	AskQuestion(CString Question, CString Answer);
int	AskQuestion(CString Question, int iAnswer);