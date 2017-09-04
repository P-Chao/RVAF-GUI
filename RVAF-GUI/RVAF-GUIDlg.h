
// RVAF-GUIDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxpropertygridctrl.h"
#include "io.hpp"
#include <string>
#include <hash_map>

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

	template<typename T>
	struct _Node{
		T		name;
		_Node*	next;
		_Node() : name(""), next(NULL){}
		_Node(T nm) : name(nm), next(NULL){}
	};

	using Node = _Node<std::string>;
	Node dummy;

	bool ready_proto;
	svaf::SvafTask m_svaftask;
	hash_map<std::string, svaf::LayerParameter> layers;
	bool ReadCheckProtoFile(std::string filename);
	void OpenProtoFile(std::string filename);
	void ShowProperties();


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
