
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
}

BEGIN_MESSAGE_MAP(CRVAFGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
	ON_BN_CLICKED(IDC_BUTTON_SETALG, &CRVAFGUIDlg::OnSelectAlgorithm)
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
	item.cxy = 120;
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
	int id = 0;

	if (!ready_proto){
		return;
	}

	Node *node = &dummy;
	while (node->next){
		CMFCPropertyGridProperty * group = NULL;
		CMFCPropertyGridProperty * pProp = NULL;
		
		
		node = node->next;
		auto type = layers[node->name].type();
		auto layer = layers[node->name];
		switch (type)
		{
		case svaf::LayerParameter_LayerType_NONE:
			break;
		case svaf::LayerParameter_LayerType_IMAGE:
			group = new CMFCPropertyGridProperty(_T("Image"));
			break;
		case svaf::LayerParameter_LayerType_IMAGE_PAIR:
			group = new CMFCPropertyGridProperty(_T("Image Pair"));

			pProp = new CMFCPropertyGridFileProperty(_T("Left Image"), TRUE, _T("value"));
			pProp->SetData(++id);
			pProp->SetOriginalValue(CString(layers[node->name].imagepair_param().pair(0).left().c_str()));
			pProp->ResetOriginalValue();
			idtable[id] = node->name;
			group->AddSubItem(pProp);

			pProp = new CMFCPropertyGridFileProperty(_T("Right Image"), TRUE, _T("value"));
			pProp->SetData(++id);
			pProp->SetOriginalValue(CString(layers[node->name].imagepair_param().pair(0).right().c_str()));
			pProp->ResetOriginalValue();
			idtable[id] = node->name;
			group->AddSubItem(pProp);

			break;
		case svaf::LayerParameter_LayerType_VIDEO:
			group = new CMFCPropertyGridProperty(_T("Video"));
			break;
		case svaf::LayerParameter_LayerType_VIDEO_PAIR:
			group = new CMFCPropertyGridProperty(_T("Video Pair"));
			break;
		case svaf::LayerParameter_LayerType_CAMERA:
			group = new CMFCPropertyGridProperty(_T("Camera"));
			break;
		case svaf::LayerParameter_LayerType_CAMERA_PAIR:
			group = new CMFCPropertyGridProperty(_T("Camera Pair"));
			break;
		case svaf::LayerParameter_LayerType_DSP:
			group = new CMFCPropertyGridProperty(_T("DSP Camera"));
			break;
		case svaf::LayerParameter_LayerType_DSP_PAIR:
			group = new CMFCPropertyGridProperty(_T("DSP Camera Pair"));
			break;
		case svaf::LayerParameter_LayerType_KINECT:
			group = new CMFCPropertyGridProperty(_T("Kinect"));
			break;
		case svaf::LayerParameter_LayerType_IMAGE_FOLDER:
			group = new CMFCPropertyGridProperty(_T("Image Folder"));
			break;
		case svaf::LayerParameter_LayerType_IMAGE_PAIR_FOLDER:
			group = new CMFCPropertyGridProperty(_T("Image Folder Pair"));
			break;
		case svaf::LayerParameter_LayerType_ADABOOST:
			group = new CMFCPropertyGridProperty(_T("Adaboost"));
			break;
		case svaf::LayerParameter_LayerType_MILTRACK:
			group = new CMFCPropertyGridProperty(_T("MILTrack"));
			break;
		case svaf::LayerParameter_LayerType_BITTRACK:
			group = new CMFCPropertyGridProperty(_T("Bino MILTrack"));
			break;
		case svaf::LayerParameter_LayerType_SIFT_POINT:
			group = new CMFCPropertyGridProperty(_T("SIFT Point"));
			break;
		case svaf::LayerParameter_LayerType_SURF_POINT:
			group = new CMFCPropertyGridProperty(_T("SURF Point"));
			break;
		case svaf::LayerParameter_LayerType_STAR_POINT:
			group = new CMFCPropertyGridProperty(_T("STAR Point"));
			break;
		case svaf::LayerParameter_LayerType_BRISK_POINT:
			group = new CMFCPropertyGridProperty(_T("Brisk Point"));
			break;
		case svaf::LayerParameter_LayerType_FAST_POINT:
			group = new CMFCPropertyGridProperty(_T("FAST Point"));
			break;
		case svaf::LayerParameter_LayerType_ORB_POINT:
			group = new CMFCPropertyGridProperty(_T("ORB Point"));
			break;
		case svaf::LayerParameter_LayerType_KAZE_POINT:
			group = new CMFCPropertyGridProperty(_T("KAZE Point"));
			break;
		case svaf::LayerParameter_LayerType_HARRIS_POINT:
			group = new CMFCPropertyGridProperty(_T("Harris Point"));
			break;
		case svaf::LayerParameter_LayerType_CV_POINT:
			group = new CMFCPropertyGridProperty(_T("CV Point"));
			break;
		case svaf::LayerParameter_LayerType_SIFT_DESP:
			group = new CMFCPropertyGridProperty(_T("SIFT Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_SURF_DESP:
			group = new CMFCPropertyGridProperty(_T("SURF Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_STAR_DESP:
			group = new CMFCPropertyGridProperty(_T("STAR Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_BRIEF_DESP:
			group = new CMFCPropertyGridProperty(_T("BRIEF Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_BRISK_DESP:
			group = new CMFCPropertyGridProperty(_T("Brisk Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_FAST_DESP:
			group = new CMFCPropertyGridProperty(_T("FAST Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_ORB_DESP:
			group = new CMFCPropertyGridProperty(_T("ORB Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_KAZE_DESP:
			group = new CMFCPropertyGridProperty(_T("KAZE Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_CV_DESP:
			group = new CMFCPropertyGridProperty(_T("CV Descriptor"));
			break;
		case svaf::LayerParameter_LayerType_KDTREE_MATCH:
			group = new CMFCPropertyGridProperty(_T("KDtree Match"));
			break;
		case svaf::LayerParameter_LayerType_EULAR_MATCH:
			group = new CMFCPropertyGridProperty(_T("Eular Match"));
			break;
		case svaf::LayerParameter_LayerType_RANSAC:
			group = new CMFCPropertyGridProperty(_T("Ransac"));
			break;
		case svaf::LayerParameter_LayerType_BF_MATCH:
			group = new CMFCPropertyGridProperty(_T("BF Match"));
			break;
		case svaf::LayerParameter_LayerType_FLANN_MATCH:
			group = new CMFCPropertyGridProperty(_T("Flann Match"));
			break;
		case svaf::LayerParameter_LayerType_EC_MATCH:
			group = new CMFCPropertyGridProperty(_T("EC Match"));
			break;
		case svaf::LayerParameter_LayerType_CV_MATCH:
			group = new CMFCPropertyGridProperty(_T("CV Match"));
			break;
		case svaf::LayerParameter_LayerType_SGM_MATCH:
			group = new CMFCPropertyGridProperty(_T("SGM Stereo Match"));
			break;
		case svaf::LayerParameter_LayerType_EADP_MATCH:
			group = new CMFCPropertyGridProperty(_T("Eadp Stereo Match"));
			break;
		case svaf::LayerParameter_LayerType_TRIANG:
			group = new CMFCPropertyGridProperty(_T("Triangle"));
			break;
		case svaf::LayerParameter_LayerType_MXMUL:
			group = new CMFCPropertyGridProperty(_T("Matrix Mul"));
			break;
		case svaf::LayerParameter_LayerType_CENTER_POS:
			group = new CMFCPropertyGridProperty(_T("Center Position"));
			break;
		case svaf::LayerParameter_LayerType_IA_EST:
			group = new CMFCPropertyGridProperty(_T("SAC-IA"));
			break;
		case svaf::LayerParameter_LayerType_IAICP_EST:
			group = new CMFCPropertyGridProperty(_T("SAC-IA ICP"));
			break;
		case svaf::LayerParameter_LayerType_IANDT_EST:
			group = new CMFCPropertyGridProperty(_T("SAC-IA NDP"));
			break;
		case svaf::LayerParameter_LayerType_SUPIX_SEG:
			group = new CMFCPropertyGridProperty(_T("Superpixel Segment"));
			break;
		case svaf::LayerParameter_LayerType_RECTIFY:
			group = new CMFCPropertyGridProperty(_T("Stereo Rectify"));
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
	auto id = pProp->GetData();
	CString s = pProp->GetName();
	COleVariant t = pProp->GetValue();
	t = pProp->GetOriginalValue();
	CString d;
	d = t.bstrVal;
	return 0;
}

