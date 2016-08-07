#include "stdafx.h"
#include "LilyPondWrapper.h"
#include "ProgressReport.h"
#include <stack>

CLilyPondWrapper::CLilyPondWrapper()
{
	m_pNarration= NULL;
	m_CacheFolderRoot = m_LilyPondPath = L"";
	m_bReady = false;
}


CLilyPondWrapper::~CLilyPondWrapper()
{
}


// Initializes LilyPond image wrapper.
void CLilyPondWrapper::Initialize(CString LilyPondPath, CString TempPath, NarratedMusicSheet* pSheet, CString FilePath)
{
	m_bReady = false;
	m_FailedItems.clear();
	m_LilyPondPath = LilyPondPath;
	m_CacheFolderRoot = TempPath + L"WhiteNoteCache";
	CreateDirectory(m_CacheFolderRoot, NULL);

	// Create Folder
	{
		m_FileCacheFolder = m_CacheFolderRoot + L"\\";

		// Drop Extention
		FilePath = FilePath.Left(FilePath.ReverseFind(L'.'));
		int	iPos = FilePath.ReverseFind(L'\\');
		if (iPos == -1)
			m_FileCacheFolder += FilePath;
		else
			m_FileCacheFolder += FilePath.Right(FilePath.GetLength() - iPos - 1);
	}

	if (!CreateDirectory(m_FileCacheFolder, NULL))
	{
		DWORD	dwError = GetLastError();
		if (dwError != ERROR_ALREADY_EXISTS)
		{
			AfxMessageBox(L"Cannot access temporary folder.\r\nImages cannot be created for this file.", MB_ICONERROR);
			m_FileCacheFolder = L"";
			return;
		}
	}

	m_pNarration = pSheet;

	// Check to see if file has changed
	VerifyCheckSum();
	m_bReady = true;
}

// Deletes cache for current image or all sheets.
void CLilyPondWrapper::DeleteCache(bool bAllSheets)
{
	std::stack<CString>	Folders, RDList;

	Folders.push(bAllSheets ? m_CacheFolderRoot : m_FileCacheFolder);

	while (Folders.size())
	{
		CString	Current = Folders.top();
		Folders.pop();

		WIN32_FIND_DATAW	FindData;
		HANDLE	hFind = FindFirstFile(Current + L"\\*.*", &FindData);
				
		if (hFind)
			do
			{ 
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if (FindData.cFileName[0] != L'.')
						Folders.push(Current + L"\\" + FindData.cFileName);
				}
				else
					DeleteFile(Current + L"\\" + FindData.cFileName);				
			} while (FindNextFile(hFind, &FindData));
		RDList.push(Current);
	}

	while (RDList.size())
	{
		RemoveDirectory(RDList.top());
		RDList.pop();
	}
}


// Checks to see if the checksum of the given sheet and cached images match, otherwise delets all images and writes the new checksum.
void CLilyPondWrapper::VerifyCheckSum()
{
#pragma message("Check sum verification is not implemented.")
	return;
}


// Returns requested measures image, either from buffer or new creation.
bool CLilyPondWrapper::GetMeasureImage(int iMovementNo, int iMeasureNo, CImage & Image, bool bForceRecheck)
{
	if (!m_bReady)
		return false;

	// Check Validity
	if (iMovementNo >= (int)m_pNarration->Movements.size() || iMeasureNo >= (int)m_pNarration->Movements[iMovementNo].Measures.size())
		return false;
	
	pair<int, int> key = make_pair(iMovementNo, iMeasureNo);
	if (bForceRecheck)
		m_FailedItems.erase(key);
	else
		if (m_FailedItems.find(key) != m_FailedItems.end())
			return false;

	if (!Image.IsNull())
		Image.Destroy();

	CString	ImageFileName, LilyFileName;
	ImageFileName.Format(L"%s\\%i_%03i.png", m_FileCacheFolder, iMovementNo, iMeasureNo);
	LilyFileName.Format(L"%s\\%i_%03i.ly", m_FileCacheFolder, iMovementNo, iMeasureNo);
	
	// See if it exists...
	if (Image.Load(ImageFileName) == S_OK)
		return true;

	// Create Lily Text File
	{
		NarratedMusicSheet::MeasureText & CM = m_pNarration->Movements[iMovementNo].Measures[iMeasureNo];
		CStringA	Text = "\\version \"2.18.2\" "
			"\\paper { "
			"indent = 0\\mm "
			"oddHeaderMarkup = \"\" "
			"evenHeaderMarkup = \"\" "
			"oddFooterMarkup = \"\" "
			"evenFooterMarkup = \"\" "
			"}\r\n<<\r\n";
	
		int		iLastStaff = -1;
		bool	bOpen = false;
		for ALL(CM.Voices, pVoice)
			if (pVoice->Lily.GetLength())
			{
				if (m_bShowVoicesOnDifferentStaffs || iLastStaff != pVoice->iStaff)
				{
					if (bOpen)
						Text += ">>\r\n";
					Text += "\\new Staff\r\n<<\r\n";
					iLastStaff = pVoice->iStaff;
					bOpen = true;
				}
				else
					Text += "\\new Voice\n\r\n";
				Text += pVoice->Lily;
			}
		if (bOpen)
			Text += ">>\r\n";
		Text += ">>\r\n";

		CFile File;
		DeleteFile(LilyFileName);
		if (!File.Open(LilyFileName, CFile::modeCreate | CFile::modeWrite))
		{
			m_FailedItems.insert(key);
			return false;
		}
		File.Write(Text.GetBuffer(), Text.GetLength());
		File.Close();

		// Check Open
		if (!File.Open(LilyFileName, CFile::modeRead))
			AfxMessageBox(L"File creation failed.");
		else
			File.Close();
	}

	// Run Lily
	{
		CString Command;
		Command.Format(L"-l=ERROR -dbackend=eps -dno-gs-load-fonts -dinclude-eps-fonts --png \"%s\"", LilyFileName);

		ShellExecute(NULL, NULL, m_LilyPondPath, Command, m_FileCacheFolder, SW_HIDE);
		int i;
		for (i = 0; i < 100; i++)
			if (Image.Load(ImageFileName) == S_OK)
				break;
			else
			{
				WIN32_FIND_DATA FindData;
				if (FindFirstFile(LilyFileName.Left(LilyFileName.GetLength() - 2) + L".log", &FindData) != INVALID_HANDLE_VALUE &&
					FindData.nFileSizeLow > 10)
				{
					AfxMessageBox(L"Failed log found");
					break;
				}
				else
					Sleep(100);
			}
		CleanTempFiles();
	}
	if (Image.IsNull())
		m_FailedItems.insert(key);
	return (!Image.IsNull());
}

// Removes temporary LilyPond files.
void CLilyPondWrapper::CleanTempFiles()
{
	CString Command;
	Command.Format(L"DEL \"%s\\*.eps\";DEL \"%s\\*.tex*\";DEL \"%s\\*.count\"", m_FileCacheFolder, m_FileCacheFolder, m_FileCacheFolder);
	system(CW2A(Command));
}


// Creats all images for all parts and measures.
void CLilyPondWrapper::CreateAllImages()
{
	if (!m_bReady)
	{
		AfxMessageBox(L"LilyPond is not ready.", MB_ICONERROR);
		return;
	}

	m_ThreadData.bRunnung = true;
	m_ThreadData.bStop = false;
	m_ThreadData.iProgress = 0;

	CProgressReport	PRDlg;
	PRDlg.m_pbStop = &m_ThreadData.bStop;
	PRDlg.m_pbRunning = &m_ThreadData.bRunnung;
	PRDlg.m_piProgress = &m_ThreadData.iProgress;

	_beginthread(CLilyPondWrapper::ImageCreationThread, 0, (void *)this);
	PRDlg.DoModal();
	if (m_ThreadData.bAllDone)
		AfxMessageBox(L"All images are ready.", MB_OK);
	else
		AfxMessageBox(L"Process canceled by user.", MB_OK | MB_ICONINFORMATION);
}

// Prepare Buffer
void __cdecl CLilyPondWrapper::ImageCreationThread(void * pParam)
{
	CLilyPondWrapper * pParent = (CLilyPondWrapper *)pParam;
	CImage	Image;
	vector<pair<int, int>> Tasks;
	pair<int, int> Task;

	for (Task.first = 0; Task.first < (int)pParent->m_pNarration->Movements.size(); Task.first++)
		for (Task.second = 0; Task.second < (int)pParent->m_pNarration->Movements[Task.first].Measures.size(); Task.second++)
			Tasks.push_back(Task);

	pParent->m_ThreadData.bAllDone = true;
	for ALL_INDICES(Tasks, i)
	{	
		pParent->m_ThreadData.iProgress = (100 * i) / (int)Tasks.size();

		pParent->GetMeasureImage(Tasks[i].first, Tasks[i].second, Image);
		if (pParent->m_ThreadData.bStop)
		{
			pParent->m_ThreadData.bAllDone = false;
			break;
		}
	}
	pParent->m_ThreadData.bRunnung = false;
}
