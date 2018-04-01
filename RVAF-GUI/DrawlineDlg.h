#pragma once
//#include "tchart1.h"
//#include "CTChart.h"
//#include "CAspect.h"
//#include "CAxis.h"
//#include "CAxes.h"
//#include "CSeries.h"
//#include "CPoint3DSeries.h"
#include "teechart\tchart.h"
#include "teechart\axes.h"
#include "teechart\axis.h"
#include "teechart\aspect.h"
#include "teechart\series.h"
#include "teechart\point3dseries.h"
#include "teechart\teepoint3d.h"

#include <list>

// DrawlineDlg dialog

struct Point{
	double x;
	double y;
	double z;
	Point(double x_, double y_, double z_) : x(x_), y(y_), z(z_){}
};

class DrawlineDlg : public CDialogEx
{
	DECLARE_DYNAMIC(DrawlineDlg)

public:
	DrawlineDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~DrawlineDlg();

// Dialog Data
	enum { IDD = IDD_DRAWLINE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTChart m_chart;
	//CTchart1 m_chart;
protected:
	int m_time;
	int m_pointCount;
	std::list<Point> points;

public:
	void SetPointCount(int pointcount);
	void AddPoint(double x, double y, double z);
	void ClearPoint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
