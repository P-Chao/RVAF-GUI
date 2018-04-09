#pragma once
#include <vtkProp.h>
#include <vtkCamera.h>
#include <vtkType.h>
#include <vtkVersionMacros.h>
#include <vtkVersion.h>
#include <vtkViewport.h>
#include <vtkIndent.h>
#include <vtkObjectFactory.h>

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

#include <vtkPolyVertex.h>
#include <vtkUnstructuredGrid.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>

#include <vtkLookupTable.h>
#include <vtkFloatArray.h>
#include <vtkPointData.h>

class vtkMydProp :
	public vtkProp
{
public:
	vtkMydProp();
	~vtkMydProp();

	//vtkTypeRevisionMacro(vtkMydProp, vtkProp);
	void PrintSelf(ostream& os, vtkIndent indent);
	static vtkMydProp* New();
	int RenderVolumetricGeometry(vtkViewport *);
	//vtkCloud * pCloud;
	vtkCamera * RtCamera;

	void ReadEularAngle(float a, float b, float c); // a 对应机器人 C角， b对应机器人的 B角
protected:
	float alpha;
	float beta;
	float gama;

private:
	vtkMydProp(const vtkMydProp&);  // Not implemented.     
	void operator=(const vtkMydProp&);  // Not implemented.
};

