#include "stdafx.h"
#include "OpenGLControl.h"

COpenGLControl::COpenGLControl()
{
	m_fPosX = 0.0f;    // X position of model in camera view
	m_fPosY = 0.0f;    // Y position of model in camera view
	m_fZoom = 10.0f;   // Zoom on model in camera view
	m_fRotX = 0.0f;    // Rotation on model in camera view
	m_fRotY = 0.0f;    // Rotation on model in camera view
	m_bIsMaximized = false;
}

COpenGLControl::~COpenGLControl()
{
}
BEGIN_MESSAGE_MAP(COpenGLControl, CWnd)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void COpenGLControl::oglCreate(CRect rect, CWnd *parent, LPCTSTR windowname)
{
	CString className = AfxRegisterWndClass(CS_HREDRAW |
		CS_VREDRAW | CS_OWNDC, NULL,
		(HBRUSH)GetStockObject(BLACK_BRUSH), NULL);

	CreateEx(0, className, windowname, WS_CHILD | WS_VISIBLE |
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rect, parent, 0);

	// Set initial variables' values
	m_oldWindow = rect;
	m_originalRect = rect;

	hWnd = parent;
}

void COpenGLControl::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	ValidateRect(NULL);

	CPaintDC dc(this);
	OnDraw(&dc);
}

void COpenGLControl::oglInitialize(void)
{
	// Initial Setup:
	//
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,    // bit depth
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		16,    // z-buffer depth
		0, 0, 0, 0, 0, 0, 0,
	};
	hdc = GetDC()->m_hDC; // Get device context only once.	
	m_nPixelFormat = ChoosePixelFormat(hdc, &pfd); // Pixel format.
	SetPixelFormat(hdc, m_nPixelFormat, &pfd);
	hrc = wglCreateContext(hdc); // Create the OpenGL Rendering Context.
	wglMakeCurrent(hdc, hrc);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);// Set color to use when clearing the background.
	glClearDepth(1.0f);
	glFrontFace(GL_CCW); // Turn on backface culling
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST); // Turn on depth testing
	glDepthFunc(GL_LEQUAL);
	OnDraw(NULL); // Send draw request
}

void COpenGLControl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent){
	case 1:{
		// Clear color and depth buffer bits
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Draw OpenGL scene
		oglDrawScene();
		// Swap buffers
		SwapBuffers(hdc);
		break;
	}
	default:
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

void COpenGLControl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	glViewport(0, 0, cx, cy); // Map the OpenGL coordinates.
	glMatrixMode(GL_PROJECTION); // Projection view
	glLoadIdentity();
	gluPerspective(35.0f, (float)cx / (float)cy, 0.01f, 2000.0f); // Set our current view perspective
	glMatrixMode(GL_MODELVIEW); // Model view

	switch (nType){ // If window resize token is "maximize"
	case SIZE_MAXIMIZED:{
		GetWindowRect(m_rect); // Get the current window rect
		MoveWindow(6, 6, cx - 14, cy - 14); // Move the window accordingly
		GetWindowRect(m_rect); // Get the new window rect
		m_oldWindow = m_rect; // Store our old window as the new rect
		break;
	}
	case SIZE_RESTORED:{ // If window resize token is "restore"
		if (m_bIsMaximized) {// If the window is currently maximized
			GetWindowRect(m_rect); // Get the current window rect
			MoveWindow(m_oldWindow.left, // Move the window accordingly (to our stored old window)
				m_oldWindow.top - 18,
				m_originalRect.Width() - 4,
				m_originalRect.Height() - 4);
			GetWindowRect(m_rect); // Get the new window rect
			m_oldWindow = m_rect; // Store our old window as the new rect
		}
		break;
	}
	}
}

void COpenGLControl::OnDraw(CDC *pDC)
{
	// TODO: Camera controls.
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -m_fZoom);
	glTranslatef(m_fPosX, m_fPosY, 0.0f);
	glRotatef(m_fRotX, 1.0f, 0.0f, 0.0f);
	glRotatef(m_fRotY, 0.0f, 1.0f, 0.0f);
}

int COpenGLControl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here
	oglInitialize();
	return 0;
}

void COpenGLControl::oglDrawScene(void)
{
	// Wireframe Mode
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES); {
		for (int i = 0; i < pointcloud.size(); ++i){
			glColor3f(pointcloud[i].r, pointcloud[i].g, pointcloud[i].b);
			glVertex3f(pointcloud[i].x, pointcloud[i].y, pointcloud[i].z);
		}
	} glEnd();

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glBegin(GL_QUADS); {
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
	//} glEnd();
}

void COpenGLControl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	int diffX = (int)(point.x - m_fLastX);
	int diffY = (int)(point.y - m_fLastY);
	m_fLastX = (float)point.x;
	m_fLastY = (float)point.y;
	// Left mouse button
	if (nFlags & MK_LBUTTON){
		m_fRotX += (float)0.5f * diffY;
		if ((m_fRotX > 360.0f) || (m_fRotX < -360.0f)){
			m_fRotX = 0.0f;
		}
		m_fRotY += (float)0.5f * diffX;
		if ((m_fRotY > 360.0f) || (m_fRotY < -360.0f)){
			m_fRotY = 0.0f;
		}
	}
	// Right mouse button
	else if (nFlags & MK_RBUTTON){
		m_fZoom -= (float)0.1f * diffY;
	}
	// Middle mouse button
	else if (nFlags & MK_MBUTTON){
		m_fPosX += (float)0.05f * diffX;
		m_fPosY -= (float)0.05f * diffY;
	}
	OnDraw(NULL);
	CWnd::OnMouseMove(nFlags, point);
}

void COpenGLControl::SetEyes(float eyeX, float eyeY, float eyeZ,
	float centerX, float centerY, float centerZ){
	m_eyeX = eyeX;
	m_eyeY = eyeY;
	m_eyeZ = eyeZ;
	m_oriX = centerX;
	m_oriY = centerY;
	m_oriZ = centerZ;
	m_centerX = eyeX - m_oriX;
	m_centerY = eyeY - m_oriY;
	m_centerZ = eyeZ - m_oriZ;
	OnDraw(NULL);
}

void COpenGLControl::SetViewPoint(cv::Scalar & viewpoint, cv::Scalar & orientation){
	m_eyeX = viewpoint.val[0];
	m_eyeY = viewpoint.val[1];
	m_eyeZ = viewpoint.val[2];
	m_oriX = orientation.val[0];
	m_oriY = orientation.val[1];
	m_oriZ = orientation.val[2];
	m_centerX = m_eyeX - m_oriX;
	m_centerY = m_eyeY - m_oriY;
	m_centerZ = m_eyeZ - m_oriZ;
}

void COpenGLControl::SnapShot(cv::Mat & m_img){
	m_snap.glGrab();
	m_snap.save2mat(m_img);
}

void COpenGLControl::SetPointCloud(std::vector<Pointf>& points){
	pointcloud = points;
}
