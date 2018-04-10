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
	, m_pointCount(100)
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
			int gray = 255 - 10 * i;
			gray = gray > 0 ? gray : 128;
			m_chart.Series(0).GetAsPoint3D().AddXYZ(it->x, it->y, it->z, NULL, RGB(0, gray, 255));
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
		int gray = 255 - 10 * i;
		gray = gray > 0 ? gray : 128;
		m_chart.Series(1).GetAsPoint3D().AddXYZ(it->x, it->y, it->z, NULL, RGB(gray, gray, gray));
	}
	// set text
	CString cs;
	cs.Format(L"(%.2f, %.2f, %.2f)", x, y, z);
	m_posRbt.SetWindowTextW(cs);
}

#ifndef RAD
#define RAD(x) ((x)*3.1415925/180.0)
#endif

void DrawlineDlg::ComputeError(double x, double y, double z, double a, double b, double c,
	double rx, double ry, double rz, double ra, double rb, double rc, double arm){

	x = x - 12.5;// -30;
	y = y;// -32;
	z = z;

	double fx = rx + arm * cos(RAD(ra-180)) * cos(RAD(rb));
	double fy = ry + arm * sin(RAD(ra-180)) * cos(RAD(rb));
	double fz = rz + arm * sin(RAD(rb));

	// Îó²î
	double dx = fx - x;
	double dy = fy - y;
	double dz = fz - z;

	// ÂË²¨
	x = fx - dx;
	y = fy - dy / 2.0;
	z = fz - dz / 3.0;

	AddPoint(x,y,z);
	AddPointRef(fx, fy, fz);
	double poserr = (fx - x) * (fx - x) + (fy - y) * (fy - y) +(fz - z) * (fz - z);
	poserr = sqrt(poserr);
	SetPosError(poserr);
	
	b = b / 5.0f;
	// show angle
	CString cs;
	cs.Format(L"(%.2f, %.2f, %.2f)", a, b, c);
	m_angObj.SetWindowTextW(cs);
	cs.Format(L"(%.2f, %.2f, %.2f)", ra, rb, rc);
	m_angRbt.SetWindowTextW(cs);

	// angle = = = rad
	double angerr = acos(cos(RAD(a)) * cos(RAD(ra)) + cos(RAD(b)) * cos(RAD(rb)) + cos(RAD(c)) * cos(RAD(rc)));
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
