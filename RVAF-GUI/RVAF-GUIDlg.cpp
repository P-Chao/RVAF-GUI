
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
END_MESSAGE_MAP()


// CRVAFGUIDlg message handlers
void CRVAFGUIDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	CloseHandle(c_hFileMapping);
	CloseHandle(c_hMutex);

	CloseHandle(d_hFileMapping);
	CloseHandle(d_hMutex);
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

	// TODO: Add extra initialization here
	InitInterprocess();

	isPause = false;

	ready_proto = false;

	isExpan = false;

	gui_type = 1;
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRVAFGUIDlg::InitInterprocess(){
	// run process
	memset(&m_pInfo, 0, sizeof(m_pInfo));

	// send mapping
	c_hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 4, _T("SVAF_GUI2ALG_CMD"));
	c_hMutex = CreateEvent(nullptr, false, false, _T("SVAF_GUI2ALG_CMD_MUTEX"));
	c_pMapping = (LPTSTR)MapViewOfFile(c_hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (c_pMapping == NULL){
		MessageBox(_T("Create Inter Process Error!"));
		exit(-1);
	}

	// recive data
	d_hFileMapping = CreateFileMapping(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0, 640*480*4*20, _T("SVAF_ALG2GUI_DATA"));
	d_hMutex = CreateEvent(nullptr, false, false, _T("SVAF_ALG2GUI_DATA_MUTEX"));
	d_pMapping = (LPTSTR)MapViewOfFile(d_hFileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	if (d_pMapping == NULL){
		MessageBox(_T("Create Inter Process Error!"));
		exit(-1);
	}

}

void CRVAFGUIDlg::SendInterprocess(){
	LPTSTR p = c_pMapping;
	((int*)p)[0] = 0;
	//SetEvent(m_hMutex);
}


void CRVAFGUIDlg::SendCommand(int cmd){
	LPTSTR p = c_pMapping;
	((int*)p)[0] = cmd;
	//SetEvent(m_hMutex);
}

void CRVAFGUIDlg::ReciveInterprocess(){
	
}

void CRVAFGUIDlg::ProcessInterprocess(){

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
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRVAFGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
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
	string type_name;

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
			gui_type = 1;

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
			gui_type = 2;

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
			gui_type = 1;

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
			gui_type = 2;

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
			gui_type = 1;

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
			gui_type = 2;

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
			gui_type = 1;

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
			gui_type = 2;

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
			gui_type = 1;

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
			gui_type = 1;

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
			gui_type = 2;

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
				gui_type = 2;
			} else{
				gui_type = 1;
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
				gui_type = 2;
			} else{
				gui_type = 1;
			}

			group = new CMFCPropertyGridProperty(_T("MILTrack"));
			break;
		case svaf::LayerParameter_LayerType_BITTRACK://32
			if (isbinocular){
				gui_type = 2;
			} else{
				gui_type = 1;
			}

			group = new CMFCPropertyGridProperty(_T("Bino MILTrack"));
			break;
		case svaf::LayerParameter_LayerType_SIFT_POINT://41
			if (isbinocular){
				gui_type = 2;
			} else{
				gui_type = 1;
			}

			group = new CMFCPropertyGridProperty(_T("SIFT Point"));
			break;
		case svaf::LayerParameter_LayerType_SURF_POINT://42
			if (isbinocular){
				gui_type = 2;
			} else{
				gui_type = 1;
			}

			group = new CMFCPropertyGridProperty(_T("SURF Point"));
			break;
		case svaf::LayerParameter_LayerType_STAR_POINT://43
			if (isbinocular){
				gui_type = 2;
			} else{
				gui_type = 1;
			}

			group = new CMFCPropertyGridProperty(_T("STAR Point"));
			break;
		case svaf::LayerParameter_LayerType_BRISK_POINT://44
			if (isbinocular){
				gui_type = 2;
			} else{
				gui_type = 1;
			}

			group = new CMFCPropertyGridProperty(_T("Brisk Point"));
			break;
		case svaf::LayerParameter_LayerType_FAST_POINT://45
			if (isbinocular){
				gui_type = 2;
			} else{
				gui_type = 1;
			}

			group = new CMFCPropertyGridProperty(_T("FAST Point"));
			break;
		case svaf::LayerParameter_LayerType_ORB_POINT://46
			if (isbinocular){
				gui_type = 2;
			} else{
				gui_type = 1;
			}

			group = new CMFCPropertyGridProperty(_T("ORB Point"));
			break;
		case svaf::LayerParameter_LayerType_KAZE_POINT://47
			if (isbinocular){
				gui_type = 2;
			} else{
				gui_type = 1;
			}

			group = new CMFCPropertyGridProperty(_T("KAZE Point"));
			break;
		case svaf::LayerParameter_LayerType_HARRIS_POINT://48
			if (isbinocular){
				gui_type = 2;
			} else{
				gui_type = 1;
			}

			group = new CMFCPropertyGridProperty(_T("Harris Point"));
			break;
		case svaf::LayerParameter_LayerType_CV_POINT://49
			if (isbinocular){
				gui_type = 2;
			} else{
				gui_type = 1;
			}

			group = new CMFCPropertyGridProperty(_T("CV Point"));
			break;
		case svaf::LayerParameter_LayerType_SIFT_DESP://51
			group = new CMFCPropertyGridProperty(_T("SIFT Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_SURF_DESP://52
			group = new CMFCPropertyGridProperty(_T("SURF Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_STAR_DESP://53
			group = new CMFCPropertyGridProperty(_T("STAR Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_BRIEF_DESP://54
			group = new CMFCPropertyGridProperty(_T("BRIEF Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_BRISK_DESP://55
			group = new CMFCPropertyGridProperty(_T("Brisk Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_FAST_DESP://56
			group = new CMFCPropertyGridProperty(_T("FAST Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_ORB_DESP://57
			group = new CMFCPropertyGridProperty(_T("ORB Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_KAZE_DESP://58
			group = new CMFCPropertyGridProperty(_T("KAZE Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_CV_DESP://59
			group = new CMFCPropertyGridProperty(_T("CV Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_KDTREE_MATCH://61
			gui_type = 3;
			group = new CMFCPropertyGridProperty(_T("KDtree Match"));
			break;
		case svaf::LayerParameter_LayerType_EULAR_MATCH://62
			gui_type = 3;
			group = new CMFCPropertyGridProperty(_T("Eular Match"));
			break;
		case svaf::LayerParameter_LayerType_RANSAC://63
			gui_type = 3;
			group = new CMFCPropertyGridProperty(_T("Ransac"));
			break;
		case svaf::LayerParameter_LayerType_BF_MATCH://64
			gui_type = 3;
			group = new CMFCPropertyGridProperty(_T("BF Match"));
			break;
		case svaf::LayerParameter_LayerType_FLANN_MATCH://65
			gui_type = 3;
			group = new CMFCPropertyGridProperty(_T("Flann Match"));
			break;
		case svaf::LayerParameter_LayerType_EC_MATCH://68
			gui_type = 3;
			group = new CMFCPropertyGridProperty(_T("EC Match"));
			break;
		case svaf::LayerParameter_LayerType_CV_MATCH://69
			gui_type = 3;
			group = new CMFCPropertyGridProperty(_T("CV Match"));
			break;
		case svaf::LayerParameter_LayerType_SGM_MATCH://71
			gui_type = 6;
			group = new CMFCPropertyGridProperty(_T("SGM Stereo Match"));
			break;
		case svaf::LayerParameter_LayerType_EADP_MATCH://72
			gui_type = 6;
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
			group = new CMFCPropertyGridProperty(_T("Matrix Mul"));
			break;
		case svaf::LayerParameter_LayerType_CENTER_POS://91
			group = new CMFCPropertyGridProperty(_T("Center Position"));
			break;
		case svaf::LayerParameter_LayerType_IA_EST://94
			gui_type = 4;

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
			gui_type = 4;
			group = new CMFCPropertyGridProperty(_T("SAC-IA ICP"));
			break;
		case svaf::LayerParameter_LayerType_IANDT_EST://96
			gui_type = 4;
			group = new CMFCPropertyGridProperty(_T("SAC-IA NDP"));
			break;
		case svaf::LayerParameter_LayerType_SUPIX_SEG://101
			if (isbinocular){
				gui_type = 2;
			} else{
				gui_type = 1;
			}
			group = new CMFCPropertyGridProperty(_T("Superpixel Segment"));
			break;
		case svaf::LayerParameter_LayerType_RECTIFY://141
			if (isbinocular){
				gui_type = 2;
			} else{
				gui_type = 1;
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
			break;
		}
		
		if (group){
			m_properaty.AddProperty(group);
		}
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
			SetMainUILayout(0);
		}
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
		SetMainUILayout(0);
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

void CRVAFGUIDlg::SetMainUILayout(int type){
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
	case 0: // 最小面板
		isExpan = false;
		m_showMore.SetWindowTextW(_T(">"));
		rc.right = rc.left + gui_nrm_w;
		rc.bottom = rc.top + gui_nrm_h;
		break;
	case 1: // 单画面面板
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
	case 3: // 单宽画幅面板
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
	case 2: // 双画幅面板
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
	case 5: // 宽三画幅面板
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
	case 4: // 四画幅面板
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
	case 6: // 三画幅面板
		isExpan = true;
		gui_exp_w = GUI_EXP_W + 186;
		gui_exp_h = GUI_EXP_H;
		m_showMore.SetWindowTextW(_T("<"));
		rc.right = rc.left + gui_exp_w;
		rc.bottom = rc.top + gui_exp_h;
		m_zoonDisplay.ShowWindow(SW_SHOW);
		m_zoonDisp2.ShowWindow(SW_SHOW);
		m_zoonDisp3.ShowWindow(SW_HIDE);
		m_zoonDisp4.ShowWindow(SW_SHOW);
		right_edge = gui_exp_w - 16;
		middle_edge = (right_edge + left_edge) / 2;
		box_height = right_edge - left_edge - DISP_MARGN - DISP_MARGN;
		margin_edge = box_height * 0.375;
		edittop_edge = box_height * 0.75;
		//m_editMsg.MoveWindow(CRect(left_edge + DISP_MARGN, edittop_edge + DISP_MARGN + DISP_MARGN, right_edge - DISP_MARGN, bottom_edge - DISP_MARGN));
		m_zoonDisplay.MoveWindow(CRect(left_edge + DISP_MARGN, top_edge, middle_edge - DISP_MARGN / 2, top_edge + margin_edge));
		m_zoonDisp2.MoveWindow(CRect(middle_edge + DISP_MARGN / 2, top_edge, right_edge - DISP_MARGN, top_edge + margin_edge));
		m_editMsg.MoveWindow(CRect(left_edge + DISP_MARGN, top_edge + margin_edge + DISP_MARGN, middle_edge - DISP_MARGN / 2, top_edge + edittop_edge + DISP_MARGN));
		m_zoonDisp4.MoveWindow(CRect(middle_edge + DISP_MARGN / 2, top_edge + margin_edge + DISP_MARGN, right_edge - DISP_MARGN, top_edge + edittop_edge + DISP_MARGN));
		m_showMore.SetWindowTextW(_T("<"));
		break;
	default:
		break;
	}
	MoveWindow(rc);
}

void CRVAFGUIDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

void CRVAFGUIDlg::OnRunSvafTask()
{
	// TODO: Add your control notification handler code here
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
			si.wShowWindow = TRUE; // TRUE 显示新建进程主窗口， FALSE 不显示

			BOOL bRet = ::CreateProcess(
				str,
				_T(" --config_file=\"cache\" --use_mapping=true"),
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
}


void CRVAFGUIDlg::OnPauseContinue()
{
	// TODO: Add your control notification handler code here
	if (!isPause){ // apply pause
		isPause = true;
		SendCommand(2);
	} else{
		isPause = false;
		SendCommand(3);
		SetEvent(c_hMutex);
	}
}
