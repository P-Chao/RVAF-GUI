
// RVAF-GUIDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RVAF-GUI.h"
#include "RVAF-GUIDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_SIZE()
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
}

BEGIN_MESSAGE_MAP(CRVAFGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_BN_CLICKED(IDC_BUTTON_SETALG, &CRVAFGUIDlg::OnSelectAlgorithm)
	ON_BN_CLICKED(IDC_BUTTON_MORE, &CRVAFGUIDlg::OnShowMoreClicked)
END_MESSAGE_MAP()


// CRVAFGUIDlg message handlers

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
	ready_proto = false;

	HDITEM item;
	item.cxy = 130;
	item.mask = HDI_WIDTH;
	m_properaty.GetHeaderCtrl().SetItem(0, new HDITEM(item));

	return TRUE;  // return TRUE  unless you set the focus to a control
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
		MessageBoxW(L"File version error, file will be recreate!", L"Tips", 0);
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

	// Read Proto Files
	if (!ReadCheckProtoFile(filename)){
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
		layers[layer.name()] = layer;
	}

	// Build linked list
	for (auto layer : layers){
		if (layer.second.name() == layer.second.bottom()){
			dummy.next = new Node(layer.second.name());
			break;
		}
	}

	int layer_cnt = 1;
	node = dummy.next;
	while (true){
		if (layers[node->name].name() == layers[node->name].top()){
			break;
		}
		if (layers.end() == layers.find(layers[node->name].top())){
			MessageBoxW(L"Layer Loss", L"Tips", 0);
			return;
		}
		node->next = new Node(layers[node->name].top());
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
		return;
	}

	const google::protobuf::Message* pMessage = NULL;
	const google::protobuf::Descriptor* pDescriptor = NULL;
	const google::protobuf::Reflection* pReflection = NULL;
	const google::protobuf::FieldDescriptor* pField = NULL;
	string type_name;

	
	Node *node = &dummy;
	while (node->next){
		CMFCPropertyGridProperty * group = NULL, * group2 = NULL;
		CMFCPropertyGridProperty * pProp = NULL;
		CString cs_temp;
		
		node = node->next;
		auto type = layers[node->name].type();
		//auto layer = layers[node->name];
		switch (type)
		{
		case svaf::LayerParameter_LayerType_NONE://00
			break;
		case svaf::LayerParameter_LayerType_IMAGE://01
			group = new CMFCPropertyGridProperty(_T("Image"));

			// 0102 bool
			id = 0102;
			pProp = new CMFCPropertyGridProperty(_T("Color"), _T(""), _T("choose output image of color or gray"));
			pProp->SetData(id);
			idtable[id] = node->name;
			
			pMessage = &layers[node->name].data_param();
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
			group = new CMFCPropertyGridProperty(_T("Image Pair"));

			// 0201 string
			id = 0301;
			pProp = new CMFCPropertyGridFileProperty(_T("Left Image"), TRUE, _T("value"));
			pProp->SetData(id); 
			pProp->SetOriginalValue(CString(layers[node->name].imagepair_param().pair(0).left().c_str()));
			pProp->ResetOriginalValue();
			idtable[id] = node->name;
			group->AddSubItem(pProp);

			// 0202 string
			id = 0302;
			pProp = new CMFCPropertyGridFileProperty(_T("Right Image"), TRUE, _T("value"));
			pProp->SetData(id);
			pProp->SetOriginalValue(CString(layers[node->name].imagepair_param().pair(0).right().c_str()));
			pProp->ResetOriginalValue();
			idtable[id] = node->name;
			group->AddSubItem(pProp);

			// 0203 bool
			id = 0203;
			pProp = new CMFCPropertyGridProperty(_T("Color"), _T(""), _T("choose output image of color or gray"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name].data_param();
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
			group = new CMFCPropertyGridProperty(_T("Video"));
			break;
		case svaf::LayerParameter_LayerType_VIDEO_PAIR://04
			group = new CMFCPropertyGridProperty(_T("Video Pair"));
			break;
		case svaf::LayerParameter_LayerType_CAMERA://05
			group = new CMFCPropertyGridProperty(_T("Camera"));
			break;
		case svaf::LayerParameter_LayerType_CAMERA_PAIR://06
			group = new CMFCPropertyGridProperty(_T("Camera Pair"));
			break;
		case svaf::LayerParameter_LayerType_DSP://07
			group = new CMFCPropertyGridProperty(_T("DSP Camera"));
			break;
		case svaf::LayerParameter_LayerType_DSP_PAIR://08
			group = new CMFCPropertyGridProperty(_T("DSP Camera Pair"));
			break;
		case svaf::LayerParameter_LayerType_KINECT://09
			group = new CMFCPropertyGridProperty(_T("Kinect"));
			break;
		case svaf::LayerParameter_LayerType_IMAGE_FOLDER://11
			group = new CMFCPropertyGridProperty(_T("Image Folder"));
			break;
		case svaf::LayerParameter_LayerType_IMAGE_PAIR_FOLDER://12
			group = new CMFCPropertyGridProperty(_T("Image Folder Pair"));
			break;
		case svaf::LayerParameter_LayerType_ADABOOST://21
			group = new CMFCPropertyGridProperty(_T("Adaboost"));

			// 2101 string
			id = 2101;
			pProp = new CMFCPropertyGridFileProperty(_T("Detector"), TRUE, _T(""));
			pProp->SetDescription(_T("detector file trained by piotr dollar toolbox"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name].adaboost_param();
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

			pMessage = &layers[node->name].adaboost_param();
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

			pMessage = &layers[node->name].adaboost_param();
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

			pMessage = &layers[node->name].adaboost_param();
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

			pMessage = &layers[node->name].adaboost_param();
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

			pMessage = &layers[node->name].adaboost_param();
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
			group = new CMFCPropertyGridProperty(_T("MILTrack"));
			break;
		case svaf::LayerParameter_LayerType_BITTRACK://32
			group = new CMFCPropertyGridProperty(_T("Bino MILTrack"));
			break;
		case svaf::LayerParameter_LayerType_SIFT_POINT://41
			group = new CMFCPropertyGridProperty(_T("SIFT Point"));
			break;
		case svaf::LayerParameter_LayerType_SURF_POINT://42
			group = new CMFCPropertyGridProperty(_T("SURF Point"));
			break;
		case svaf::LayerParameter_LayerType_STAR_POINT://43
			group = new CMFCPropertyGridProperty(_T("STAR Point"));
			break;
		case svaf::LayerParameter_LayerType_BRISK_POINT://44
			group = new CMFCPropertyGridProperty(_T("Brisk Point"));
			break;
		case svaf::LayerParameter_LayerType_FAST_POINT://45
			group = new CMFCPropertyGridProperty(_T("FAST Point"));
			break;
		case svaf::LayerParameter_LayerType_ORB_POINT://46
			group = new CMFCPropertyGridProperty(_T("ORB Point"));
			break;
		case svaf::LayerParameter_LayerType_KAZE_POINT://47
			group = new CMFCPropertyGridProperty(_T("KAZE Point"));
			break;
		case svaf::LayerParameter_LayerType_HARRIS_POINT://48
			group = new CMFCPropertyGridProperty(_T("Harris Point"));
			break;
		case svaf::LayerParameter_LayerType_CV_POINT://49
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
			group = new CMFCPropertyGridProperty(_T("KDtree Match"));
			break;
		case svaf::LayerParameter_LayerType_EULAR_MATCH://62
			group = new CMFCPropertyGridProperty(_T("Eular Match"));
			break;
		case svaf::LayerParameter_LayerType_RANSAC://63
			group = new CMFCPropertyGridProperty(_T("Ransac"));
			break;
		case svaf::LayerParameter_LayerType_BF_MATCH://64
			group = new CMFCPropertyGridProperty(_T("BF Match"));
			break;
		case svaf::LayerParameter_LayerType_FLANN_MATCH://65
			group = new CMFCPropertyGridProperty(_T("Flann Match"));
			break;
		case svaf::LayerParameter_LayerType_EC_MATCH://68
			group = new CMFCPropertyGridProperty(_T("EC Match"));
			break;
		case svaf::LayerParameter_LayerType_CV_MATCH://69
			group = new CMFCPropertyGridProperty(_T("CV Match"));
			break;
		case svaf::LayerParameter_LayerType_SGM_MATCH://71
			group = new CMFCPropertyGridProperty(_T("SGM Stereo Match"));
			break;
		case svaf::LayerParameter_LayerType_EADP_MATCH://72
			group = new CMFCPropertyGridProperty(_T("Eadp Stereo Match"));
			
			// 7201 int32
			id = 7201;
			pProp = new CMFCPropertyGridProperty(_T("Max Disparity"), _T(""), _T("maximum disparity of stereo matching"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name].eadp_param();
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

			pMessage = &layers[node->name].eadp_param();
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

			pMessage = &layers[node->name].eadp_param();
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

			pMessage = &layers[node->name].eadp_param();
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

			pMessage = &layers[node->name].eadp_param();
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

			pMessage = &layers[node->name].eadp_param();
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

			pMessage = &layers[node->name].eadp_param();
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

			pMessage = &layers[node->name].eadp_param();
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

			pMessage = &layers[node->name].triang_param();
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

			pMessage = &layers[node->name].triang_param();
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

			pMessage = &layers[node->name].triang_param();
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
			group = new CMFCPropertyGridProperty(_T("SAC-IA"));

			// 9401 string
			id = 9401;
			pProp = new CMFCPropertyGridFileProperty(_T("Referance Model"), TRUE, _T(""));
			pProp->SetDescription(_T("referance pcd file"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name].sacia_param();
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

				pMessage = &layers[node->name].sacia_param().ia_param();
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

				pMessage = &layers[node->name].sacia_param().ia_param();
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

				pMessage = &layers[node->name].sacia_param().ia_param();
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

				pMessage = &layers[node->name].sacia_param().ia_param();
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

				pMessage = &layers[node->name].sacia_param().ia_param();
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

				pMessage = &layers[node->name].sacia_param().ia_param();
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

				pMessage = &layers[node->name].sacia_param().coor_param();
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

				pMessage = &layers[node->name].sacia_param().coor_param();
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

				pMessage = &layers[node->name].sacia_param().coor_param();
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

				pMessage = &layers[node->name].sacia_param().coor_param();
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

				pMessage = &layers[node->name].sacia_param().coor_param();
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

				pMessage = &layers[node->name].sacia_param().coor_param();
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
			group = new CMFCPropertyGridProperty(_T("SAC-IA ICP"));
			break;
		case svaf::LayerParameter_LayerType_IANDT_EST://96
			group = new CMFCPropertyGridProperty(_T("SAC-IA NDP"));
			break;
		case svaf::LayerParameter_LayerType_SUPIX_SEG://101
			group = new CMFCPropertyGridProperty(_T("Superpixel Segment"));
			break;
		case svaf::LayerParameter_LayerType_RECTIFY://141
			group = new CMFCPropertyGridProperty(_T("Stereo Rectify"));

			// 14101 string
			id = 14101;
			pProp = new CMFCPropertyGridFileProperty(_T("Camera Param"), TRUE, _T(""));
			pProp->SetDescription(_T("calibration file of binocular camera"));
			pProp->SetData(id);
			idtable[id] = node->name;

			pMessage = &layers[node->name].rectify_param();
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
	
}

void CRVAFGUIDlg::OnSelectAlgorithm()
{
	// TODO: Add your control notification handler code here
	CFileDialog opendlg(TRUE, _T("*"), NULL, OFN_OVERWRITEPROMPT, _T("Proto files(*.pbf; *.pbt)|*.pbf*;*.pbt|All files(*.*)|*.*||"), NULL);
	if (opendlg.DoModal() == IDOK){
		CString filename = opendlg.GetPathName();
		OpenProtoFile((LPCSTR)CStringA(filename));
		GenerateProperties();
	}

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
	case 0102://bool
		if (d == "TRUE"){
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, true);
		} else{
			pack[id].pR->SetBool(pack[id].pM, pack[id].pF, false);
		}
		break;
	case 0203://bool
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
	
	return 0;
}

/*
	展开界面
*/

void CRVAFGUIDlg::OnShowMoreClicked()
{
	// TODO: Add your control notification handler code here

}


void CAboutDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}
