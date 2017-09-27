#pragma once
#include <afxwin.h>
#include <opencv2\opencv.hpp>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "glGraber.h"

#pragma comment(lib, "glew32s.lib")
#ifdef _DEBUG
#pragma comment(lib, "GL/lib/d/freeglut_static.lib")
#else
#pragma comment(lib, "freeglut_static.lib")
#endif

using Pointf = struct _Pointf{
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
};

class COpenGLControl :
	public CWnd
{
public:
	COpenGLControl();
	~COpenGLControl();

	void	oglCreate(CRect rect, CWnd *parent, LPCTSTR);
	void	oglInitialize(void);
	void	oglDrawScene(void);

public:
	UINT_PTR	m_unpTimer;
	float		m_fLastX;
	float		m_fLastY;
	bool		m_bIsMaximized;
	GLfloat		m_fPosX;
	GLfloat		m_fPosY;
	GLfloat		m_fZoom;
	GLfloat		m_fRotX;
	GLfloat		m_fRotY;
	double		m_eyeX;
	double		m_eyeY;
	double		m_eyeZ;
	GLdouble	m_centerX = 0;
	GLdouble	m_centerY = 0;
	GLdouble	m_centerZ = 10;
	GLdouble	m_oriX = 0, m_oriY = 0, m_oriZ = 1;
	GLdouble	m_upX = 0;
	GLdouble	m_upY = 1;
	GLdouble	m_upZ = 0;
	GLdouble	m_posx = 0, m_posy = 0, m_posz = 0;
	GLdouble	m_poslx = 0, m_posly = 0, m_poslz = 0;

	std::vector<Pointf> pointcloud;

protected:
	GLfloat LightAmbient[4];
	GLfloat LightDiffuse[4];
	GLfloat LightPosition[4];
	
	glGraber m_snap;

public:
	CWnd*	hWnd;
	HDC		hdc;
	HGLRC	hrc;
	int		m_nPixelFormat;
	CRect	m_rect;
	CRect	m_oldWindow;
	CRect	m_originalRect;

	void SnapShot(cv::Mat &);
	void SetViewPoint(cv::Scalar &, cv::Scalar &);
	void SetEyes(float, float, float, float, float, float);
	void SetPointCloud(std::vector<Pointf>&);

	afx_msg	void OnPaint();
	afx_msg void OnDraw(CDC *pDC);
	afx_msg	void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

};