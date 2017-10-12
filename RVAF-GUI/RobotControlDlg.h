#pragma once
#include "RobotControl.h"
#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "MySpin.h"

// CRobotControlDlg dialog

class CRobotControlDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRobotControlDlg)

public:
	CRobotControlDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRobotControlDlg();

// Dialog Data
	enum { IDD = IDD_ROBOT_DIALOG };

protected:
	CRobotControl m_rbc;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	void UiConnectedLock(bool);
	int m_port;
	float m_A1status;
	float m_A2status;
	float m_A3status;
	float m_A4status;
	float m_A5status;
	float m_A6status;
	float m_Xstatus;
	float m_Ystatus;
	float m_Zstatus;
	float m_Astatus;
	float m_Bstatus;
	float m_Cstatus;
	float m_acc;
	float m_vec;
	float m_ABase;
	float m_BBase;
	float m_CBase;
	float m_SBase;
	float m_XBase;
	float m_YBase;
	float m_ZBase;
	float m_TBase;
	float m_A1axis;
	float m_A2axis;
	float m_A3axis;
	float m_A4axis;
	float m_A5axis;
	float m_A6axis;
	CMySpin m_spin1;
	CMySpin m_spin2;
	CMySpin m_spin3;
	CMySpin m_spin4;
	CMySpin m_spin5;
	CMySpin m_spin6;
	CComboBox m_combo;
	afx_msg void OnLink();
	afx_msg void OnLinkOff();
	afx_msg void OnMoveToBaseMark();
	afx_msg void OnResetBaseMark();
	afx_msg void OnMoveToAxisMark();
	afx_msg void OnResetAxisMark();
	afx_msg void OnGetCurrentLocation();
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnSpinRelease(WPARAM, LPARAM);
	afx_msg void OnComboSelection();
};
