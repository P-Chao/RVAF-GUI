#pragma once
#include "afxwin.h"
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL)
VTK_MODULE_INIT(vtkInteractionStyle)
//VTK_MODULE_INIT(vtkRenderer)
//VTK_MODULE_INIT(vtkRenderingWindow)
#include <vtkResliceCursor.h>  
#include <vtkResliceCursorWidget.h>  

#include <vtkPlane.h>  
#include <vtkPlaneSource.h>  
#include <vtkPlaneWidget.h>  

#include <vtkImagePlaneWidget.h>  
#include <vtkResliceCursorThickLineRepresentation.h>  
#include <vtkResliceCursor.h>  

#include <vtkCommand.h>  
#include <vtkViewport.h>  
#include <vtkViewDependentErrorMetric.h>  

#include <vtkSmartPointer.h>  

#include <vtkRenderer.h>  
#include <vtkRendererSource.h>  
#include <vtkRenderingOpenGLModule.h>  
#include <vtkRenderWindow.h>  
#include <vtkWin32OpenGLRenderWindow.h>  
#include <vtkWin32RenderWindowInteractor.h> 



class CVtkViewer : public CStatic
{
	DECLARE_DYNAMIC(CVtkViewer)

public:
	CVtkViewer();
	virtual ~CVtkViewer();

public:
	//3.2 重载CvtkView类PreSubclassWindow（）函数和OnPaint()函数  
	//PreSubclassWindow函数负责创建VTK可视化管线，OnPaint()函数负责客户区内场景渲染。  
	//vtkAcor,vtkRenderer,vtkRenderWindow,vtkRenderWindowInteractor四个部分。当然根据需要还可以设置vtkRenderWindowInteractorStyle,以及光照，材质，颜色等。  
	//在CvtkView类头文件中定义相关对象，并在PreSubclassWindow函数中实例化和构建可视化管线  
	void PreSubclassWindow();
	void SetImageData(vtkSmartPointer<vtkImageData> ImageData);
	void SetupReslice();

private:
	vtkSmartPointer< vtkImagePlaneWidget >   m_ImagePlaneWidget;
	vtkSmartPointer< vtkResliceCursorWidget> m_ResliceCursorWidget;
	vtkSmartPointer< vtkResliceCursor >      m_ResliceCursor;
	vtkSmartPointer< vtkResliceCursorThickLineRepresentation > m_ResliceCursorRep;

	vtkSmartPointer<vtkRenderer>         m_Renderer;
	vtkSmartPointer<vtkRenderWindow> m_RenderWindow;
	vtkSmartPointer<vtkImageData>       m_ImageData;

	//m_Direction为方向标志，取值分别为0,1和2，分别代表X轴，Y轴和Z轴方向，  
	int m_Direction;

protected:
	DECLARE_MESSAGE_MAP()
};

//当用户改变图像切分的坐标轴时（旋转坐标轴或者平移坐标系），图像切分平面会产生相应的改变，  
//如果将新的切分平面更新到二维视图的vtkImagePlaneWidget对象中，即可实现三维视图的同步更新操作。  
///基于以上设计，实现一个vtkCommand子类，来监听vtkResliceCursorWidget::ResliceAxesChangedEvent消息，并实现相应的更新操作。  
//class vtkResliceCursorCallback : public vtkCommand
//{
//public:
//	static vtkResliceCursorCallback *New()
//	{
//		return new vtkResliceCursorCallback;
//	}
//
//	CVtkViewer* view[4];
//
//public:
//	void Execute(vtkObject *caller, unsigned long /*ev*/,
//		void *callData)
//	{
//		vtkResliceCursorWidget *rcw = dynamic_cast<vtkResliceCursorWidget *>(caller);
//		if (rcw)
//		{
//			for (int i = 0; i < 3; i++)
//			{
//				vtkPlaneSource *ps =
//					static_cast<vtkPlaneSource *>(view[i]->GetImagePlaneWidget()->GetPolyDataAlgorithm());
//
//				ps->SetOrigin(view[i]->GetResliceCursorWidget()->
//					GetResliceCursorRepresentation()->GetPlaneSource()->GetOrigin());
//				ps->SetPoint1(view[i]->GetResliceCursorWidget()->
//					GetResliceCursorRepresentation()->GetPlaneSource()->GetPoint1());
//				ps->SetPoint2(view[i]->GetResliceCursorWidget()->
//					GetResliceCursorRepresentation()->GetPlaneSource()->GetPoint2());
//
//				view[i]->GetImagePlaneWidget()->UpdatePlacement();
//				view[i]->Render();
//			}
//			view[3]->Render();
//		}
//
//	}
//
//	vtkResliceCursorCallback() {}
//};