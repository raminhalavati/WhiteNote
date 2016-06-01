// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "WhiteNote.h"
#include "MyEdit.h"

// CMyEdit

IMPLEMENT_DYNAMIC(CMyEdit, CEdit)

CMyEdit::CMyEdit()
{
	m_bNormalLine = true;
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()



// CMyEdit message handlers


void CMyEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (m_bNormalLine)
		if (GetParent()->SendMessage(WM_USER, (WPARAM)nChar, (LPARAM)(GetKeyState(VK_CONTROL) == -127)))
			return;
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CMyEdit::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	if (m_bNormalLine)
	{
		int		iPos1, iPos2;

		GetSel(iPos1, iPos2);
		if (iPos1 != iPos2)
			SetSel(iPos1, iPos1);
	}
	else
		SetSel(0, 0);
}