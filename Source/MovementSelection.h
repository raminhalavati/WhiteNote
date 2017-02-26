#pragma once
#include "afxwin.h"

#include "Common.h"
#include "MusicSheet.h"

// CMovementSelection dialog

class CMovementSelection : public CDialog
{
	DECLARE_DYNAMIC(CMovementSelection)

public:
	CMovementSelection(NarratedMusicSheet* sheet, int current_movement, CWnd* pParent = NULL);   // standard constructor
	virtual ~CMovementSelection();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOVEMENTSELECTION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CEdit m_MovementNumber;
  CListBox m_MovementsList;
  afx_msg void OnEnChangeMovementNumber();
  afx_msg void OnLbnSelchangeMovementsList();
  afx_msg void OnBnClickedOk();
  virtual BOOL OnInitDialog();

  NarratedMusicSheet * sheet_;
  int current_movement_;
  char last_change_;
};
