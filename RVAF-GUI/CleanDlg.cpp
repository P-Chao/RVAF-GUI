// CleanDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RVAF-GUI.h"
#include "CleanDlg.h"
#include "afxdialogex.h"


// CCleanDlg dialog

IMPLEMENT_DYNAMIC(CCleanDlg, CDialogEx)

CCleanDlg::CCleanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCleanDlg::IDD, pParent)
{

}

CCleanDlg::~CCleanDlg()
{
}

void CCleanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCleanDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCleanDlg::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CCleanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(0);
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


// CCleanDlg message handlers


void CCleanDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1)){
		system("del /f /s /q log\\*");
	}

	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK2)){
		system("del /f /s /q log.txt");
	}

	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK3)){
		system("del /f /s /q tmp\\*.txt");
	}

	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK4)){
		system("del /f /s /q tmp\\*.bmp");
		system("del /f /s /q tmp\\*.jpg");
		system("del /f /s /q tmp\\*.png");
	}

	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK5)){
		system("del /f /s /q tmp\\*.pcd");
	}

	CDialogEx::OnOK();
}


