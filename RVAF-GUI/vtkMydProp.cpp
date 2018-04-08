#include "stdafx.h"
#include "vtkMydProp.h"


vtkStandardNewMacro(vtkMydProp);

vtkMydProp::vtkMydProp()
{
	RtCamera = vtkCamera::New();
	//RtCamera->SetViewUp(0, 0, -1); // 视角位置
	RtCamera->SetPosition(0, 0, -10); // 观察位置
	//RtCamera->SetFocalPoint(0, 0, -10); // 设置焦点
	//RtCamera->ComputeViewPlaneNormal(); // 自动
}


vtkMydProp::~vtkMydProp()
{
}

void vtkMydProp::PrintSelf(ostream& os, vtkIndent indent){
	
}

int vtkMydProp::RenderVolumetricGeometry(vtkViewport *){
	//glLoadIdentity();
	//glTranslatef(0.0f, 0.0f, -10.0f);
	//glTranslatef(0.0f, 0.0f, 0.0f);
	//glRotated();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_QUADS); {
		glColor3f(1.0, 0.0, 0.0);
		// Top Side
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		// Bottom Side
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		// Front Side
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		// Back Side
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		// Left Side
		glVertex3f(-1.0f, -1.0f, -1.0f);
		glVertex3f(-1.0f, -1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, 1.0f);
		glVertex3f(-1.0f, 1.0f, -1.0f);
		// Right Side
		glVertex3f(1.0f, 1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, 1.0f);
		glVertex3f(1.0f, -1.0f, -1.0f);
		glVertex3f(1.0f, 1.0f, -1.0f);
	}
	glEnd();
	return 1;
}

