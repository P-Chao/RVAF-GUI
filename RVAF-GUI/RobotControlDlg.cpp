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
{

}

CRobotControlDlg::~CRobotControlDlg()
{
}

void CRobotControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRobotControlDlg, CDialogEx)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CRobotControlDlg message handlers


void CRobotControlDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}
