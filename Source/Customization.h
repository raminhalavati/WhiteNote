#pragma once
#include "afxwin.h"


// CCustomization dialog
typedef struct
{
	int		iPageSize;
	bool	bPlayNavigationalSounds;
	bool	bRepeatSignaturesOnMeasureChange;
  bool	bRepeatSignaturesOnVoiceChange;
	bool	bShowDetailedText;
  bool	bShowMeasureEnds;
	bool	bUseUnicodeCharacters;
  bool  bLettersForPersianNumbers;
} Customizations;

class CCustomization : public CDialogEx
{
	DECLARE_DYNAMIC(CCustomization)

public:
	CCustomization(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCustomization();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSTOMIZATION };
#endif

	Customizations m_Values;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CButton m_RepeatSignaturesOnMeasureChange;
  CButton m_RepeatSignaturesOnVoiceChange;
	CButton m_ShowDetailedText;
  CButton m_ShowMeasureEnds;
	CButton m_PlayNavigationalSounds;
	CButton m_UseUnicodeCharacters;
	CButton m_PersianNumbers;
  CEdit m_PageSize;
};
