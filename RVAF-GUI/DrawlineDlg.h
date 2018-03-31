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

// DrawlineDlg dialog

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
};
