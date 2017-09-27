/*	This File is part of ModelLabelTool Project.
*	More Reference at http://www.p-chao.com
*	4-15-2016, total line: 25
*	Copyright © 2016 Peng Chao
*	All rights reserved.
*/

#ifndef GL_GRABER_H_
#define GL_GRABER_H_
#include <opencv2\opencv.hpp>
#include <GL\GL.h>
class glGraber
{
public:
	glGraber();
	~glGraber();
	void glGrab();
	void saveColorImg(std::string &);
	void save2mat(cv::Mat &);
private:
	GLbyte* colorArr;
	GLint	viewPort[4];
	cv::Size win;
};
#endif
