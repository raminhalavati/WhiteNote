#pragma once
#include "afxwin.h"


// CCommentDialog dialog

class CCommentDialog : public CDialog
{
	DECLARE_DYNAMIC(CCommentDialog)

public:
	CCommentDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCommentDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMENTDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CEdit	m_TextBox;
	CButton m_Cancel;

	bool	m_bEditable;
	CString	m_Text;
};

CString	ShowComment(bool bEditable, CString Text);
