
// RVAF-GUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RVAF-GUI.h"
#include "RVAF-GUIDlg.h"
#include "afxdialogex.h"
#include <tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define GUI_NRM_W 393
#define GUI_NRM_H 697
#define GUI_EXP_W 1128
#define GUI_EXP_H GUI_NRM_H
#define GUI_OFF_X 200 // 出口相对屏幕初始位置
#define GUI_OFF_Y 200
#define TOOL_MARGN 1 // 工具与左侧间隔
#define TOOL_WIDTH 45 // 工具与左侧宽度
#define DISP_MARGN 2 // 显示区边界宽度

#define PROCESS_DETECT_TIMER 1

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnSize(UINT nType, int cx, int cy);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Work Thread

UINT ThreadReciveData(LPVOID lpParam){
	CRVAFGUIDlg *maindlg = (CRVAFGUIDlg*)lpParam;
	maindlg->ReciveDataInterprocess();
	return 0;
}

UINT ThreadReciveInfo(LPVOID lpParam){
	CRVAFGUIDlg *maindlg = (CRVAFGUIDlg*)lpParam;
	maindlg->ReciveInfoInterprocess();
	return 0;
}

// CRVAFGUIDlg dialog



CRVAFGUIDlg::CRVAFGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRVAFGUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRVAFGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SETALG, m_selectAlgorithm);
	DDX_Control(pDX, IDC_MFCPROPERTYGRID1, m_properaty);
	DDX_Control(pDX, IDC_BUTTON_MORE, m_showMore);
	DDX_Control(pDX, IDC_ZOON_TOOL, m_zoonTool);
	DDX_Control(pDX, IDC_ZOON_DISP, m_zoonDisplay);
	DDX_Control(pDX, IDC_ZOON_DISP2, m_zoonDisp2);
	DDX_Control(pDX, IDC_ZOON_DISP3, m_zoonDisp3);
	DDX_Control(pDX, IDC_ZOON_DISP4, m_zoonDisp4);
	DDX_Control(pDX, IDC_RICHEDIT21, m_editMsg);
	DDX_Control(pDX, IDC_VTK1, m_vtk1);
	DDX_Control(pDX, IDC_VTK2, m_vtk2);
}

BEGIN_MESSAGE_MAP(CRVAFGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_BN_CLICKED(IDC_BUTTON_SETALG, &CRVAFGUIDlg::OnSelectAlgorithm)
	ON_BN_CLICKED(IDC_BUTTON_MORE, &CRVAFGUIDlg::OnShowMoreClicked)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON6, &CRVAFGUIDlg::OnSaveProtoText)
	ON_BN_CLICKED(IDC_BUTTON7, &CRVAFGUIDlg::OnSaveProtoBinary)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON3, &CRVAFGUIDlg::OnRunSvafTask)
	ON_BN_CLICKED(IDC_BUTTON5, &CRVAFGUIDlg::OnStopSvafTask)
	ON_BN_CLICKED(IDC_BUTTON4, &CRVAFGUIDlg::OnPauseContinue)
	ON_BN_CLICKED(IDC_BUTTON12, &CRVAFGUIDlg::OnOpenRobotCtrlDlg)
	ON_BN_CLICKED(IDC_BUTTON15, &CRVAFGUIDlg::OnClearProgram)
	ON_WM_TIMER()
ON_BN_CLICKED(IDC_BUTTON8, &CRVAFGUIDlg::SettingAlgorithm)
ON_BN_CLICKED(IDC_BUTTON9, &CRVAFGUIDlg::OpenLog)
ON_BN_CLICKED(IDC_BUTTON10, &CRVAFGUIDlg::ShowResultFiles)
ON_BN_CLICKED(IDC_BUTTON14, &CRVAFGUIDlg::OnBnClickedDrawLine)
END_MESSAGE_MAP()


// CRVAFGUIDlg message handlers
void CRVAFGUIDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	KillTimer(PROCESS_DETECT_TIMER);

	CloseHandle(c_hFileMapping);
	CloseHandle(c_hMutex);

	CloseHandle(d_hFileMapping);
	CloseHandle(d_hMutex);

	CloseHandle(i_hFileMapping);
	CloseHandle(i_hMutex);

	delete pRobotCtrlDlg;
	pRobotCtrlDlg = NULL;

	delete pDrawLineDlg;
	pDrawLineDlg = NULL;
}

BOOL CRVAFGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// init bitmap resource
	//HBITMAP	hBitmap;
	//hBitmap = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_RUN));
	//((CButton*)GetDlgItem(IDC_BUTTON3))->SetBitmap(hBitmap);

	EnableToolTips();
	m_toolTip.Create(this);
	m_toolTip.Activate(true);
	m_toolTip.SetDelayTime(150);
	m_toolTip.SetTipTextColor(RGB(0, 0, 255));
	m_toolTip.SetTipBkColor(RGB(255, 255, 255));

	HICON hIcon;
	hIcon = AfxGetApp()->LoadIconW(IDI_CONTINUE);
	((CButton*)GetDlgItem(IDC_BUTTON3))->SetIcon(hIcon);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON3), _T("Start Algorithm"));

	hIcon = AfxGetApp()->LoadIconW(IDI_PAUSE);
	((CButton*)GetDlgItem(IDC_BUTTON4))->SetIcon(hIcon);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON4), _T("Pause Algorithm"));

	hIcon = AfxGetApp()->LoadIconW(IDI_STOP);
	((CButton*)GetDlgItem(IDC_BUTTON5))->SetIcon(hIcon);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON5), _T("Terminate Algorithm"));

	hIcon = AfxGetApp()->LoadIconW(IDI_SAVE);
	((CButton*)GetDlgItem(IDC_BUTTON6))->SetIcon(hIcon);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON6), _T("Save Algorithm Configuration"));

	hIcon = AfxGetApp()->LoadIconW(IDI_SAVE);
	((CButton*)GetDlgItem(IDC_BUTTON7))->SetIcon(hIcon);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON7), _T("Save Algorithm Configuration"));

	hIcon = AfxGetApp()->LoadIconW(IDI_SETTING);
	((CButton*)GetDlgItem(IDC_BUTTON8))->SetIcon(hIcon);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON8), _T("Algorithm Setting"));

	hIcon = AfxGetApp()->LoadIconW(IDI_SHOW);
	((CButton*)GetDlgItem(IDC_BUTTON9))->SetIcon(hIcon);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON9), _T("Algorithm Logs"));

	hIcon = AfxGetApp()->LoadIconW(IDI_FILES);
	((CButton*)GetDlgItem(IDC_BUTTON10))->SetIcon(hIcon);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON10), _T("Algorithm Results Files"));

	hIcon = AfxGetApp()->LoadIconW(IDI_TXT);
	((CButton*)GetDlgItem(IDC_BUTTON11))->SetIcon(hIcon);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON11), _T("XXX"));

	hIcon = AfxGetApp()->LoadIconW(IDI_ROBOT);
	((CButton*)GetDlgItem(IDC_BUTTON12))->SetIcon(hIcon);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON12), _T("Robot Control"));

	hIcon = AfxGetApp()->LoadIconW(IDI_CONTINUE2);
	((CButton*)GetDlgItem(IDC_BUTTON13))->SetIcon(hIcon);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON13), _T("XXX"));

	hIcon = AfxGetApp()->LoadIconW(IDI_STOP2);
	((CButton*)GetDlgItem(IDC_BUTTON14))->SetIcon(hIcon);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON14), _T("XXX"));

	hIcon = AfxGetApp()->LoadIconW(IDI_CLEAN);
	((CButton*)GetDlgItem(IDC_BUTTON15))->SetIcon(hIcon);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON15), _T("Clear Cache Files"));

	hIcon = AfxGetApp()->LoadIconW(IDI_HELP);
	((CButton*)GetDlgItem(IDC_BUTTON16))->SetIcon(hIcon);
	m_toolTip.AddTool(GetDlgItem(IDC_BUTTON15), _T("Help"));

	// TODO: Add extra initialization here
	CloseProgram(_T("SVAF.exe"));
	m_pMsgCtrl = &m_editMsg;
	
	
	InitInterprocess();

	// 非模拟态对话框初始化
	pRobotCtrlDlg = new CRobotControlDlg();
	pRobotCtrlDlg->Create(IDD_ROBOT_DIALOG);

	pDrawLineDlg = new DrawlineDlg();
	pDrawLineDlg->Create(IDD_DRAWLINE_DIALOG);

	d_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadReciveData, this, CREATE_SUSPENDED, &d_ThreadID);
	ResumeThread(d_hThread);

	i_hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadReciveInfo, this, CREATE_SUSPENDED, &i_ThreadID);
	ResumeThread(i_hThread);

	isPause = false;

	ready_proto = false;

	isExpan = false;

	gui_type = svaf::GUIType::ONE;
	//
	gui_nrm_w = GUI_NRM_W;
	gui_nrm_h = GUI_NRM_H;
	gui_exp_w = GUI_EXP_W;
	gui_exp_h = GUI_EXP_H;
	CRect rc(0 + GUI_OFF_X, 0 + GUI_OFF_Y, gui_nrm_w + GUI_OFF_X, gui_nrm_h + GUI_OFF_Y);
	ScreenToClient(rc);
	MoveWindow(rc);

	m_properaty.MoveWindow(CRect(0, 23, gui_nrm_w - 16, gui_exp_h - 31 - 8));
	HDITEM item;
	item.cxy = 140;
	item.mask = HDI_WIDTH;
	m_properaty.GetHeaderCtrl().SetItem(0, new HDITEM(item));

	m_zoonTool.MoveWindow(CRect(gui_nrm_w - 16 + TOOL_MARGN, 0, gui_nrm_w - 16 + TOOL_MARGN + TOOL_WIDTH, gui_exp_h - 31 - 8));
	toolid = vector<int>{IDC_BUTTON3, IDC_BUTTON4, IDC_BUTTON5, IDC_BUTTON6, IDC_BUTTON7, IDC_BUTTON8, IDC_BUTTON9, 
		IDC_BUTTON10, IDC_BUTTON11, IDC_BUTTON12, IDC_BUTTON13, IDC_BUTTON14, IDC_BUTTON15, IDC_BUTTON16};
	
	m_zoonTool.GetWindowRect(rc);
	ScreenToClient(rc);
	int toolbtnsize = rc.right - rc.left;
	int totallength = rc.bottom - rc.top;
	int steplength = totallength / toolid.size();
	while (steplength <= toolbtnsize){
		toolbtnsize--;
	}

	for (int i = 0; i < toolid.size(); ++i){
		CWnd* pWnd = GetDlgItem(toolid[i]);
		pWnd->MoveWindow(CRect(rc.left, i*steplength, rc.left + toolbtnsize, i*steplength + toolbtnsize));
	}
	m_zoonTool.ShowWindow(SW_HIDE);

	SetTopButtonLayout();

	hide_vtk = true;
	m_vtk1.ShowWindow(SW_HIDE);
	m_vtk2.ShowWindow(SW_HIDE);

	AppendMessage(_T("RVAF-GUI copyright(c) Peng Chao 2017\r\n"));

	SetRuningStatus(false);
	SetTimer(PROCESS_DETECT_TIMER, 1000, nullptr);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRVAFGUIDlg::InitInterprocess(){
	// run process
	memset(&m_pInfo, 0, sizeof(m_pInfo));

	// send mapping
	c_hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 4, _T("SVAF_GUI2ALG_CMD"));
	c_hMutex = CreateEvent(nullptr, false, false, _T("SVAF_GUI2ALG_CMD_MUTEX"));
	c_pMsg = (LPTSTR)MapViewOfFile(c_hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (c_pMsg == NULL){
		MessageBox(_T("Create Inter Process Error!"));
		exit(-1);
	}

	// recive data
	d_hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 640*480*3*20, _T("SVAF_ALG2GUI_DATA"));
	d_hMutex = CreateEvent(nullptr, false, false, _T("SVAF_ALG2GUI_DATA_MUTEX"));
	d_pMsg = (LPTSTR)MapViewOfFile(d_hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (d_pMsg == NULL){
		MessageBox(_T("Create Inter Process Error!"));
		exit(-1);
	}

	// recive info
	i_hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 1024, _T("SVAF_ALG2GUI_INFO"));
	i_hMutex = CreateEvent(nullptr, false, false, _T("SVAF_ALG2GUI_INFO_MUTEX"));
	i_pMsg = (LPTSTR)MapViewOfFile(i_hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (i_pMsg == NULL){
		MessageBox(_T("Create Inter Process Error!"));
		exit(-1);
	}

}

void CRVAFGUIDlg::SendInterprocess(){
	LPTSTR p = c_pMsg;
	((int*)p)[0] = 0;
	//SetEvent(m_hMutex);
}


void CRVAFGUIDlg::SendCommand(int cmd){
	LPTSTR p = c_pMsg;
	((int*)p)[0] = cmd;
	//SetEvent(m_hMutex);
}

using Bucket = struct{
	char	head[4];
	int		msgCount;
	int		imgCount;
	int		cols[8];
	int		rows[8];
	int		chns[8];
	int		offs[8];
	int		PointSize[4];
	int		PointChns[4];// xyz(3) or xyzrgb(6)
	int		PointOffs[4];
	int		pclCount;
	int		fetchtype; // 0 dont fetch, 1 world coord
	float	x;
	float	y;
	float	z;
	float	a;
	float	b;
	float	c;
};

void CRVAFGUIDlg::ReciveDataInterprocess(){

	while (true){
		WaitForSingleObject(d_hMutex, INFINITE);
		m_imgs.clear();
		pointclouds.clear();
		LPTSTR p = d_pMsg;
		Bucket* pBucket = (Bucket*)d_pMsg;
		char *pBuf = (char *)p;
		int offset = sizeof(Bucket);
		int frameCount = pBucket->imgCount;
		int pointCount = pBucket->pclCount;
		for (int i = 0; i < frameCount; ++i){
			int type = (pBucket->chns[i] == 1) ? CV_8UC1 : CV_8UC3;
			cv::Mat img(pBucket->rows[i], pBucket->cols[i], type, pBuf + pBucket->offs[i]);
			m_imgs.push_back(img.clone());
		}
		for (int i = 0; i < pointCount; ++i){
			int pnts = pBucket->PointSize[i];
			int chns = pBucket->PointChns[i];
			float *pPC = (float*)(pBuf + pBucket->PointOffs[i]);
			vector<Pointf> pointcloud;
			pointcloud.resize(pnts);
			if (chns == 3){
				for (int j = 0; j < pnts; ++j){
					pointcloud[j].x = pPC[j * chns];
					pointcloud[j].y = pPC[j * chns + 1];
					pointcloud[j].z = pPC[j * chns + 2];
					pointcloud[j].r = 1;
					pointcloud[j].g = 1;
					pointcloud[j].b = 0.5;
				}
			} else if(chns == 6){
				for (int j = 0; j < pnts; ++j){
					pointcloud[j].x = pPC[j * chns];
					pointcloud[j].y = pPC[j * chns + 1];
					pointcloud[j].z = pPC[j * chns + 2];
					pointcloud[j].r = pPC[j * chns + 3];
					pointcloud[j].g = pPC[j * chns + 4];
					pointcloud[j].b = pPC[j * chns + 5];
				}
			}
			pointclouds.push_back(pointcloud);
		}

		if (pBucket->fetchtype){
			a = pBucket->a;
			b = pBucket->b;
			c = pBucket->c;
			x = pBucket->x;
			y = pBucket->y;
			z = pBucket->z;
			pDrawLineDlg->AddPoint(x, y, z);
			RobotFetch(x, y, z, a, b, c);
			CloseProgram(_T("SVAF.exe"));
		}

		SendMessage(WM_PAINT);
		
		continue;
	}
	return;
}

void CRVAFGUIDlg::ReciveInfoInterprocess(){

	CString cs;
	while (true){
		WaitForSingleObject(i_hMutex, INFINITE);
		cs = (char *)i_pMsg;
		cs += _T("\n");
		AppendMessage(_T("SVAF"), cs);
	}
}

void CRVAFGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRVAFGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		// TODO:
		int zoondisp[4] = { IDC_ZOON_DISP, IDC_ZOON_DISP2, IDC_ZOON_DISP3, IDC_ZOON_DISP4 };

		int zoon_id = 0;
		CImage CI;
		for (int i = 0; i < m_imgs.size(); ++i){
			zoon_id = i;
			if (i > 3){
				break;
			}
			CWnd * pWnd = GetDlgItem(zoondisp[i]);
			CDC *pDC = pWnd->GetWindowDC();
			
			CRect rect;
			m_zoonDisplay.GetWindowRect(&rect);
			m_zoonDisplay.ScreenToClient(&rect);

			cv::Mat imgt;
			cv::resize(m_imgs[i], imgt, cv::Size(rect.right - rect.left, rect.bottom - rect.top));

			CI.Destroy();
			CI.Create(imgt.cols, imgt.rows, 8 * imgt.channels());

			if (imgt.channels() == 1){
				cv::cvtColor(imgt, imgt, CV_GRAY2RGB);
			}

			if (imgt.channels() == 3){
				uchar *pS;
				uchar *pImg = (uchar*)CI.GetBits();
				int step = CI.GetPitch();
				for (int x = 0; x < imgt.rows; ++x){
					pS = imgt.ptr<uchar>(x);
					for (int y = 0; y < imgt.cols; ++y){
						*(pImg + x * step + y * 3) = pS[y * 3];
						*(pImg + x * step + y * 3 + 1) = pS[y * 3 + 1];
						*(pImg + x * step + y * 3 + 2) = pS[y * 3 + 2];
					}
				}
			}
			
			CI.StretchBlt(pDC->m_hDC, rect, SRCCOPY);

			if (pointclouds.size() > 0){
				hide_vtk = false;
				CRect rect;
				m_zoonDisp3.GetWindowRect(rect);
				m_zoonDisp3.ShowWindow(SW_HIDE);
				ScreenToClient(rect);
				m_vtk1.ReadPointCloud(pointclouds[0]);
				m_vtk1.MoveWindow(rect);
				m_vtk1.ShowWindow(SW_SHOW);
			}

			if (pointclouds.size() == 2){
				CRect rect;
				m_zoonDisp4.GetWindowRect(rect);
				m_zoonDisp4.ShowWindow(SW_HIDE);
				ScreenToClient(rect);
				m_vtk2.ReadPointCloud(pointclouds[1]);
				m_vtk2.MoveWindow(rect);
				m_vtk2.ShowWindow(SW_SHOW);
			}

		}
		
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRVAFGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRVAFGUIDlg::AppendMessage(LPCTSTR user, LPCTSTR strText){
	if (NULL == m_pMsgCtrl){
		return;
	}
	CString addTimeStr;
	addTimeStr.Format(_T("%s>> %s"), user, strText);
	HWND hWnd = m_pMsgCtrl->GetSafeHwnd();
	ULONG_PTR dwResult = 0;
	ULONG_PTR p = dwResult;
	if (SendMessageTimeoutW(hWnd, WM_GETTEXTLENGTH, 0, 0, SMTO_NORMAL, 1000L, &dwResult) != 0)
	{
		int nLen = (int)dwResult;
		if (SendMessageTimeout(hWnd, EM_SETSEL, nLen, nLen, SMTO_NORMAL, 1000L, &dwResult) != 0)
		{
			if (SendMessageTimeout(hWnd, EM_REPLACESEL, FALSE, (LPARAM)(LPCTSTR)addTimeStr,
				SMTO_NORMAL, 1000L, &dwResult) != 0)
			{
			}
		}
	}
	m_pMsgCtrl->LineScroll(1);
}

void CRVAFGUIDlg::AppendMessage(LPCTSTR strText){
	if (NULL == m_pMsgCtrl){
		return;
	}
	CString addTimeStr;
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	addTimeStr.Format(_T("%4d/%02d/%02d %02d:%02d:%02d: %s"), sysTime.wYear, sysTime.wMonth,
		sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, strText);
	//addTimeStr.Format(_T(">>> "));
	HWND hWnd = m_pMsgCtrl->GetSafeHwnd();
	ULONG_PTR dwResult = 0;
	ULONG_PTR p = dwResult;
	if (SendMessageTimeoutW(hWnd, WM_GETTEXTLENGTH, 0, 0, SMTO_NORMAL, 1000L, &dwResult) != 0)
	{
		int nLen = (int)dwResult;
		if (SendMessageTimeout(hWnd, EM_SETSEL, nLen, nLen, SMTO_NORMAL, 1000L, &dwResult) != 0)
		{
			if (SendMessageTimeout(hWnd, EM_REPLACESEL, FALSE, (LPARAM)(LPCTSTR)addTimeStr,
				SMTO_NORMAL, 1000L, &dwResult) != 0)
			{
			}
		}
	}
	m_pMsgCtrl->LineScroll(1);
}

#define REC_VERSION "v_1.22"
bool CRVAFGUIDlg::ReadCheckProtoFile(std::string filename){

	if (!svaf::ReadProtoFromTextFile(filename, &m_svaftask)){
		if (!svaf::ReadProtoFromBinaryFile(filename, &m_svaftask)){
			MessageBoxW(L"File Open Failed!", L"Tips", 0);
			return false;
		}
	}

	if (m_svaftask.version() != REC_VERSION){
		MessageBoxW(L"File version error, open failed!", L"Tips", 0);
		return false;
	}
	return true;
}
#undef REC_VERSION


void CRVAFGUIDlg::OpenProtoFile(std::string filename){
	// Clear previour object
	ready_proto = false;
	layers.clear();
	m_svaftask.Clear();
	Node *node = dummy.next;
	while (node){
		auto temp = node;
		node = node->next;
		delete temp;
	}
	dummy.next = NULL;

	// Read Proto Files
	if (!ReadCheckProtoFile(filename)){
		m_selectAlgorithm.SetWindowTextW(CString("Select Algorithm"));
		return;
	}
	m_selectAlgorithm.SetWindowTextW(CString(m_svaftask.name().c_str()));
	
	if (m_svaftask.layer_size() < 1){
		MessageBoxW(L"Layer empty!", L"Tips", 0);
		return;
	}

	// Read proto layers
	for (int i = 0; i < m_svaftask.layer_size(); ++i){
		svaf::LayerParameter layer = m_svaftask.layer(i);
		layers[layer.name()] = const_cast<svaf::LayerParameter*>(&m_svaftask.layer(i));
	}

	// Build linked list
	for (auto layer : layers){
		if (layer.second->name() == layer.second->bottom()){
			dummy.next = new Node(layer.second->name());
			break;
		}
	}

	int layer_cnt = 1;
	node = dummy.next;
	while (true){
		if (layers[node->name]->name() == layers[node->name]->top()){
			break;
		}
		if (layers.end() == layers.find(layers[node->name]->top())){
			MessageBoxW(L"Layer Loss", L"Tips", 0);
			return;
		}
		node->next = new Node(layers[node->name]->top());
		node = node->next;
		layer_cnt++;
		if (layer_cnt > m_svaftask.layer_size()){
			MessageBoxW(L"Layer Linked List Build Error", L"Tips", 0);
			return;
		}
	}

	ready_proto = true;
}

void CRVAFGUIDlg::GenerateProperties(){
	m_properaty.RemoveAll();
	idtable.clear();
	pack.clear();
	
	int id = 0;
	
	if (!ready_proto){
		m_properaty.RedrawWindow();
		return;
	}

	const google::protobuf::Message* pMessage = NULL;
	const google::protobuf::Descriptor* pDescriptor = NULL;
	const google::protobuf::Reflection* pReflection = NULL;
	const google::protobuf::FieldDescriptor* pField = NULL;
	//string type_name;

	bool isbinocular = false;
	Node *node = &dummy;
	while (node->next){
		CMFCPropertyGridProperty * group = NULL, * group2 = NULL;
		CMFCPropertyGridProperty * pProp = NULL;
		CString cs_temp;
		
		node = node->next;
		auto type = layers[node->name]->type();
		//auto layer = layers[node->name];
		switch (type)
		{
		case svaf::LayerParameter_LayerType_NONE://00
			break;
		case svaf::LayerParameter_LayerType_IMAGE://01
			isbinocular = false;
			task_type = svaf::SvafApp::S_SHOW;

			group = new CMFCPropertyGridProperty(_T("Image"));

			// 0100 enum
			id = 100;
			pProp = new CMFCPropertyGridProperty(_T("Data Stream"), _T(""), _T("select input data type"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pProp->SetOriginalValue(_T("IMAGE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(_T("IMAGE"));
			pProp->AddOption(_T("IMAGE PAIR"));
			pProp->AddOption(_T("VIDEO"));
			pProp->AddOption(_T("VIDEO PAIR"));
			pProp->AddOption(_T("CAMERA"));
			pProp->AddOption(_T("CAMERA PAIR"));
			pProp->AddOption(_T("DSP CAMERA"));
			pProp->AddOption(_T("DSP CAMERA PAIR"));
			pProp->AddOption(_T("KINECT"));
			pProp->AddOption(_T("IMAGE FOLDER"));
			pProp->AddOption(_T("IMAGE FOLDER PAIR"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 0101 repeated string
			id = 101;
			pProp = new CMFCPropertyGridFileProperty(_T("Image File"), TRUE, _T(""));
			pProp->SetDescription(_T("input image file"));
			pProp->SetData(id);
			idtable[id] = node->name;

			if (layers[node->name]->imagedata_param().name_size() == 0){
				const_cast<::svaf::ImageDataParameter&>(layers[node->name]->imagedata_param()).add_name((const char*)"");
			} else{
				std::string tempstr = layers[node->name]->imagedata_param().name(0);
				const_cast<::svaf::ImageDataParameter&>(layers[node->name]->imagedata_param()).clear_name();
				const_cast<::svaf::ImageDataParameter&>(layers[node->name]->imagedata_param()).add_name(tempstr);
			}
			pMessage = &layers[node->name]->imagedata_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("name");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(CString(pReflection->GetRepeatedString(*pMessage, pField, 0).c_str()));
			pProp->ResetOriginalValue();
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 0102 bool
			id = 102;
			pProp = new CMFCPropertyGridProperty(_T("Color"), _T(""), _T("choose output image of color or gray"));
			pProp->SetData(id);
			idtable[id] = node->name;
			
			pMessage = &layers[node->name]->data_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("color");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_IMAGE_PAIR://02
			isbinocular = true;
			task_type = svaf::SvafApp::B_SHOW;

			group = new CMFCPropertyGridProperty(_T("Image Pair"));

			// 0200 enum
			id = 200;
			pProp = new CMFCPropertyGridProperty(_T("Data Stream"), _T(""), _T("select input data type"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pProp->SetOriginalValue(_T("IMAGE PAIR"));
			pProp->ResetOriginalValue();
			pProp->AddOption(_T("IMAGE"));
			pProp->AddOption(_T("IMAGE PAIR"));
			pProp->AddOption(_T("VIDEO"));
			pProp->AddOption(_T("VIDEO PAIR"));
			pProp->AddOption(_T("CAMERA"));
			pProp->AddOption(_T("CAMERA PAIR"));
			pProp->AddOption(_T("DSP CAMERA"));
			pProp->AddOption(_T("DSP CAMERA PAIR"));
			pProp->AddOption(_T("KINECT"));
			pProp->AddOption(_T("IMAGE FOLDER"));
			pProp->AddOption(_T("IMAGE FOLDER PAIR"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			if (layers[node->name]->imagepair_param().pair_size() == 0){
				const_cast<::svaf::ImagePairParameter&>(layers[node->name]->imagepair_param()).add_pair();
			} else{
				auto temppair = layers[node->name]->imagepair_param().pair(0);
				const_cast<::svaf::ImagePairParameter&>(layers[node->name]->imagepair_param()).clear_pair();
				const_cast<::svaf::ImagePairParameter&>(layers[node->name]->imagepair_param()).add_pair();
				const_cast<::svaf::BinocularPair&>(layers[node->name]->imagepair_param().pair(0)).set_left(temppair.left());
				const_cast<::svaf::BinocularPair&>(layers[node->name]->imagepair_param().pair(0)).set_right(temppair.right());
			}
			// 0201 string
			id = 201;
			pProp = new CMFCPropertyGridFileProperty(_T("Left Image"), TRUE, _T(""));
			pProp->SetDescription(_T("input left image"));
			pProp->SetData(id);
			idtable[id] = node->name;
			
			pMessage = &layers[node->name]->imagepair_param().pair(0);
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("left");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(CString(pReflection->GetString(*pMessage, pField).c_str()));
			pProp->ResetOriginalValue();
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 0202 string
			id = 202;
			pProp = new CMFCPropertyGridFileProperty(_T("Right Image"), TRUE, _T(""));
			pProp->SetDescription(_T("input right image"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->imagepair_param().pair(0);
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("right");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(CString(pReflection->GetString(*pMessage, pField).c_str()));
			pProp->ResetOriginalValue();
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);
			

			// 0203 bool
			id = 203;
			pProp = new CMFCPropertyGridProperty(_T("Color"), _T(""), _T("choose output image of color or gray"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->data_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("color");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_VIDEO://03
			isbinocular = false;
			task_type = svaf::SvafApp::S_SHOW;

			group = new CMFCPropertyGridProperty(_T("Video"));

			// 0300 enum
			id = 300;
			pProp = new CMFCPropertyGridProperty(_T("Data Stream"), _T(""), _T("select input data type"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pProp->SetOriginalValue(_T("VIDEO"));
			pProp->ResetOriginalValue();
			pProp->AddOption(_T("IMAGE"));
			pProp->AddOption(_T("IMAGE PAIR"));
			pProp->AddOption(_T("VIDEO"));
			pProp->AddOption(_T("VIDEO PAIR"));
			pProp->AddOption(_T("CAMERA"));
			pProp->AddOption(_T("CAMERA PAIR"));
			pProp->AddOption(_T("DSP CAMERA"));
			pProp->AddOption(_T("DSP CAMERA PAIR"));
			pProp->AddOption(_T("KINECT"));
			pProp->AddOption(_T("IMAGE FOLDER"));
			pProp->AddOption(_T("IMAGE FOLDER PAIR"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 0301 repeated string
			id = 301;
			pProp = new CMFCPropertyGridFileProperty(_T("Video File"), TRUE, _T(""));
			pProp->SetDescription(_T("input video image"));
			pProp->SetData(id);
			idtable[id] = node->name;

			if (layers[node->name]->videodata_param().name_size() == 0){
				const_cast<::svaf::VideoDataParameter&>(layers[node->name]->videodata_param()).add_name((const char*)"");
			} else{
				std::string tempstr = layers[node->name]->videodata_param().name(0);
				const_cast<::svaf::VideoDataParameter&>(layers[node->name]->videodata_param()).clear_name();
				const_cast<::svaf::VideoDataParameter&>(layers[node->name]->videodata_param()).add_name(tempstr);
			}
			pMessage = &layers[node->name]->videodata_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("name");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(CString(pReflection->GetRepeatedString(*pMessage, pField, 0).c_str()));
			pProp->ResetOriginalValue();
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 0303 bool
			id = 303;
			pProp = new CMFCPropertyGridProperty(_T("Color"), _T(""), _T("choose output image of color or gray"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->data_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("color");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_VIDEO_PAIR://04
			isbinocular = true;
			task_type = svaf::SvafApp::B_SHOW;

			group = new CMFCPropertyGridProperty(_T("Video Pair"));

			// 0400 enum
			id = 400;
			pProp = new CMFCPropertyGridProperty(_T("Data Stream"), _T(""), _T("select input data type"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pProp->SetOriginalValue(_T("VIDEO PAIR"));
			pProp->ResetOriginalValue();
			pProp->AddOption(_T("IMAGE"));
			pProp->AddOption(_T("IMAGE PAIR"));
			pProp->AddOption(_T("VIDEO"));
			pProp->AddOption(_T("VIDEO PAIR"));
			pProp->AddOption(_T("CAMERA"));
			pProp->AddOption(_T("CAMERA PAIR"));
			pProp->AddOption(_T("DSP CAMERA"));
			pProp->AddOption(_T("DSP CAMERA PAIR"));
			pProp->AddOption(_T("KINECT"));
			pProp->AddOption(_T("IMAGE FOLDER"));
			pProp->AddOption(_T("IMAGE FOLDER PAIR"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			if (layers[node->name]->videopair_param().pair_size() == 0){
				const_cast<::svaf::VideoPairParameter&>(layers[node->name]->videopair_param()).add_pair();
			} else{
				auto temppair = layers[node->name]->videopair_param().pair(0);
				const_cast<::svaf::VideoPairParameter&>(layers[node->name]->videopair_param()).clear_pair();
				const_cast<::svaf::VideoPairParameter&>(layers[node->name]->videopair_param()).add_pair();
				const_cast<::svaf::BinocularPair&>(layers[node->name]->videopair_param().pair(0)).set_left(temppair.left());
				const_cast<::svaf::BinocularPair&>(layers[node->name]->videopair_param().pair(0)).set_right(temppair.right());
			}
			// 0401 string
			id = 401;
			pProp = new CMFCPropertyGridFileProperty(_T("Left Video"), TRUE, _T(""));
			pProp->SetDescription(_T("input left video"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->videopair_param().pair(0);
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("left");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(CString(pReflection->GetString(*pMessage, pField).c_str()));
			pProp->ResetOriginalValue();
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 0402 string
			id = 402;
			pProp = new CMFCPropertyGridFileProperty(_T("Right Video"), TRUE, _T(""));
			pProp->SetDescription(_T("input right video"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->videopair_param().pair(0);
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("right");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(CString(pReflection->GetString(*pMessage, pField).c_str()));
			pProp->ResetOriginalValue();
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 0403 bool
			id = 403;
			pProp = new CMFCPropertyGridProperty(_T("Color"), _T(""), _T("choose output image of color or gray"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->data_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("color");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_CAMERA://05
			isbinocular = false;
			task_type = svaf::SvafApp::S_SHOW;

			group = new CMFCPropertyGridProperty(_T("Camera"));

			// 0500 enum
			id = 500;
			pProp = new CMFCPropertyGridProperty(_T("Data Stream"), _T(""), _T("select input data type"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pProp->SetOriginalValue(_T("CAMERA"));
			pProp->ResetOriginalValue();
			pProp->AddOption(_T("IMAGE"));
			pProp->AddOption(_T("IMAGE PAIR"));
			pProp->AddOption(_T("VIDEO"));
			pProp->AddOption(_T("VIDEO PAIR"));
			pProp->AddOption(_T("CAMERA"));
			pProp->AddOption(_T("CAMERA PAIR"));
			pProp->AddOption(_T("DSP CAMERA"));
			pProp->AddOption(_T("DSP CAMERA PAIR"));
			pProp->AddOption(_T("KINECT"));
			pProp->AddOption(_T("IMAGE FOLDER"));
			pProp->AddOption(_T("IMAGE FOLDER PAIR"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 0501 int32
			id = 501;
			pProp = new CMFCPropertyGridProperty(_T("Camera ID"), _T(""), _T("open usb camera"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->cameradata_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("camera");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			cs_temp.Format(_T("%d"), (pReflection->GetInt32(*pMessage, pField)));
			pProp->SetOriginalValue(cs_temp);
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 0503 bool
			id = 503;
			pProp = new CMFCPropertyGridProperty(_T("Color"), _T(""), _T("choose output image of color or gray"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->data_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("color");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_CAMERA_PAIR://06
			isbinocular = true;
			task_type = svaf::SvafApp::B_SHOW;

			group = new CMFCPropertyGridProperty(_T("Camera Pair"));

			// 0600 enum
			id = 600;
			pProp = new CMFCPropertyGridProperty(_T("Data Stream"), _T(""), _T("select input data type"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pProp->SetOriginalValue(_T("CAMERA PAIR"));
			pProp->ResetOriginalValue();
			pProp->AddOption(_T("IMAGE"));
			pProp->AddOption(_T("IMAGE PAIR"));
			pProp->AddOption(_T("VIDEO"));
			pProp->AddOption(_T("VIDEO PAIR"));
			pProp->AddOption(_T("CAMERA"));
			pProp->AddOption(_T("CAMERA PAIR"));
			pProp->AddOption(_T("DSP CAMERA"));
			pProp->AddOption(_T("DSP CAMERA PAIR"));
			pProp->AddOption(_T("KINECT"));
			pProp->AddOption(_T("IMAGE FOLDER"));
			pProp->AddOption(_T("IMAGE FOLDER PAIR"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 0601 int32
			id = 601;
			pProp = new CMFCPropertyGridProperty(_T("Left Camera"), _T(""), _T("left usb camera id"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->camerapair_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("leftcamera");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			cs_temp.Format(_T("%d"), (pReflection->GetInt32(*pMessage, pField)));
			pProp->SetOriginalValue(cs_temp);
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 0602 int32
			id = 602;
			pProp = new CMFCPropertyGridProperty(_T("Right Camera"), _T(""), _T("right usb camera id"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->camerapair_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("rightcamera");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			cs_temp.Format(_T("%d"), (pReflection->GetInt32(*pMessage, pField)));
			pProp->SetOriginalValue(cs_temp);
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 0603 bool
			id = 603;
			pProp = new CMFCPropertyGridProperty(_T("Color"), _T(""), _T("choose output image of color or gray"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->data_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("color");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_DSP://07
			isbinocular = false;
			task_type = svaf::SvafApp::S_SHOW;

			group = new CMFCPropertyGridProperty(_T("DSP Camera"));

			// 0700 enum
			id = 700;
			pProp = new CMFCPropertyGridProperty(_T("Data Stream"), _T(""), _T("select input data type"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pProp->SetOriginalValue(_T("DSP CAMERA"));
			pProp->ResetOriginalValue();
			pProp->AddOption(_T("IMAGE"));
			pProp->AddOption(_T("IMAGE PAIR"));
			pProp->AddOption(_T("VIDEO"));
			pProp->AddOption(_T("VIDEO PAIR"));
			pProp->AddOption(_T("CAMERA"));
			pProp->AddOption(_T("CAMERA PAIR"));
			pProp->AddOption(_T("DSP CAMERA"));
			pProp->AddOption(_T("DSP CAMERA PAIR"));
			pProp->AddOption(_T("KINECT"));
			pProp->AddOption(_T("IMAGE FOLDER"));
			pProp->AddOption(_T("IMAGE FOLDER PAIR"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 0703 bool
			id = 703;
			pProp = new CMFCPropertyGridProperty(_T("Color"), _T(""), _T("choose output image of color or gray"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->data_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("color");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_DSP_PAIR://08
			isbinocular = true;
			task_type = svaf::SvafApp::B_SHOW;

			group = new CMFCPropertyGridProperty(_T("DSP Camera Pair"));

			// 0800 enum
			id = 800;
			pProp = new CMFCPropertyGridProperty(_T("Data Stream"), _T(""), _T("select input data type"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pProp->SetOriginalValue(_T("DSP CAMERA PAIR"));
			pProp->ResetOriginalValue();
			pProp->AddOption(_T("IMAGE"));
			pProp->AddOption(_T("IMAGE PAIR"));
			pProp->AddOption(_T("VIDEO"));
			pProp->AddOption(_T("VIDEO PAIR"));
			pProp->AddOption(_T("CAMERA"));
			pProp->AddOption(_T("CAMERA PAIR"));
			pProp->AddOption(_T("DSP CAMERA"));
			pProp->AddOption(_T("DSP CAMERA PAIR"));
			pProp->AddOption(_T("KINECT"));
			pProp->AddOption(_T("IMAGE FOLDER"));
			pProp->AddOption(_T("IMAGE FOLDER PAIR"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 0803 bool
			id = 803;
			pProp = new CMFCPropertyGridProperty(_T("Color"), _T(""), _T("choose output image of color or gray"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->data_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("color");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_KINECT://09
			isbinocular = false;
			task_type = svaf::SvafApp::S_SHOW;

			group = new CMFCPropertyGridProperty(_T("Kinect"));

			// 0900 enum
			id = 900;
			pProp = new CMFCPropertyGridProperty(_T("Data Stream"), _T(""), _T("select input data type"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pProp->SetOriginalValue(_T("KINECT"));
			pProp->ResetOriginalValue();
			pProp->AddOption(_T("IMAGE"));
			pProp->AddOption(_T("IMAGE PAIR"));
			pProp->AddOption(_T("VIDEO"));
			pProp->AddOption(_T("VIDEO PAIR"));
			pProp->AddOption(_T("CAMERA"));
			pProp->AddOption(_T("CAMERA PAIR"));
			pProp->AddOption(_T("DSP CAMERA"));
			pProp->AddOption(_T("DSP CAMERA PAIR"));
			pProp->AddOption(_T("KINECT"));
			pProp->AddOption(_T("IMAGE FOLDER"));
			pProp->AddOption(_T("IMAGE FOLDER PAIR"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 0903 bool
			id = 903;
			pProp = new CMFCPropertyGridProperty(_T("Color"), _T(""), _T("choose output image of color or gray"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->data_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("color");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_IMAGE_FOLDER://11
			isbinocular = false;
			task_type = svaf::SvafApp::S_SHOW;

			group = new CMFCPropertyGridProperty(_T("Image Folder"));

			// 1100 enum
			id = 1100;
			pProp = new CMFCPropertyGridProperty(_T("Data Stream"), _T(""), _T("select input data type"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pProp->SetOriginalValue(_T("IMAGE FOLDER"));
			pProp->ResetOriginalValue();
			pProp->AddOption(_T("IMAGE"));
			pProp->AddOption(_T("IMAGE PAIR"));
			pProp->AddOption(_T("VIDEO"));
			pProp->AddOption(_T("VIDEO PAIR"));
			pProp->AddOption(_T("CAMERA"));
			pProp->AddOption(_T("CAMERA PAIR"));
			pProp->AddOption(_T("DSP CAMERA"));
			pProp->AddOption(_T("DSP CAMERA PAIR"));
			pProp->AddOption(_T("KINECT"));
			pProp->AddOption(_T("IMAGE FOLDER"));
			pProp->AddOption(_T("IMAGE FOLDER PAIR"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 1101 repeated string
			id = 1101;
			pProp = new CMFCPropertyGridProperty(_T("Folder Dir"), _T(""), _T("image folder dir"));
			pProp->SetData(id);
			idtable[id] = node->name;

			if (layers[node->name]->folder_param().name_size() == 0){
				const_cast<::svaf::ImageFolderParameter&>(layers[node->name]->folder_param()).add_name((const char*)"");
			} else{
				std::string tempstr = layers[node->name]->folder_param().name(0);
				const_cast<::svaf::ImageFolderParameter&>(layers[node->name]->folder_param()).clear_name();
				const_cast<::svaf::ImageFolderParameter&>(layers[node->name]->folder_param()).add_name(tempstr);
			}
			pMessage = &layers[node->name]->folder_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("name");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(CString(pReflection->GetRepeatedString(*pMessage, pField, 0).c_str()));
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 1103 bool
			id = 1103;
			pProp = new CMFCPropertyGridProperty(_T("Color"), _T(""), _T("choose output image of color or gray"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->data_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("color");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_IMAGE_PAIR_FOLDER://12
			isbinocular = true;
			task_type = svaf::SvafApp::B_SHOW;

			group = new CMFCPropertyGridProperty(_T("Image Folder Pair"));

			// 1200 enum
			id = 1200;
			pProp = new CMFCPropertyGridProperty(_T("Data Stream"), _T(""), _T("select input data type"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pProp->SetOriginalValue(_T("IMAGE FOLDER PAIR"));
			pProp->ResetOriginalValue();
			pProp->AddOption(_T("IMAGE"));
			pProp->AddOption(_T("IMAGE PAIR"));
			pProp->AddOption(_T("VIDEO"));
			pProp->AddOption(_T("VIDEO PAIR"));
			pProp->AddOption(_T("CAMERA"));
			pProp->AddOption(_T("CAMERA PAIR"));
			pProp->AddOption(_T("DSP CAMERA"));
			pProp->AddOption(_T("DSP CAMERA PAIR"));
			pProp->AddOption(_T("KINECT"));
			pProp->AddOption(_T("IMAGE FOLDER"));
			pProp->AddOption(_T("IMAGE FOLDER PAIR"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			if (layers[node->name]->pairfolder_param().pair_size() == 0){
				const_cast<::svaf::ImagePairFolderParameter&>(layers[node->name]->pairfolder_param()).add_pair();
			} else{
				auto temppair = layers[node->name]->pairfolder_param().pair(0);
				const_cast<::svaf::ImagePairFolderParameter&>(layers[node->name]->pairfolder_param()).clear_pair();
				const_cast<::svaf::ImagePairFolderParameter&>(layers[node->name]->pairfolder_param()).add_pair();
				const_cast<::svaf::BinocularPair&>(layers[node->name]->pairfolder_param().pair(0)).set_left(temppair.left());
				const_cast<::svaf::BinocularPair&>(layers[node->name]->pairfolder_param().pair(0)).set_right(temppair.right());
			}
			// 1201 string
			id = 1201;
			pProp = new CMFCPropertyGridProperty(_T("Folder Dir"), _T(""), _T("image folder dir"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->pairfolder_param().pair(0);
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("left");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(CString(pReflection->GetString(*pMessage, pField).c_str()));
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 1202 string
			id = 1202;
			pProp = new CMFCPropertyGridProperty(_T("Folder Dir"), _T(""), _T("image folder dir"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->pairfolder_param().pair(0);
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("right");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(CString(pReflection->GetString(*pMessage, pField).c_str()));
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 1203 bool
			id = 1203;
			pProp = new CMFCPropertyGridProperty(_T("Color"), _T(""), _T("choose output image of color or gray"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->data_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("color");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_ADABOOST://21
			group = new CMFCPropertyGridProperty(_T("Adaboost"));

			if (isbinocular){
				task_type = svaf::SvafApp::B_DETECT;
			} else{
				task_type = svaf::SvafApp::S_DETECT;
			}

			// 2101 string
			id = 2101;
			pProp = new CMFCPropertyGridFileProperty(_T("Detector"), TRUE, _T(""));
			pProp->SetDescription(_T("detector file trained by piotr dollar toolbox"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->adaboost_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("detector");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(CString(pReflection->GetString(*pMessage, pField).c_str()));
			pProp->ResetOriginalValue();
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 2102 bool
			id = 2102;
			pProp = new CMFCPropertyGridProperty(_T("SyncFrame"), _T(""), _T("keep scale of two(or more) images not change in the same moment"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->adaboost_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("sync_frame");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 2103 bool
			id = 2103;
			pProp = new CMFCPropertyGridProperty(_T("SyncVideo"), _T(""), _T("keep scale not change or make epipolar restriction in the hole sequence"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->adaboost_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("sync_video");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 2104 bool
			id = 2104;
			pProp = new CMFCPropertyGridProperty(_T("SyncEpipolar"), _T(""), _T("apply epipolar restriction and keep scale not change in a moment"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->adaboost_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("sync_epipolar");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			// 2105 float
			id = 2105;
			pProp = new CMFCPropertyGridProperty(_T("Thresh"), _T(""), _T("adaboost positive sample decition threshold"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->adaboost_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("thresh");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);
			
			cs_temp.Format(_T("%.2f"), (pReflection->GetFloat(*pMessage, pField)));
			pProp->SetOriginalValue(cs_temp);
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 2106 float
			id = 2106;
			pProp = new CMFCPropertyGridProperty(_T("NMS"), _T(""), _T("non-maximum suppression, result rectangles that overlap area percentage lower than this value will be merged"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->adaboost_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("nms");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			cs_temp.Format(_T("%.2f"), (pReflection->GetFloat(*pMessage, pField)));
			pProp->SetOriginalValue(cs_temp);
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_MILTRACK://31
			if (isbinocular){
				task_type = svaf::SvafApp::B_DETECT;
			} else{
				task_type = svaf::SvafApp::S_DETECT;
			}

			group = new CMFCPropertyGridProperty(_T("MILTrack"));
			break;
		case svaf::LayerParameter_LayerType_BITTRACK://32
			if (isbinocular){
				task_type = svaf::SvafApp::B_DETECT;
			} else{
				task_type = svaf::SvafApp::S_DETECT;
			}

			group = new CMFCPropertyGridProperty(_T("Bino MILTrack"));
			break;
		case svaf::LayerParameter_LayerType_SIFT_POINT://41
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINT;
			} else{
				task_type = svaf::SvafApp::S_POINT;
			}

			group = new CMFCPropertyGridProperty(_T("SIFT Point"));
			break;
		case svaf::LayerParameter_LayerType_SURF_POINT://42
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINT;
			} else{
				task_type = svaf::SvafApp::S_POINT;
			}

			group = new CMFCPropertyGridProperty(_T("SURF Point"));
			break;
		case svaf::LayerParameter_LayerType_STAR_POINT://43
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINT;
			} else{
				task_type = svaf::SvafApp::S_POINT;
			}

			group = new CMFCPropertyGridProperty(_T("STAR Point"));
			break;
		case svaf::LayerParameter_LayerType_BRISK_POINT://44
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINT;
			} else{
				task_type = svaf::SvafApp::S_POINT;
			}

			group = new CMFCPropertyGridProperty(_T("Brisk Point"));
			break;
		case svaf::LayerParameter_LayerType_FAST_POINT://45
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINT;
			} else{
				task_type = svaf::SvafApp::S_POINT;
			}

			group = new CMFCPropertyGridProperty(_T("FAST Point"));
			break;
		case svaf::LayerParameter_LayerType_ORB_POINT://46
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINT;
			} else{
				task_type = svaf::SvafApp::S_POINT;
			}

			group = new CMFCPropertyGridProperty(_T("ORB Point"));
			break;
		case svaf::LayerParameter_LayerType_KAZE_POINT://47
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINT;
			} else{
				task_type = svaf::SvafApp::S_POINT;
			}

			group = new CMFCPropertyGridProperty(_T("KAZE Point"));
			break;
		case svaf::LayerParameter_LayerType_HARRIS_POINT://48
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINT;
			} else{
				task_type = svaf::SvafApp::S_POINT;
			}

			group = new CMFCPropertyGridProperty(_T("Harris Point"));
			break;
		case svaf::LayerParameter_LayerType_CV_POINT://49
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINT;
			} else{
				task_type = svaf::SvafApp::S_POINT;
			}

			group = new CMFCPropertyGridProperty(_T("CV Point"));
			break;
		case svaf::LayerParameter_LayerType_SIFT_DESP://51
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINTDESP;
			} else{
				task_type = svaf::SvafApp::S_POINTDESP;
			}

			group = new CMFCPropertyGridProperty(_T("SIFT Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_SURF_DESP://52
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINTDESP;
			} else{
				task_type = svaf::SvafApp::S_POINTDESP;
			}

			group = new CMFCPropertyGridProperty(_T("SURF Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_STAR_DESP://53
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINTDESP;
			} else{
				task_type = svaf::SvafApp::S_POINTDESP;
			}

			group = new CMFCPropertyGridProperty(_T("STAR Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_BRIEF_DESP://54
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINTDESP;
			} else{
				task_type = svaf::SvafApp::S_POINTDESP;
			}

			group = new CMFCPropertyGridProperty(_T("BRIEF Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_BRISK_DESP://55
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINTDESP;
			} else{
				task_type = svaf::SvafApp::S_POINTDESP;
			}

			group = new CMFCPropertyGridProperty(_T("Brisk Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_FAST_DESP://56
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINTDESP;
			} else{
				task_type = svaf::SvafApp::S_POINTDESP;
			}

			group = new CMFCPropertyGridProperty(_T("FAST Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_ORB_DESP://57
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINTDESP;
			} else{
				task_type = svaf::SvafApp::S_POINTDESP;
			}

			group = new CMFCPropertyGridProperty(_T("ORB Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_KAZE_DESP://58
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINTDESP;
			} else{
				task_type = svaf::SvafApp::S_POINTDESP;
			}

			group = new CMFCPropertyGridProperty(_T("KAZE Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_CV_DESP://59
			if (isbinocular){
				task_type = svaf::SvafApp::B_POINTDESP;
			} else{
				task_type = svaf::SvafApp::S_POINTDESP;
			}

			group = new CMFCPropertyGridProperty(_T("CV Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_KDTREE_MATCH://61
			task_type = svaf::SvafApp::POINT_MATCH;
			group = new CMFCPropertyGridProperty(_T("KDtree Match"));
			break;
		case svaf::LayerParameter_LayerType_EULAR_MATCH://62
			task_type = svaf::SvafApp::POINT_MATCH;
			group = new CMFCPropertyGridProperty(_T("Eular Match"));
			break;
		case svaf::LayerParameter_LayerType_RANSAC://63
			task_type = svaf::SvafApp::POINT_MATCH;
			group = new CMFCPropertyGridProperty(_T("Ransac"));
			break;
		case svaf::LayerParameter_LayerType_BF_MATCH://64
			task_type = svaf::SvafApp::POINT_MATCH;
			group = new CMFCPropertyGridProperty(_T("BF Match"));
			break;
		case svaf::LayerParameter_LayerType_FLANN_MATCH://65
			task_type = svaf::SvafApp::POINT_MATCH;
			group = new CMFCPropertyGridProperty(_T("Flann Match"));
			break;
		case svaf::LayerParameter_LayerType_EC_MATCH://68
			task_type = svaf::SvafApp::POINT_MATCH;
			group = new CMFCPropertyGridProperty(_T("EC Match"));
			break;
		case svaf::LayerParameter_LayerType_CV_MATCH://69
			task_type = svaf::SvafApp::POINT_MATCH;
			group = new CMFCPropertyGridProperty(_T("CV Match"));
			break;
		case svaf::LayerParameter_LayerType_SGM_MATCH://71
			task_type = svaf::SvafApp::STEREO_MATCH;
			group = new CMFCPropertyGridProperty(_T("SGM Stereo Match"));
			break;
		case svaf::LayerParameter_LayerType_EADP_MATCH://72
			task_type = svaf::SvafApp::STEREO_MATCH;
			group = new CMFCPropertyGridProperty(_T("Eadp Stereo Match"));
			
			// 7201 int32
			id = 7201;
			pProp = new CMFCPropertyGridProperty(_T("Max Disparity"), _T(""), _T("maximum disparity of stereo matching"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->eadp_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("max_disp");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			cs_temp.Format(_T("%d"), (pReflection->GetInt32(*pMessage, pField)));
			pProp->SetOriginalValue(cs_temp);
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 7202 int32
			id = 7202;
			pProp = new CMFCPropertyGridProperty(_T("Factor"), _T(""), _T("multiply factor used to disparity map visualization"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->eadp_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("factor");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			cs_temp.Format(_T("%d"), (pReflection->GetInt32(*pMessage, pField)));
			pProp->SetOriginalValue(cs_temp);
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 7203 int32
			id = 7203;
			pProp = new CMFCPropertyGridProperty(_T("Guildmr"), _T(""), _T("radius of median filter for guild image"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->eadp_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("guidmr");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			cs_temp.Format(_T("%d"), (pReflection->GetInt32(*pMessage, pField)));
			pProp->SetOriginalValue(cs_temp);
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 7204 int32
			id = 7204;
			pProp = new CMFCPropertyGridProperty(_T("Dispmr"), _T(""), _T("radius of median filter for labeled disparity map"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->eadp_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("dispmr");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			cs_temp.Format(_T("%d"), (pReflection->GetInt32(*pMessage, pField)));
			pProp->SetOriginalValue(cs_temp);
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 7205 float
			id = 7205;
			pProp = new CMFCPropertyGridProperty(_T("Sg"), _T(""), _T("inverse proportion factor used to compute weight table that map gradient to cost value"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->eadp_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("sg");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
			pProp->SetOriginalValue(cs_temp);
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 7206 float
			id = 7206;
			pProp = new CMFCPropertyGridProperty(_T("Sc"), _T(""), _T("sigma factor used to compute weight table that map gradient to cost value"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->eadp_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("sc");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
			pProp->SetOriginalValue(cs_temp);
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 7207 float
			id = 7207;
			pProp = new CMFCPropertyGridProperty(_T("R1"), _T(""), _T("penny factor of continue disparity"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->eadp_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("r1");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
			pProp->SetOriginalValue(cs_temp);
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 7208 float
			id = 7208;
			pProp = new CMFCPropertyGridProperty(_T("R2"), _T(""), _T("penny factor of uncontinue disparity"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->eadp_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("r2");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
			pProp->SetOriginalValue(cs_temp);
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_TRIANG://81
			task_type = svaf::SvafApp::PC_TRIANGLE;
			group = new CMFCPropertyGridProperty(_T("Triangle"));

			// 8102 string
			id = 8102;
			pProp = new CMFCPropertyGridProperty(_T("Toolbox Dir"), _T(""), _T("matlab calibration toolbox directory"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->triang_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("toolbox_dir");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(CString(pReflection->GetString(*pMessage, pField).c_str()));
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 8103 string
			id = 8103;
			pProp = new CMFCPropertyGridProperty(_T("Calibmat Dir"), _T(""), _T("matlab calibration toolbox directory"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->triang_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("calibmat_dir");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(CString(pReflection->GetString(*pMessage, pField).c_str()));
			pProp->ResetOriginalValue();
			group->AddSubItem(pProp);

			// 8104 bool
			id = 8104;
			pProp = new CMFCPropertyGridProperty(_T("SavePCD"), _T(""), _T("whether to save 3d result pcd file"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->triang_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("savepc");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(pReflection->GetBool(*pMessage, pField) ? CString("TRUE") : CString("FALSE"));
			pProp->ResetOriginalValue();
			pProp->AddOption(CString("TRUE"));
			pProp->AddOption(CString("FALSE"));
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_MXMUL://82
			task_type = svaf::SvafApp::PC_MULMATRIX;
			group = new CMFCPropertyGridProperty(_T("Matrix Mul"));
			break;
		case svaf::LayerParameter_LayerType_CENTER_POS://91
			task_type = svaf::SvafApp::PR_CENTER;
			group = new CMFCPropertyGridProperty(_T("Center Position"));
			break;
		case svaf::LayerParameter_LayerType_IA_EST://94
			task_type = svaf::SvafApp::PC_REGISTRATION;

			group = new CMFCPropertyGridProperty(_T("SAC-IA"));

			// 9401 string
			id = 9401;
			pProp = new CMFCPropertyGridFileProperty(_T("Referance Model"), TRUE, _T(""));
			pProp->SetDescription(_T("referance pcd file"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->sacia_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("pcd_filename");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(CString(pReflection->GetString(*pMessage, pField).c_str()));
			pProp->ResetOriginalValue();
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			{
				group2 = new CMFCPropertyGridProperty(_T("SAC-IA Registration"));

				// 94201 int32
				id = 94201;
				pProp = new CMFCPropertyGridProperty(_T("Max Iteration"), _T(""), _T("maximum iteration of sac-ia ransac algorithm"));
				pProp->SetData(id);
				idtable[id] = node->name;

				pMessage = &layers[node->name]->sacia_param().ia_param();
				pDescriptor = pMessage->GetDescriptor();
				pReflection = pMessage->GetReflection();
				pField = pDescriptor->FindFieldByName("max_iter");
				pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

				cs_temp.Format(_T("%d"), (pReflection->GetInt32(*pMessage, pField)));
				pProp->SetOriginalValue(cs_temp);
				pProp->ResetOriginalValue();
				group2->AddSubItem(pProp);

				// 94202 float
				id = 94202;
				pProp = new CMFCPropertyGridProperty(_T("Min Corresponse"), _T(""), _T("minimum response of sac-ia algorithm"));
				pProp->SetData(id);
				idtable[id] = node->name;

				pMessage = &layers[node->name]->sacia_param().ia_param();
				pDescriptor = pMessage->GetDescriptor();
				pReflection = pMessage->GetReflection();
				pField = pDescriptor->FindFieldByName("min_cors");
				pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

				cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
				pProp->SetOriginalValue(cs_temp);
				pProp->ResetOriginalValue();
				group2->AddSubItem(pProp);

				// 94203 float
				id = 94203;
				pProp = new CMFCPropertyGridProperty(_T("Min Corresponse"), _T(""), _T("maximum response of sac-ia algorithm"));
				pProp->SetData(id);
				idtable[id] = node->name;

				pMessage = &layers[node->name]->sacia_param().ia_param();
				pDescriptor = pMessage->GetDescriptor();
				pReflection = pMessage->GetReflection();
				pField = pDescriptor->FindFieldByName("max_cors");
				pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

				cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
				pProp->SetOriginalValue(cs_temp);
				pProp->ResetOriginalValue();
				group2->AddSubItem(pProp);

				// 94204 float
				id = 94204;
				pProp = new CMFCPropertyGridProperty(_T("Voxel Grid"), _T(""), _T("grid size of grid filter"));
				pProp->SetData(id);
				idtable[id] = node->name;

				pMessage = &layers[node->name]->sacia_param().ia_param();
				pDescriptor = pMessage->GetDescriptor();
				pReflection = pMessage->GetReflection();
				pField = pDescriptor->FindFieldByName("voxel_grid");
				pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

				cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
				pProp->SetOriginalValue(cs_temp);
				pProp->ResetOriginalValue();
				group2->AddSubItem(pProp);

				// 94205 float
				id = 94205;
				pProp = new CMFCPropertyGridProperty(_T("Normal Radius"), _T(""), _T("radius used to computer pointcloud normals"));
				pProp->SetData(id);
				idtable[id] = node->name;

				pMessage = &layers[node->name]->sacia_param().ia_param();
				pDescriptor = pMessage->GetDescriptor();
				pReflection = pMessage->GetReflection();
				pField = pDescriptor->FindFieldByName("norm_rad");
				pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

				cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
				pProp->SetOriginalValue(cs_temp);
				pProp->ResetOriginalValue();
				group2->AddSubItem(pProp);

				// 94206 float
				id = 94206;
				pProp = new CMFCPropertyGridProperty(_T("Feature Radius"), _T(""), _T("radius used to compute fast point feature histogram feature"));
				pProp->SetData(id);
				idtable[id] = node->name;

				pMessage = &layers[node->name]->sacia_param().ia_param();
				pDescriptor = pMessage->GetDescriptor();
				pReflection = pMessage->GetReflection();
				pField = pDescriptor->FindFieldByName("feat_rad");
				pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

				cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
				pProp->SetOriginalValue(cs_temp);
				pProp->ResetOriginalValue();
				group2->AddSubItem(pProp);

				group->AddSubItem(group2);
			}

			{
				group2 = new CMFCPropertyGridProperty(_T("Referance Position"));

				// 94301 float
				id = 94301;
				pProp = new CMFCPropertyGridProperty(_T("X"), _T(""), _T("x position of module"));
				pProp->SetData(id);
				idtable[id] = node->name;

				pMessage = &layers[node->name]->sacia_param().coor_param();
				pDescriptor = pMessage->GetDescriptor();
				pReflection = pMessage->GetReflection();
				pField = pDescriptor->FindFieldByName("x");
				pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

				cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
				pProp->SetOriginalValue(cs_temp);
				pProp->ResetOriginalValue();
				group2->AddSubItem(pProp);

				// 94302 float
				id = 94302;
				pProp = new CMFCPropertyGridProperty(_T("Y"), _T(""), _T("y position of module"));
				pProp->SetData(id);
				idtable[id] = node->name;

				pMessage = &layers[node->name]->sacia_param().coor_param();
				pDescriptor = pMessage->GetDescriptor();
				pReflection = pMessage->GetReflection();
				pField = pDescriptor->FindFieldByName("y");
				pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

				cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
				pProp->SetOriginalValue(cs_temp);
				pProp->ResetOriginalValue();
				group2->AddSubItem(pProp);

				// 94303 float
				id = 94303;
				pProp = new CMFCPropertyGridProperty(_T("Z"), _T(""), _T("z position of module"));
				pProp->SetData(id);
				idtable[id] = node->name;

				pMessage = &layers[node->name]->sacia_param().coor_param();
				pDescriptor = pMessage->GetDescriptor();
				pReflection = pMessage->GetReflection();
				pField = pDescriptor->FindFieldByName("z");
				pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

				cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
				pProp->SetOriginalValue(cs_temp);
				pProp->ResetOriginalValue();
				group2->AddSubItem(pProp);

				// 94304 float
				id = 94304;
				pProp = new CMFCPropertyGridProperty(_T("A"), _T(""), _T("alpha angle(deg) of module"));
				pProp->SetData(id);
				idtable[id] = node->name;

				pMessage = &layers[node->name]->sacia_param().coor_param();
				pDescriptor = pMessage->GetDescriptor();
				pReflection = pMessage->GetReflection();
				pField = pDescriptor->FindFieldByName("a");
				pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

				cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
				pProp->SetOriginalValue(cs_temp);
				pProp->ResetOriginalValue();
				group2->AddSubItem(pProp);

				// 94305 float
				id = 94305;
				pProp = new CMFCPropertyGridProperty(_T("B"), _T(""), _T("beta angle(deg) of module"));
				pProp->SetData(id);
				idtable[id] = node->name;

				pMessage = &layers[node->name]->sacia_param().coor_param();
				pDescriptor = pMessage->GetDescriptor();
				pReflection = pMessage->GetReflection();
				pField = pDescriptor->FindFieldByName("b");
				pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

				cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
				pProp->SetOriginalValue(cs_temp);
				pProp->ResetOriginalValue();
				group2->AddSubItem(pProp);

				// 94306 float
				id = 94306;
				pProp = new CMFCPropertyGridProperty(_T("C"), _T(""), _T("gama angle(deg) of module"));
				pProp->SetData(id);
				idtable[id] = node->name;

				pMessage = &layers[node->name]->sacia_param().coor_param();
				pDescriptor = pMessage->GetDescriptor();
				pReflection = pMessage->GetReflection();
				pField = pDescriptor->FindFieldByName("c");
				pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

				cs_temp.Format(_T("%.1f"), (pReflection->GetFloat(*pMessage, pField)));
				pProp->SetOriginalValue(cs_temp);
				pProp->ResetOriginalValue();
				group2->AddSubItem(pProp);

				group->AddSubItem(group2);
			}

			break;
		case svaf::LayerParameter_LayerType_IAICP_EST://95
			task_type = svaf::SvafApp::PC_REGISTRATION;
			group = new CMFCPropertyGridProperty(_T("SAC-IA ICP"));
			break;
		case svaf::LayerParameter_LayerType_IANDT_EST://96
			task_type = svaf::SvafApp::PC_REGISTRATION;
			group = new CMFCPropertyGridProperty(_T("SAC-IA NDP"));
			break;
		case svaf::LayerParameter_LayerType_SUPIX_SEG://101
			if (isbinocular){
				task_type = svaf::SvafApp::B_SUPIX;
			} else{
				task_type = svaf::SvafApp::S_SUPIX;
			}
			group = new CMFCPropertyGridProperty(_T("Superpixel Segment"));
			break;
		case svaf::LayerParameter_LayerType_RECTIFY://141
			if (isbinocular){
				task_type = svaf::SvafApp::B_RECTIFY;
			} else{
				task_type = svaf::SvafApp::S_RECTIFY;
			}
			group = new CMFCPropertyGridProperty(_T("Stereo Rectify"));

			// 14101 string
			id = 14101;
			pProp = new CMFCPropertyGridFileProperty(_T("Camera Param"), TRUE, _T(""));
			pProp->SetDescription(_T("calibration file of binocular camera"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name]->rectify_param();
			pDescriptor = pMessage->GetDescriptor();
			pReflection = pMessage->GetReflection();
			pField = pDescriptor->FindFieldByName("filename");
			pack[id] = ReflectPackage(pMessage, pReflection, pField, node->name);

			pProp->SetOriginalValue(CString(pReflection->GetString(*pMessage, pField).c_str()));
			pProp->ResetOriginalValue();
			pProp->AllowEdit(FALSE);
			group->AddSubItem(pProp);

			break;
		default:
			MessageBox(L"Unrecoganize Task Layer");
			break;
		}
		
		if (group){
			m_properaty.AddProperty(group);
		}
	}

	switch (task_type)
	{
	case svaf::NONE:
		gui_type = svaf::GUIType::PROTO;
		break;
	case svaf::S_SHOW:
		gui_type = svaf::GUIType::ONE;
		break;
	case svaf::B_SHOW:
		gui_type = svaf::GUIType::TWO;
		break;
	case svaf::S_RECTIFY:
		gui_type = svaf::GUIType::ONE;
		break;
	case svaf::B_RECTIFY:
		gui_type = svaf::GUIType::TWO;
		break;
	case svaf::S_DETECT:
		gui_type = svaf::GUIType::ONE;
		break;
	case svaf::B_DETECT:
		gui_type = svaf::GUIType::TWO;
		break;
	case svaf::S_POINT:
		gui_type = svaf::GUIType::ONE;
		break;
	case svaf::B_POINT:
		gui_type = svaf::GUIType::TWO;
		break;
	case svaf::S_POINTDESP:
		gui_type = svaf::GUIType::ONE;
		break;
	case svaf::B_POINTDESP:
		gui_type = svaf::GUIType::TWO;
		break;
	case svaf::S_SUPIX:
		gui_type = svaf::GUIType::ONE;
		break;
	case svaf::B_SUPIX:
		gui_type = svaf::GUIType::TWO;
		break;
	case svaf::POINT_MATCH:
		gui_type = svaf::GUIType::ONE_BIG;
		break;
	case svaf::RANSAC_MATCH:
		gui_type = svaf::GUIType::ONE_BIG;
		break;
	case svaf::STEREO_MATCH:
		gui_type = svaf::GUIType::FOUR;
		break;
	case svaf::PC_TRIANGLE:
		gui_type = svaf::GUIType::THREE_BIG;
		break;
	case svaf::PC_MULMATRIX:
		gui_type = svaf::GUIType::THREE_BIG;
		break;
	case svaf::PC_REGISTRATION:
		gui_type = svaf::GUIType::FOUR;
		break;
	case svaf::PR_CENTER:
		gui_type = svaf::GUIType::TWO;
		break;
	case svaf::SITCH:
		gui_type = svaf::GUIType::THREE_BIG;
		break;
	default:
		MessageBox(L"Unrecoganize Task Type");
		break;
	}

	m_properaty.RedrawWindow();
}

void CRVAFGUIDlg::OnSelectAlgorithm()
{
	// TODO: Add your control notification handler code here
	CFileDialog opendlg(TRUE, _T("*"), NULL, OFN_OVERWRITEPROMPT, _T("Proto files(*.pbf; *.pbt)|*.pbf*;*.pbt|All files(*.*)|*.*||"), NULL);
	if (opendlg.DoModal() == IDOK){
		CString filename = opendlg.GetPathName();
		OpenProtoFile((LPCSTR)CStringA(filename));
		GenerateProperties();
		if (!ready_proto){
			SetMainUILayout(svaf::GUIType::PROTO);
		}
		hide_vtk = true;
		pointclouds.clear();
		m_imgs.clear();
		if (isExpan){
			SetMainUILayout(gui_type);
		}
		Invalidate(true);
		//PostMessage(WM_PAINT);
	}

	SetTopButtonLayout();

}

LRESULT CRVAFGUIDlg::OnPropertyChanged(WPARAM wParam, LPARAM lParam){
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lParam;
	CString s = pProp->GetName();
	COleVariant t = pProp->GetValue();
	//t = pProp->GetOriginalValue();
	CString d;
	d = t.bstrVal;
	int id = pProp->GetData();
	switch (id){
	case 100://process data source
	case 200:
	case 300:
	case 400:
	case 500:
	case 600:
	case 700:
	case 800:
	case 900:
	case 1100:
	case 1200:
		MessageBox(_T("Please make sure the datasourse is avaliable, \nNot change datasource from binocular sourse \nand monocular source."));
		if (d == _T("IMAGE")){
			layers[idtable[id]]->set_type(svaf::LayerParameter_LayerType_IMAGE);
		} else if (d == _T("IMAGE PAIR")){
			layers[idtable[id]]->set_type(svaf::LayerParameter_LayerType_IMAGE_PAIR);
		} else if (d == _T("VIDEO")){
			layers[idtable[id]]->set_type(svaf::LayerParameter_LayerType_VIDEO);
		} else if (d == _T("VIDEO PAIR")){
			layers[idtable[id]]->set_type(svaf::LayerParameter_LayerType_VIDEO_PAIR);
		} else if (d == _T("CAMERA")){
			layers[idtable[id]]->set_type(svaf::LayerParameter_LayerType_CAMERA);
		} else if (d == _T("CAMERA PAIR")){
			layers[idtable[id]]->set_type(svaf::LayerParameter_LayerType_CAMERA_PAIR);
		} else if (d == _T("DSP CAMERA")){
			layers[idtable[id]]->set_type(svaf::LayerParameter_LayerType_DSP);
		} else if (d == _T("DSP CAMERA PAIR")){
			layers[idtable[id]]->set_type(svaf::LayerParameter_LayerType_DSP_PAIR);
		} else if (d == _T("KINECT")){
			layers[idtable[id]]->set_type(svaf::LayerParameter_LayerType_KINECT);
		} else if (d == _T("IMAGE FOLDER")){
			layers[idtable[id]]->set_type(svaf::LayerParameter_LayerType_IMAGE_FOLDER);
		} else if (d == _T("IMAGE FOLDER PAIR")){
			layers[idtable[id]]->set_type(svaf::LayerParameter_LayerType_IMAGE_PAIR_FOLDER);
		}
		
		GenerateProperties();
		break;
	case 101://repeated string
		pack[id].pR->SetRepeatedString(pack[id].pM, pack[id].pF, 0, std::string((LPCSTR)CStringA(d)));
		break;
	case 102://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 201://string
		pack[id].pR->SetString(pack[id].pM, pack[id].pF, std::string((LPCSTR)CStringA(d)));
		break;
	case 202:
		pack[id].pR->SetString(pack[id].pM, pack[id].pF, std::string((LPCSTR)CStringA(d)));
		break;
	case 203://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 301://reapeated string
		pack[id].pR->SetRepeatedString(pack[id].pM, pack[id].pF, 0, std::string((LPCSTR)CStringA(d)));
		break;
	case 303://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 401://string
		pack[id].pR->SetString(pack[id].pM, pack[id].pF, std::string((LPCSTR)CStringA(d)));
		break;
	case 402://string
		pack[id].pR->SetString(pack[id].pM, pack[id].pF, std::string((LPCSTR)CStringA(d)));
		break;
	case 403://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 501://int32
		pack[id].pR->SetInt32(pack[id].pM, pack[id].pF, atoi(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 503://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 601://int32
		pack[id].pR->SetInt32(pack[id].pM, pack[id].pF, atoi(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 602://int32
		pack[id].pR->SetInt32(pack[id].pM, pack[id].pF, atoi(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 603://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 703://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 803://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 903://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 1101://repeated string
		pack[id].pR->SetRepeatedString(pack[id].pM, pack[id].pF, 0, std::string((LPCSTR)CStringA(d)));
		break;
	case 1103://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 1201://string
		pack[id].pR->SetString(pack[id].pM, pack[id].pF, std::string((LPCSTR)CStringA(d)));
		break;
	case 1202://string
		pack[id].pR->SetString(pack[id].pM, pack[id].pF, std::string((LPCSTR)CStringA(d)));
		break;
	case 1203://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 2101://string
		pack[id].pR->SetString(pack[id].pM, pack[id].pF, std::string((LPCSTR)CStringA(d)));
		break;
	case 2102://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 2103://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 2104://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 2105://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 2106://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 7201://int32
		pack[id].pR->SetInt32(pack[id].pM, pack[id].pF, atoi(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 7202://int32
		pack[id].pR->SetInt32(pack[id].pM, pack[id].pF, atoi(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 7203://int32
		pack[id].pR->SetInt32(pack[id].pM, pack[id].pF, atoi(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 7204://int32
		pack[id].pR->SetInt32(pack[id].pM, pack[id].pF, atoi(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 7205://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 7206://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 7207://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 7208://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 8102://string
		pack[id].pR->SetString(pack[id].pM, pack[id].pF, std::string((LPCSTR)CStringA(d)));
		break;
	case 8103://string
		pack[id].pR->SetString(pack[id].pM, pack[id].pF, std::string((LPCSTR)CStringA(d)));
		break;
	case 8104://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 9401://string
		pack[id].pR->SetString(pack[id].pM, pack[id].pF, std::string((LPCSTR)CStringA(d)));
		break;
	case 94201://int32
		pack[id].pR->SetInt32(pack[id].pM, pack[id].pF, atoi(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 94202://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 94203://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 94204://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 94205://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 94206://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 94301://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 94302://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 94303://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 94304://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 94305://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 94306://float
		pack[id].pR->SetFloat(pack[id].pM, pack[id].pF, atof(std::string((LPCSTR)CStringA(d)).c_str()));
		break;
	case 14101://string
		pack[id].pR->SetString(pack[id].pM, pack[id].pF, std::string((LPCSTR)CStringA(d)));
		break;
	default:
		break;
	}

	//GenerateProperties();
	return 0;
}

/*
	展开界面
*/

void CRVAFGUIDlg::OnShowMoreClicked()
{
	// TODO: Add your control notification handler code here
	if (isExpan){
		SetMainUILayout(svaf::GUIType::PROTO);
	} else{
		SetMainUILayout(gui_type);
	}

}

void CRVAFGUIDlg::SetTopButtonLayout(){
	m_showMore.MoveWindow(CRect(354, 0, gui_nrm_w - 16, 23));
	if (ready_proto){
		m_showMore.ShowWindow(SW_SHOW);
		m_selectAlgorithm.MoveWindow(CRect(0, 0, 354, 23));
	} else{
		m_showMore.ShowWindow(SW_HIDE);
		m_selectAlgorithm.MoveWindow(CRect(0, 0, gui_nrm_w - 16, 23));
	}
}

void CRVAFGUIDlg::SetMainUILayout(svaf::GUIType type){
	CRect rc, rc2;
	GetWindowRect(rc);
	// 展示计算方法
	int left_edge = gui_nrm_w - 16 + TOOL_MARGN + TOOL_WIDTH; // 不变
	int top_edge = 0; // 不变
	int right_edge = gui_nrm_w - 16; // 动态
	int bottom_edge = gui_nrm_h - 31 - 8; // 不变
	int box_height = 0;
	int edittop_edge = bottom_edge - 200; // 动态
	int middle_edge = 0;
	int margin_edge = 0; // 横，中间额外划分的3/4用

	switch (type)
	{
	case svaf::GUIType::PROTO: // 最小面板
		isExpan = false;
		m_showMore.SetWindowTextW(_T(">"));
		rc.right = rc.left + gui_nrm_w;
		rc.bottom = rc.top + gui_nrm_h;
		break;
	case svaf::GUIType::ONE: // 单画面面板
		isExpan = true;
		gui_exp_w = GUI_EXP_W;
		gui_exp_h = GUI_EXP_H;
		m_showMore.SetWindowTextW(_T("<"));
		rc.right = rc.left + gui_exp_w;
		rc.bottom = rc.top + gui_exp_h;
		m_zoonDisplay.ShowWindow(SW_SHOW);
		m_zoonDisp2.ShowWindow(SW_HIDE);
		m_zoonDisp3.ShowWindow(SW_HIDE);
		m_zoonDisp4.ShowWindow(SW_HIDE);
		right_edge = gui_exp_w - 16;
		box_height = right_edge - left_edge - DISP_MARGN - DISP_MARGN;
		edittop_edge = box_height * 0.75;
		m_editMsg.MoveWindow(CRect(left_edge + DISP_MARGN, edittop_edge + DISP_MARGN, right_edge - DISP_MARGN, bottom_edge - DISP_MARGN));
		m_zoonDisplay.MoveWindow(CRect(left_edge + DISP_MARGN, top_edge, right_edge - DISP_MARGN, top_edge + edittop_edge));
		break;
	case svaf::GUIType::ONE_BIG: // 单宽画幅面板
		isExpan = true;
		gui_exp_w = GUI_EXP_W + 550;
		gui_exp_h = GUI_EXP_H;
		m_showMore.SetWindowTextW(_T("<"));
		rc.right = rc.left + gui_exp_w;
		rc.bottom = rc.top + gui_exp_h;
		m_zoonDisplay.ShowWindow(SW_SHOW);
		m_zoonDisp2.ShowWindow(SW_HIDE);
		m_zoonDisp3.ShowWindow(SW_HIDE);
		m_zoonDisp4.ShowWindow(SW_HIDE);
		right_edge = gui_exp_w - 16;
		box_height = right_edge - left_edge - DISP_MARGN - DISP_MARGN;
		edittop_edge = box_height * 0.375;
		m_editMsg.MoveWindow(CRect(left_edge + DISP_MARGN, edittop_edge + DISP_MARGN, right_edge - DISP_MARGN, bottom_edge - DISP_MARGN));
		m_zoonDisplay.MoveWindow(CRect(left_edge + DISP_MARGN, top_edge, right_edge - DISP_MARGN, top_edge + edittop_edge));
		break;
	case svaf::GUIType::TWO: // 双画幅面板
		isExpan = true;
		gui_exp_w = GUI_EXP_W + 550;
		gui_exp_h = GUI_EXP_H;
		m_showMore.SetWindowTextW(_T("<"));
		rc.right = rc.left + gui_exp_w;
		rc.bottom = rc.top + gui_exp_h;
		m_zoonDisplay.ShowWindow(SW_SHOW);
		m_zoonDisp2.ShowWindow(SW_SHOW);
		m_zoonDisp3.ShowWindow(SW_HIDE);
		m_zoonDisp4.ShowWindow(SW_HIDE);
		right_edge = gui_exp_w - 16;
		middle_edge = (right_edge + left_edge) / 2;
		box_height = right_edge - left_edge - DISP_MARGN - DISP_MARGN;
		edittop_edge = box_height * 0.375;
		m_editMsg.MoveWindow(CRect(left_edge + DISP_MARGN, edittop_edge + DISP_MARGN, right_edge - DISP_MARGN, bottom_edge - DISP_MARGN));
		m_zoonDisplay.MoveWindow(CRect(left_edge + DISP_MARGN, top_edge, middle_edge - DISP_MARGN / 2, top_edge + edittop_edge));
		m_zoonDisp2.MoveWindow(CRect(middle_edge + DISP_MARGN / 2, top_edge, right_edge - DISP_MARGN, top_edge + edittop_edge));
		m_showMore.SetWindowTextW(_T("<"));
		break;
	case svaf::GUIType::THREE_BIG: // 宽三画幅面板
		isExpan = true;
		gui_exp_w = GUI_EXP_W + 100;
		gui_exp_h = GUI_EXP_H;
		m_showMore.SetWindowTextW(_T("<"));
		rc.right = rc.left + gui_exp_w;
		rc.bottom = rc.top + gui_exp_h;
		m_zoonDisplay.ShowWindow(SW_SHOW);
		m_zoonDisp2.ShowWindow(SW_SHOW);
		m_zoonDisp3.ShowWindow(SW_SHOW);
		m_zoonDisp4.ShowWindow(SW_HIDE);
		right_edge = gui_exp_w - 16;
		middle_edge = (right_edge + left_edge) / 2;
		box_height = right_edge - left_edge - DISP_MARGN - DISP_MARGN;
		margin_edge = box_height * 0.375;
		edittop_edge = box_height * 0.75;
		m_editMsg.MoveWindow(CRect(left_edge + DISP_MARGN, edittop_edge + DISP_MARGN + DISP_MARGN, right_edge - DISP_MARGN, bottom_edge - DISP_MARGN));
		m_zoonDisplay.MoveWindow(CRect(left_edge + DISP_MARGN, top_edge, middle_edge - DISP_MARGN / 2, top_edge + margin_edge));
		m_zoonDisp2.MoveWindow(CRect(middle_edge + DISP_MARGN / 2, top_edge, right_edge - DISP_MARGN, top_edge + margin_edge));
		m_zoonDisp3.MoveWindow(CRect(left_edge + DISP_MARGN, top_edge + margin_edge + DISP_MARGN, right_edge - DISP_MARGN, top_edge + edittop_edge + DISP_MARGN));
		m_showMore.SetWindowTextW(_T("<"));
		break;
	case svaf::GUIType::FOUR: // 四画幅面板
		isExpan = true;
		gui_exp_w = GUI_EXP_W + 100;
		gui_exp_h = GUI_EXP_H;
		m_showMore.SetWindowTextW(_T("<"));
		rc.right = rc.left + gui_exp_w;
		rc.bottom = rc.top + gui_exp_h;
		m_zoonDisplay.ShowWindow(SW_SHOW);
		m_zoonDisp2.ShowWindow(SW_SHOW);
		m_zoonDisp3.ShowWindow(SW_SHOW);
		m_zoonDisp4.ShowWindow(SW_SHOW);
		right_edge = gui_exp_w - 16;
		middle_edge = (right_edge + left_edge) / 2;
		box_height = right_edge - left_edge - DISP_MARGN - DISP_MARGN;
		margin_edge = box_height * 0.375;
		edittop_edge = box_height * 0.75;
		m_editMsg.MoveWindow(CRect(left_edge + DISP_MARGN, edittop_edge + DISP_MARGN + DISP_MARGN, right_edge - DISP_MARGN, bottom_edge - DISP_MARGN));
		m_zoonDisplay.MoveWindow(CRect(left_edge + DISP_MARGN, top_edge, middle_edge - DISP_MARGN / 2, top_edge + margin_edge));
		m_zoonDisp2.MoveWindow(CRect(middle_edge + DISP_MARGN / 2, top_edge, right_edge - DISP_MARGN, top_edge + margin_edge));
		m_zoonDisp3.MoveWindow(CRect(left_edge + DISP_MARGN, top_edge + margin_edge + DISP_MARGN, middle_edge - DISP_MARGN / 2, top_edge + edittop_edge + DISP_MARGN));
		m_zoonDisp4.MoveWindow(CRect(middle_edge + DISP_MARGN / 2, top_edge + margin_edge + DISP_MARGN, right_edge - DISP_MARGN, top_edge + edittop_edge + DISP_MARGN));
		m_showMore.SetWindowTextW(_T("<"));
		break;
	case svaf::GUIType::THREE: // 三画幅面板
		isExpan = true;
		gui_exp_w = GUI_EXP_W + 186;
		gui_exp_h = GUI_EXP_H;
		m_showMore.SetWindowTextW(_T("<"));
		rc.right = rc.left + gui_exp_w;
		rc.bottom = rc.top + gui_exp_h;
		m_zoonDisplay.ShowWindow(SW_SHOW);
		m_zoonDisp2.ShowWindow(SW_SHOW);
		m_zoonDisp3.ShowWindow(SW_SHOW);
		m_zoonDisp4.ShowWindow(SW_HIDE);
		right_edge = gui_exp_w - 16;
		middle_edge = (right_edge + left_edge) / 2;
		box_height = right_edge - left_edge - DISP_MARGN - DISP_MARGN;
		margin_edge = box_height * 0.375;
		edittop_edge = box_height * 0.75;
		//m_editMsg.MoveWindow(CRect(left_edge + DISP_MARGN, edittop_edge + DISP_MARGN + DISP_MARGN, right_edge - DISP_MARGN, bottom_edge - DISP_MARGN));
		m_zoonDisplay.MoveWindow(CRect(left_edge + DISP_MARGN, top_edge, middle_edge - DISP_MARGN / 2, top_edge + margin_edge));
		m_zoonDisp2.MoveWindow(CRect(middle_edge + DISP_MARGN / 2, top_edge, right_edge - DISP_MARGN, top_edge + margin_edge));
		m_editMsg.MoveWindow(CRect(left_edge + DISP_MARGN, top_edge + margin_edge + DISP_MARGN, middle_edge - DISP_MARGN / 2, top_edge + edittop_edge + DISP_MARGN));
		m_zoonDisp3.MoveWindow(CRect(middle_edge + DISP_MARGN / 2, top_edge + margin_edge + DISP_MARGN, right_edge - DISP_MARGN, top_edge + edittop_edge + DISP_MARGN));
		m_showMore.SetWindowTextW(_T("<"));
		break;
	default:
		break;
	}
	MoveWindow(rc);
	if (hide_vtk){
		m_vtk1.ShowWindow(SW_HIDE);
		m_vtk2.ShowWindow(SW_HIDE);
	} else if (pointclouds.size() == 1){
		m_zoonDisp3.ShowWindow(SW_HIDE);
	} else if (pointclouds.size() == 2){
		m_zoonDisp3.ShowWindow(SW_HIDE);
		m_zoonDisp4.ShowWindow(SW_HIDE);
	}
	
}

void CRVAFGUIDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

void CRVAFGUIDlg::OnRunSvafTask()
{
	// TODO: Add your control notification handler code here
	// close svaf.exe
	pDrawLineDlg->ClearPoint();
	CloseProgram(_T("SVAF.exe"));

	//svaf::WriteProtoToBinaryFile(m_svaftask, "cache");
	svaf::WriteProtoToTextFile(m_svaftask, "cache");
	CString str(_T("SVAF.exe"));
	if (str.GetLength() != 0){
		DWORD	ExitCode;
		GetExitCodeProcess(m_pInfo.hProcess, &ExitCode);
		if (ExitCode != STILL_ACTIVE){
			isPause = false;
			STARTUPINFO si = { sizeof(si) };

			si.dwFlags = STARTF_USESHOWWINDOW; // wShowWindow有效
#ifdef _DEBUG
			si.wShowWindow = TRUE; // TRUE 显示新建进程主窗口， FALSE 不显示
#else
			si.wShowWindow = FALSE;
#endif

			BOOL bRet = ::CreateProcess(
				str,
				_T(" --config_file=\"cache\" --use_gui=true"),
				NULL,
				NULL,
				FALSE,
				CREATE_NEW_CONSOLE,
				NULL,
				NULL,
				&si,
				&m_pInfo);
			if (bRet == FALSE){
				MessageBox(_T("Create Process Failed!"));
			} else{
				AppendMessage(_T("Run SVAF.exe"));
				SetRuningStatus(true);
			}
		} else{
			MessageBox(_T("Please Wait Current Task Finished!"));
		}
	}

}

void CRVAFGUIDlg::OnSaveProtoText()
{
	// TODO: Add your control notification handler code here
	CString saveFileName;
	CFileDialog dlg(FALSE, _T("pbf"), _T(""), OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, _T("ProtoBufFile|*.pbf|All Files|*||"));
	dlg.m_ofn.lpstrTitle = _T("Save Algorithm Configuration File");
	if (dlg.DoModal() == IDOK){
		saveFileName = dlg.GetPathName();
		svaf::WriteProtoToTextFile(m_svaftask, (LPCSTR)CStringA(saveFileName));
		GenerateProperties();
		AppendMessage(saveFileName + CString(" have been saved."));
	}
}


void CRVAFGUIDlg::OnSaveProtoBinary()
{
	// TODO: Add your control notification handler code here
	CString saveFileName;
	CFileDialog dlg(FALSE, _T("pbf"), _T(""), OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY, _T("ProtoBufFile|*.pbf|All Files|*||"));
	dlg.m_ofn.lpstrTitle = _T("Save Algorithm Configuration File");
	if (dlg.DoModal() == IDOK){
		saveFileName = dlg.GetPathName();
		svaf::WriteProtoToBinaryFile(m_svaftask, (LPCSTR)CStringA(saveFileName));
		GenerateProperties();
	}
}

DWORD CRVAFGUIDlg::GetProcessIdFromName(LPCTSTR name)    //通过执行文件名获得进程ID的方法
{
	PROCESSENTRY32 pe;
	DWORD id = 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
		return 0;

	do
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (Process32Next(hSnapshot, &pe) == FALSE)
			break;
		if (strcmp((char*)pe.szExeFile, (char*)name) == 0)
		{
			id = pe.th32ProcessID;
			break;
		}

	} while (1);

	CloseHandle(hSnapshot);

	return id;
}

void CRVAFGUIDlg::CloseProgram(CString strProgram)
{
	HANDLE handle; //定义CreateToolhelp32Snapshot系统快照句柄   
	HANDLE handle1; //定义要结束进程句柄   
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//获得系统快照句柄   
	PROCESSENTRY32 *info; //定义PROCESSENTRY32结构字指   
	//PROCESSENTRY32 结构的 dwSize 成员设置成 sizeof(PROCESSENTRY32)   

	info = new PROCESSENTRY32;
	info->dwSize = sizeof(PROCESSENTRY32);
	//调用一次 Process32First 函数，从快照中获取进程列表   
	Process32First(handle, info);
	//重复调用 Process32Next，直到函数返回 FALSE 为止   
	while (Process32Next(handle, info) != FALSE)
	{
		CString strTmp = info->szExeFile;     //指向进程名字     
		//strcmp字符串比较函数同要结束相同     
		//if(strcmp(c, info->szExeFile) == 0 )     
		if (strProgram.CompareNoCase(info->szExeFile) == 0)
		{
			//PROCESS_TERMINATE表示为结束操作打开,FALSE=可继承,info->th32ProcessID=进程ID 
			//MessageBox(L"Current SVAF Process will be killed.");
			handle1 = OpenProcess(PROCESS_TERMINATE, FALSE, info->th32ProcessID);
			//结束进程      
			TerminateProcess(handle1, 0);
		}
	}
	delete info;

	CloseHandle(handle);
}

void CRVAFGUIDlg::OnStopSvafTask()
{
	// TODO: Add your control notification handler code here
	CloseProgram(_T("SVAF.exe"));
	SetRuningStatus(false);
	AppendMessage(_T("SVAF has been terminate"));
}


void CRVAFGUIDlg::OnPauseContinue()
{
	// TODO: Add your control notification handler code here
	if (!isRunning){
		isPause = false;
		return;
	}

	if (!isPause){ // apply pause
		isPause = true;
		SendCommand(2);
	} else{
		isPause = false;
		SendCommand(3);
		SetEvent(c_hMutex);
	}
}


void CRVAFGUIDlg::OnOpenRobotCtrlDlg()
{
	// TODO: Add your control notification handler code here
	//pRobotCtrlDlg = new CRobotControlDlg();
	//pRobotCtrlDlg->Create(IDD_ROBOT_DIALOG);
	pRobotCtrlDlg->ShowWindow(SW_SHOW);
}


void CRVAFGUIDlg::OnClearProgram()
{
	// TODO: Add your control notification handler code here
	CCleanDlg clearProgramDlg;
	clearProgramDlg.DoModal();
	AppendMessage(L"cache has been cleared.");
}


void CRVAFGUIDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CString str(_T("SVAF.exe"));
	switch (nIDEvent)
	{
	case PROCESS_DETECT_TIMER:
		if (str.GetLength() != 0){
			DWORD	ExitCode;
			GetExitCodeProcess(m_pInfo.hProcess, &ExitCode);
			if (ExitCode != STILL_ACTIVE){
				SetRuningStatus(false);
			} else{
				SetRuningStatus(true);
			}
		}
		break;
	default:
		break;
	}


	CDialogEx::OnTimer(nIDEvent);
}

void CRVAFGUIDlg::SettingAlgorithm()
{
	// TODO: Add your control notification handler code here

}


void CRVAFGUIDlg::OpenLog()
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, L"explore", L"log", NULL, NULL, SW_SHOW);
	//CString logfilename;
	//ShellExecute(NULL, _T("open"), logfilename, NULL, NULL, SW_SHOW);
}


void CRVAFGUIDlg::ShowResultFiles()
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL, L"explore", L"tmp", NULL, NULL, SW_SHOW);
}


BOOL CRVAFGUIDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_MOUSEMOVE){
		m_toolTip.RelayEvent(pMsg);
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CRVAFGUIDlg::SetRuningStatus(bool running){
	HICON hIcon;
	isRunning = running;
	if (isRunning){
		hIcon = AfxGetApp()->LoadIconW(IDI_WAIT);
		((CButton*)GetDlgItem(IDC_BUTTON3))->SetIcon(hIcon);
		m_toolTip.AddTool(GetDlgItem(IDC_BUTTON3), _T("Wait Algorithm Terminate"));
		GetDlgItem(IDC_BUTTON3)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON5)->EnableWindow(true);
	} else{
		hIcon = AfxGetApp()->LoadIconW(IDI_CONTINUE);
		((CButton*)GetDlgItem(IDC_BUTTON3))->SetIcon(hIcon);
		m_toolTip.AddTool(GetDlgItem(IDC_BUTTON3), _T("Start Algorithm"));
		GetDlgItem(IDC_BUTTON3)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON4)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON5)->EnableWindow(false);
	}
}

void CRVAFGUIDlg::RobotFetch(float x, float y, float z, float a, float b, float c){
	if (!pRobotCtrlDlg->m_checkEnable.GetCheck()){
		return;
	}
	if (y < -25){
		y = -25;
	}
	if (y > 25){
		y = 25;
	}
	if (z < 50){
		z = 50;
	}
	a = 180;
	b = 0;
	c = 180;

	if (RobotRestriction(x, y, z+5, a, b, c) && RobotRestriction(x, y, z+15, a, b, c)){
		//if (RobotIsOpen()){
		if (true){
			CString cs;
			cs.Format(L"Fetch command x:%7.2f, y:%7.2f, z:%7.2f", x, y, z);
			AppendMessage(cs);
			MessageBox(cs);
			MessageBox(L"Fetch Confirm Stage1");
			RobotMove(x, y, z + 25, a, b, c);
			MessageBox(L"Fetch Confirm Stage2");
			RobotMove(x, y, z, a, b, c);
			MessageBox(L"Return Wait Location");
			RobotMove(625, 0, 200, a, b, c);
		} else{
			MessageBox(L"Robot Closed");
		}
	} else{
		MessageBox(L"Beyond Restriction");
	}
}

void CRVAFGUIDlg::RobotMove(float x, float y, float z, float a, float b, float c){
	pRobotCtrlDlg->m_XBase = x;
	pRobotCtrlDlg->m_YBase = y;
	pRobotCtrlDlg->m_ZBase = z;
	pRobotCtrlDlg->m_ABase = a;
	pRobotCtrlDlg->m_BBase = b;
	pRobotCtrlDlg->m_CBase = c;
	pRobotCtrlDlg->UpdateData(false);
	pRobotCtrlDlg->m_rbc.MoveToBaseMark();
}

bool CRVAFGUIDlg::RobotRestriction(float x, float y, float z, float a, float b, float c){
	if (x < 0 || x > 950){
		return false;
	}
	if (y < -25 || y > 25){
		return false;
	}
	if (z < 40 || z > 400){
		return false;
	}
	return true;
}

void CRVAFGUIDlg::RobotReset(){

}

bool CRVAFGUIDlg::RobotIsOpen(){
	return pRobotCtrlDlg->m_rbc.IsLinked;
}

void CRVAFGUIDlg::OnBnClickedDrawLine()
{
	// TODO: Add your control notification handler code here
	pDrawLineDlg->ShowWindow(SW_SHOW);
}
