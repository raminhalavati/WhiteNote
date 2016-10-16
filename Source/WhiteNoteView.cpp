// WhiteNoteView.cpp : implementation of the CWhiteNoteView class
//

#include "stdafx.h"
#include "Mmsystem.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WhiteNote.h"
#endif

#include "WhiteNoteDoc.h"
#include "WhiteNoteView.h"
#include "SimpleQuestion.h"
#include "CommentDialog.h"
#include "LilyPondInstaller.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//#define BETA_VERSION
#define RETURN_IF_NOT_LOADED if (!m_pNarration || ! m_pNarration->Movements.size()) return

#define LILYPOND_ACTIVE
// CWhiteNoteView

IMPLEMENT_DYNCREATE(CWhiteNoteView, CFormView)

BEGIN_MESSAGE_MAP(CWhiteNoteView, CFormView)
	ON_WM_SIZE()
	ON_MESSAGE(WM_USER + 0, OnChildKeyPress)
	ON_COMMAND(ID_SETTINGS_CHANGE_PAGE_SIZE, &CWhiteNoteView::OnPlayChangepagesize)
	ON_COMMAND(ID_NAVIGATE_SELECT_MOVEMENT, &CWhiteNoteView::OnPlaySelectMovement)
	ON_COMMAND(ID_NAVIGATE_GOTO_MEASURE, &CWhiteNoteView::OnPlayGotomeasure)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATE_SELECT_MOVEMENT, &CWhiteNoteView::OnUpdateSelectMovement)
	ON_COMMAND(ID_FILE_SAVEAS, &CWhiteNoteView::OnFileSaveas)
	ON_COMMAND(ID_NAVIGATE_RIGHT_MEASURE, &CWhiteNoteView::OnRightMeasure)
	ON_COMMAND(ID_NAVIGATE_LEFT_MEASURE, &CWhiteNoteView::OnLeftMeasure)
	ON_COMMAND(ID_SETTINGS_LANGUAGE_ENGLISH, &CWhiteNoteView::OnLanguageEnglish)
	ON_COMMAND(ID_SETTINGS_LANGUAGE_FARSI, &CWhiteNoteView::OnLanguageFarsi)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_LANGUAGE_ENGLISH, &CWhiteNoteView::OnUpdateLanguageEnglish)
	ON_UPDATE_COMMAND_UI(ID_SETTINGS_LANGUAGE_FARSI, &CWhiteNoteView::OnUpdateLanguageFarsi)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATE_LEFT_MEASURE, &CWhiteNoteView::OnUpdateLeftMeasure)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATE_RIGHT_MEASURE, &CWhiteNoteView::OnUpdateRightMeasure)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATE_GOTO_MEASURE, &CWhiteNoteView::OnUpdateActiveWhenLoaded)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEAS, &CWhiteNoteView::OnUpdateActiveWhenLoaded)
	ON_COMMAND(ID_NAVIGATE_PREVIOUSVOICE, &CWhiteNoteView::OnNavigatePreviousvoice)
	ON_COMMAND(ID_NAVIGATE_NEXTVOICE, &CWhiteNoteView::OnNavigateNextvoice)
	ON_COMMAND(ID_NAVIGATE_PREVIOUSHAND, &CWhiteNoteView::OnNavigatePrevioushand)
	ON_COMMAND(ID_NAVIGATE_NEXTHAND, &CWhiteNoteView::OnNavigateNexthand)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATE_PREVIOUSHAND, &CWhiteNoteView::OnUpdateNavigatePrevioushand)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATE_NEXTHAND, &CWhiteNoteView::OnUpdateNavigateNexthand)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATE_PREVIOUSVOICE, &CWhiteNoteView::OnUpdateNavigatePreviousvoice)
	ON_UPDATE_COMMAND_UI(ID_NAVIGATE_NEXTVOICE, &CWhiteNoteView::OnUpdateNavigateNextvoice)
	ON_COMMAND(ID_OPTIONS_BEEPONCOMMANDS, &CWhiteNoteView::OnOptionsBeeponcommands)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_BEEPONCOMMANDS, &CWhiteNoteView::OnUpdateOptionsBeeponcommands)
	ON_COMMAND(ID_OPTIONS_ALWAYSSHOWSIGNATURES, &CWhiteNoteView::OnOptionsAlwaysshowsignatures)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ALWAYSSHOWSIGNATURES, &CWhiteNoteView::OnUpdateOptionsAlwaysshowsignatures)
	ON_COMMAND(ID_NAVIGATE_SHOW_SIGNATURE, &CWhiteNoteView::OnShowSignature)
	ON_WM_DRAWITEM()
	ON_COMMAND(ID_LILYPOND_CHANGEPATH, &CWhiteNoteView::OnLilypondChangepath)
	ON_COMMAND(ID_LILYPOND_AUTOMATICREFRESH, &CWhiteNoteView::OnLilypondAutomaticrefresh)
	ON_UPDATE_COMMAND_UI(ID_LILYPOND_AUTOMATICREFRESH, &CWhiteNoteView::OnUpdateLilypondAutomaticrefresh)
	ON_COMMAND(ID_LILYPOND_SHOWIMAGE, &CWhiteNoteView::OnLilypondShowimage)
	ON_COMMAND(ID_DELETECACHE_CURRENTSHEET, &CWhiteNoteView::OnDeletecacheCurrentsheet)
	ON_COMMAND(ID_DELETECACHE_ALLSHEETS, &CWhiteNoteView::OnDeletecacheAllsheets)
	ON_COMMAND(ID_DELETECACHE_AUTODELETEONEXIT, &CWhiteNoteView::OnDeletecacheAutodeleteonexit)
	ON_UPDATE_COMMAND_UI(ID_DELETECACHE_AUTODELETEONEXIT, &CWhiteNoteView::OnUpdateDeletecacheAutodeleteonexit)
	ON_COMMAND(ID_LILYPOND_PRECREATEALLIMAGES, &CWhiteNoteView::OnLilypondPrecreateallimages)
	ON_UPDATE_COMMAND_UI(ID_LILYPOND_PRECREATEALLIMAGES, &CWhiteNoteView::OnUpdateLilypondPrecreateallimages)
	ON_COMMAND(ID_IMAGES_SHOWVOICESONSEPARATESTAFFS, &CWhiteNoteView::OnImagesShowvoicesonseparatestaffs)
	ON_UPDATE_COMMAND_UI(ID_IMAGES_SHOWVOICESONSEPARATESTAFFS, &CWhiteNoteView::OnUpdateImagesShowvoicesonseparatestaffs)
	ON_COMMAND(ID_IMAGES_CHANGETEMPFOLDER, &CWhiteNoteView::OnImagesChangetempfolder)
	ON_COMMAND(ID_OPTIONS_DETAILEDTEXT, &CWhiteNoteView::OnOptionsDetailedtext)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_DETAILEDTEXT, &CWhiteNoteView::OnUpdateOptionsDetailedtext)
	ON_UPDATE_COMMAND_UI(ID_FILE_RELOAD, &CWhiteNoteView::OnUpdateFileReload)
	ON_COMMAND(ID_FILE_RELOAD, &CWhiteNoteView::OnFileReload)
	ON_COMMAND(ID_HELP_LILYPONDWEBSITE, &CWhiteNoteView::OnHelpLilypondwebsite)
	ON_COMMAND(ID_HELP_DOWNLOADLILYPOND, &CWhiteNoteView::OnHelpDownloadlilypond)
	ON_COMMAND(ID_COMMENTS_SHOW, &CWhiteNoteView::OnCommentsShow)
	ON_UPDATE_COMMAND_UI(ID_COMMENTS_SHOW, &CWhiteNoteView::OnUpdateCommentsShow)
	ON_COMMAND(ID_COMMENTS_ADD, &CWhiteNoteView::OnCommentsAdd)
	ON_COMMAND(ID_COMMENTS_SELECT_FILE, &CWhiteNoteView::OnCommentsSelectFile)
	ON_UPDATE_COMMAND_UI(ID_COMMENTS_AUTOSAVE, &CWhiteNoteView::OnUpdateCommentsAutosave)
	ON_COMMAND(ID_COMMENTS_AUTOSAVE, &CWhiteNoteView::OnCommentsAutosave)
	ON_COMMAND(ID_COMMENTS_SAVE, &CWhiteNoteView::OnCommentsSave)
	ON_UPDATE_COMMAND_UI(ID_LILYPOND_SHOWIMAGE, &CWhiteNoteView::OnUpdateLilypondShowimage)
	ON_UPDATE_COMMAND_UI(ID_DELETECACHE_CURRENTSHEET, &CWhiteNoteView::OnUpdateDeletecacheCurrentsheet)
	ON_UPDATE_COMMAND_UI(ID_DELETECACHE_ALLSHEETS, &CWhiteNoteView::OnUpdateDeletecacheAllsheets)
	ON_UPDATE_COMMAND_UI(ID_COMMENTS_ADD, &CWhiteNoteView::OnUpdateCommentsAdd)
	ON_UPDATE_COMMAND_UI(ID_COMMENTS_SAVE, &CWhiteNoteView::OnUpdateCommentsSave)
	ON_UPDATE_COMMAND_UI(ID_COMMENTS_SELECT_FILE, &CWhiteNoteView::OnUpdateCommentsSelectFile)
END_MESSAGE_MAP()

// CWhiteNoteView construction/destruction

CWhiteNoteView::CWhiteNoteView()
	: CFormView(CWhiteNoteView::IDD)
{
	m_OriginalSize.cx = m_OriginalSize.cy = 0;
	m_pNarration = NULL;
	m_pNarrationTB = NULL;
	m_CurrentImage = make_pair(-1, -1);
	m_Defaults.bLilyPondPathWarned = false;
}

CWhiteNoteView::~CWhiteNoteView()
{
	if (m_Defaults.bAutoDeleteCache)
		m_Lily.DeleteCache(true);
}

void CWhiteNoteView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NARRATION_LEFT, m_NarrationL);
	DDX_Control(pDX, IDC_NARRATION_RIGHT, m_NarrationR);
	DDX_Control(pDX, IDC_SUMMARY, m_Summary);
	DDX_Control(pDX, IDC_IMAGE, m_Image);
	DDX_Control(pDX, IDC_NARRATION_LABEL, m_NarrationLabel);
}

BOOL CWhiteNoteView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

//
//	ON UPDATE AREA
//

void CWhiteNoteView::OnUpdateLanguageEnglish(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Defaults.Language == L"EN");
}


void CWhiteNoteView::OnUpdateLanguageFarsi(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Defaults.Language == L"FA");
}


void CWhiteNoteView::OnUpdateActiveWhenLoaded(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pNarration && m_pNarration->Movements.size());
}


void CWhiteNoteView::OnUpdateSelectMovement(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pNarration && m_pNarration->Movements.size() > 1);
}


void CWhiteNoteView::OnUpdateLeftMeasure(CCmdUI *pCmdUI)
{
	bool	bReverse = m_Defaults.Language == L"FA";

	pCmdUI->Enable(m_pNarration != NULL); // GetOtherBlock('m', bReverse) != -1);
	pCmdUI->SetText(bReverse ? L"&Next Measure\tAlt+Left" : L"&Previous Measure\tAlt+Left");
}


void CWhiteNoteView::OnUpdateRightMeasure(CCmdUI *pCmdUI)
{
	bool	bReverse = m_Defaults.Language == L"FA";

	pCmdUI->Enable(m_pNarration != NULL); //GetOtherBlock('m', !bReverse) != -1);
	pCmdUI->SetText(bReverse ? L"&Previous Measure\tAlt + Right" : L"&Next Measure\tAlt + Right");
}


void CWhiteNoteView::OnUpdateNavigatePrevioushand(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetOtherBlock('h', false) != -1);
}


void CWhiteNoteView::OnUpdateNavigateNexthand(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetOtherBlock('h', true) != -1);
}


void CWhiteNoteView::OnUpdateOptionsBeeponcommands(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Defaults.bBeep);
}


void CWhiteNoteView::OnUpdateNavigatePreviousvoice(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetOtherBlock('v', false) != -1);
}


void CWhiteNoteView::OnUpdateNavigateNextvoice(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetOtherBlock('v', true) != -1);
}


void CWhiteNoteView::OnUpdateOptionsAlwaysshowsignatures(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Defaults.bShowAllSignatureText);
}


void CWhiteNoteView::OnUpdateLilypondAutomaticrefresh(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Lily.m_bReady && m_Defaults.bAutoRefreshImages);
	pCmdUI->Enable(m_Lily.m_bReady);
}

void CWhiteNoteView::OnUpdateLilypondPrecreateallimages(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Lily.m_bReady && m_pNarration);
}


void CWhiteNoteView::OnUpdateImagesShowvoicesonseparatestaffs(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Defaults.bShowVoicesOnDifferentStaffs);
	pCmdUI->Enable(m_Lily.m_bReady);
}


void CWhiteNoteView::OnUpdateLilypondShowimage(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Lily.m_bReady);
}


void CWhiteNoteView::OnUpdateDeletecacheCurrentsheet(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Lily.m_bReady);
}


void CWhiteNoteView::OnUpdateDeletecacheAllsheets(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Lily.m_bReady);
}


void CWhiteNoteView::OnUpdateDeletecacheAutodeleteonexit(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Defaults.bAutoDeleteCache);
	pCmdUI->Enable(m_Lily.m_bReady);
}


void CWhiteNoteView::OnUpdateOptionsDetailedtext(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Defaults.bDetailedText);
}


void CWhiteNoteView::OnUpdateFileReload(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pNarration != NULL);
}


void CWhiteNoteView::OnUpdateCommentsAutosave(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_Defaults.bAutoSaveComments);
	pCmdUI->Enable(m_pNarration != NULL);
}


void CWhiteNoteView::OnUpdateCommentsShow(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pNarration && GetSetComment().GetLength());
}


void CWhiteNoteView::OnUpdateCommentsSave(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pNarration != NULL);
}


void CWhiteNoteView::OnUpdateCommentsAdd(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pNarration != NULL);
}


void CWhiteNoteView::OnUpdateCommentsSelectFile(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_pNarration != NULL);
}

//
// GENERAL SETUP
//

void CWhiteNoteView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	// Get Original Size
	{
		CRect	Rect;
		GetClientRect(Rect);
		m_OriginalSize = Rect.Size();
	}
	m_Summary.m_bNormalLine = false;
	m_pNarration = &GetDocument()->m_Narration;

	SerializeDefaults(true);
	
	if (!m_pNarration->Movements.size())
	{
		m_pNarration = NULL;
		CString	Text, Version = theApp.m_FileVersion;
		if (theApp.m_bNewVersionExists)
			Version += L"\r\n\tNewer version exists at www.white-note.com";
		if (theApp.m_WebsiteMessage.GetLength())
			if (theApp.m_WebsiteMessage[0] == L'!' || theApp.m_bNewVersionExists)
			{
				CString	Message = theApp.m_WebsiteMessage.Right(theApp.m_WebsiteMessage.GetLength() - (theApp.m_WebsiteMessage[0] == L'!' ? 1 : 0));
				Version += L"\r\n\t" + Message;
			}
		Text.Format(L"WhiteNote None Visual Access to Music Sheets, Version %s.\r\nOpen an XML or MXL music file to proceed. Press F1 for help.", Version);
#ifdef BETA_VERSION
		Text += "\r\nThis version is only for internal testing and may not be working correctly. Please download official versions from www.white-note.com.";
#endif
		if (!m_Defaults.LilyPondPath.GetLength())
		{
			Text += "\r\nLilyPond is not installed. Only text description of music sheets is provided.";
			CString CurDate = CTime::GetCurrentTime().Format(L"%y%m%d");
			CString StoredDate = theApp.GetProfileString(L"Message", L"AskInstallLily", L"");
			if (StoredDate < CurDate)
			{
				CLilyPondInstaller	LilyDlg;
				switch (LilyDlg.DoModal())
				{
				case 0:
					OnHelpDownloadlilypond();
					break;

				case 1:
					theApp.WriteProfileString(L"Message", L"AskInstallLily", L"20200101");
					break;
				case 2:
					StoredDate = (CTime::GetCurrentTime() + CTimeSpan(30, 0, 0, 0)).Format(L"%y%m%d");
					theApp.WriteProfileString(L"Message", L"AskInstallLily", StoredDate);
				}					
			}
	}
		m_Summary.SetWindowText(Text);
	}
	else
	{
#ifdef LILYPOND_ACTIVE
		InitializeLilyPond();
#endif
		// Set Description
		CString	Summary, Temp;
		
		Summary.Format(L"File Name: %s", GetDocument()->m_FileName);
		
		if (m_pNarration->Credits.GetLength())
		{
			Temp.Format(L"\r\nCredits: %s", m_pNarration->Credits);
			Summary += Temp;
		}

		if (m_pNarration->Movements.size() > 1)
		{
			Temp.Format(L"\r\nMovements Count: %i ", m_pNarration->Movements.size());
			Summary += Temp;
			bool	bHasName = false;
			for ALL(m_pNarration->Movements, pMovement)
				if (pMovement->MovementName.GetLength())
					bHasName = true;
			if (bHasName)
				for ALL_INDICES(m_pNarration->Movements, i)
				{
					Temp.Format(L", %s (%i measures, %i voices)", 
            m_pNarration->Movements[i].MovementName.GetLength() ? m_pNarration->Movements[i].MovementName : L"No Name",
            m_pNarration->Movements[i].Measures.size(),
            m_pNarration->Movements[i].Measures.size() ? m_pNarration->Movements[i].Measures[0].Voices.size() : 0);
					Summary += Temp;
				}
		}
		else if (m_pNarration->Movements[0].MovementName.GetLength())
		{
			Temp.Format(L"\r\nMovement Name: %s, Measures Count: %i, Voices Count: %i", 
        m_pNarration->Movements[0].MovementName, m_pNarration->Movements[0].Measures.size(),
        m_pNarration->Movements[0].Measures.size() ? m_pNarration->Movements[0].Measures[0].Voices.size() : 0);
			Summary += Temp;
		}
    else
    {
      Temp.Format(L"\r\nOne movement with %i measures and %i voices", m_pNarration->Movements[0].Measures.size(),
        m_pNarration->Movements[0].Measures.size() ? m_pNarration->Movements[0].Measures[0].Voices.size() : 0);
      Summary += Temp;
    }

		m_Summary.SetWindowText(Summary);
		SetMovement(0);
		
		m_pNarrationTB->EnableWindow();
		m_NarrationLabel.EnableWindow();
		m_Summary.SetFocus();
		m_CurrentImage = make_pair(-1, -1);
		CreateImage();

		// Comments File
		m_Comments.FileName = ((CWhiteNoteDoc*) m_pDocument)->m_FilePath;
		if (m_Comments.FileName.GetLength() > 4)
			m_Comments.FileName = m_Comments.FileName.Left(m_Comments.FileName.GetLength() - 3) + L"comments.txt";
		LoadComments(m_Comments.FileName);
	}	
}

// Initializes LilyPond Wrapper
void CWhiteNoteView::InitializeLilyPond()
{
	if (m_Defaults.LilyPondPath.GetLength() && m_Defaults.TempFolder.GetLength() && m_pNarration)
	{
		m_Lily.Initialize(
			m_Defaults.LilyPondPath,
			m_Defaults.TempFolder,
			m_pNarration,
			GetDocument()->GetPathName());
	}
	m_CurrentImage = make_pair(-1, -1);
}


// CWhiteNoteView diagnostics

#ifdef _DEBUG
void CWhiteNoteView::AssertValid() const
{
	CFormView::AssertValid();
}

void CWhiteNoteView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CWhiteNoteDoc* CWhiteNoteView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWhiteNoteDoc)));
	return (CWhiteNoteDoc*)m_pDocument;
}
#endif //_DEBUG


// CWhiteNoteView message handlers


void CWhiteNoteView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CRect	Rect;
	GetClientRect(Rect);

	if (m_OriginalSize.cx  && ::IsWindow(m_Summary) && Rect.Width())
	{
		CSize	Delta(Rect.Width() - m_OriginalSize.cx, Rect.Height() - m_OriginalSize.cy);
		m_OriginalSize = Rect.Size();

#define RESIZE_ITEM( X , W , H )	{( X ).GetWindowRect( Rect ) ; ScreenToClient( Rect ) ; ( X ).SetWindowPos( NULL , 0 , 0 , Rect.Width() + W * Delta.cx , Rect.Height() + H * Delta.cy , SWP_NOZORDER | SWP_NOMOVE ) ;}

		RESIZE_ITEM(m_Summary, 1, 0);
		RESIZE_ITEM(m_NarrationL, 1, 0);
		RESIZE_ITEM(m_NarrationR, 1, 0);
		RESIZE_ITEM(m_Image, 1, 1);
		m_CurrentImage = make_pair(-1, -1);
		CreateImage();
	}
}


// Loads/Saves default values.
void CWhiteNoteView::SerializeDefaults(bool bLoad)
{
	if (bLoad)
	{
		m_Defaults.Language = theApp.GetProfileString(L"Defaults", L"Language", L"");
		if (m_Defaults.Language == L"")
		{
			if (AfxMessageBox(L"Do you want to set the language to Persian?", MB_YESNO | MB_ICONQUESTION) == IDNO)
			{
				m_Defaults.Language = L"EN";
				m_Defaults.bLTR = true;
			}
			else
			{
				m_Defaults.Language = L"FA";
				m_Defaults.bLTR = false;
			}

			theApp.WriteProfileString(L"Defaults", L"Language", m_Defaults.Language);
			theApp.WriteProfileInt(L"Defaults", L"LTR", m_Defaults.bLTR);
		}
		m_Defaults.bLTR = (theApp.GetProfileInt(L"Defaults", L"LTR", 1) != 0);
		m_Defaults.iPageSize = theApp.GetProfileInt(L"Defaults", L"PageSize", 8);
		m_Defaults.bBeep = (theApp.GetProfileInt(L"Defaults", L"Beep", 1) != 0);
		m_Defaults.bShowAllSignatureText = (theApp.GetProfileInt(L"Defaults", L"ShowAllSignature", 1) != 0);
		m_Defaults.LilyPondPath = theApp.GetProfileString(L"Defaults", L"LilyPondPath", L"");
		m_Defaults.bAutoRefreshImages = (theApp.GetProfileInt(L"Defaults", L"AutoRefreshImages", 1) != 0);
		m_Defaults.bAutoDeleteCache = (theApp.GetProfileInt(L"Defaults", L"AutoDeleteCache", 0) != 0);
		m_Defaults.bShowVoicesOnDifferentStaffs = (theApp.GetProfileInt(L"Defaults", L"ShowVoicesOnDifferentStaffs", 0) != 0);
		m_Defaults.TempFolder = theApp.GetProfileString(L"Defaults", L"TempFolder", L"");
		m_Defaults.bDetailedText = (theApp.GetProfileInt(L"Defaults", L"DetailedText", 1) == 1);
		m_Defaults.bAutoSaveComments = (theApp.GetProfileInt(L"Defaults", L"AutoSaveComments", 1) == 1);

		if (!m_Defaults.LilyPondPath.GetLength())
		{
			TCHAR pf[MAX_PATH];
			if (SHGetSpecialFolderPath(0, pf, CSIDL_PROGRAM_FILES, FALSE))
			{
				CString	FullPath;
				FullPath.Format(L"%s\\LilyPond\\usr\\bin\\lilypond.exe", pf);
				FILE *	hFile;
				if (!_wfopen_s(&hFile, FullPath, L"rb"))
				{
					fclose(hFile);
					m_Defaults.LilyPondPath = FullPath;
					theApp.WriteProfileString(L"Defaults", L"LilyPondPath", m_Defaults.LilyPondPath);					
				}
			}
		}

		if (!m_Defaults.TempFolder.GetLength())
		{
			AutomaticallyChooseTemp();
			if (m_Defaults.TempFolder.GetLength())
				theApp.WriteProfileString(L"Defaults", L"TempFolder", m_Defaults.TempFolder);
		}
	}
	else
	{
		theApp.WriteProfileString(L"Defaults", L"Language", m_Defaults.Language);
		theApp.WriteProfileInt(L"Defaults", L"LTR", m_Defaults.bLTR);
		theApp.WriteProfileInt(L"Defaults", L"PageSize", m_Defaults.iPageSize);
		theApp.WriteProfileInt(L"Defaults", L"Beep", m_Defaults.bBeep);
		theApp.WriteProfileInt(L"Defaults", L"ShowAllSignature", m_Defaults.bShowAllSignatureText);
		theApp.WriteProfileString(L"Defaults", L"LilyPondPath", m_Defaults.LilyPondPath);
		theApp.WriteProfileInt(L"Defaults", L"AutoRefreshImages", m_Defaults.bAutoRefreshImages);
		theApp.WriteProfileInt(L"Defaults", L"AutoDeleteCache", m_Defaults.bAutoDeleteCache);
		theApp.WriteProfileInt(L"Defaults", L"ShowVoicesOnDifferentStaffs", m_Defaults.bShowVoicesOnDifferentStaffs);
		theApp.WriteProfileString(L"Defaults", L"TempFolder", m_Defaults.TempFolder);
		theApp.WriteProfileInt(L"Defaults", L"DetailedText", m_Defaults.bDetailedText);
		theApp.WriteProfileInt(L"Defaults", L"AutoSaveComments", m_Defaults.bAutoSaveComments);
	}

	m_Translator.SetLanguage(m_Defaults.Language);
	
	if (m_pNarration && m_pNarration->Movements.size())
	{
		m_NarrationR.ShowWindow(m_Defaults.bLTR ? SW_HIDE : SW_SHOW);
		m_NarrationL.ShowWindow(m_Defaults.bLTR ? SW_SHOW : SW_HIDE);
		m_NarrationR.EnableWindow(!m_Defaults.bLTR);
		m_NarrationL.EnableWindow(m_Defaults.bLTR);
		m_pNarrationTB = m_Defaults.bLTR ? &m_NarrationL : &m_NarrationR;
	}
	m_Lily.m_bShowVoicesOnDifferentStaffs = m_Defaults.bShowVoicesOnDifferentStaffs;
}

void CWhiteNoteView::OnShowSignature()
{
	RefreshNarration(false, false, true);
}

// Refreshes lines based on current selected line.
void CWhiteNoteView::RefreshNarration(bool bVoiceChanged, bool bGoToEnd, bool bForceSingatures)
{
	RETURN_IF_NOT_LOADED;

	try
	{
		NarratedMusicSheet::MeasureText & CurMeasure = m_pNarration->Movements[m_Playing.iMovement].Measures[m_Playing.iMeasure];
		
		CStringA	Temp, LineText = "";
		{
			int	iCurStaff = CurMeasure.Voices[m_Playing.iVoice].iStaff;
			int	iCurVoice = CurMeasure.Voices[m_Playing.iVoice].iVoice;
			
			for ALL(CurMeasure.Voices, pVoice)
				if (pVoice->iStaff != iCurStaff)
				{
					LineText += CStringA(CurMeasure.Voices[m_Playing.iVoice].iStaff ? "Left" : "Right") + "_Hand; ";
					break;
				}

			for ALL(CurMeasure.Voices, pVoice)
				if (pVoice->iStaff == iCurStaff && pVoice->iVoice != iCurVoice)
				{
					Temp.Format("Voice %i; ", CurMeasure.Voices[m_Playing.iVoice].iVoice);
					LineText += Temp;
					break;
				}
			Temp = LineText;
			Temp.Remove(';');
			Temp.Replace('_', ' ');
			m_NarrationLabel.SetWindowText(CA2W(Temp));
		}

		bool	bRepeatStarters = bVoiceChanged || m_Defaults.bShowAllSignatureText || bForceSingatures;
		
		if (!bRepeatStarters)
			LineText = "";

		if (GetSetComment().GetLength() && !m_Defaults.bBeep)
			LineText += "Has_Comments;";

		vector<CStringA> & Measure = CurMeasure.Voices[m_Playing.iVoice].Text;

		for ALL(Measure, pText)
		{
			// If it is astrix marked, it should be shown only at the beginning of the line.
			if (pText->GetAt(0) == '*')
				if (bRepeatStarters)
					LineText += pText->GetBuffer() + 1;
				else
					continue;
			else
				LineText += *pText;
			LineText += "; ";
		}
		
		CString	Translation = m_Translator.TranslateText(LineText);
		m_Playing.iMeasureEndPosition = Translation.GetLength();

		Translation += m_Translator.TranslateStatement("Measure_End");
		m_Playing.iMeasureTotalSize = Translation.GetLength();
		
		m_pNarrationTB->SetWindowText(Translation);

		if (bGoToEnd)
			m_pNarrationTB->SetSel(Translation.GetLength(), Translation.GetLength());

		// Sound
		CString	Sound(L"");
		if (m_Playing.iLastMeasure != -1)
		{
			NarratedMusicSheet::MeasureText & PrevMeasure = m_pNarration->Movements[m_Playing.iMovement].Measures[m_Playing.iLastMeasure];
			for ALL_INDICES(CurMeasure.Voices, v)
			{
				bool	bCurHas = CurMeasure.Voices[v].Text.size() > 2;
				bool	bPrevHas = PrevMeasure.Voices[v].Text.size() > 2;

				if (bCurHas && !bPrevHas)
					Sound = L"MoveUp";
				else if (!bCurHas && bPrevHas)
					Sound = L"MoveDown";
				if (Sound.GetLength())
					break;
			}
		}
		if (GetSetComment().GetLength())
			Sound = L"Comment";
		else if (!Sound.GetLength())
			Sound = m_Playing.iLastMeasure == m_Playing.iMeasure ? L"VoiceChange" : L"MeasureChange";
		VoiceMessage(Sound);
		m_Playing.iLastMeasure = m_Playing.iMeasure;
		if (m_Defaults.bAutoRefreshImages)
			UpdateImage();
		else
			ShowHideImage();
	}
	catch (...)
	{
		m_pNarrationTB->SetWindowText(L"Error");
	}
}

void CWhiteNoteView::OnNavigatePreviousvoice()
{
	Move('v', false);
}


void CWhiteNoteView::OnNavigateNextvoice()
{
	Move('v', true);
}


void CWhiteNoteView::OnNavigatePrevioushand()
{
	Move('h', false);
}


void CWhiteNoteView::OnNavigateNexthand()
{
	Move('h', true);
}

void CWhiteNoteView::OnRightMeasure()
{
	Move('m', m_Defaults.bLTR);
}


void CWhiteNoteView::OnLeftMeasure()
{
	Move('m', !m_Defaults.bLTR);
}

// Returning 1 means it's processed and should not be passed to other layers.
LRESULT CWhiteNoteView::OnChildKeyPress(WPARAM wParam, LPARAM lParam)
{
	RETURN_IF_NOT_LOADED 0;
	
	if (m_pNarration)
	{
		UINT	nChar = (UINT)wParam;
		bool	bCtrl = (bool)(lParam != 0);

		if (!m_Defaults.bLTR)
			if (nChar == VK_LEFT)
				nChar = VK_RIGHT;
			else if (nChar == VK_RIGHT)
				nChar = VK_LEFT;

		switch (nChar)
		{
		case VK_DOWN:
		case VK_UP:
			if (!bCtrl)
			{
				Move('h', nChar == VK_DOWN);
				return 1;
			}
			break;
		case VK_PRIOR:
		case VK_NEXT:
			Move('p', nChar == VK_NEXT);
			return 1;
		case VK_HOME:
			if (bCtrl)
			{
				m_Playing.iMeasure = 0;
				RefreshNarration(true);
				return 1;
			}
			else
				return 0;
		case VK_END:
			if (bCtrl)
			{
				m_Playing.iMeasure = (int)m_pNarration->Movements[m_Playing.iMovement].Measures.size() - 1;
				RefreshNarration(true);
				return 1;
			}
			else 
				return 0;
		case VK_LEFT:
			if (!(int)(m_pNarrationTB->GetSel() & 0xffff))
				Move('m', false, true);
			return 0;

		case VK_RIGHT:
			int	iPos = (int)(m_pNarrationTB->GetSel() & 0xffff);
			if ((bCtrl && iPos >= m_Playing.iMeasureEndPosition) || (!bCtrl && iPos >= m_Playing.iMeasureTotalSize - 1))
			{
				Move('m', true, false);
				return 1;
			}
			else
				return 0;
		}
	}

	return 0;
}


void CWhiteNoteView::OnPlayChangepagesize()
{
	int	iNewPageSize = AskQuestion(L"Set page size:", m_Defaults.iPageSize);

	if (iNewPageSize <= 0)
		return;
		
	CString	Text;
	Text.Format(L"Set page size to %i measures?", iNewPageSize);
	if (AfxMessageBox(Text, MB_ICONQUESTION | MB_YESNO) == IDYES)
	{
		m_Defaults.iPageSize = iNewPageSize;
		SerializeDefaults(false);
	}
}


void CWhiteNoteView::OnPlaySelectMovement()
{
	RETURN_IF_NOT_LOADED;

	CString	Text;
	Text.Format(L"Select movement number between 1 and %i", m_pNarration->Movements.size());

	int	iMovementNo = AskQuestion(Text, m_Playing.iMovement + 1) - 1;

	if (iMovementNo == -2)
		return;
	else
		if (IsInRange(iMovementNo, 0, (int)m_pNarration->Movements.size()-1))
			SetMovement(iMovementNo);
		else
			AfxMessageBox(L"Selection is out of range.", MB_ICONERROR);
}


void CWhiteNoteView::OnPlayGotomeasure()
{
	RETURN_IF_NOT_LOADED;

	CString	Text;
	Text.Format(L"Select measure number between 1 and %i:", (int)m_pNarration->Movements[m_Playing.iMovement].Measures.size());

	int	iMeasure = AskQuestion(Text, m_Playing.iMeasure + 1) - 1;

	if (iMeasure == -2)
		return;
	else
		if (IsInRange(iMeasure, 0 , (int)m_pNarration->Movements[m_Playing.iMovement].Measures.size()-1))
		{
			m_Playing.iMeasure = iMeasure;
			RefreshNarration(false);
		}
		else
			AfxMessageBox(L"Selection is out of range.", MB_ICONERROR);
}


void CWhiteNoteView::OnFileSaveas()
{
	CWhiteNoteDoc * pDoc = GetDocument();

	if (!pDoc || ! m_pNarration)
		return;
	CString	FileName = pDoc->m_FileName;
	int		iPos = FileName.ReverseFind(L'.');
	if (iPos != -1)
		FileName = FileName.Left(iPos);
	
	CFileDialog	FDlg(false, L"txt", FileName, 6, L"Text File (*.txt)|*.txt|All Files (*.*)|*.*||");

	if (FDlg.DoModal() == IDOK)
	{
		FILE *	hFile;

		if (_wfopen_s(&hFile, FDlg.GetPathName(), L"wt, ccs=UTF-8"))
		{
			AfxMessageBox(L"Could not open file.", MB_ICONERROR);
			return;
		}

		if (m_pNarration->Credits.GetLength())
			fwprintf_s(hFile, L"%s\r\n", (LPCTSTR) m_pNarration->Credits);

		for ALL_INDICES(m_pNarration->Movements, p)
		{
			CStringA	Text;
			if (m_pNarration->Movements.size() || m_pNarration->Movements[p].MovementName.GetLength())
			{
				Text.Format("Movement %i: %S\r\n", p + 1, m_pNarration->Movements[p].MovementName);
				fwprintf_s(hFile, (TCHAR *)(m_Translator.TranslateText(Text).GetBuffer()));
			}
			
			for ALL_INDICES(m_pNarration->Movements[p].Measures, m)
				for ALL(m_pNarration->Movements[p].Measures[m].Voices, pVoice)
				{
					Text.Format("Staff %i; Voice %i; ", pVoice->iStaff + 1, pVoice->iVoice);
					fwprintf_s(hFile, (TCHAR *)(m_Translator.TranslateText(Text).GetBuffer()));

					for ALL(pVoice->Text, pLine)
					{
						Text.Format("%s; ", *pLine);
						fwprintf_s(hFile, (TCHAR *)(m_Translator.TranslateText(Text).GetBuffer()));
					}
					fwprintf_s(hFile, L"\r\n");
				}
		}

		fclose(hFile);
	}
}


// Go to Next/Previous Page/Measure/Staff
bool CWhiteNoteView::Move(char chWhat, bool bNext, bool bGoToEnd) // 'p'age , 'm'easure  ,'v'oice
{
	try
	{
		switch (chWhat)
		{
		case 'm': // Measure
		case 'h': // Hand
		case 'v': // Voice
		{
			int	iOther = GetOtherBlock(chWhat, bNext);
			if (iOther != -1)
			{
				if (chWhat == 'm')
					m_Playing.iMeasure = iOther;
				else
					m_Playing.iVoice = iOther;
				RefreshNarration(chWhat != 'm', bGoToEnd);
			}
			else
				throw 0;
		}
		break;

		case 'p': // Page
			{
				_ASSERTE(!bGoToEnd);
				m_Playing.iMeasure += ((bNext) ? +1 : -1) * m_Defaults.iPageSize;
				m_Playing.iMeasure = max(m_Playing.iMeasure, 0);
				m_Playing.iMeasure = min(m_Playing.iMeasure, (int)m_pNarration->Movements[m_Playing.iMovement].Measures.size() - 1);
				m_Playing.iVoice = 0;
				RefreshNarration(true, bGoToEnd);
				_ASSERTE(!bGoToEnd);
			}
			break;
		default:
			AfxMessageBox(L"Unexpected input.", MB_ICONERROR);
			return false;
		}

		return true;
	}
	catch (...)
	{
		VoiceMessage(L"error");

		return false;
	}
}

// Sets the Current Movement
void CWhiteNoteView::SetMovement(int iMovementNo)
{
	m_Playing.iMovement = iMovementNo;
	m_Playing.iMeasure = m_Playing.iVoice = 0;
	m_Playing.iLastMeasure = -1;
	RefreshNarration(true);
}


void CWhiteNoteView::OnOptionsBeeponcommands()
{
	m_Defaults.bBeep = !m_Defaults.bBeep;
	SerializeDefaults(false);
	RefreshNarration(false);
}


void CWhiteNoteView::OnLanguageEnglish()
{
	m_Defaults.Language = L"EN";
	m_Defaults.bLTR = true;
	SerializeDefaults(false);
	RefreshNarration(false);
}


void CWhiteNoteView::OnLanguageFarsi()
{
	m_Defaults.Language = L"FA";
	m_Defaults.bLTR = false;
	SerializeDefaults(false);
	RefreshNarration(false);
	if (m_pNarration)
		m_pNarrationTB->SetFocus();
}


void CWhiteNoteView::OnOptionsAlwaysshowsignatures()
{
	m_Defaults.bShowAllSignatureText = !m_Defaults.bShowAllSignatureText;
	SerializeDefaults(false);
	RefreshNarration(false);
}


void CWhiteNoteView::OnImagesShowvoicesonseparatestaffs()
{
	m_Defaults.bShowVoicesOnDifferentStaffs = !m_Defaults.bShowVoicesOnDifferentStaffs;
	SerializeDefaults(false);
	m_Lily.DeleteCache(false);
	if (m_Defaults.bAutoRefreshImages)
		UpdateImage();
}


void CWhiteNoteView::OnLilypondPrecreateallimages()
{
	m_Lily.CreateAllImages();
}

// Returns the index of Previous/Next measure/hand/voice, -1 if not available.
int CWhiteNoteView::GetOtherBlock(char chWhat, bool bNext)
{
	RETURN_IF_NOT_LOADED - 1;

	vector<NarratedMusicSheet::Voice> & Voices = m_pNarration->Movements[m_Playing.iMovement].Measures[m_Playing.iMeasure].Voices;

	switch (chWhat)
	{
	case 'm':
		if (bNext)
		{
			if (m_Playing.iMeasure + 1 < (int)m_pNarration->Movements[m_Playing.iMovement].Measures.size())
				return m_Playing.iMeasure + 1;
		}
		else
			if (m_Playing.iMeasure)
				return m_Playing.iMeasure - 1;
		break;

	case 'h':
		if (bNext)
		{
			for (int i = m_Playing.iVoice + 1; i < (int)Voices.size(); i++)
				if (Voices[i].iStaff != Voices[m_Playing.iVoice].iStaff)
					return i;
		}
		else
		{
			if (Voices[0].iStaff != Voices[m_Playing.iVoice].iStaff)
				return 0;
		}
		break;

	case 'v':
		if (bNext)
		{
			if (m_Playing.iVoice + 1 < (int)Voices.size() && Voices[m_Playing.iVoice].iStaff == Voices[m_Playing.iVoice + 1].iStaff)
				return m_Playing.iVoice + 1;
		}
		else
		{
			if (m_Playing.iVoice > 0 && Voices[m_Playing.iVoice].iStaff == Voices[m_Playing.iVoice - 1].iStaff)
				return m_Playing.iVoice - 1;
		}
	}

	return -1;
}

// Makes a sound
void CWhiteNoteView::VoiceMessage(CString What)
{
	if (m_Defaults.bBeep)
		try
		{
			CString	Path;
			Path.Format(L"%s\\Sounds\\%s.wav", theApp.m_Path, What);
			if (!PlaySound(Path, NULL, SND_FILENAME | SND_ASYNC))
				throw 0;
		}
		catch (...)
		{
			AfxMessageBox(L"Cannot play sounds.", MB_ICONERROR);
			m_Defaults.bBeep = false;
		}
}

void CWhiteNoteView::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (nIDCtl == IDC_IMAGE && !m_MeasureImage.IsNull())
	{
		BitBlt(
			lpDrawItemStruct->hDC, 
			0, 0, m_MeasureImage.GetWidth(), m_MeasureImage.GetHeight(),
			m_MeasureImage.GetDC(), 0, 0, SRCCOPY);
		m_MeasureImage.ReleaseDC();
	}
	// CFormView::OnDrawItem(nIDCtl, lpDrawItemStruct);
}


// Creates measure image.
void CWhiteNoteView::CreateImage()
{
	CRect Rect;
	m_Image.GetWindowRect(Rect);

	if (!m_MeasureImage.IsNull())
		m_MeasureImage.Destroy();
	m_MeasureImage.Create(Rect.Width(), Rect.Height(), 32);
	HDC hDC = m_MeasureImage.GetDC();
	FillRect(hDC, CRect(0, 0, m_MeasureImage.GetWidth(), m_MeasureImage.GetHeight()), GetSysColorBrush(COLOR_BTNFACE));
	m_MeasureImage.ReleaseDC();
	if (m_Defaults.bAutoRefreshImages)
		UpdateImage();
}

void CWhiteNoteView::OnLilypondChangepath()
{
	CFileDialog	FDlg(true, L"exe", m_Defaults.LilyPondPath.GetLength() ? m_Defaults.LilyPondPath : L"LilyPond.exe", 6, L"Executable Files (*.exe)||");
	if (FDlg.DoModal() == IDOK)
		m_Defaults.LilyPondPath = FDlg.GetPathName();
	else
		return;

	AfxMessageBox(L"LilyPond Path Saved.");
	SerializeDefaults(false);
	if (m_pNarration && m_pNarration->Movements.size())
		InitializeLilyPond();
}


void CWhiteNoteView::OnLilypondAutomaticrefresh()
{
	m_Defaults.bAutoRefreshImages = !m_Defaults.bAutoRefreshImages;
	SerializeDefaults(false);
	if (m_Defaults.bAutoRefreshImages)
		UpdateImage();
}


void CWhiteNoteView::OnDeletecacheCurrentsheet()
{
	m_Lily.DeleteCache(false);
}


void CWhiteNoteView::OnDeletecacheAllsheets()
{
	m_Lily.DeleteCache(true);
}


void CWhiteNoteView::OnDeletecacheAutodeleteonexit()
{
	m_Defaults.bAutoDeleteCache = !m_Defaults.bAutoDeleteCache;
	SerializeDefaults(false);
}


void CWhiteNoteView::OnOptionsDetailedtext()
{
	m_Defaults.bDetailedText = !m_Defaults.bDetailedText;
	SerializeDefaults(false);
	if (m_pNarration)
		OnFileReload();
}


void CWhiteNoteView::OnFileReload()
{
	if (GetDocument()->Reload())
		RefreshNarration(true, false, true);
	else
	{
		AfxMessageBox(L"File was not read correctly. WhiteNote should close.", MB_ICONERROR);
		PostMessage(WM_QUIT);
	}
}

void CWhiteNoteView::OnLilypondShowimage()
{
	UpdateImage(true);
}


// Shows/Hides image if it matches current text.
void CWhiteNoteView::ShowHideImage()
{
	m_Image.ShowWindow((m_CurrentImage.first == m_Playing.iMovement && m_CurrentImage.second == m_Playing.iMeasure) ? SW_SHOW : SW_HIDE);
}

// Updates measure image.
void CWhiteNoteView::UpdateImage(bool bForceRefresh)
{	
#ifdef LILYPOND_ACTIVE
	// If image is changed or asked to be refreshed
	if (!m_MeasureImage.IsNull() &&
		m_Lily.m_bReady &&
		(m_CurrentImage.first != m_Playing.iMovement || m_CurrentImage.second != m_Playing.iMeasure || bForceRefresh))
	{
		HDC hDC = m_MeasureImage.GetDC();
		FillRect(hDC, CRect(0, 0, m_MeasureImage.GetWidth(), m_MeasureImage.GetHeight()), GetSysColorBrush(COLOR_BTNFACE));

		// Get and show new image.
		CImage	Image;
		// Ask for reload of the image if you are on current image.
		if (m_Lily.GetMeasureImage(m_Playing.iMovement, m_Playing.iMeasure, Image,
			m_CurrentImage.first == m_Playing.iMovement && m_CurrentImage.second == m_Playing.iMeasure))
		{
			CSize ImageSize(Image.GetWidth(), Image.GetHeight());
			CSize ViewSize(m_MeasureImage.GetWidth(), m_MeasureImage.GetHeight());
			double dRatio = 1;

			if (ImageSize.cx > ViewSize.cx || ImageSize.cy > ViewSize.cy)
			{
				while (ImageSize.cx * dRatio > ViewSize.cx || ImageSize.cy * dRatio > ViewSize.cy)
					dRatio /= 2;
			}
			else
			{
				while (ImageSize.cx * dRatio < ViewSize.cx / 2 && ImageSize.cy * dRatio < ViewSize.cy / 2)
					dRatio *= 2;
			}

			StretchBlt(hDC, 0, 0, (int)(ImageSize.cx * dRatio), (int)(ImageSize.cy * dRatio), Image.GetDC(),
				0, 0, ImageSize.cx, ImageSize.cy, SRCCOPY);
			Image.ReleaseDC();
		}
		else
			LilyPondCheck();
		m_CurrentImage = make_pair(m_Playing.iMovement, m_Playing.iMeasure);

		// Release DC and update window.
		m_MeasureImage.ReleaseDC();
		m_Image.Invalidate();
	}
	ShowHideImage();
#endif
}


void CWhiteNoteView::OnImagesChangetempfolder()
{
	CFolderPickerDialog FPD(m_Defaults.TempFolder);

	if (FPD.DoModal() == IDOK)
	{
		OnDeletecacheAllsheets();
		m_Defaults.TempFolder = FPD.GetFolderPath();
		SerializeDefaults(false);
		InitializeLilyPond();
	}
}


// Chooses a temp folder automatically.
void CWhiteNoteView::AutomaticallyChooseTemp()
{
	CString	Temp, Current;

	GetTempPath(MAX_PATH, Temp.GetBufferSetLength(MAX_PATH));
	Temp.ReleaseBuffer();

	GetCurrentDirectory(MAX_PATH, Current.GetBufferSetLength(MAX_PATH));
	Current.ReleaseBuffer();

	for (TCHAR drive = L'C'; drive <= 'X'; drive++)
		try
		{
			// Check if it exists
		bool	bTempExists = (SetCurrentDirectory(Temp) == TRUE);
			// If not exists, create it
			if (!bTempExists)
				CreateDirectory(Temp, NULL);

			// Check if it's writable.
			FILE * hFile;
			bool	bWritable = (!_wfopen_s(& hFile, Temp + L"WhiteNoteTemp.txt", L"wb"));
			if (bWritable)
			{
				fclose(hFile);
				DeleteFile(Temp + L"WhiteNoteTemp.txt");
			}
			
			SetCurrentDirectory(Current);

			if (!bTempExists)
				RemoveDirectory(Temp);

			if (bWritable)
			{
				m_Defaults.TempFolder = Temp;
				return;
			}
			// Check Next
			Temp.Format(L"%c:\\Temp\\", drive);
		}
	catch (...)
	{ 
	}
}


void CWhiteNoteView::OnHelpLilypondwebsite()
{
	theApp.OpenInBrowser(L"http://www.lilypond.org/windows.html");
}


void CWhiteNoteView::OnHelpDownloadlilypond()
{
	theApp.OpenInBrowser(L"http://download.linuxaudio.org/lilypond/binaries/mingw/lilypond-2.18.2-1.mingw.exe");
}

// Loads the comments file.
bool CWhiteNoteView::LoadComments(CString FilePath)
{
	try
	{
		m_Comments.Texts.clear();
		FILE * hFile;
		if (_wfopen_s(&hFile, m_Comments.FileName, L"r, ccs=UTF-8"))
			throw 0;
		int	iCount;
		fwscanf_s(hFile, L"%i\n", &iCount);
		while (iCount--)
		{
			int	m, v;
			CString	Text;

			fwscanf_s(hFile, L"%i,%i\n", &m, &v);
			fgetws(Text.GetBufferSetLength(1000), 1000, hFile);
			Text.ReleaseBuffer();
			if (Text.GetLength() && Text.GetAt(Text.GetLength() - 1) == '\n')
				Text = Text.Left(Text.GetLength() - 1);
			m_Comments.Texts.insert(make_pair(make_pair(m, v), Text));
		}
		fclose(hFile);
	}
	catch (...)
	{
	}
	return false;
}

// Returns current position's comments.
CString CWhiteNoteView::GetSetComment(CString *pNewValue)
{
	pair<int, int> key = make_pair(m_Playing.iMovement * 1000 + m_Playing.iMeasure, m_Playing.iVoice);
	decltype(m_Comments.Texts.end()) pStoredValue = m_Comments.Texts.find(key);
	bool	bHasFormerValue = (pStoredValue != m_Comments.Texts.end());

	if (pNewValue)
		if (!pNewValue->GetLength())
		{
			if (bHasFormerValue)
				m_Comments.Texts.erase(key);			
		}
		else
			if (bHasFormerValue)
				pStoredValue->second = *pNewValue;
			else
				m_Comments.Texts.insert(make_pair(key, *pNewValue));
				
	else
		if (bHasFormerValue)
			return m_Comments.Texts.find(key)->second;
	return L"";
}


void CWhiteNoteView::OnCommentsShow()
{
	ShowComment(false, GetSetComment(), m_Playing.iMeasure);
}


void CWhiteNoteView::OnCommentsAdd()
{
	CString Text = ShowComment(true, GetSetComment(), m_Playing.iMeasure);
	if (Text != L"-1")
	{
		GetSetComment(&Text);
		if (m_Defaults.bAutoSaveComments)
			OnCommentsSave();
		RefreshNarration(false);
	}
}


void CWhiteNoteView::OnCommentsSelectFile()
{
	CFileDialog	FDlg(true, L"txt", m_Comments.FileName, 6, L"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
	if (FDlg.DoModal() == IDOK)
	{
		m_Comments.FileName = FDlg.GetPathName();
	}
}


void CWhiteNoteView::OnCommentsAutosave()
{
	m_Defaults.bAutoSaveComments = !m_Defaults.bAutoSaveComments;
	SerializeDefaults(false);
}


void CWhiteNoteView::OnCommentsSave()
{
	try
	{
		FILE * hFile;
		if (_wfopen_s(&hFile, m_Comments.FileName, L"w, ccs=UTF-8"))
			throw 0;
		fwprintf_s(hFile, L"%i\n", m_Comments.Texts.size());
		for ALL(m_Comments.Texts, pItem)
			fwprintf_s(hFile, L"%i,%i\n%s\n", 
				pItem->first.first , 
				pItem->first.second ,
				pItem->second.GetBuffer());
		fclose(hFile);
	}
	catch (...)
	{
		AfxMessageBox(L"Could not save comments file.", MB_ICONERROR);
	}
}

// Checks if LilyPond seems ok?
void CWhiteNoteView::LilyPondCheck()
{
	if (m_Defaults.LilyPondPath.MakeLower().Right(12) != L"lilypond.exe" && !m_Defaults.bLilyPondPathWarned)
	{
		if (AfxMessageBox(L"Lilypond's executable file seems not correct. Expected (lilypond.exe). Do you want to correct it?", MB_YESNO | MB_ICONQUESTION) == IDYES)
			OnLilypondChangepath();
		else
			m_Defaults.bLilyPondPathWarned = true;
	}
}
