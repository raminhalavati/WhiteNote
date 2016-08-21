
// WhiteNoteView.h : interface of the CWhiteNoteView class
//

#pragma once

#include "resource.h"
#include "afxwin.h"
#include "MyEdit.h"
#include "WhiteNoteDoc.h"
#include "Translator.h"
#include "LilyPondWrapper.h"

class CWhiteNoteView : public CFormView
{
protected: // create from serialization only
	CWhiteNoteView();
	DECLARE_DYNCREATE(CWhiteNoteView)

public:
	enum _FormResource { IDD = IDD_WHITENOTE_FORM };

// Attributes
public:
	CWhiteNoteDoc* GetDocument() const;
	NarratedMusicSheet *	m_pNarration;
	CTranslator				m_Translator;
	CLilyPondWrapper		m_Lily;

	struct _Defaults
	{
		CString	Language;
		bool	bLTR;
		int		iPageSize;
		bool	bBeep;
		bool	bShowAllSignatureText;
		CString	LilyPondPath;
		CString	TempFolder;
		bool	bAutoRefreshImages;
		bool	bAutoDeleteCache;
		bool	bShowVoicesOnDifferentStaffs;
		bool	bDetailedText;
		bool	bAutoSaveComments;
	} m_Defaults;

	struct _Comments
	{
		map<pair<int, int>, CString>	Texts;
		CString							FileName;
	} m_Comments;

	struct _Playing
	{
		int		iMovement;
		int		iVoice;
		int		iMeasure;
		int		iLastMeasure;
		int		iMeasureEndPosition;
		int		iMeasureTotalSize;
	} m_Playing;
	pair<int, int> m_CurrentImage;
// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CWhiteNoteView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnChildKeyPress(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPlayChangepagesize();
	afx_msg void OnPlaySelectMovement();
	afx_msg void OnPlayGotomeasure();
	afx_msg void OnFileSaveas();
	afx_msg void OnRightMeasure();
	afx_msg void OnLeftMeasure();
	afx_msg void OnLanguageEnglish();
	afx_msg void OnLanguageFarsi();
	afx_msg void OnUpdateLanguageEnglish(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLanguageFarsi(CCmdUI *pCmdUI);
	afx_msg void OnUpdateActiveWhenLoaded(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSelectMovement(CCmdUI *pCmdUI);
	afx_msg void OnUpdateLeftMeasure(CCmdUI *pCmdUI);
	afx_msg void OnUpdateRightMeasure(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNavigatePrevioushand(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNavigateNexthand(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNavigatePreviousvoice(CCmdUI *pCmdUI);
	afx_msg void OnUpdateNavigateNextvoice(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOptionsBeeponcommands(CCmdUI *pCmdUI);
	afx_msg void OnUpdateOptionsAlwaysshowsignatures(CCmdUI *pCmdUI);
	afx_msg void OnNavigatePreviousvoice();
	afx_msg void OnNavigateNextvoice();
	afx_msg void OnNavigatePrevioushand();
	afx_msg void OnNavigateNexthand();
	afx_msg void OnOptionsBeeponcommands();
	afx_msg void OnOptionsAlwaysshowsignatures();
	afx_msg void OnShowSignature();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnLilypondChangepath();
	afx_msg void OnLilypondAutomaticrefresh();
	afx_msg void OnUpdateLilypondAutomaticrefresh(CCmdUI *pCmdUI);
	afx_msg void OnLilypondShowimage();
	afx_msg void OnDeletecacheCurrentsheet();
	afx_msg void OnDeletecacheAllsheets();
	afx_msg void OnDeletecacheAutodeleteonexit();
	afx_msg void OnUpdateDeletecacheAutodeleteonexit(CCmdUI *pCmdUI);
	afx_msg void OnLilypondPrecreateallimages();
	afx_msg void OnUpdateLilypondPrecreateallimages(CCmdUI *pCmdUI);
	afx_msg void OnImagesShowvoicesonseparatestaffs();
	afx_msg void OnUpdateImagesShowvoicesonseparatestaffs(CCmdUI *pCmdUI);
	afx_msg void OnImagesChangetempfolder();
	afx_msg void OnOptionsDetailedtext();
	afx_msg void OnUpdateOptionsDetailedtext(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFileReload(CCmdUI *pCmdUI);
	afx_msg void OnFileReload();
	afx_msg void OnHelpLilypondwebsite();
	afx_msg void OnHelpDownloadlilypond();
	afx_msg void OnCommentsShow();
	afx_msg void OnUpdateCommentsShow(CCmdUI *pCmdUI);
	afx_msg void OnCommentsAdd();
	afx_msg void OnCommentsSelectFile();
	afx_msg void OnUpdateCommentsAutosave(CCmdUI *pCmdUI);
	afx_msg void OnCommentsAutosave();
	afx_msg void OnCommentsSave();
	afx_msg void OnUpdateLilypondShowimage(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDeletecacheCurrentsheet(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDeletecacheAllsheets(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommentsAdd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommentsSave(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCommentsSelectFile(CCmdUI *pCmdUI);

	CMyEdit	*	m_pNarrationTB;
	CMyEdit		m_NarrationL;
	CMyEdit		m_NarrationR;
	SIZE		m_OriginalSize;
	CMyEdit		m_Summary;
	CButton		m_Image;
	CStatic		m_NarrationLabel;
	CImage		m_MeasureImage;

	// Loads/Saves default values.
	void SerializeDefaults(bool bLoad);
	// Refreshes lines based on durrent selected line.
	void RefreshNarration(bool bStaffChanged, bool bGoToEnd = false, bool bForceSingatures = false);
	// Page/Measure/Staff
	bool Move(char chWhat, bool bNext, bool bGoToEnd = false); // 'p'age , 'm'easure  ,'s'taff
	// Sets the Current Movement
	void SetMovement(int iMovementNo);
	// Returns the index of Previous/Next measure/hand/voice, -1 if not available.
	int GetOtherBlock(char chWhat, bool bNext);
	// Makes a sound
	void VoiceMessage(CString What);
	// Updates measure image.
	void UpdateImage(bool bForceRefresh=false);
	// Creates measure image.
	void CreateImage();
	// Initializes LilyPond Wrapper
	void InitializeLilyPond();
	// Chooses a temp folder automatically.
	void AutomaticallyChooseTemp();
	// Loads the comments file.
	bool LoadComments(CString FilePath);
	// Returns current position's comments.
	CString GetSetComment(CString *pNewValue=NULL);
	// Shows/Hides image if it matches current text.
	void ShowHideImage();
};

#ifndef _DEBUG  // debug version in WhiteNoteView.cpp
inline CWhiteNoteDoc* CWhiteNoteView::GetDocument() const
   { return reinterpret_cast<CWhiteNoteDoc*>(m_pDocument); }
#endif

