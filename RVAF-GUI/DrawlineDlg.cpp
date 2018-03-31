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
	ON_WM_TIMER()
END_MESSAGE_MAP()


// DrawlineDlg message handlers


BOOL DrawlineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	SetTimer(1, 100, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void DrawlineDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent = 1){
		
	}
	CDialogEx::OnTimer(nIDEvent);
}
