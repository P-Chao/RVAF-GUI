#pragma once
#include "afxcmn.h"
class CMySpin :
	public CSpinButtonCtrl
{
public:
	CMySpin();
	~CMySpin();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

