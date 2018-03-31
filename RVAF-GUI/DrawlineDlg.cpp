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
	DDX_Control(pDX, IDC_TCHART1, m_chart);
}


BEGIN_MESSAGE_MAP(DrawlineDlg, CDialogEx)
END_MESSAGE_MAP()


// DrawlineDlg message handlers


BOOL DrawlineDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	/*CSeries lineSeries = (CSeries)m_chart.Series(0);
	lineSeries.Clear();
	((CPoint3DSeries)lineSeries.get_asPoint3D()).AddXYZ(1, 2, 3, NULL, RGB(0, 0, 255));
	((CPoint3DSeries)lineSeries.get_asPoint3D()).AddXYZ(10, 3, 7, NULL, RGB(0, 0, 255));
	lineSeries.Clear();*/

	m_chart.GetAspect().SetView3D(TRUE);
	const double pi = 3.14;
	for (int i = 0; i < 300; ++i){
		m_chart.Series(0).GetAsPoint3D().AddXYZ(cos(2*pi*i/100), sin(2*pi*i/100), i/10,NULL,RGB(0, 0, 255));
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
