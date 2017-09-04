
// RVAF-GUIDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxpropertygridctrl.h"
#include "io.hpp"
#include <string>

// CRVAFGUIDlg dialog
class CRVAFGUIDlg : public CDialogEx
{
// Construction
public:
	CRVAFGUIDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RVAFGUI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	svaf::SvafTask m_svaftask;
	bool ReadCheckProtoFile(std::string filename);
	void OpenProtoFile(std::string filename);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CButton m_selectAlgorithm;
	CMFCPropertyGridCtrl m_properaty;
	afx_msg void OnSelectAlgorithm();
};
