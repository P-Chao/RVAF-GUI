#include "stdafx.h"
#include "MySpin.h"


CMySpin::CMySpin()
{
}


CMySpin::~CMySpin()
{
}
BEGIN_MESSAGE_MAP(CMySpin, CSpinButtonCtrl)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


void CMySpin::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CWnd * pDlg = (CWnd*)this->GetParent();
	pDlg->SendMessage(WM_USER+8, 0, 0);
	CSpinButtonCtrl::OnLButtonUp(nFlags, point);
}
