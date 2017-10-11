#pragma once
#include <afxwin.h>
#include <afxsock.h>

#define COMMANDLENGTH 64

using POS = struct{ float X, Y, Z, A, B, C; };
using E6POS = struct{ float X, Y, Z, A, B, C, S, T; };
using E6AXIS = struct{ float A1, A2, A3, A4, A5, A6, E1, E2, E3, E4, E5, E6; };

/*STRUC FDAT INT TOOL_NO, BASE_NO, IPO_MODE IPO_FRAME, CHAR POINT2[24], BOOL TQ_STATE*/
using FDAT = struct{
	int	TOOL_NO;
	int BASE_NO;
	int IPO_MODE;
	int IPO_FRAME;
	char POINT2[24];
	bool TQ_STATE;
};

/*PDAT REAL VEL, ACC, APO_DIST, APO_MODE_T APO_MODE, REAL GEAR_JERK*/
using PDAT = struct{
	float VEL, ACC;
	int BASE_NO;
	int IPO_MODE;
	int IPO_FRAME;
	char POINT2[24];
	bool TQ_STATE;
};

class CRobotControl;
class CRobotControlDlg;

class RSocket : public CSocket
{
public:
	RSocket(){}
	virtual ~RSocket(){}
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
	CRobotControl *par;
};

class CRobotControl
{
public:
	CRobotControl();
	~CRobotControl();

	void Create(CRobotControlDlg*); // 创建
	bool Link(); // 建立Socket连接
	void LinkOff(); // 断开Socket连接
	void OnClose(); // Socket重载，销毁
	void OnAccept();
	void OnReceive();

	void SetAxis(int);
	void SetPort(int);
	void SetDlg(CRobotControlDlg*);

	void AddA1();
	void DecA1();
	void AddA2();
	void DecA2();
	void AddA3();
	void DecA3();
	void AddA4();
	void DecA4();
	void AddA5();
	void DecA5();
	void AddA6();
	void DecA6();
	void StopMove();

	void MoveToAxisMark(); // 移动到指定轴坐标
	void MoveToBaseMark(); // 移动到指定基坐标

	void GetCurrentMark(); // 获取当前坐标
	void GetCurrentBaseMark();
	void ResetAxisMark();
	void ResetBaseMark();

	void DataFromDlg();
	void DataToDlg();

protected:
	float A1, A2, A3, A4, A5, A6;
	char RobotCommond[COMMANDLENGTH];
	bool IsRecv = false;
	bool SendError = false;
	int RecvMode; // 0 char[], 1 E6AXIS, 2 POS
	float f_DataRecv[64]; // Recive value send by robot

	E6AXIS	PA1;
	POS		PP1;
	float	A1Recv, A2Recv, A3Recv, A4Recv, A5Recv, A6Recv;
	float	XRecv, YRecv, ZRecv, ARecv, BRecv, CRecv;
	float	XBase, YBase, ZBase, ABase, BBase, CBase;
	int		SBase; // 基坐标系运动下状态
	int		TBase; // 极坐标系下转角方向
	float	VelBase; // 基坐标系下直线运动的速度
	float	AccBase; // 基坐标系下直线运动的加速度
	
	CRobotControlDlg *pDlg;
	enum AXISSYS{
		AXIS = 0,
		BASE = 1,
		TOOL = 2
	};
	AXISSYS axis;

	UINT	nPort;
	RSocket m_ServerSocket;
	RSocket m_ClientSocket;

};

