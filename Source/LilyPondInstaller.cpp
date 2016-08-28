// LilyPondInstaller.cpp : implementation file
//

#include "stdafx.h"
#include "WhiteNote.h"
#include "LilyPondInstaller.h"
#include "afxdialogex.h"


// CLilyPondInstaller dialog

IMPLEMENT_DYNAMIC(CLilyPondInstaller, CDialog)

CLilyPondInstaller::CLilyPondInstaller(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_LILYPONDINSTALLER, pParent)
{

}

CLilyPondInstaller::~CLilyPondInstaller()
{
}

void CLilyPondInstaller::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGE, m_Message);
}


BEGIN_MESSAGE_MAP(CLilyPondInstaller, CDialog)
	ON_BN_CLICKED(IDC_YES, &CLilyPondInstaller::OnBnClickedYes)
	ON_BN_CLICKED(IDC_NO_DONT_REMIND, &CLilyPondInstaller::OnBnClickedNoDontRemind)
	ON_BN_CLICKED(IDC_NO_DONT_REMIND_SOON, &CLilyPondInstaller::OnBnClickedNoDontRemindSoon)
END_MESSAGE_MAP()


// CLilyPondInstaller message handlers


void CLilyPondInstaller::OnBnClickedYes()
{
	EndDialog(0);
}


void CLilyPondInstaller::OnBnClickedNoDontRemind()
{
	EndDialog(1);
}


void CLilyPondInstaller::OnBnClickedNoDontRemindSoon()
{
	EndDialog(2);
}


BOOL CLilyPondInstaller::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Message.SetWindowText(L"LilyPond software is not installed and is required to show images. If you don't need image view, you don't need to install it. It's about 25 megabytes, you can also install it later by going to Help/Install Lilipond menu. Do you want to go to it's website and install it?");

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
