
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

	vector<int> toolid;

	bool isExpan;
	bool ready_proto;

	int gui_nrm_w;
	int gui_nrm_h;
	int gui_exp_w;
	int gui_exp_h;

	svaf::SvafTask m_svaftask;
	hash_map<std::string, svaf::LayerParameter> layers;
	hash_map<int, std::string> idtable;
	bool ReadCheckProtoFile(std::string filename);
	void OpenProtoFile(std::string filename);
	void GenerateProperties();
	void SetMainUILayout(int type = 0);
	void SetTopButtonLayout();

	struct ReflectPackage{
		std::string layer_name;
		google::protobuf::Message* pM;
		google::protobuf::Reflection* pR;
		google::protobuf::FieldDescriptor* pF;
		ReflectPackage() : pM(NULL), pR(NULL), pF(NULL){}
		ReflectPackage(const google::protobuf::Message* pMessage, const google::protobuf::Reflection* pReflection, const google::protobuf::FieldDescriptor* pField, std::string node_name) :
			pM(const_cast<google::protobuf::Message*>(pMessage)), pR(const_cast<google::protobuf::Reflection*>(pReflection)), pF(const_cast<google::protobuf::FieldDescriptor*>(pField)), layer_name(node_name){}
	};
	hash_map<int, ReflectPackage> pack;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnPropertyChanged(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	CButton m_selectAlgorithm;
	CMFCPropertyGridCtrl m_properaty;
	afx_msg void OnSelectAlgorithm();
	afx_msg void OnShowMoreClicked();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_showMore;
	CStatic m_zoonTool;
	CStatic m_zoonDisplay;
};
