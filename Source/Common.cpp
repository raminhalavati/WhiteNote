#include "StdAfx.h"
#include "Common.h"
#include <math.h>

int		Round(double d)
{
	double	f = floor(d) ;
	double	c = ceil(d) ;

	return (d - f < c - d) ? (int) f : (int) c ;
}

int CALLBACK BrowseForFolderCallback(HWND hwnd,UINT uMsg,LPARAM lp, LPARAM pData)
{
	TCHAR szPath[MAX_PATH];

	switch(uMsg)
	{
		case BFFM_INITIALIZED:
			SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
			break;

		case BFFM_SELCHANGED: 
			if (SHGetPathFromIDList((LPITEMIDLIST) lp ,szPath)) 
			{
				SendMessage(hwnd, BFFM_SETSTATUSTEXT,0,(LPARAM)szPath);	

			}
			break;
	}

	return 0;
}

bool	SelectFolder(HWND hWnd , CString Title , CString & theFolder)
{
	bool	bResult = FALSE ;

	try
	{
		BROWSEINFO bi;
		TCHAR szPath[ MAX_PATH + 1 ] ;

		LPITEMIDLIST pidl ;
		LPMALLOC pMalloc;

		if (SUCCEEDED(SHGetMalloc(& pMalloc))) 
		{
			bi.hwndOwner = hWnd ;
			bi.pidlRoot = NULL ;
			bi.pszDisplayName = NULL ;
			bi.lpszTitle = Title.GetBuffer() ;
			bi.ulFlags = BIF_STATUSTEXT | BIF_NEWDIALOGSTYLE | BIF_EDITBOX ;
			bi.lpfn = BrowseForFolderCallback;
			if (theFolder.GetLength())
				bi.lParam = (LPARAM) theFolder.GetBuffer() ;
			else
				bi.lParam = NULL ;
			
			pidl = SHBrowseForFolder(& bi);

			if (pidl)
			{
				if (SHGetPathFromIDList(pidl , szPath)) 
					bResult = true ;
				
			   pMalloc->Free(pidl) ;
			   pMalloc->Release() ;            			
			}

			theFolder = szPath ;
		}

		if (! bResult)
			throw 0 ;
	}
	catch(...)
	{
		return false ;
	}

	return bResult ;
}

void	GetFiles(vector<CString> & FileNames , CString Filter , CString Title , CString InitDir)
{
	CFileDialog	FDlg(true , NULL , NULL , 6 | OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST , Filter) ;

	if (Title.GetLength())
		FDlg.m_ofn.lpstrTitle = Title.GetBuffer() ;

	if (InitDir.GetLength())
		FDlg.m_ofn.lpstrInitialDir = InitDir.GetBuffer() ;

	FDlg.m_ofn.lpstrFile = new TCHAR[ (MAX_PATH + 1) * 10000 ] ;
	FDlg.m_ofn.nMaxFile = (MAX_PATH + 1) * 10000 ;
	FDlg.m_ofn.lpstrFile[ 0 ] = 0 ;

	if (FDlg.DoModal() == IDOK)
	{
		POSITION	Pos = FDlg.GetStartPosition() ;

		while(Pos)
			FileNames.push_back(FDlg.GetNextPathName(Pos)) ;		
	}
	
	delete FDlg.m_ofn.lpstrFile ;
}

template<class T>
tinyxml2::XMLElement * GetXMLNestedElement(T * pDoc , char * pNode0 , char * pNode1 , char * pNode2 , char * pNode3  , char * pNode4 , char * pNode5)
{
	tinyxml2::XMLElement *	pItem = pDoc->FirstChildElement(pNode0) ;

	char *	pNodes[ 6 ] ;

	pNodes[ 0 ] = pNode0 ;
	pNodes[ 1 ] = pNode1 ;
	pNodes[ 2 ] = pNode2 ;
	pNodes[ 3 ] = pNode3 ;
	pNodes[ 4 ] = pNode4 ;
	pNodes[ 5 ] = pNode5 ;

	for DRANGE(i, 1, 6)
		if (pItem && pNodes[i])
			pItem = pItem->FirstChildElement(pNodes[i]);
		else
			break;

	return pItem ;
}

template<class T>
const char * GetXMLNestedText(T * pDoc , char * pNode0 , char * pNode1 , char * pNode2 , char * pNode3 , char * pNode4 , char * pNode5)
{
	tinyxml2::XMLElement *	pItem = GetXMLNestedElement(pDoc , pNode0 , pNode1 , pNode2 , pNode3 , pNode4 , pNode5) ;

	if (pItem)
		return pItem->GetText() ;
	else
		return NULL ;
}

void	GetXMLNestedTemplateActivator()
{
	tinyxml2::XMLDocument *	pDoc = NULL ;
	tinyxml2::XMLElement *	pElem = NULL ;

	GetXMLNestedElement(pDoc , "") ;
	GetXMLNestedElement(pElem , "") ;

	GetXMLNestedText(pDoc , "") ;
	GetXMLNestedText(pElem , "") ;
}

// Unzip
void UnZipFile(CString strSrc, CString strDest)
{
	BSTR source = strSrc.AllocSysString();
	BSTR dest = strDest.AllocSysString();

	HRESULT hResult = S_FALSE;
	IShellDispatch *pIShellDispatch = NULL;
	Folder *pToFolder = NULL;		
	VARIANT variantDir, variantFile, variantOpt;

	CoInitialize(NULL);

	hResult = CoCreateInstance(CLSID_Shell, NULL, CLSCTX_INPROC_SERVER, 
		IID_IShellDispatch, (void **)&pIShellDispatch);
	if (SUCCEEDED(hResult))
	{
		VariantInit(&variantDir);
		variantDir.vt = VT_BSTR;
		variantDir.bstrVal = dest;
		hResult = pIShellDispatch->NameSpace(variantDir, &pToFolder);

		if  (SUCCEEDED(hResult))
		{
			Folder *pFromFolder = NULL;
			VariantInit(&variantFile);
            variantFile.vt = VT_BSTR;
            variantFile.bstrVal = source;
			pIShellDispatch->NameSpace(variantFile, &pFromFolder);

			FolderItems *fi = NULL;
			pFromFolder->Items(&fi);

			VariantInit(&variantOpt);
            variantOpt.vt = VT_I4;
            variantOpt.lVal = FOF_NO_UI;			

			VARIANT newV;
			VariantInit(&newV);
			newV.vt = VT_DISPATCH;
			newV.pdispVal = fi;
			hResult = pToFolder->CopyHere(newV, variantOpt);
			Sleep(1000);
			pFromFolder->Release();
            pToFolder->Release();
		}
		pIShellDispatch->Release();
	}

	CoUninitialize();
}

int	_safe_atoi(const char * pchText)
{
	if (pchText && pchText[0])
		return Round(atof(pchText));
	else
		return -1;
}

char _safe_first_upper(const char * pchText)
{
	if (pchText)
		return toupper(pchText[0]);
	else
		return 0;
}

CString	NodeToText(XMLElem * pNode)
{
	CString	Text(L"[");

	Text += pNode->Name();

	const tinyxml2::XMLAttribute * pAttr = pNode->FirstAttribute();

	if (pAttr)
	{
		Text += " ";
		Text += pAttr->Name();
		Text += "=";
		Text += pAttr->Value();
		Text += " ";
	}

	Text += "]";

	return Text;
}
