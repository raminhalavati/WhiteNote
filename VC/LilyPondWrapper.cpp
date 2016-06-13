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
}


CLilyPondWrapper::~CLilyPondWrapper()
{
}


// Initializes LilyPond image wrapper.
void CLilyPondWrapper::Initialize(CString LilyPondPath, NarratedMusicSheet* pSheet, CString FilePath, bool bAutoRefresh)
{
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

	CImage	Image;
	GetMeasureImage(0, 0, Image);
	Image.Save(L"L:\\Temp.png");
}


// Deletes cache for current image or all sheets.
void CLilyPondWrapper::DeleteCache(bool bAllSheets)
{
	std::stack<CString>	Folders;

	Folders.push(bAllSheets ? m_CacheFolderRoot : m_FileCacheFolder);

	while (Folders.size())
	{
		CString	Current = Folders.top();
		Folders.pop();

		WIN32_FIND_DATAW	FindData;
		HANDLE	hFind = FindFirstFile(Current, &FindData);
				
		if (hFind)
			do
			{ 
				if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					Folders.push(Current + L"\\" + FindData.cFileName);
				else
					DeleteFile(Current + L"\\" + FindData.cFileName);				
			} while (FindNextFile(hFind, &FindData));
	}	
}


// Checks to see if the checksum of the given sheet and cached images match, otherwise delets all images and writes the new checksum.
void CLilyPondWrapper::VerifyCheckSum()
{
#pragma message("Check sum verification is not implemented.")
	return;
}


// Returns requested measures image, either from buffer or new creation.
bool CLilyPondWrapper::GetMeasureImage(int iPartNo, int iMeasureNo, CImage & Image)
{
	// Check Validity
	if (iPartNo >= (int)m_pNarration->Parts.size() || iMeasureNo >= (int)m_pNarration->Parts[iPartNo].Measures.size())
		return false;

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
			"} ";
	
		for ALL(CM.Voices, pVoice)
			Text += pVoice->Lily;

		CFile File;
		DeleteFile(LilyFileName);
		if (!File.Open(LilyFileName, CFile::modeCreate | CFile::modeWrite))
			return false;
		File.Write(Text.GetBuffer(), Text.GetLength());
		File.Close();
	}

	// Run Lily
	{
		CString	CurrentFolder;
		GetCurrentDirectory(MAX_PATH, CurrentFolder.GetBufferSetLength(MAX_PATH));
		SetCurrentDirectory(m_FileCacheFolder);

		CString Command;
		Command.Format(L"-dbackend=eps -dno-gs-load-fonts -dinclude-eps-fonts --png \"%s\"", LilyFileName);

		SHELLEXECUTEINFOW	SHI;
		
		memset(&SHI, 0, sizeof(SHI));
		SHI.cbSize = sizeof(SHI);
		SHI.lpFile = m_LilyPondPath.GetBuffer();
		SHI.lpParameters = Command.GetBuffer();
		SHI.nShow = SW_HIDE;

		ShellExecuteExW(&SHI);
		SetCurrentDirectory(CurrentFolder);
		CleanTempFiles();
	}
	
	return true;
}


// Removes temporary LilyPond files.
void CLilyPondWrapper::CleanTempFiles()
{
	CString Command;
	Command.Format(L"DEL \"%s\\*.eps\";DEL \"%s\\*.tex*\";DEL \"%s\\*.count\"", m_FileCacheFolder, m_FileCacheFolder, m_FileCacheFolder);
	system(CW2A(Command));
}
