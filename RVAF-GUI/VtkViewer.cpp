#include "stdafx.h"  
#include "VtkViewer.h"  


// CVtkView  

IMPLEMENT_DYNAMIC(CVtkViewer, CStatic)

CVtkViewer::CVtkViewer()
{
	//在实例化时需要注意，该视图类在默认情况下渲染的是vtkResliceCursorWidget对象的输出，  
	//因此需要为vtkResliceCursorWidget对象指定相应的vtkRenderer对象，  
	//m_ResliceCursorWidget->SetInteractor(m_RenderWindow->GetInteractor());  
	//m_ResliceCursorWidget->SetDefaultRenderer(m_Renderer);  
}

CVtkViewer::~CVtkViewer()
{
}


void CVtkViewer::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class    
	CRect rect;
	GetClientRect(rect);

	m_Renderer = vtkSmartPointer<vtkRenderer>::New();
	m_RenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	m_RenderWindow->SetParentId(this->m_hWnd);
	m_RenderWindow->SetSize(rect.Width(), rect.Height());
	m_RenderWindow->AddRenderer(m_Renderer);

	if (m_RenderWindow->GetInteractor() == NULL)
	{
		vtkSmartPointer<vtkRenderWindowInteractor> RenderWindowInteractor =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
		RenderWindowInteractor->SetRenderWindow(m_RenderWindow);
		RenderWindowInteractor->Initialize();
	}

	m_RenderWindow->Start();
	CStatic::PreSubclassWindow();
}

void CVtkViewer::SetImageData(vtkSmartPointer<vtkImageData> ImageData)
{
	if (ImageData == NULL) return;

	m_ImageData = ImageData;
	SetupReslice();
}
void CVtkViewer::SetupReslice()
{
	if (m_ImageData == NULL) return;
	int dims[3];
	m_ImageData->GetDimensions(dims);

	//////////////////////////////////////////////////////////////////////////    
	m_ImagePlaneWidget->SetInputData(m_ImageData);
	m_ImagePlaneWidget->SetPlaneOrientation(m_Direction);
	m_ImagePlaneWidget->SetSliceIndex(dims[m_Direction] / 2);
	m_ImagePlaneWidget->On();
	m_ImagePlaneWidget->InteractionOn();

	//////////////////////////////////////////////////////////////////////////    
	m_ResliceCursor->SetCenter(m_ImageData->GetCenter());
	m_ResliceCursor->SetImage(m_ImageData);
	m_ResliceCursor->SetThickMode(0);

	//m_ResliceCursorRep->GetResliceCursorActor()->GetCursorAlgorithm()->SetResliceCursor(m_ResliceCursor);  
	//m_ResliceCursorRep->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(m_Direction);  

	m_ResliceCursorWidget->SetEnabled(1);
	m_Renderer->ResetCamera();

	//////////////////////////////////////////////////////////////////////////    
	double range[2];
	m_ImageData->GetScalarRange(range);
	m_ResliceCursorWidget->GetResliceCursorRepresentation()->
		SetWindowLevel(range[1] - range[0], (range[0] + range[1]) / 2.0);
	m_ImagePlaneWidget->SetWindowLevel(range[1] - range[0], (range[0] + range[1]) / 2.0);
}

BEGIN_MESSAGE_MAP(CVtkViewer, CStatic)
END_MESSAGE_MAP()
