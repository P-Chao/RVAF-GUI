// RobotControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RVAF-GUI.h"
#include "RobotControlDlg.h"
#include "afxdialogex.h"


// CRobotControlDlg dialog

IMPLEMENT_DYNAMIC(CRobotControlDlg, CDialogEx)

CRobotControlDlg::CRobotControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRobotControlDlg::IDD, pParent)
	, m_port(6006)
	, m_A1status(0)
	, m_A2status(0)
	, m_A3status(0)
	, m_A4status(0)
	, m_A5status(0)
	, m_A6status(0)
	, m_Xstatus(0)
	, m_Ystatus(0)
	, m_Zstatus(0)
	, m_Astatus(0)
	, m_Bstatus(0)
	, m_Cstatus(0)
	, m_acc(0.5)
	, m_vec(0.1)
	, m_ABase(0)
	, m_BBase(0)
	, m_CBase(0)
	, m_XBase(0)
	, m_YBase(0)
	, m_ZBase(0)
	, m_SBase(6)
	, m_TBase(50)
	, m_A1axis(0)
	, m_A2axis(-90)
	, m_A3axis(90)
	, m_A4axis(0)
	, m_A5axis(0)
	, m_A6axis(0)
{

}

CRobotControlDlg::~CRobotControlDlg()
{
}

void CRobotControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT7, m_port);
	DDV_MinMaxInt(pDX, m_port, 0, 65536);
	DDX_Text(pDX, IDC_EDIT8, m_A1status);
	DDX_Text(pDX, IDC_EDIT9, m_A2status);
	DDX_Text(pDX, IDC_EDIT10, m_A3status);
	DDX_Text(pDX, IDC_EDIT11, m_A4status);
	DDX_Text(pDX, IDC_EDIT12, m_A5status);
	DDX_Text(pDX, IDC_EDIT13, m_A6status);
	DDX_Text(pDX, IDC_EDIT14, m_Xstatus);
	DDX_Text(pDX, IDC_EDIT15, m_Ystatus);
	DDX_Text(pDX, IDC_EDIT16, m_Zstatus);
	DDX_Text(pDX, IDC_EDIT17, m_Astatus);
	DDX_Text(pDX, IDC_EDIT18, m_Bstatus);
	DDX_Text(pDX, IDC_EDIT19, m_Cstatus);
	DDX_Text(pDX, IDC_EDIT26, m_acc);
	DDX_Text(pDX, IDC_EDIT31, m_vec);
	DDX_Text(pDX, IDC_EDIT27, m_ABase);
	DDV_MinMaxFloat(pDX, m_ABase, -180, 180);
	DDX_Text(pDX, IDC_EDIT28, m_BBase);
	DDV_MinMaxFloat(pDX, m_BBase, -180, 180);
	DDX_Text(pDX, IDC_EDIT29, m_CBase);
	DDV_MinMaxFloat(pDX, m_CBase, -180, 180);
	DDX_Text(pDX, IDC_EDIT30, m_SBase);
	DDX_Text(pDX, IDC_EDIT32, m_XBase);
	DDX_Text(pDX, IDC_EDIT33, m_YBase);
	DDX_Text(pDX, IDC_EDIT34, m_ZBase);
	DDX_Text(pDX, IDC_EDIT35, m_TBase);
	DDX_Text(pDX, IDC_EDIT20, m_A1axis);
	DDX_Text(pDX, IDC_EDIT21, m_A2axis);
	DDX_Text(pDX, IDC_EDIT22, m_A3axis);
	DDX_Text(pDX, IDC_EDIT23, m_A4axis);
	DDX_Text(pDX, IDC_EDIT24, m_A5axis);
	DDX_Text(pDX, IDC_EDIT25, m_A6axis);
	DDX_Control(pDX, IDC_COMBO1, m_combo);
}


BEGIN_MESSAGE_MAP(CRobotControlDlg, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CRobotControlDlg::OnLink)
	ON_BN_CLICKED(IDC_BUTTON7, &CRobotControlDlg::OnLinkOff)
	ON_BN_CLICKED(IDC_BUTTON5, &CRobotControlDlg::OnMoveToBaseMark)
	ON_BN_CLICKED(IDC_BUTTON6, &CRobotControlDlg::OnResetBaseMark)
	ON_BN_CLICKED(IDC_BUTTON3, &CRobotControlDlg::OnMoveToAxisMark)
	ON_BN_CLICKED(IDC_BUTTON4, &CRobotControlDlg::OnResetAxisMark)
	ON_BN_CLICKED(IDC_BUTTON2, &CRobotControlDlg::OnGetCurrentLocation)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CRobotControlDlg::OnComboSelection)
END_MESSAGE_MAP()


// CRobotControlDlg message handlers

void CRobotControlDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}


BOOL CRobotControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_rbc.Create(this);
	UiConnectedLock(false);

	m_combo.AddString(_T("Axis"));
	m_combo.AddString(_T("Base"));
	m_combo.AddString(_T("Tool"));
	m_combo.SetCurSel(0);
	m_rbc.SetAxis(0);
	//SetDlgItemText();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRobotControlDlg::UiConnectedLock(bool linked){
	if (linked){
		CWnd *p = GetDlgItem(IDC_BUTTON1);
		p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON2);
		p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON3);
		p->EnableWindow(true);
		//p = GetDlgItem(IDC_BUTTON4);
		//p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON5);
		p->EnableWindow(true);
		//p = GetDlgItem(IDC_BUTTON6);
		//p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON7);
		p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON8);
		p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON9);
		p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON10);
		p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON11);
		p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON12);
		p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON13);
		p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON14);
		p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON15);
		p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON16);
		p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON17);
		p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON18);
		p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON19);
		p->EnableWindow(true);
	} else{
		CWnd *p = GetDlgItem(IDC_BUTTON1);
		p->EnableWindow(true);
		p = GetDlgItem(IDC_BUTTON2);
		p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON3);
		p->EnableWindow(false);
		//p = GetDlgItem(IDC_BUTTON4);
		//p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON5);
		p->EnableWindow(false);
		//p = GetDlgItem(IDC_BUTTON6);
		//p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON7);
		p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON8);
		p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON9);
		p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON10);
		p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON11);
		p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON12);
		p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON13);
		p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON14);
		p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON15);
		p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON16);
		p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON17);
		p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON18);
		p->EnableWindow(false);
		p = GetDlgItem(IDC_BUTTON19);
		p->EnableWindow(false);
	}
}

void CRobotControlDlg::OnLink()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_rbc.SetPort(m_port);
	if (m_rbc.Link()){
		UiConnectedLock(true);
	} else{
		UiConnectedLock(false);
		MessageBox(L"Á¬½ÓÊ§°Ü");
	}
}


void CRobotControlDlg::OnLinkOff()
{
	// TODO: Add your control notification handler code here
	m_rbc.LinkOff();
	UiConnectedLock(false);
}


void CRobotControlDlg::OnMoveToBaseMark()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_rbc.MoveToBaseMark();
}


void CRobotControlDlg::OnResetBaseMark()
{
	// TODO: Add your control notification handler code here
	m_rbc.ResetBaseMark();
}


void CRobotControlDlg::OnMoveToAxisMark()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_rbc.MoveToAxisMark();
}


void CRobotControlDlg::OnResetAxisMark()
{
	// TODO: Add your control notification handler code here
	m_rbc.ResetAxisMark();
}


void CRobotControlDlg::OnGetCurrentLocation()
{
	// TODO: Add your control notification handler code here
	m_rbc.GetCurrentMark();
}


void CRobotControlDlg::OnComboSelection()
{
	// TODO: Add your control notification handler code here
	int n_Sel = m_combo.GetCurSel();
	m_rbc.SetAxis(n_Sel);
}


BOOL CRobotControlDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (WM_LBUTTONDOWN == pMsg->message)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON8)->m_hWnd)
			m_rbc.DecA1();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON9)->m_hWnd)
			m_rbc.AddA1();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON10)->m_hWnd)
			m_rbc.DecA2();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON11)->m_hWnd)
			m_rbc.AddA2();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON12)->m_hWnd)
			m_rbc.DecA3();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON13)->m_hWnd)
			m_rbc.AddA3();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON14)->m_hWnd)
			m_rbc.DecA4();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON15)->m_hWnd)
			m_rbc.AddA4();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON16)->m_hWnd)
			m_rbc.DecA5();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON17)->m_hWnd)
			m_rbc.AddA5();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON18)->m_hWnd)
			m_rbc.DecA6();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON19)->m_hWnd)
			m_rbc.AddA6();
	}
	if (WM_LBUTTONUP == pMsg->message)
	{
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON8)->m_hWnd)
			m_rbc.StopMove();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON9)->m_hWnd)
			m_rbc.StopMove();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON10)->m_hWnd)
			m_rbc.StopMove();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON11)->m_hWnd)
			m_rbc.StopMove();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON12)->m_hWnd)
			m_rbc.StopMove();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON13)->m_hWnd)
			m_rbc.StopMove();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON14)->m_hWnd)
			m_rbc.StopMove();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON15)->m_hWnd)
			m_rbc.StopMove();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON16)->m_hWnd)
			m_rbc.StopMove();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON17)->m_hWnd)
			m_rbc.StopMove();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON18)->m_hWnd)
			m_rbc.StopMove();
		if (pMsg->hwnd == GetDlgItem(IDC_BUTTON19)->m_hWnd)
			m_rbc.StopMove();
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
