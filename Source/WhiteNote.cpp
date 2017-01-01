
// WhiteNote.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "WhiteNote.h"
#include "MainFrm.h"

#include "WhiteNoteDoc.h"
#include "WhiteNoteView.h"
#include <afxinet.h>
#include "afxwin.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CWhiteNoteApp

BEGIN_MESSAGE_MAP(CWhiteNoteApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CWhiteNoteApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_HELP_HELP, &CWhiteNoteApp::OnHelpHelp)
	ON_COMMAND(ID_HELP_CHECKFORUPDATE, &CWhiteNoteApp::OnHelpCheckforupdate)
END_MESSAGE_MAP()


// CWhiteNoteApp construction

CWhiteNoteApp::CWhiteNoteApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Rama.WhiteNote.2.0.0.1"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bNewVersionExists = false;
	m_FileVersion = L"2.0";
	m_WebsiteMessage = L"";
	m_bTerminateToUpdate = false;
	try
	{
		HRSRC res = ::FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
		DWORD size = ::SizeofResource(NULL, res);
		HGLOBAL mem = ::LoadResource(NULL, res);
		LPVOID raw_data = ::LockResource(mem);
		for (unsigned i = 0; i < size / 2 - 20; i++)
			if (!memcmp((TCHAR*)raw_data + i, L"FileVersion", strlen("FileVersion") * 2))
			{
				m_FileVersion = (TCHAR*)raw_data + i + 13;
				break;
			}
		::FreeResource(mem);
	}
	catch (...)
	{
	}
}

// The one and only CWhiteNoteApp object

CWhiteNoteApp theApp;


// CWhiteNoteApp initialization

BOOL CWhiteNoteApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("RAMA"));
	LoadStdProfileSettings(10);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CWhiteNoteDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CWhiteNoteView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Set Help Path
	SetHelpMode(afxHTMLHelp);
	EnableHtmlHelp();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	UpdateCheck();

	// Truncate File Version
	while (m_FileVersion.GetLength() > 2 && m_FileVersion.Right(2) == L".0")
		m_FileVersion = m_FileVersion.Left(m_FileVersion.GetLength() - 2);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Get Path
	GetModuleFileName(theApp.m_hInstance, m_Path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);

	m_Path.ReleaseBuffer();

	if (m_Path.ReverseFind(L'\\') != -1)
		m_Path = m_Path.Left(m_Path.ReverseFind(L'\\'));

	// Check if cmdInfo has file name to open, and a default path is specified is specified, check if default path should be added to the path.
	if (cmdInfo.m_strFileName.GetLength())
	{
		CString DefaultXMLPath = GetProfileString(L"Defaults", L"XMLDefaultPath", L"");
		if (DefaultXMLPath.GetLength())
		{
			FILE * hFile;
			if (_wfopen_s(&hFile, cmdInfo.m_strFileName, L"r"))
				cmdInfo.m_strFileName = DefaultXMLPath + L"\\" + cmdInfo.m_strFileName;
			else
				fclose(hFile);
		}
	}
	
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

int CWhiteNoteApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CWhiteNoteApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Title;
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TITLE, m_Title);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString	Title;
	Title.Format(L"WhiteNote None Visual Access for Music Sheets, Version %s", theApp.m_FileVersion);
	m_Title.SetWindowText(Title);

	return TRUE;  // return TRUE unless you set the focus to a control
}


// App command to run the dialog
void CWhiteNoteApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CWhiteNoteApp message handlers

void CWhiteNoteApp::OnHelpHelp()
{
	CString Language = GetProfileString(L"Defaults", L"Language", L"");

	if (Language == L"")
		Language = (AfxMessageBox(L"Help in English?", MB_YESNO | MB_ICONQUESTION)) ? L"EN" : L"FA";

	CString FileName;
	FileName.Format(L"%s\\Help\\help-%s.htm", m_Path, Language);
	OpenInBrowser(FileName);
}

vector<int>	ParseVersion(CString Version)
{
	vector<int>	Result;
	int		iCurPos = 0;
	CString	Token = Version.Tokenize(L".", iCurPos);
	while (Token != L"")
	{
		Result.push_back(_wtoi(Token));
		Token = Version.Tokenize(L".", iCurPos);
	}
	return Result;
}

// Checks for update.
bool CWhiteNoteApp::UpdateCheck(bool bForceCheck)
{
	m_WebsiteMessage = GetProfileString(L"VersionCheck", L"WebsiteMessage", L"");

	unsigned	uCheckDate;
	// Check if check is needed.
	{
		CString	LastFile = GetProfileString(L"VersionCheck", L"LastSelfVersion");
		// If the file is not changed, and we have previously found a newer version, just say it.
		if (LastFile == m_FileVersion)
		{
			if (GetProfileInt(L"VersionCheck", L"NewerExists", 0))
			{
				m_bNewVersionExists = true;
				return true;
			}
		}
		else // If the file is changed, reset flags and check agagin.
		{
			WriteProfileInt(L"VersionCheck", L"NewerExists", 0);
			WriteProfileInt(L"VersionCheck", L"LastCheck", 0);
		}
		
		// Recently Checked?
		{
			CTime	Time = CTime::GetCurrentTime();
			uCheckDate = (Time.GetYear() % 100) * 10000 + Time.GetMonth() * 100 + Time.GetDay();

			if (uCheckDate < GetProfileInt(L"VersionCheck", L"LastCheck", 0) + 7)
				return true;
		}
	}

	// Get Version from Internet.
	CString	WebsiteVersion, Message;
	try
	{
		CInternetSession	IS;
		CStdioFile *	pFile = IS.OpenURL(L"http://white-note.com/version2.htm", 1, INTERNET_FLAG_TRANSFER_BINARY);

		CString	Text;

		if (pFile)
		{
			byte *	pBuffer = new byte[10000];
			int		iSize = pFile->Read(pBuffer, 9999);
			pBuffer[iSize] = 0;
			Text = pBuffer;

			delete pBuffer;
			pFile->Close();
			delete pFile;
		}

		int	iPos1, iPos2;
		
		iPos1 = Text.Find(L"WhiteNoteVersion{");
		if (iPos1 == -1)
			return false;
		iPos1 += strlen("WhiteNoteVersion{");
		iPos2 = Text.Find(L"}", iPos1);
		if (iPos2 == -1)
			return false;
		WebsiteVersion = Text.Mid(iPos1, iPos2 - iPos1);

		iPos1 = Text.Find(L"WhiteNoteMessage{");
		if (iPos1 == -1)
			return false;
		iPos1 += strlen("WhiteNoteMessage{");
		iPos2 = Text.Find(L"}", iPos1);
		if (iPos2 == -1)
			return false;
		Message = Text.Mid(iPos1, iPos2 - iPos1);

		iPos1 = Text.Find(L"LatestFile{");
		if (iPos1 != -1)
		{
			iPos1 += strlen("LatestFile{");
			iPos2 = Text.Find(L"}", iPos1);
			if (iPos2 != -1)
				m_LatestFileLocation = Text.Mid(iPos1, iPos2 - iPos1);
		}
		else
			m_LatestFileLocation = L"";
	}
	catch (...)
	{
		return false;
	}
	m_WebsiteMessage = Message;
	
	// Compare
	{
		vector<int>	Web = ParseVersion(WebsiteVersion);
		vector<int> File = ParseVersion(CString(CW2A(m_FileVersion)));

		for (unsigned i = 0; i < min(Web.size(), File.size()); i++)
			if (Web[i] > File[i])
			{
				WriteProfileInt(L"VersionCheck", L"NewerExists", 1);
				m_bNewVersionExists = true;
				NewVersionAvailable();
			}
			else
				if (Web[i] < File[i])
					break;
	}

	WriteProfileInt(L"VersionCheck", L"LastCheck", uCheckDate);
	WriteProfileString(L"VersionCheck", L"LastSelfVersion", m_FileVersion);
	WriteProfileString(L"VersionCheck", L"WebsiteMessage", m_WebsiteMessage);

	return true;
}

void CWhiteNoteApp::OnHelpCheckforupdate()
{
	if (!UpdateCheck(true))
		AfxMessageBox(L"Could not check for update.\r\nPlease check your internet connection or refer to www.white-note.com", MB_ICONERROR);
	else
		if (!m_bNewVersionExists)
			AfxMessageBox(L"Your application is the latest available version.\r\nAutomatic check will be done next week.");	
}

// Opens a file/website in browser
void CWhiteNoteApp::OpenInBrowser(CString Path)
{
	SHELLEXECUTEINFOW	SHI;

	memset(&SHI, 0, sizeof(SHI));
	SHI.cbSize = sizeof(SHI);
	SHI.lpFile = Path.GetBuffer();
	SHI.nShow = SW_SHOW;

	ShellExecuteExW(&SHI);
}

// Is called when a newer version is available.
int CWhiteNoteApp::NewVersionAvailable()
{
	if (m_LatestFileLocation.GetLength())
	{
		if (AfxMessageBox(L"A newer version of WhiteNote can be downloaded from www.white-note.com. Do you want to do it now?", MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			AfxMessageBox(L"WhiteNote will close and new installer will be downloaded. Run it to update WhiteNote.", MB_ICONINFORMATION);
			OpenInBrowser(m_LatestFileLocation);
			m_bTerminateToUpdate = true;
			return 1;
		}
	}
	else
		AfxMessageBox(L"A newer version of WhiteNote can be downloaded from www.white-note.com.", MB_ICONINFORMATION);
	return 0;
}


BOOL CWhiteNoteApp::OnIdle(LONG lCount)
{
	if (m_bTerminateToUpdate)
		m_pMainWnd->PostMessage(WM_QUIT, 0, 0);

	return CWinApp::OnIdle(lCount);
}
