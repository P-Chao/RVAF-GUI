#include "stdafx.h"  
#include "VtkViewer.h"  

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

void CVtkViewer::ReadEularAngle(float a, float b, float c){
	vtkMydProp *mydprop;
	mydprop = vtkMydProp::New();
	mydprop->ReadEularAngle(a, b, c);

	m_Renderer->RemoveActor(actor);
	m_Renderer->RemoveViewProp(mydprop);
	m_Renderer->AddViewProp(mydprop);
	m_Renderer->SetBackground(0, 0, 0);
	m_Renderer->SetActiveCamera(mydprop->RtCamera);
	//m_Renderer->ResetCamera();
	m_RenderWindow->Render();

	return;
}

void CVtkViewer::ReadPointCloud(std::vector<Pointf>& pointcloud){
	
	int n = pointcloud.size();
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	points->SetNumberOfPoints(n);
	vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
	lut->SetNumberOfTableValues(n);
	lut->Build();
	for (int i = 0; i < n; ++i){
		points->InsertPoint(i, pointcloud[i].x, pointcloud[i].y, pointcloud[i].z);
		lut->SetTableValue(i, pointcloud[i].r, pointcloud[i].g, pointcloud[i].b);
	}
	vtkSmartPointer<vtkPolyVertex> polyVertex = vtkSmartPointer<vtkPolyVertex>::New();
	polyVertex->GetPointIds()->SetNumberOfIds(n);
	vtkSmartPointer<vtkFloatArray> pointsScalars = vtkSmartPointer<vtkFloatArray>::New();
	pointsScalars->SetNumberOfTuples(n);
	for (int i = 0; i < n; ++i){
		polyVertex->GetPointIds()->SetId(i, i);
		pointsScalars->InsertValue(i, i);
	}

	vtkSmartPointer<vtkUnstructuredGrid> grid = vtkSmartPointer<vtkUnstructuredGrid>::New();
	grid->Allocate(1, 1);
	grid->SetPoints(points);
	grid->GetPointData()->SetScalars(pointsScalars);
	grid->InsertNextCell(polyVertex->GetCellType(), polyVertex->GetPointIds());

	vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
	mapper->SetInputData(grid);
	mapper->ScalarVisibilityOn();
	mapper->SetScalarRange(0, n-1);
	mapper->SetLookupTable(lut);

	m_Renderer->RemoveActor(actor);
	actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->GetProperty()->SetRepresentationToPoints();
	actor->GetProperty()->SetPointSize(2);

	m_Renderer->AddActor(actor);
	m_Renderer->SetBackground(0, 0, 0);
	m_Renderer->ResetCamera();
	m_RenderWindow->Render();

	return;

	// 点云全部显示同样的颜色
/*	vtkPoints * points = vtkPoints::New();
	int n = pointcloud.size(); int idx = 0;
	for (int i = 0; i < n; ++i){
		points->InsertPoint(i, pointcloud[i].x, pointcloud[i].y, pointcloud[i].z);
	}

	vtkPolyVertex * polyvertex = vtkPolyVertex::New();
	polyvertex->GetPointIds()->SetNumberOfIds(n);
	for (int i = 0; i < n; ++i){
		polyvertex->GetPointIds()->SetId(i, i);
	}

	vtkUnstructuredGrid * grid = vtkUnstructuredGrid::New();
	grid->SetPoints(points);
	grid->InsertNextCell(polyvertex->GetCellType(), polyvertex->GetPointIds());

	vtkDataSetMapper * map = vtkDataSetMapper::New();
	map->SetInputData(grid);

	vtkActor * actor = vtkActor::New();
	actor->SetMapper(map);
	actor->GetProperty()->SetColor(0.194, 0.562, 0.75);
	
	m_Renderer->AddActor(actor);
	m_Renderer->SetBackground(0, 0, 0);
	m_Renderer->ResetCamera();
	m_RenderWindow->Render();

	map->Delete();
	grid->Delete();
	actor->Delete();
	points->Delete();
	polyvertex->Delete();*/
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

void CVtkViewer::MoveWindow(CRect rect){
	m_RenderWindow->SetSize(rect.Width(), rect.Height());
	CStatic::MoveWindow(rect);
}

BEGIN_MESSAGE_MAP(CVtkViewer, CStatic)
END_MESSAGE_MAP()
