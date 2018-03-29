// DrawlineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RVAF-GUI.h"
#include "DrawlineDlg.h"
#include "afxdialogex.h"


// DrawlineDlg dialog

IMPLEMENT_DYNAMIC(DrawlineDlg, CDialogEx)

DrawlineDlg::DrawlineDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(DrawlineDlg::IDD, pParent)
{

}

DrawlineDlg::~DrawlineDlg()
{
}

void DrawlineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DrawlineDlg, CDialogEx)
END_MESSAGE_MAP()


// DrawlineDlg message handlers


BOOL DrawlineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
