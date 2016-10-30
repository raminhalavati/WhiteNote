
// WhiteNoteDoc.cpp : implementation of the CWhiteNoteDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "WhiteNote.h"
#endif

#include "WhiteNoteDoc.h"
#include "MusixXMLParser.h"
#include "MusicSheetNarrator.h"
#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWhiteNoteDoc

IMPLEMENT_DYNCREATE(CWhiteNoteDoc, CDocument)

BEGIN_MESSAGE_MAP(CWhiteNoteDoc, CDocument)
END_MESSAGE_MAP()


// CWhiteNoteDoc construction/destruction

CWhiteNoteDoc::CWhiteNoteDoc()
{
	// TODO: add one-time construction code here

}

CWhiteNoteDoc::~CWhiteNoteDoc()
{
}

BOOL CWhiteNoteDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CWhiteNoteDoc serialization

void CWhiteNoteDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		m_Narration.Clear();
		m_FileName = ar.GetFile()->GetFileName();
		CString FilePath = ar.GetFile()->GetFilePath();
		m_FilePath = FilePath;
		if (m_FileName.GetLength() > 3 && m_FileName.Right(3).MakeLower() == "mxl")
			FilePath = GetXMLFromMXL(FilePath);
		if (!ReadXMLFile(FilePath))
			AfxThrowFileException(CFileException::invalidFile);
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CWhiteNoteDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CWhiteNoteDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CWhiteNoteDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CWhiteNoteDoc diagnostics

#ifdef _DEBUG
void CWhiteNoteDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWhiteNoteDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CWhiteNoteDoc commands


// Extracts XML File from MXL File.
CString CWhiteNoteDoc::GetXMLFromMXL(CString MXLFilePath)
{
	CString	TempFolder;

	// Get Temp Folder
	{
		GetTempPath(MAX_PATH, TempFolder.GetBufferSetLength(MAX_PATH + 1));
		TempFolder.ReleaseBuffer();

		TempFolder += L"WhiteNoteTemp";
	}

	// Clean it
	{
		vector<CString>	FileNames;
		WIN32_FIND_DATA	FindData;

		HANDLE	hFind = FindFirstFile(TempFolder + L"\\*.*", &FindData);

		if (hFind && hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					FileNames.push_back(FindData.cFileName);

			} while (FindNextFile(hFind, &FindData));

			for ALL(FileNames, pFileName)
				DeleteFile(TempFolder + CString(L"\\") + *pFileName);

			DeleteFile(TempFolder + L"\\META-INF\\container.xml");
		}
	}

	CreateDirectory(TempFolder, NULL);

	// Copy and Extract
	{
		CString	ZipPath = TempFolder + L"\\MXL.ZIP";

		CopyFile(MXLFilePath, ZipPath, false);

		UnZipFile(ZipPath, TempFolder);
	}

	// Find XML
	{
		MXLFilePath = L"";

		WIN32_FIND_DATA	FindData;

		HANDLE	hFind = FindFirstFile(TempFolder + L"\\*.xml", &FindData);

		if (hFind && hFind != INVALID_HANDLE_VALUE)
			MXLFilePath = TempFolder + CString(L"\\") + FindData.cFileName;
	}

	return MXLFilePath;
}


// Reads the XML File stored in FilePath
bool CWhiteNoteDoc::ReadXMLFile(CString FilePath)
{
	bool	bDetailedText = (theApp.GetProfileInt(L"Defaults", L"DetailedText", 1) == 1);

	return CMusicXMLParser().ParsXML(FilePath, m_MusicSheet) &&
		   CMusicSheetNarrator().ConvertToText(m_MusicSheet, m_Narration, bDetailedText);
}

// Reloads a document to reflect external or settings changes.
bool CWhiteNoteDoc::Reload()
{
	return ReadXMLFile(m_strPathName);
}
