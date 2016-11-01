#pragma once
#include "afxwin.h"


// CTextOutputOptions dialog

class CTextOutputOptions : public CDialogEx
{
	DECLARE_DYNAMIC(CTextOutputOptions)

public:
	CTextOutputOptions(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTextOutputOptions();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXTOUTPUTOPTIONS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();

	struct
	{
		bool	bAllVoicesFirst;
		bool	bAllMeasuresFirst;
		bool	bRepeatSignatures;
	} m_Options;
	// Serializes settings.

	void Serialize(bool bLoad);
	
	CButton m_VoicesFirst;
	CButton m_MeasuresFirst;
	CButton m_RepeatSignatures;
	afx_msg void OnBnClickedVoicesFirst();
	afx_msg void OnBnClickedMeasuresFirst();
};
