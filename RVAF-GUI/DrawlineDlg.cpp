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
	, m_pointCount(20)
	, m_time(0)
{

}

DrawlineDlg::~DrawlineDlg()
{
}

void DrawlineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART1, m_chart);
	DDX_Control(pDX, IDC_POS_OBJ, m_posObj);
	DDX_Control(pDX, IDC_POS_RBT, m_posRbt);
	DDX_Control(pDX, IDC_POS_ERR, m_posErr);
	DDX_Control(pDX, IDC_ANG_OBJ, m_angObj);
	DDX_Control(pDX, IDC_ANG_RBT, m_angRbt);
	DDX_Control(pDX, IDC_ANG_ERR, m_angErr);
}


BEGIN_MESSAGE_MAP(DrawlineDlg, CDialogEx)
	ON_WM_TIMER()
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
	SetTimer(1, 1000, NULL);
	points.clear();
	pointsRef.clear();
	m_chart.GetAspect().SetView3D(TRUE);
	
	//const double pi = 3.14;
	//for (int i = 0; i < 300; ++i){
		//m_chart.Series(0).GetAsPoint3D().AddXYZ(cos(2*pi*i/100), sin(2*pi*i/100), i/10,NULL,RGB(0, 0, 255));
	//}
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void DrawlineDlg::SetPointCount(int pointcount){
	m_pointCount = pointcount > 0 ? pointcount : 0;
}

void DrawlineDlg::AddPoint(double x, double y, double z){
	// set graph
	points.push_back({ x, y, z });
	if (points.size() > m_pointCount){
		points.pop_front();
	}
	m_chart.Series(0).Clear();
	int i = 0;
	for (auto it = points.begin(); it != points.end(); it++, ++i){
		if (it == --points.end()){
			m_chart.Series(0).GetAsPoint3D().AddXYZ(it->x, it->y, it->z, NULL, RGB(255, 0, 0));
		}
		else{
			m_chart.Series(0).GetAsPoint3D().AddXYZ(it->x, it->y, it->z, NULL, RGB(0, 255 - 10 * i, 255));
		}
	}
	// set text
	CString cs;
	cs.Format(L"(%.2f, %.2f, %.2f)", x, y, z);
	m_posObj.SetWindowTextW(cs);
}

void DrawlineDlg::AddPointRef(double x, double y, double z){
	pointsRef.push_back({ x, y, z });
	if (pointsRef.size() > m_pointCount){
		pointsRef.pop_front();
	}
	m_chart.Series(1).Clear();
	int i = 0;
	for (auto it = pointsRef.begin(); it != pointsRef.end(); it++, ++i){
		m_chart.Series(1).GetAsPoint3D().AddXYZ(it->x, it->y, it->z, NULL, RGB(255 - 10 * i, 255 - 10 * i, 255 - 10 * i));
	}
	// set text
	CString cs;
	cs.Format(L"(%.2f, %.2f, %.2f)", x, y, z);
	m_posRbt.SetWindowTextW(cs);
}

void DrawlineDlg::ComputeError(double x, double y, double z, double a, double b, double c,
	double rx, double ry, double rz, double ra, double rb, double rc, double arm){
	double poserr = (rx - x) * (rx - x) + (ry - y) * (ry - y) + (rz - z) * (rz - z);
	poserr = sqrt(poserr);
	SetPosError(poserr);
	// angle = = = rad
	double angerr = acos(cos(a) * cos(ra) + cos(b) * cos(rb) + cos(c) * cos(rc));
	SetAngError(angerr);
}

void DrawlineDlg::SetPosError(double err){
	CString cs;
	cs.Format(L"%.2f", err);
	m_posErr.SetWindowTextW(cs);
}

void DrawlineDlg::SetAngError(double err){
	CString cs;
	cs.Format(L"%.2f", err);
	m_angErr.SetWindowTextW(cs);
}

void DrawlineDlg::ClearPoint(){
	KillTimer(1);
	points.clear();
	pointsRef.clear();
	m_chart.Series(0).Clear();
	m_chart.Series(1).Clear();
}

void DrawlineDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1){
		m_time = m_time + 1;
		AddPoint((double)m_time + 800, cos((double)m_time / 20.0), sin((double)m_time / 20.0));
		AddPointRef((double)m_time + 800, cos((double)m_time / 10.0), sin((double)m_time / 10.0));
	}
	CDialogEx::OnTimer(nIDEvent);
}
