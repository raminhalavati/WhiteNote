// MovementSelection.cpp : implementation file
//

#include "stdafx.h"
#include "WhiteNote.h"
#include "MovementSelection.h"
#include "afxdialogex.h"


// CMovementSelection dialog

IMPLEMENT_DYNAMIC(CMovementSelection, CDialog)

CMovementSelection::CMovementSelection(NarratedMusicSheet* sheet, int current_movement, CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MOVEMENTSELECTION, pParent),
  sheet_(sheet),
  current_movement_(current_movement)
{
}

CMovementSelection::~CMovementSelection()
{
}

void CMovementSelection::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_MOVEMENT_NUMBER, m_MovementNumber);
  DDX_Control(pDX, IDC_MOVEMENTS_LIST, m_MovementsList);
}


BEGIN_MESSAGE_MAP(CMovementSelection, CDialog)
  ON_EN_CHANGE(IDC_MOVEMENT_NUMBER, &CMovementSelection::OnEnChangeMovementNumber)
  ON_LBN_SELCHANGE(IDC_MOVEMENTS_LIST, &CMovementSelection::OnLbnSelchangeMovementsList)
  ON_BN_CLICKED(IDOK, &CMovementSelection::OnBnClickedOk)
END_MESSAGE_MAP()


// CMovementSelection message handlers


BOOL CMovementSelection::OnInitDialog()
{
  CDialog::OnInitDialog();

  CString temp;

  temp.Format(L"%i", current_movement_ + 1);
  m_MovementNumber.SetWindowText(temp);
  m_MovementNumber.SetSel(-1);

  for ALL_INDICES(sheet_->Movements, movement) {
    if (sheet_->Movements[movement].MovementName.GetLength())
      temp.Format(L"%s (%i)", sheet_->Movements[movement].MovementName, movement + 1);
    else
      temp.Format(L"Number %i", movement + 1);
    m_MovementsList.InsertString(movement, temp);
  }
  m_MovementsList.SetCurSel(current_movement_);
  
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}


void CMovementSelection::OnEnChangeMovementNumber()
{
  CString temp;
  m_MovementNumber.GetWindowText(temp);
  current_movement_ = _wtoi(temp) - 1 ;
  if (0 <= current_movement_ && current_movement_ < (int)sheet_->Movements.size() && m_MovementsList.GetCurSel() !=current_movement_)
    m_MovementsList.SetCurSel(current_movement_);
}


void CMovementSelection::OnLbnSelchangeMovementsList()
{
  current_movement_ = m_MovementsList.GetCurSel();
  if (current_movement_ != -1) {
    CString old_value, new_value;
    new_value.Format(L"%i", current_movement_ + 1);
    m_MovementNumber.GetWindowText(old_value);
    if (old_value != new_value)
      m_MovementNumber.SetWindowText(new_value);
  }
}


void CMovementSelection::OnBnClickedOk()
{
  /*CString temp;
  m_MovementNumber.GetWindowText(temp);
  current_movement_ = _wtoi(temp);
  if (!current_movement_ || current_movement_ > (int)sheet_->Movements.size()) {
    AfxMessageBox(L"Selected movement is out of range.", MB_ICONERROR);
    return;
  }
  else
    current_movement_--;
  */CDialog::OnOK();
}

