#pragma once


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
};
