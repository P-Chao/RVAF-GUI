#pragma once


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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};
