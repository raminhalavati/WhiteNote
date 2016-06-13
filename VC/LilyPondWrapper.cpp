#include "stdafx.h"
#include "LilyPondWrapper.h"
#include <stack>

CLilyPondWrapper::CLilyPondWrapper()
{
	m_pNarration= NULL;
	GetTempPath(MAX_PATH, m_CacheFolderRoot.GetBufferSetLength(MAX_PATH));
	m_CacheFolderRoot.ReleaseBuffer();
	m_CacheFolderRoot += L"WhiteNoteCache";
	m_LilyPondPath = L"";
	CreateDirectory(m_CacheFolderRoot, NULL);
	m_ThreadCurrent = make_pair(-1, -1);
	m_bThreadRunning = false;
	m_bStopThread = false;
}


CLilyPondWrapper::~CLilyPondWrapper()
{
	m_bStopThread = true;
	while (m_bThreadRunning)
		Sleep(100);
}


// Initializes LilyPond image wrapper.
void CLilyPondWrapper::Initialize(CString LilyPondPath, NarratedMusicSheet* pSheet, CString FilePath, bool bAutoRefresh)
{
	m_ThreadCurrent = make_pair(-1, -1);
	m_bThreadRunning = false;
	m_bStopThread = false;
	m_FailedItems.clear();
	m_LilyPondPath = LilyPondPath;
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

	if (bAutoRefresh)
	{
		m_bThreadRunning = true;
		_beginthread(CLilyPondWrapper::BufferBuilder, 0, (void *)this);
	}
}

// Prepare Buffer
void __cdecl CLilyPondWrapper::BufferBuilder(void * pParam)
{
	CLilyPondWrapper * pParent = (CLilyPondWrapper *)pParam;
	CImage	Image;
	for ALL_INDICES(pParent->m_pNarration->Parts, iPart)
		for ALL_INDICES(pParent->m_pNarration->Parts[iPart].Measures, iMeasure)
		{			
			pParent->GetMeasureImage(iPart, iMeasure, Image, true);
			if (pParent->m_bStopThread)
			{
				iPart = 1000;
				break;
			}
		}
	pParent->m_ThreadCurrent = make_pair(-1, -1);
	pParent->m_bThreadRunning = false;
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
bool CLilyPondWrapper::GetMeasureImage(int iPartNo, int iMeasureNo, CImage & Image, bool bCalledFromThread, bool bForceRecheck)
{
	if (bCalledFromThread)
		m_ThreadCurrent = make_pair(iPartNo, iMeasureNo);
	else
		while (iPartNo == m_ThreadCurrent.first && iMeasureNo == m_ThreadCurrent.second)
			Sleep(100);

	// Check Validity
	if (iPartNo >= (int)m_pNarration->Parts.size() || iMeasureNo >= (int)m_pNarration->Parts[iPartNo].Measures.size())
		return false;
	
	pair<int, int> key = make_pair(iPartNo, iMeasureNo);
	if (bForceRecheck)
		m_FailedItems.erase(key);
	else
		if (m_FailedItems.find(key) != m_FailedItems.end())
			return false;

	if (!Image.IsNull())
		Image.Destroy();

	CString	ImageFileName, LilyFileName;
	ImageFileName.Format(L"%s\\%i_%03i.png", m_FileCacheFolder, iPartNo, iMeasureNo);
	LilyFileName.Format(L"%s\\%i_%03i.ly", m_FileCacheFolder, iPartNo, iMeasureNo);
	
	// See if it exists...
	if (Image.Load(ImageFileName) == S_OK)
		return true;

	// Create Lily Text File
	{
		NarratedMusicSheet::MeasureText & CM = m_pNarration->Parts[iPartNo].Measures[iMeasureNo];
		CStringA	Text = "\\version \"2.18.2\" "
			"\\paper { "
			"indent = 0\\mm "
			"oddHeaderMarkup = \"\" "
			"evenHeaderMarkup = \"\" "
			"oddFooterMarkup = \"\" "
			"evenFooterMarkup = \"\" "
			"} <<";
	
		for ALL(CM.Voices, pVoice)
			Text += pVoice->Lily;
		Text += " >>";

		CFile File;
		DeleteFile(LilyFileName);
		if (!File.Open(LilyFileName, CFile::modeCreate | CFile::modeWrite))
		{
			m_FailedItems.insert(key);
			return false;
		}
		File.Write(Text.GetBuffer(), Text.GetLength());
		File.Close();
	}

	// Run Lily
	{
		CString Command;
		Command.Format(L"-l=ERROR -dbackend=eps -dno-gs-load-fonts -dinclude-eps-fonts --png \"%s\"", LilyFileName);

		SHELLEXECUTEINFOW	SHI;
		
		memset(&SHI, 0, sizeof(SHI));
		SHI.cbSize = sizeof(SHI);
		SHI.lpFile = m_LilyPondPath.GetBuffer();
		SHI.lpParameters = Command.GetBuffer();
		SHI.lpDirectory = m_FileCacheFolder.GetBuffer();
		SHI.nShow = SW_HIDE;

		ShellExecuteExW(&SHI);
		for (int i = 0; i < 30; i++)
			if (Image.Load(ImageFileName) == S_OK)
				break;
			else
			{
				WIN32_FIND_DATA FindData;
				if (FindFirstFile(LilyFileName.Left(LilyFileName.GetLength() - 2) + L".log", &FindData) != INVALID_HANDLE_VALUE &&
					FindData.nFileSizeLow > 10)
					break;
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
