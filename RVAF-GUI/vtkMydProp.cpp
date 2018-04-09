#include "stdafx.h"
#include "vtkMydProp.h"

//   A-B
// D-C E-F
float A[3] = { 0.0f, -0.62f, 1.0f };
float B[3] = { 0.0f, 0.62f, 1.0f };
float C[3] = { -1.0f, 0.62f, 0.0f };
float D[3] = { -1.0f, -0.62f, 0.0f };
float E[3] = { 1.0f, -0.62f, 0.0f };
float F[3] = { 1.0f, 0.62f, 0.0f };
float A_[3], B_[3], C_[3], D_[3], E_[3], F_[3];

vtkStandardNewMacro(vtkMydProp);

vtkMydProp::vtkMydProp()
{
	RtCamera = vtkCamera::New();
	//RtCamera->SetViewUp(0, 0, -1); // 视角位置
	RtCamera->SetPosition(0, 0, -10); // 观察位置
	//RtCamera->SetFocalPoint(0, 0, -10); // 设置焦点
	//RtCamera->ComputeViewPlaneNormal(); // 自动

	ReadEularAngle(0, 0, 0);
}


vtkMydProp::~vtkMydProp()
{
}

void vtkMydProp::PrintSelf(ostream& os, vtkIndent indent){
	
}

#ifndef RAD
#define RAD(x) ((x)*3.1415925/180.0)
#endif

void vtkMydProp::ReadEularAngle(float a, float b, float c){
	float M[3][3];
	alpha = a;
	beta = b;
	gama = c;

	float ra = RAD(a), rb = RAD(b), rc = RAD(c);
	// x = M[0][0] x' + MM[0][1] y' + ...
	M[0][0] = cos(rb) * cos(rc);
	M[0][1] = cos(rb) * sin(rc);
	M[0][2] = -sin(rb);
	M[1][0] = sin(ra) * sin(rb) * cos(rc) - cos(ra) * sin(rc);
	M[1][1] = sin(ra) * sin(rb) * sin(rc) + cos(ra) * cos(rc);
	M[1][2] = sin(ra) * cos(rb);
	M[2][0] = cos(ra) * sin(rb) * cos(rc) + sin(ra) * sin(rc);
	M[2][1] = cos(ra) * sin(rb) * sin(rc) - sin(ra) * cos(rc);
	M[2][2] = cos(ra) * cos(rb);

	A_[0] = M[0][0] * A[0] + M[0][1] * A[1] + M[0][2] * A[2];
	A_[1] = M[1][0] * A[0] + M[1][1] * A[1] + M[1][2] * A[2];
	A_[2] = M[2][0] * A[0] + M[2][1] * A[1] + M[2][2] * A[2];

	B_[0] = M[0][0] * B[0] + M[0][1] * B[1] + M[0][2] * B[2];
	B_[1] = M[1][0] * B[0] + M[1][1] * B[1] + M[1][2] * B[2];
	B_[2] = M[2][0] * B[0] + M[2][1] * B[1] + M[2][2] * B[2];

	C_[0] = M[0][0] * C[0] + M[0][1] * C[1] + M[0][2] * C[2];
	C_[1] = M[1][0] * C[0] + M[1][1] * C[1] + M[1][2] * C[2];
	C_[2] = M[2][0] * C[0] + M[2][1] * C[1] + M[2][2] * C[2];

	D_[0] = M[0][0] * D[0] + M[0][1] * D[1] + M[0][2] * D[2];
	D_[1] = M[1][0] * D[0] + M[1][1] * D[1] + M[1][2] * D[2];
	D_[2] = M[2][0] * D[0] + M[2][1] * D[1] + M[2][2] * D[2];

	E_[0] = M[0][0] * E[0] + M[0][1] * E[1] + M[0][2] * E[2];
	E_[1] = M[1][0] * E[0] + M[1][1] * E[1] + M[1][2] * E[2];
	E_[2] = M[2][0] * E[0] + M[2][1] * E[1] + M[2][2] * E[2];

	F_[0] = M[0][0] * F[0] + M[0][1] * F[1] + M[0][2] * F[2];
	F_[1] = M[1][0] * F[0] + M[1][1] * F[1] + M[1][2] * F[2];
	F_[2] = M[2][0] * F[0] + M[2][1] * F[1] + M[2][2] * F[2];

}

#undef RAD

int vtkMydProp::RenderVolumetricGeometry(vtkViewport *){
	//glLoadIdentity();
	//glTranslatef(0.0f, 0.0f, -10.0f);
	//glTranslatef(0.0f, 0.0f, 0.0f);
	//glRotated();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	
	glLineWidth(1.0f);
	glBegin(GL_QUADS); {
		// Right: AEFB
		glColor3f(0.1, 0.7, 0.1);
		glVertex3fv(A);
		glVertex3fv(E);
		glVertex3fv(F);
		glVertex3fv(B);
		// Left: ABCD
		glVertex3fv(A);
		glVertex3fv(B);
		glVertex3fv(C);
		glVertex3fv(D);
		// Bottom: CDEF
		glVertex3fv(C);
		glVertex3fv(D);
		glVertex3fv(E);
		glVertex3fv(F);
	}glEnd();

	
	glLineWidth(3.0f);
	glBegin(GL_QUADS); {
		glColor3f(1.0, 0.0, 0.0);
		// Right: AEFB
		glVertex3fv(A_);
		glVertex3fv(E_);
		glVertex3fv(F_);
		glVertex3fv(B_);
		// Left: ABCD
		glVertex3fv(A_);
		glVertex3fv(B_);
		glVertex3fv(C_);
		glVertex3fv(D_);
		// Bottom: CDEF
		glVertex3fv(C_);
		glVertex3fv(D_);
		glVertex3fv(E_);
		glVertex3fv(F_);
	}glEnd();

	return 1;
	//glBegin(GL_QUADS); {
	//	glColor3f(1.0, 0.0, 0.0);
	//	// Top Side
	//	glVertex3f(1.0f, 1.0f, 1.0f);
	//	glVertex3f(1.0f, 1.0f, -1.0f);
	//	glVertex3f(-1.0f, 1.0f, -1.0f);
	//	glVertex3f(-1.0f, 1.0f, 1.0f);
	//	// Bottom Side
	//	glVertex3f(-1.0f, -1.0f, -1.0f);
	//	glVertex3f(1.0f, -1.0f, -1.0f);
	//	glVertex3f(1.0f, -1.0f, 1.0f);
	//	glVertex3f(-1.0f, -1.0f, 1.0f);
	//	// Front Side
	//	glVertex3f(1.0f, 1.0f, 1.0f);
	//	glVertex3f(-1.0f, 1.0f, 1.0f);
	//	glVertex3f(-1.0f, -1.0f, 1.0f);
	//	glVertex3f(1.0f, -1.0f, 1.0f);
	//	// Back Side
	//	glVertex3f(-1.0f, -1.0f, -1.0f);
	//	glVertex3f(-1.0f, 1.0f, -1.0f);
	//	glVertex3f(1.0f, 1.0f, -1.0f);
	//	glVertex3f(1.0f, -1.0f, -1.0f);
	//	// Left Side
	//	glVertex3f(-1.0f, -1.0f, -1.0f);
	//	glVertex3f(-1.0f, -1.0f, 1.0f);
	//	glVertex3f(-1.0f, 1.0f, 1.0f);
	//	glVertex3f(-1.0f, 1.0f, -1.0f);
	//	// Right Side
	//	glVertex3f(1.0f, 1.0f, 1.0f);
	//	glVertex3f(1.0f, -1.0f, 1.0f);
	//	glVertex3f(1.0f, -1.0f, -1.0f);
	//	glVertex3f(1.0f, 1.0f, -1.0f);
	//}
	//glEnd();
	//return 1;
}

