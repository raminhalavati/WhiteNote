
// WhiteNoteDoc.h : interface of the CWhiteNoteDoc class
//


#pragma once
#include "Common.h"
#include "MusicSheet.h"

class CWhiteNoteDoc : public CDocument
{
protected: // create from serialization only
	CWhiteNoteDoc();
	DECLARE_DYNCREATE(CWhiteNoteDoc)

// Attributes
public:
	CString				m_FileName;
	MusicSheet			m_MusicSheet;
	NarratedMusicSheet	m_Narration;

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CWhiteNoteDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	// Extracts XML File from MXL File.
	CString GetXMLFromMXL(CString MXLFilePath);

	// Reads the XML File stored in FilePath
	bool ReadXMLFile(CString FilePath);
};
